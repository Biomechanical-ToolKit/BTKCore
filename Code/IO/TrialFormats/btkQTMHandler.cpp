/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009-2015, Arnaud Barré
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 
 *     * Redistributions of source code must retain the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer.
 *     * Redistributions in binary form must reproduce the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer in the documentation and/or other materials
 *       provided with the distribution.
 *     * Neither the name(s) of the copyright holders nor the names
 *       of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written
 *       permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "btkQTMHandler.h"
#include "btkQTMHandler_p.h"
#include "btkIODevice.h"
#include "btkBinaryStream.h"
#include "btkEndianFormat.h"
#include "btkAny.h"
#include "btkNode.h"
#include "btkTrial.h"
#include "btkTimeSequence.h"
#include "btkEvent.h"
#include "btkLogger.h"
#include "btkBuffer.h"
#include "btkutils.h"
#include "btklzo.h"

  #include <iostream>

// -------------------------------------------------------------------------- //
//                                 PRIVATE API                                //
// -------------------------------------------------------------------------- //

// #define _BTK_IO_QTM_DEBUG
#ifdef _BTK_IO_QTM_DEBUG
  #include <iostream>
  void _btk_io_qtm_debug_disp_compound_directoy(btk::QTMCompoundDocument* olecf, btk::QTMCompoundDocument::DictEntry* entry, const std::string& indent = "");
#endif

namespace btk
{
  QTMConfiguration::Item::~Item()
  {
    for (auto property: this->Properties)
      delete property;
    for (auto child: this->Children)
      delete child;
  };
  
  const QTMConfiguration::Property* QTMConfiguration::Item::property(const std::string& name) const _BTK_NOEXCEPT
  {
    for (const auto property: this->Properties)
    {
      if (property->DictEntry->Name.compare(name) == 0)
        return property;
    }
    return nullptr;
  };
  
  QTMConfiguration::~QTMConfiguration()
  {
    delete this->RootItem;
  }
  
  const QTMConfiguration::Item* QTMConfiguration::findItem(const std::string& name) const _BTK_NOEXCEPT
  {
    for (const auto child: this->RootItem->Children)
    {
      if (child->DictEntry->Name.compare(name) == 0)
        return child;
    }
    return nullptr;
  };
  
  // ----------------------------------------------------------------------- //
  
  QTMCompoundDocument::QTMCompoundDocument()
  : sector_size(0), 
    short_sector_size(0), 
    num_sectors_SAT(0), 
    first_sid_DIR(0), 
    standard_stream_minimum_size(0), 
    first_sid_SSAT(0), 
    num_sectors_SSAT(0), 
    first_sid_MSAT(0), 
    num_sectors_MSAT(0), 
    sids_MSAT(nullptr), 
    sids_SAT(nullptr), 
    sids_SSAT(nullptr),
    sids_DRE(nullptr),
    dir_entries()
  {};
  
  QTMCompoundDocument::~QTMCompoundDocument()
  {
    if (this->sids_MSAT != 0)
      delete[] this->sids_MSAT;
    if (this->sids_SAT != 0)
      delete[] this->sids_SAT;
    if (this->sids_SSAT != 0)
      delete[] this->sids_SSAT;
    for (std::vector<DictEntry*>::iterator it = this->dir_entries.begin() ; it != this->dir_entries.end() ; ++it)
      delete *it;
    this->dir_entries.clear();
  };
  
  void QTMCompoundDocument::load(BinaryStream* stream)
  {
    // GUID - Class identifier (not used and seems to be set to 0 everytime)
    stream->device()->seek(16, IODevice::Origin::Current);
    // File format revision number (should be all the time 4.62)
    int16_t minor = stream->readI16();
    int16_t major = stream->readI16();
    if ((minor != 62) && (major != 4))
      throw(QTMFormatError("QTM - Only the compound format version 4.62 is supported"));
    // Byte order identifier (already done)
    stream->device()->seek(2, IODevice::Origin::Current);
    // Size of a sector in the compound document file
    this->sector_size = static_cast<int32_t>(std::pow(2.0f, stream->readI16()));
    // Size of a short-sector in the short-stream container stream
    this->short_sector_size = static_cast<int32_t>(std::pow(2.0f, stream->readI16()));
    // Next 10 bytes might not used but the 7th byte has a value (0x06 or 0x08)...
    stream->device()->seek(10, IODevice::Origin::Current);
    // Total number of sectors used for the sector allocation table (SAT).
    this->num_sectors_SAT = stream->readI32();
    // Sector identifier (SID) of first sector of the directory stream
    this->first_sid_DIR = stream->readI32();
    // Next 4 bytes are not used
    stream->device()->seek(4, IODevice::Origin::Current);
    // Minimum number of bytes for a stream
    this->standard_stream_minimum_size = stream->readU32();
    // SID for the first sector in the Short SAT (SSAT)
    this->first_sid_SSAT = stream->readI32();
    // Total number of sectors used for the SSAT
    this->num_sectors_SSAT = stream->readI32();
    // SID for the first sector in the Master SAT (MSAT)
    this->first_sid_MSAT = stream->readI32();
    // Total number of sectors used for the MSAT
    this->num_sectors_MSAT = stream->readI32();
    // ------------------------------------------------------------------- //
    //                           ALLOCATION TABLES
    // ------------------------------------------------------------------- //
    // MSAT
    const int olecfNumSecIDsBySector_MSAT = (this->sector_size / 4) - 1;
    int olecfNumSecIDs_MSAT = 109 + this->num_sectors_MSAT * olecfNumSecIDsBySector_MSAT;
    this->sids_MSAT = new int32_t[olecfNumSecIDs_MSAT];
    stream->readI32(109, this->sids_MSAT);
    int olecfNumSecIDsRead_MSAT = 109;
    int32_t olecfNextSID_MSAT = this->first_sid_MSAT;
    for (int i = 0 ; i < this->num_sectors_MSAT ; ++i)
    {
      stream->device()->seek((olecfNextSID_MSAT+1) * this->sector_size, IODevice::Origin::Begin); // +1: Header
      stream->readI32(olecfNumSecIDsBySector_MSAT, this->sids_MSAT+olecfNumSecIDsBySector_MSAT);
      olecfNumSecIDsRead_MSAT += olecfNumSecIDsBySector_MSAT;
      olecfNextSID_MSAT = stream->readI32();
    }
    if (olecfNextSID_MSAT != -2)
      throw(QTMFormatError("QTM - Wrong SID for the last sector identifier used by the master sector allocation table."));
    // SAT
    const int olecfNumSecIDsBySector_SAT = this->sector_size / 4;
    int olecfNumSecIDs_SAT = this->num_sectors_SAT * olecfNumSecIDsBySector_SAT;
    this->sids_SAT = new int32_t[olecfNumSecIDs_SAT];
    for (int i = 0 ; i < olecfNumSecIDsRead_MSAT ; ++i)
    {
      if (this->sids_MSAT[i] == -1)
        continue;
      stream->device()->seek((this->sids_MSAT[i]+1) * this->sector_size, IODevice::Origin::Begin);
      stream->readI32(olecfNumSecIDsBySector_SAT, this->sids_SAT+i*olecfNumSecIDsBySector_SAT);
    }
    if (this->sids_SAT[0] != -3)
      throw(QTMFormatError("QTM - Corrupted file. Wrong SID for the first sector identifier used by the sector allocation table."));
    // SSAT
    const int olecfNumSecIDsBySector_SSAT = this->sector_size / 4;
    int olecfNumSecIDs_SSAT = this->num_sectors_SSAT * olecfNumSecIDsBySector_SSAT;
    this->sids_SSAT = new int32_t[olecfNumSecIDs_SSAT];
    int32_t olecfNextSID_SSAT = this->first_sid_SSAT;
    for (int32_t i = 0 ; i < this->num_sectors_SSAT ; ++i)
    {
      stream->device()->seek((olecfNextSID_SSAT+1) * this->sector_size, IODevice::Origin::Begin);
      stream->readI32(olecfNumSecIDsBySector_SSAT, this->sids_SSAT+i*olecfNumSecIDsBySector_SSAT);
      olecfNextSID_SSAT = this->sids_SAT[olecfNextSID_SSAT];
    }
    if (olecfNextSID_SSAT != -2)
      throw(QTMFormatError("QTM - Corrupted file. Wrong SID for the last sector identifier used by the short sector allocation table."));
    // ------------------------------------------------------------------- //
    //                                DIRECTORY
    // ------------------------------------------------------------------- //
    const int olecfNumEntriesBySector_DIR = this->sector_size / 128;
    int32_t next_sid_DIR = this->first_sid_DIR;
    // Try to guess the number of entries to reserve enough space for them
    size_t olecfNumEntries_DIR = 0;
    while (next_sid_DIR != -2)
    {
      olecfNumEntries_DIR += olecfNumEntriesBySector_DIR;
      next_sid_DIR = this->sids_SAT[next_sid_DIR];
    }
    if (olecfNumEntries_DIR == 0)
      throw(QTMFormatError("QTM - Corrupted file. There is no entry in the QTMCompoundDocument dictionary which could not be possible."));
    this->dir_entries.reserve(olecfNumEntries_DIR);
    // Extract entries
    next_sid_DIR = this->first_sid_DIR;
    while (next_sid_DIR != -2)
    {
      stream->device()->seek((next_sid_DIR+1) * this->sector_size, IODevice::Origin::Begin);
      for (int i = 0 ; i < olecfNumEntriesBySector_DIR ; ++i)
      {
        QTMCompoundDocument::DictEntry* entry = new QTMCompoundDocument::DictEntry;
        char dirNameWStr[64] = {0};
        stream->readChar(64, dirNameWStr);
        // We assume the wide string contains only extended ASCII characters.
        // Only the even characters are kept from 0 to 62. The last character must be always the null characeter.
        int16_t dirNameNumChar = stream->readI16()/2 - 1;
        dirNameNumChar = (dirNameNumChar < 0 ) ? 0 : dirNameNumChar;
        entry->name.resize(dirNameNumChar, ' ');
        for (int j = 0 ; j < dirNameNumChar ; ++j)
          entry->name[j] = dirNameWStr[2*j];
        trim_string(&(entry->name),0x00);
        entry->type = stream->readI8();
        // The next byte is the color of the entry stored a red/black tree. But we do not use this information here.
        stream->device()->seek(1, IODevice::Origin::Current);
        entry->left_id = stream->readI32();
        entry->right_id = stream->readI32();
        entry->sub_id = stream->readI32();
        // Skip the next 36 bytes are they do not seem important here (Class identifier, user flags, creation time, modification time,)
        stream->device()->seek(36, IODevice::Origin::Current);
        entry->first_sid = stream->readI32();
        entry->size = stream->readU32();
        // Last 4 bytes are reserved
        stream->device()->seek(4, IODevice::Origin::Current);
        
        this->dir_entries.push_back(entry);
      }
      next_sid_DIR = this->sids_SAT[next_sid_DIR];
    }
    if (this->dir_entries[0]->name.compare("Root Entry") != 0)
      throw(QTMFormatError("QTM - Corrupted file. Unexpected name for the first entry of the QTMCompoundDocument dictionary."));
    else
    {
      // Create the table to find where the SSAT are stored in the SAT
      int32_t num_sids_DRE = static_cast<int32_t>(std::ceil(static_cast<float>(this->dir_entries[0]->size) / static_cast<float>(this->sector_size)));
      this->sids_DRE = new int32_t[num_sids_DRE];
      int32_t entry_next_sid = this->dir_entries[0]->first_sid;
      int32_t inc = 0;
      while (entry_next_sid != -2)
      {
        assert(inc < num_sids_DRE);
        this->sids_DRE[inc] = entry_next_sid + 1;
        entry_next_sid = this->sids_SAT[entry_next_sid];
        ++inc;
      }
    }
    
#ifdef _BTK_IO_QTM_DEBUG
    std::cout << std::endl;
    _btk_io_qtm_debug_disp_compound_directoy(this, this->dir_entries[0]);
#endif
  };
  
  bool QTMCompoundDocument::setBufferToEntry(Buffer* buffer, const std::string& path)
  {
    QTMCompoundDocument::DictEntry* entry = this->findEntry(this->dir_entries[0], path);
    if ((entry == 0) || (entry->size == 0))
      return false;
    int32_t entry_next_sid = entry->first_sid;
    bool shortTable = (entry->size < this->standard_stream_minimum_size);
    int32_t chunk_size = shortTable ? this->short_sector_size : this->sector_size;
    int32_t shift_sid = shortTable ? 0 : 1;
    std::vector<size_t> chunk_ids;
    chunk_ids.reserve(entry->size / chunk_size);
    while (entry_next_sid != -2)
    {
      int32_t chunk_id = entry_next_sid + shift_sid;
      if (shortTable)
      {
        entry_next_sid = this->sids_SSAT[entry_next_sid];
        int32_t ssid = chunk_id * this->short_sector_size;
        int32_t sid = ssid / this->sector_size;
        chunk_id = (this->sids_DRE[sid] * this->sector_size + (ssid % this->sector_size)) / this->short_sector_size;
      }
      else
      {
        entry_next_sid = this->sids_SAT[entry_next_sid];
      }  
      chunk_ids.push_back(chunk_id);
    }
    buffer->setChunks(chunk_ids, chunk_size);
    return true;
  };
  
  // Find the entry with the given label. The character '/' has a special meaning used to split child from parent.
  QTMCompoundDocument::DictEntry* QTMCompoundDocument::findEntry(QTMCompoundDocument::DictEntry* parent, const std::string& path) const
  {
    if ((parent == 0) || (parent->sub_id == -1))
      return 0;
    QTMCompoundDocument::DictEntry* entry = 0;
    std::string current = path, next;
    size_t pos = path.find_first_of('/');
    if (pos != std::string::npos)
    {  
      current = path.substr(0,pos);
      next = path.substr(pos+1);
    }
    QTMCompoundDocument::DictEntry* child = this->findLevelEntry(this->dir_entries[parent->sub_id], current);
    if (next.empty())
      entry = child;
    else
      entry = this->findEntry(child, next);
    return entry;
  };
  
  QTMCompoundDocument::DictEntry* QTMCompoundDocument::findLevelEntry(QTMCompoundDocument::DictEntry* level, const std::string& name) const
  {
    if (level->name.compare(name) == 0)
      return level;
    if (level->left_id != -1)
    {
      QTMCompoundDocument::DictEntry* entry = this->findLevelEntry(this->dir_entries[level->left_id], name);
      if (entry != 0)
        return entry;
    }
    if (level->right_id != -1)
    {  
      QTMCompoundDocument::DictEntry* entry = this->findLevelEntry(this->dir_entries[level->right_id], name);
      if (entry != 0)
        return entry;
    }
    return 0;
  }
  
  // ----------------------------------------------------------------------- //
  
  QTMHandlerPrivate::QTMHandlerPrivate()
  : IOHandlerPrivate()
  {};
  
  QTMHandlerPrivate::~QTMHandlerPrivate() _BTK_NOEXCEPT = default;
  
  
  void QTMHandlerPrivate::extractConfigurationProperties(QTMConfiguration::Item* item, std::vector<QTMConfiguration::DictPropertyEntry>* properties, BinaryStream* stream)
  {
    int32_t numProperties = stream->readI32();
    item->Properties.reserve(numProperties);
    for (int32_t i = 0 ; i < numProperties ; ++i)
    {
      int32_t idx = stream->readI32();
      if (idx > static_cast<int32_t>(properties->size()))
        throw(QTMFormatError("QTM - Corrupted file. Index for one property is out of range"));
      auto prop = new QTMConfiguration::Property;
      prop->DictEntry = &(properties->operator[](idx-1));
      int32_t offset = stream->readI32();
      int32_t len = 0;
      switch(prop->DictEntry->Flags[0])
      {
      case 3: // Signed integer 32-bit (4 bytes)
        prop->Value = stream->readI32();
        len = 4;
        break;
      case 7: // Unsigned integer 32-bit (4 bytes)
        prop->Value = stream->readU32();
        len = 4;
        break;
      case 9: // Float32 (4 bytes)
        prop->Value = stream->readFloat();
        len = 4;
        break;
      case 10: // Float64 (8 bytes)
        prop->Value = stream->readDouble();
        len = 8;
        break;
      case 11: // Boolean  (1 byte)
        prop->Value = stream->readI8();
        len = 1;
        break;
      case 13: // Enum (4 bytes)
        //  NOTE: The enumators have to be found from the 4th proprerty
        prop->Value = stream->readI32();
        len = 4;
        break;
      case 14: // Enumerators
        // It seems the 4th proprerty link to next enumators
        throw(QTMFormatError("QTM - Type '14' used by the metadata '" + prop->DictEntry->Name + "' is recognized but not yet supported"));
        break;
      case 18: // Date (4 bytes)
        prop->Value = stream->readI32();
        len = 4;
        break;
      case 19: // TimeOfDay (4 bytes)
        prop->Value = stream->readI32();
        len = 4;
        break;
      case 20: // GUID (16 bytes)
        prop->Value = stream->readString(16);
        len = 16;
        break;
      case 21: // UnicodeString (dynamic length)
        {
        // FIXME: It it necessary to manage the encoding based on the content of the third flag (prop->DictEntry->Flags[2])?
        len = stream->readI32()*2 + 2; // *2: UTF-16 ; +2: Null character
        std::vector<char> temp(len, 0x00);
        stream->readChar(len,temp.data());
        std::string str(len/2 - 1, '\0');
        for (size_t j = 0 ; j < str.length() ; ++j)
          str[j] = temp[2*j];
        trim_string(&str,0x00);
        prop->Value = str;
        len += 4; // +4: dynamic size
        }
        break;
      case 23: // Array?
        throw(QTMFormatError("QTM - Type '23' used by the metadata '" + prop->DictEntry->Name + "' is recognized but not yet supported"));
        break;
      case 24: // Structure
        {
        // NOTE: Due to the large possibilities of a structure, only the raw binary data are stored in the property. A btk::Buffer associated with a btk::BinaryStream should be used to extract the exact content of a structure
        std::vector<char> temp(offset, 0x00);
        stream->readChar(offset,temp.data());
        prop->Value = temp;
        len = offset;
        }
        break;
      case 25: // ContainerReference
        // FIXME: How to manage this type?
        prop->Value = stream->readI32();
        len = 4;
        break;
      default:
        throw(QTMFormatError("QTM - Unexpected type for property '" + prop->DictEntry->Name + "': " + std::to_string(prop->DictEntry->Flags[0])));
      }
      stream->device()->seek(offset-len, Buffer::Origin::Current);
      item->Properties.emplace_back(prop);
    }
    // Extra values?
    int32_t numExtraValues = stream->readI32();
    if (numExtraValues != 0)
    {
      item->Extras.resize(numExtraValues);
      for (int32_t i = 0 ; i < numExtraValues ; ++i)
      {
        int32_t indexExtra = stream->readI32();
        if ((indexExtra < 1) || (indexExtra > numExtraValues))
          throw(QTMFormatError("QTM - Corrupted file. Index for extra value is out of range"));
        item->Extras[indexExtra-1] = stream->readI32();
      }
    }
  };
  
  void QTMHandlerPrivate::extractConfigurationChildren(QTMConfiguration::Item* item, QTMConfiguration* config, BinaryStream* stream)
  {
    if (item->DictIndex < 1)
      throw(QTMFormatError("QTM - Corrupted file. An index value is negative"));
    if (item->DictIndex > static_cast<int32_t>(config->DictItemEntries.size()))
      throw(QTMFormatError("QTM - Corrupted file. An index value exceeds the number of extracted item(s)"));
    item->DictEntry = &(config->DictItemEntries[item->DictIndex-1]);
    // std::cout << "\n * " << (*groups)[idx-1].name << std::endl;
    this->extractConfigurationProperties(item, &(config->DictPropertyEntries), stream);
    int32_t numChildren = stream->readI32();
    item->Children.reserve(numChildren);
    for (int32_t i = 0 ; i < numChildren ; ++i)
    {
      auto child = new QTMConfiguration::Item;
      child->DictIndex = stream->readI32();
      child->GlobalIndex = stream->readI32(); // ???
      this->extractConfigurationChildren(child, config, stream);
      item->Children.emplace_back(child);
    }
    int32_t numTOC = stream->readI32();
    if (numTOC != 0)
    {
      for (int32_t i = 0 ; i < numTOC ; ++i)
      {
#if 1
        stream->device()->seek(4, Buffer::Origin::Current);
#else
        int32_t category = stream->readI32();
#endif
        int32_t numItems = stream->readI32();
        // Let's skip this part for the moment. Give the order for the read items
        stream->device()->seek(numItems*4, Buffer::Origin::Current);
      }
    }
  };
  
  void QTMHandlerPrivate::extractConfiguration(QTMConfiguration* config, const std::string& configname, QTMCompoundDocument* doc, BinaryStream* stream)
  {
    IODevice* origDevice = stream->device();
    // The information given to the buffer device is not complete. The chunks associated with each entry are set in the method QTMCompoundDocument::setBufferToEntry.
    Buffer buffer;
    buffer.open(origDevice->data(), origDevice->size());
    // The binary stream now rely on the data of the buffer.
    stream->setDevice(&buffer);
    // Look for the metadata...
    // ------------------------
    if (doc->setBufferToEntry(&buffer, configname + "/Metadata") == 0)
      throw(QTMFormatError("QTM - Corrupted file. One entry for the project settings was not found (Project Metadata/Metadata)"));
    // 1. Extract the items configuration (name, type)
    int32_t numItems = stream->readI32();
    if (stream->readI32() != 192)
      throw(QTMFormatError("QTM - Unexpected key to extract entries for the item dictionary. This might be a limitation of the parser"));
    config->DictItemEntries.reserve(numItems);
    for (int32_t i = 0 ; i < numItems ; ++i)
    {
      // Name: ASCII string
      std::string name = stream->readString(stream->readI32());
      trim_string(&name,0x00);
      // Name: UTF-16 string (not used)
      stream->device()->seek(stream->readI32(), Buffer::Origin::Current);
      // Type?
      config->DictItemEntries.emplace_back(name, stream->readI32());
      // std::cout << " * " << groups.size() << ". " << entry.name << " (type: " << entry.value.cast<std::string>() << ")" << std::endl;
    }
    int32_t numProperties = stream->readI32();
    if (stream->readI32() != 276)
      throw(QTMFormatError("QTM - Unexpected key to extract entries for the propriety dictionary. This might be a limitation of the parser"));
    config->DictPropertyEntries.reserve(numProperties);
    for (int32_t i = 0 ; i < numProperties ; ++i)
    {
      // Name: ASCII string
      std::string name = stream->readString(stream->readI32());
      trim_string(&name,0x00);
      // Description? (ASCII string)
      std::string description = stream->readString(stream->readI32());
      trim_string(&description,0x00);
      // Name: UTF-16 string (not used)
      stream->device()->seek(stream->readI32(), Buffer::Origin::Current);
      // Flags? (20 bytes)
      std::array<int32_t,5> flags;
      stream->readI32(5,flags.data());
      config->DictPropertyEntries.emplace_back(name, description, flags);

#if 1      
      const auto& entry = config->DictPropertyEntries.back();
      std::cout << " - " << config->DictPropertyEntries.size() << ". " << entry.Name << " (type: " << entry.Flags[0] << " ; f[1]: " << entry.Flags[1] << " ; f[2]: " << entry.Flags[2] << " ; f[3]: " << entry.Flags[3] << " ; f[4]: " << entry.Flags[4] << ")" << std::endl;
#endif
    }
    // ... And then the associated values
    // ----------------------------------
    if (doc->setBufferToEntry(&buffer, configname + "/Data Items") == 0)
      throw(QTMFormatError("QTM - Corrupted file. One entry for the project settings was not found (Project Metadata/Data Items)"));
    config->RootItem = new QTMConfiguration::Item;
    config->RootItem->DictIndex = 1;
    this->extractConfigurationChildren(config->RootItem, config, stream);
    // Set back the stream to its original device
    stream->setDevice(origDevice);
  };
  
};

#ifdef _BTK_IO_QTM_DEBUG
  void _btk_io_qtm_debug_disp_compound_directoy(btk::QTMCompoundDocument* olecf, btk::QTMCompoundDocument::DictEntry* entry, const std::string& indent)
  {
    std::cout << indent;
    if (entry->type == btk::QTMCompoundDocument::DictEntry::RootStorage)
      std::cout << "* ";
    else if (entry->type == btk::QTMCompoundDocument::DictEntry::Storage)
      std::cout << "+ ";
    else if (entry->type == btk::QTMCompoundDocument::DictEntry::Stream)
      std::cout << "- ";
    else
      std::cout << "? ";
    std::cout << entry->name;
    if ((entry->type == btk::QTMCompoundDocument::DictEntry::Stream) || (entry->type == btk::QTMCompoundDocument::DictEntry::RootStorage))
    {
      std::cout << " (" << entry->size << " bytes)";
      int32_t entry_next_sid = entry->first_sid;
      bool shortTable = false;
      if (entry->size < olecf.standard_stream_minimum_size)
      {
        std::cout << " - SSAT";
        shortTable = true;
      }
      else
      {
        std::cout << " - SAT";
      }
      std::cout << " - SIDs: [";
      while (entry_next_sid != -2)
      {
        std::cout << entry_next_sid + (shortTable ? 0 : 1);
        entry_next_sid = shortTable ? olecf.sids_SSAT[entry_next_sid] : olecf.sids_SAT[entry_next_sid];
        if (entry_next_sid != -2)
          std::cout << ",";
      }
      std::cout << "]";
    }
    std::cout << std::endl;
    if (entry->sub_id != -1)
      _btk_io_qtm_debug_disp_compound_directoy(olecf, olecf.dir_entries[entry->sub_id], indent+"  ");
    if (entry->left_id != -1)
      _btk_io_qtm_debug_disp_compound_directoy(olecf, olecf.dir_entries[entry->left_id], indent);
    if (entry->right_id != -1)
      _btk_io_qtm_debug_disp_compound_directoy(olecf, olecf.dir_entries[entry->right_id], indent);
  };
#endif

_BTK_CONSTEXPR uint8_t _btk_qtm_olecf_magic[8] = {0xD0,0xCF,0x11,0xE0,0xA1,0xB1,0x1A,0xE1};

// -------------------------------------------------------------------------- //
//                                 PUBLIC API                                 //
// -------------------------------------------------------------------------- //

namespace btk
{

  /**
   * @class QTMHandler btkQTMHandler.h
   * @brief Interface to read QTM files.
   *
   * The QTM file format is used by the Qualisys AB company (Gothenburg, Sweden).
   * It stores video and analog data as well as the configuration of a trial.
   *
   * The QTM file format uses internally the compound document format to store binary data.
   * The comprehension of the compound document format would have not been possible without the following documents:
   *  - "Object Linking and Embedding (OLE) Compound File (CF) format specification" (Joachim Metz - https://googledrive.com/host/0B3fBvzttpiiSS0hEb0pjU2h6a2c/OLE%20Compound%20File%20format.pdf) 
   *  - "The Microsoft Compound Document File Format" (OpenOffice - http://www.openoffice.org/sc/compdocfileformat.pdf)
   *
   * The extraction of trial content from the binary data had been done by reverse engineering. Some specific information were enlightened by the Qualysis software development team
   *
   * @ingroup BTKIO
   */

  /**
   * Default constructor
   */
  QTMHandler::QTMHandler()
  : TrialIOHandler(*new QTMHandlerPrivate)
  {};
  
  QTMHandler::~QTMHandler() _BTK_NOEXCEPT = default;
  
  QTMHandler::Capability QTMHandler::capability() const _BTK_NOEXCEPT
  {
    return Capability::Read;
  };
  
  void QTMHandler::readDevice(Node* output)
  {
    auto optr = this->pimpl();
    BinaryStream stream(optr->Device);
    
    optr->Device->setExceptions(IODevice::State::End | IODevice::State::Fail | IODevice::State::Error);
    // Jump to Byte order identifier to extract correctly the other information
    // The signature is not checked as it is already done
    optr->Device->seek(28, IODevice::Origin::Begin);
    char byteOrder[2] = {0};
    optr->Device->read(byteOrder,2);
    if ((byteOrder[0] == char(0xFE)) && (byteOrder[1] == char(0xFF)))
    {
      this->setOption<ByteOrderFormat>(EndianFormat::IEEELittleEndian);
      stream.setEndianFormat(EndianFormat::IEEELittleEndian);
    }
    else if ((byteOrder[0] == char(0xFF)) && (byteOrder[1] == char(0xFE)))
    {
      this->setOption<ByteOrderFormat>(EndianFormat::IEEEBigEndian);
      stream.setEndianFormat(EndianFormat::IEEEBigEndian);
    }
    else
      throw(FormatError("QTM - Unknown byte order identifier"));
    
    // Go back after the signature
    optr->Device->seek(8, IODevice::Origin::Begin);
    // Load the internal compound document
    QTMCompoundDocument doc;
    doc.load(&stream);
    
    // At this point the trial can be created
    Trial* trial = new Trial(utils::strip_path(optr->Device->name()),output);
    
    // The binary stream now relies on the buffer content.
    Buffer buffer;
    buffer.open(optr->Device->data(), optr->Device->size());
    stream.setDevice(&buffer);
    
    // Extract markers data
    // FIXME: What is the unit for markers' coordinates?
    // FIXME: What is the samples frequency for markers?
    if (doc.setBufferToEntry(&buffer, "Measurement/3D points") != 0)
    {
      // NOTE: This is not sure if the internal structure is fixed or not
      //       but the tested QTM files have each time 4 elements in the
      //       entry "Measurement/3D points":
      //       1. Unknown (was 0 each time)
      //       2. Unknown (was 0 each time)
      //       3. Markers data
      //       4. Seems to be a table with some indexes inside. 
      //          This part is not yet understood
      if (stream.readI32() != 4)
        throw(FormatError("QTM - Unsupported number of elements for the entry 'Measurement/3D points'. This might be a limitation of the parser"));
      for (int32_t i = 0 ; i < 4 ; ++i)
      {
        uint32_t blockIndex = stream.readU32();
        uint32_t blockSize = stream.readU32();
        // The index 3 is the block with the markers data
        if (blockIndex == 3)
        {
          // General number of samples 
          uint32_t numSamples = stream.readU32();
          // Array with (valid?) points' ID
          uint32_t numValidMarkers = stream.readU32() / 4;
          std::vector<uint32_t> pids(numValidMarkers,0);
          stream.readU32(pids.size(), pids.data());
          uint32_t numStoredMarkers = stream.readU32();
          for (uint32_t j = 0 ; j < numStoredMarkers ; ++j)
          {
            uint32_t indexMarker = stream.readU32();
            uint32_t dataSize = stream.readU32();
            // In case the extracted ID matches one of the value in the array
            if (std::find(pids.begin(), pids.end(), indexMarker) != pids.end())
            {
              --numValidMarkers;
              // The next 36 bytes are not understood...
              buffer.seek(36, IODevice::Origin::Current);
              // Marker's name
              int32_t numBytesLabel = stream.readI32();
              int32_t numCharLabel = numBytesLabel / 2 - 1;
              std::vector<char> temp(numBytesLabel, 0x00);
              stream.readChar(temp.size(), temp.data());
              std::string label;
              if (numBytesLabel != 2)
              {
                label.resize(numCharLabel);
                for (int32_t k = 0 ; k < numCharLabel ; ++k)
                  label[k] = temp[2*k];
                trim_string(&(label),0x00);
              }
              else // Null character
                label = "uname*" + std::to_string(j+1);
              // std::cout << " - Marker's name: " << label << std::endl;
              // Number of parts in the reconstructed trajectory
              uint32_t numParts = stream.readU32();
              // std::cout << "   - parts: " << numParts << std::endl;
              // For each part...
              for (uint32_t k = 0 ; k < numParts ; ++k)
              {
                // Size of this part
                uint32_t partSize = stream.readU32();
                // What is the meaning of these 2 bytes?
                if (stream.readU16() != 1)
                  throw(FormatError("QTM - Unexpected extracted value inside markers part. This might be a limitation of the parser"));
                // Starting sample (seems to be a 1-based index)
                uint32_t partStartIndex = stream.readU32() - 1;
                // Number of samples
                uint32_t partNumSamples = stream.readU32();
                if ((partStartIndex + partNumSamples) > numSamples)
                  throw(FormatError("QTM - Total number of samples for marker '" + label + "' exceeded"));
                // Array of 'partNumSamples' x 4 double (8 bytes)
                buffer.seek(partSize-10, IODevice::Origin::Current);
              }
            }
            // Otherwise, let's move to the next marker
            else
              buffer.seek(dataSize, IODevice::Origin::Current);
          }
          if (numValidMarkers != 0)
            Logger::warning("%s - QTM - Not every valid markers were found", optr->Device->name());
          break;
        }
        else
        {
          // Let's move to the next block
          buffer.seek(blockSize, IODevice::Origin::Current);
        }
      }
    }
    
    // FIXME: How-to retrieve the number of force platforms?
    //        No usefull information was found in Measurement/Forceplates/Data Members
#if 0
    // Extract force platform configuration
    if (0)
    {
      uint32_t numForceplates = 0; // ???
      for (uint32_t i = 0 ; i < numForceplates ; ++i)
      {
        // FIXME: Extract the good name associated with each force platform
        if (doc.setBufferToEntry(&buffer, "Measurement/Forceplates/???/Settings") != 0)
        {
          int32_t numElements = stream.readI32();
            throw(FormatError("QTM - Unsupported number of elements for the entry 'Measurement/3D points'. This might be a limitation of the parser"));
          for (int32_t j = 0 ; j < numElements ; ++j)
          {
            uint32_t blockIndex = stream.readU32();
            uint32_t blockSize = stream.readU32();
            // #1: FIXME: What contains this element?
            // #2: FIXME: What contains this element? Could be the activation of the forceplate
            // #3: FIXME: What contains this element?
            // #4: Name (UTF-16)
            if (blockIndex == 4)
            {
              std::vector<char> temp(blockSize*2, 0x00);
              stream.readChar(temp.size(), temp.data());
              std::string label(blockSize-1, 0x00);
              for (size_t k = 0 ; k < label.size() ; ++k)
                label[k] = temp[2*k];
              trim_string(&label,0x00);
            }
            // #5: Corners coordinates
            else if (blockIndex == 5)
            {
              if (blockSize < 48) // 4 corners x 3 coordinates x 4 bytes (float 32-bit)
                throw(FormatError("QTM - Wrong data size for corners coordinates"));
              std::array<float,12> corners;
              stream.readFloat(corners.size(), corners.data());
            }
            // #6: Analog channel indices
            // NOTE: Use the property 'Channel number' of each child of 'Analog Data' to get the associated signal
            else if (blockIndex == 6)
            {
              // FIXME: How do you know the number of channels associated with this forceplate?
              uint32_t numChannels = blockSize / 4;
              std::vector<int32_t> indexChannels(numChannels,-1);
              stream.readI32(indexChannels.size(), indexChannels.data());
            }
            // #7: FIXME: What contains this element? Seems to be used to scale something
            // #8: Calibration matrix
            else if (blockIndex == 8)
            {
              uint32_t num = blockSize / 8;
              // TODO: Had a test to check is the size of the calibration matrix corresponds to the input/output of the platform
              std::vector<double> calibration(num,0.0);
              stream.readDouble(calibration.size(), calibration.data());
            }
            // #9: FIXME: What contains this element?
            // #10: FIXME: What contains this element?
            // #11: FIXME: What contains this element?
            // #12: Width (unit? Millimeter everytime?)
            else if (blockIndex == 12)
            {
              double with = stream.readDouble();
            }
            // #13: Length (unit? Millimeter everytime?)
            else if (blockIndex == 13)
            {
              double length = stream.readDouble();
            }
            // #14: Origin X
            else if (blockIndex == 14)
            {
              double origX = stream.readDouble();
            }
            // #15: Origin Y
            else if (blockIndex == 15)
            {
              double origY = stream.readDouble();
            }
            // #16: Origin Z
            else if (blockIndex == 16)
            {
              double origZ = stream.readDouble();
            }
            else // Let's move to the next block
              buffer.seek(blockSize, IODevice::Origin::Current);
            // #17 and more ... FIXME: What contains these elements?
          }
          
        }
      }
    }
#endif
    
    // Extract events
    if (doc.setBufferToEntry(&buffer, "Measurement/Events") != 0)
    {
      if (stream.readI32() != 2)
        throw(FormatError("QTM - Unsupported number of elements for the entry 'Measurement/Events'. This might be a limitation of the parser"));
      const uint32_t eventBlockSize = 112;
      uint32_t numEvents = -1;
      for (int32_t i = 0 ; i < 2 ; ++i)
      {
        uint32_t blockIndex = stream.readU32();
        uint32_t blockSize = stream.readU32();
        // The first block contains the label and the time associated with each event
        if (blockIndex == 1)
        {
          numEvents = blockSize / eventBlockSize;
          for (uint32_t j = 0 ; j < numEvents ; ++j)
          {
            // Time (in seconds)
            double t = stream.readDouble();
            // Label
            std::string n = stream.readString(4);
            trim_string(&n,0x00);
            // Next 100 bytes could be to store a descrption?
            buffer.seek(100, IODevice::Origin::Current);
            
            std::cout << "- Event" << n << ": " << t << std::endl;
          }
        }
        // The second block might be related to the activation of an event but this is not sure ... Some tests are added below in case unexpected values were found. This would help to uderstand the exact content of this block.
        else if (blockIndex == 2)
        {
          // NOTE: It is assumed that the second block is everytime after the first block.
          if (blockSize/4 != numEvents)
            throw(FormatError("QTM - Unexpected result in the second block of the entry 'Measurement/Events'. This might be a limitation of the parser"));
          for (uint32_t j = 0 ; j < numEvents ; ++j)
          {
            if (stream.readU32() != 255)
              throw(FormatError("QTM - Unexpected value in the second block of the entry 'Measurement/Events'. This might be a limitation of the parser"));
          }
        }
        // This would not be possible...
        else
          throw(FormatError("QTM - Unsupported block index for the entry 'Measurement/Events'. This might be a limitation of the parser"));
      }
    }
    
    // Extract the configuration "Measurement NBC" which contains the analog data
    QTMConfiguration measurementNBC;
    optr->extractConfiguration(&measurementNBC, "Measurement NBC", &doc, &stream);
    auto analogConfig = measurementNBC.findItem("Analog Data");
    if (analogConfig != nullptr)
    {
      int inc = 1;
      std::string label;
      std::vector<char> encoded, decoded;
      for (const auto child: analogConfig->Children)
      {
        auto labelProp = child->property("Label");
        if (labelProp == nullptr)
          label = "uname*" + std::to_string(inc);
        else
          label = labelProp->Value.cast<std::string>();
        auto seriesidProp = child->property("Series ID");
        if (seriesidProp == nullptr)
          throw(FormatError("QTM - Corrupted file. Series ID associated with analog data '" + label + "' not found"));
        if (child->Extras.empty())
          throw(FormatError("QTM - Corrupted file. Number of samples for analog data '" + label + "' not found"));
        uint32_t numSamples = child->Extras[0].cast<uint32_t>();
        struct DataSeriesIndex
        {
          uint32_t offsetIn;
          uint32_t offsetOut;
          uint32_t sizeIn;
          uint32_t sizeOut;
        };
        std::vector<DataSeriesIndex> buffersInfo;
        // Data series index: contains information of compressed buffers
        if (doc.setBufferToEntry(&buffer, "Measurement NBC/Data series/" + seriesidProp->Value.cast<std::string>() + "Index") != 0)
        {
          buffersInfo.resize(stream.readU32());
          for (size_t i = 0 ; i < buffersInfo.size() ; ++i)
          {
            auto& info = buffersInfo[i];
            info.offsetIn = stream.readU32();
            info.offsetOut = stream.readU32();
            info.sizeIn = stream.readU32();
            info.sizeOut = stream.readU32();
          }
        }
        else
          throw(FormatError("QTM - Corrupted file. Data series index associated with analog data '" + label + "' not found"));
        // Data series: contains compressed buffer(s)
        if (doc.setBufferToEntry(&buffer, "Measurement NBC/Data series/" + seriesidProp->Value.cast<std::string>()) != 0)
        {
          uint32_t offsetIn = 0, readIn = 0, offsetOut = 0;
          for (size_t i = 0 ; i < buffersInfo.size() ; ++i)
          {
            auto& info = buffersInfo[i];
            offsetIn += info.offsetIn;
            buffer.seek(offsetIn-readIn, IODevice::Origin::Current);
            encoded.resize(info.sizeIn);
            decoded.resize(info.sizeOut);
            int remOutLen = static_cast<int>(decoded.size());
            int remInLen = static_cast<int>(encoded.size());
            stream.readChar(encoded.size(), encoded.data());
            btk::lzo1x_decode(decoded.data(), &remOutLen, encoded.data(), &remInLen);
            if ((remInLen != 0) || (remOutLen != 0))
              throw(FormatError("QTM - Error during data decompression for analog data '" + label + "'"));
            readIn += info.sizeIn;
            offsetOut += info.offsetOut + info.sizeOut;
          }
            
          // Need to set back to the buffer associated with the original device
          stream.setDevice(&buffer);

          // std::cout << label << ": " << encodedSize  << ", "<< child->property("Time Base ID")->Value.cast<std::string>() << std::endl;
        }
        else
          throw(FormatError("QTM - Corrupted file. Data series associated with analog data '" + label + "' not found"));
        ++inc;
      }
    }
  };
  
  std::vector<const char*> QTMHandler::supportedTrialFormats() const _BTK_NOEXCEPT
  {
    return {"QTM"};
  };
  
  QTMHandler::Signature QTMHandler::validateSignature() const _BTK_NOEXCEPT
  {
    auto optr = this->pimpl();
    char signature[8] = {0};
    optr->Device->peek(signature,sizeof(signature));
    for (size_t i = 0 ; i < sizeof(signature) ; ++i)
    {
      if (static_cast<uint8_t>(signature[i]) != _btk_qtm_olecf_magic[i])
        return Signature::Invalid;
    }
    return Signature::Valid;
  };
};
