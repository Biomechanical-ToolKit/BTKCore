/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009-2012, Arnaud Barré
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

#include "btkCodamotionFileIOUtils_p.h"

namespace btk
{
  void FillAcquisitionFromOpen3DMotion_p(Acquisition::Pointer output, const std::string& filename, 
                                         Open3DMotion::MotionFileFormat* ff, const Open3DMotion::MotionFileHandler& handler, 
                                         std::ifstream& ifs, Open3DMotion::BinMemFactory& memfactory, const Open3DMotion::TreeValue* readoptions)
  {
    ifs.clear();
    ifs.seekg(0, std::ios::beg);
    std::auto_ptr<Open3DMotion::TreeValue> trialcontents = std::auto_ptr<Open3DMotion::TreeValue>(ff->Read(handler, ifs, memfactory, readoptions));
    // Build trial
    std::auto_ptr<Open3DMotion::Trial> trial(new Open3DMotion::Trial);
    trial->FromTree(trialcontents.get());
    // Retrieve sequences (analog & marker)
    std::vector<const Open3DMotion::TimeSequence*> o3dm_markers;
    std::vector<const Open3DMotion::TimeSequence*> o3dm_analogs;
    trial->Acq.GetTSGroup(o3dm_markers, Open3DMotion::TrialSectionAcq::TSGroupMarker);
    trial->Acq.GetTSGroup(o3dm_analogs, Open3DMotion::TrialSectionAcq::TSGroupAnalog);
    
    // As Open3DMotion gives the possibility for each marker to have its own sample rate,
    // but not within BTK, it is needed to check that all markers were recorded at the same
    // frequency. The number of frames can be different too. In this second case, the maximum
    // number of frames is taken and the extra frame for some markers will be set as invalid
    // NOTE: From the code of Open3DMotion, it seems that unit for markers is the millimeter.
    int numPointFrames = (o3dm_markers.size() == 0) ? 0 : o3dm_markers[0]->NumFrames();
    double pointFrequency = (o3dm_markers.size() == 0) ? 0.0 : o3dm_markers[0]->Rate;
    int firstFrame = (o3dm_markers.size() == 0) ? 1 : (static_cast<int>(o3dm_markers[0]->Start * o3dm_markers[0]->Rate) + 1);
    double pointStart = (o3dm_markers.size() == 0) ? 0.0 : o3dm_markers[0]->Start;
    bool mixedPointNumFrames = false;
    for (size_t i = 0 ; i < o3dm_markers.size() ; ++i)
    {
      if (fabs(o3dm_markers[i]->Rate - pointFrequency) > 1e-6)
        throw(CodamotionFileIOException("The sample rate of at least one marker is not the same than the other markers."));
      if ((static_cast<int>(o3dm_markers[i]->Start * o3dm_markers[i]->Rate) + 1) != firstFrame)
        throw(CodamotionFileIOException("The first frame of at least one marker is not the same than the other markers."));
      if (numPointFrames != o3dm_markers[i]->NumFrames())
      {
        numPointFrames = numPointFrames > o3dm_markers[i]->NumFrames() ? numPointFrames : o3dm_markers[i]->NumFrames();
        mixedPointNumFrames = true;
      }
    }
    if (mixedPointNumFrames)
    {
      btkIOErrorMacro(filename, "The number of frames is not the same for every markers. Extra frames will be added to some markers.")
    }
    // For analog channels, this is the same. The number of frames must be a multiple of the
    // number of video frames (i.e. the analogs' frequency must be a multiple of the markers' frequency)
    // If it is not the case, then the data are interpolated.
    double analogStart = (o3dm_analogs.size() == 0) ? pointStart : o3dm_analogs[0]->Start;
    if (fabs(pointStart - analogStart) > 1e-6)
      throw(CodamotionFileIOException("Analog data are not synchronized with marker data."));
    double commonAnalogFrequency = 0.0;
    bool mixedAnalogSampleFrequencies = false;
    // NOTE: The following code is inspired by Open3DMotion to be able to write a C3D file
    std::vector<int> analogs_subsample(o3dm_analogs.size());
    if (!o3dm_analogs.empty())
    {
      // Rates across all analog channels
      std::vector<double> all_rates(o3dm_analogs.size() + 1);
      for (size_t i = 0; i < o3dm_analogs.size(); i++)
        all_rates[i] = o3dm_analogs[i]->Rate;
      // Include marker rate (or zero if not present)
      all_rates.back() = pointFrequency;
      // Choose lowest common multiple across all analog rates, and common marker rate
      commonAnalogFrequency = floor(all_rates[0] - 1.0); // Initial guess
      bool found = false;
      while (!found)
      {
        commonAnalogFrequency++;
        found = true;
        for (std::vector<double>::const_iterator it = all_rates.begin(); it != all_rates.end(); ++it)
        {
          if (fmod(commonAnalogFrequency,*it) > 1e-6)
          {
            found = false;
            break;
          }
        }
      }
      // subsamples required to interpolate to new rate
      for (size_t i = 0 ; i < o3dm_analogs.size() ; ++i)
      {
        analogs_subsample[i] = static_cast<int>(commonAnalogFrequency / all_rates[i]);
        if (analogs_subsample[i] != 1)
          mixedAnalogSampleFrequencies = true;
      }
    }
    if (mixedAnalogSampleFrequencies)
    {
      btkIOErrorMacro(filename, "Some analog channels have a sample frequency which is not multiple of the markers' sample frequency. Their data will be interpolated in consequence.")
    }
    // Finaly, it is needed to do some check between markers and analogs data
    if (pointFrequency == 0.0)
      pointFrequency = commonAnalogFrequency;
    else if (commonAnalogFrequency == 0.0)
      commonAnalogFrequency = pointFrequency;
    if ((pointFrequency == 0.0) && (commonAnalogFrequency == 0.0))
      throw(CodamotionFileIOException("The case where there is not frame for marker and analog is not handled by BTK."));
    int numAnalogSamplesPerFrame = static_cast<int>(commonAnalogFrequency / pointFrequency);
    int numAnalogFrames = numPointFrames * numAnalogSamplesPerFrame;
    
    // Store the markers data in the BTK Acquisition object
    PointCollection::Pointer points = output->GetPoints();
    for (size_t i = 0 ; i < o3dm_markers.size() ; ++i)
    {
      btk::Point::Pointer pt = btk::Point::New(o3dm_markers[i]->Channel, o3dm_markers[i]->NumFrames());
      // Get sequence & iterator (may throw exception if missing fields)
      const Open3DMotion::TimeSequence* ts = o3dm_markers[i];
      Open3DMotion::TSOccVector3ConstIter iter_ts(*ts);
      for (int j = 0 ; j < o3dm_markers[i]->NumFrames() ; ++j, iter_ts.Next())
        pt->SetDataSlice(j, iter_ts.Value()[0], iter_ts.Value()[1], iter_ts.Value()[2], iter_ts.Occluded() ? -1.0 : 0.0);
      for (int j = o3dm_markers[i]->NumFrames() ; j < numPointFrames ; ++j)
        pt->SetDataSlice(j, 0.0, 0.0, 0.0, -1.0); // Invalid
      points->InsertItem(pt);
    }

    // Store the analogs data in the BTK Acquisition object
    AnalogCollection::Pointer analogs = output->GetAnalogs();
    for (size_t i = 0 ; i < o3dm_analogs.size() ; ++i)
    {
      int numFrames = numAnalogFrames < o3dm_analogs[i]->NumFrames() ? numAnalogFrames : o3dm_analogs[i]->NumFrames();
      btk::Analog::Pointer an = btk::Analog::New(o3dm_analogs[i]->Channel, numAnalogFrames);
      double offset = o3dm_analogs[i]->Offset.Value();
      double scale = o3dm_analogs[i]->Scale.Value();
      an->SetOffset(static_cast<int>(offset));
      an->SetScale(scale);
      an->SetUnit(o3dm_analogs[i]->Units.Value());
      // Get sequence & iterator (may throw exception if missing fields)
      const Open3DMotion::TimeSequence* ts = o3dm_analogs[i];
      Open3DMotion::TSScalarConstIter iter_ts(*ts);
      /// Extract data (without or with the need of interpolation)
      if (analogs_subsample[i] == 1)
      {
         for (int j = 0 ; j < numFrames ; ++j, iter_ts.Next())
           an->SetDataSlice(j, (iter_ts.Value() - offset) * scale);
      }
      else if (numFrames > 0)
      {
        double val0 = (iter_ts.Value() - offset) * scale; an->SetDataSlice(0, val0); iter_ts.Next();
        for (int j = 1 ; j < numFrames ; ++j, iter_ts.Next())
        {
          double val1 = (iter_ts.Value() - offset) * scale;
          an->SetDataSlice(j * analogs_subsample[i], val1);
          // Linear interpolation
          for (int k = 1 ; k < analogs_subsample[i] ; ++k)
          {
            int frame = (j-1) * analogs_subsample[i] + k;
            double lambda = static_cast<double>(frame % analogs_subsample[i]) / static_cast<double>(analogs_subsample[i]);
            an->SetDataSlice(frame, (1.0-lambda) * val0 + lambda * val1);
          }
          val0 = val1;
        }
      }
      analogs->InsertItem(an);
    }
    
    // Set the configuration of the force platforms
    std::list<const Open3DMotion::ForcePlate*> forcePlates;
    int16_t numPlatforms = 0;
    int numChannelPerPlatform = 0;
    bool hasCalibrationMatrix = false;
    // - Determine some internal format for the force platforms
    std::vector<int16_t> typeData;
    for (size_t i = 0 ; i < trial->Acq.ForcePlates.NumElements(); ++i)
    {
      const Open3DMotion::ForcePlate* fpm = &(trial->Acq.ForcePlates[i]);
      // AMTI
      if (fpm->Type.Value().compare(Open3DMotion::ForcePlate::TypeAMTI) == 0)
      {
        ++numPlatforms;
        forcePlates.push_back(fpm);
        numChannelPerPlatform = std::max(numChannelPerPlatform, 6);
        if (fpm->Calibration.NumElements() == 36)
        {
          typeData.push_back(4);
          hasCalibrationMatrix |= true;
        }
        else
          typeData.push_back(2);
      }
      // Kistler
      else if (fpm->Type.Value().compare(Open3DMotion::ForcePlate::TypeKistler) == 0)
      {
        ++numPlatforms;
        forcePlates.push_back(fpm);
        numChannelPerPlatform =  std::max(numChannelPerPlatform, 8);
        typeData.push_back(3);
      }
      // Unknown
      else
      {
        btkIOErrorMacro(filename, "Unknown force platform! Please contact the developers of BTK to update this reader.");
        continue;
      }
    }
    // - Create all the metadata required in BTK to configure force platforms
    std::vector<float> cornersData(3*4*numPlatforms,0.0f);
    std::vector<float> originData(3*numPlatforms,0.0f);
    std::vector<int16_t> channelData(numChannelPerPlatform * numPlatforms, (int16_t)0);
    std::vector<float> calmatrixData(hasCalibrationMatrix ? numChannelPerPlatform * numChannelPerPlatform * numPlatforms : 0, 0.0f);
    int inc = 0;
    for (std::list<const Open3DMotion::ForcePlate*>::const_iterator it = forcePlates.begin() ; it != forcePlates.end() ; ++it)
    {
      bool kfp = (typeData[inc] == 3);  // Is it a Kistler forceplate?
      std::string suffix = (typeData.size() > 1) ? (kfp ? "_" : "") + ToString(inc+1) : ""; 
      // Corners
      for (int i = 0 ; i < 4 ; ++i)
      {
        cornersData[12*inc+3*((i+2)%4)+0] = static_cast<float>((*it)->Outline[i].X.Value());
        cornersData[12*inc+3*((i+2)%4)+1] = static_cast<float>((*it)->Outline[i].Y.Value());
        cornersData[12*inc+3*((i+2)%4)+2] = static_cast<float>((*it)->Outline[i].Z.Value());
      }
      // Origin
      if (kfp) // Kistler
      {
        originData[3*inc+0] = static_cast<float>((*it)->SensorSeparation.Y.Value());
        originData[3*inc+1] = static_cast<float>((*it)->SensorSeparation.X.Value());
        originData[3*inc+2] = static_cast<float>(-(*it)->SensorSeparation.Z.Value());
      }
      else // AMTI
      {
        originData[3*inc+0] = static_cast<float>(-(*it)->CentreOffset.X.Value());
        originData[3*inc+1] = static_cast<float>(-(*it)->CentreOffset.Y.Value());
        originData[3*inc+2] = static_cast<float>(-(*it)->CentreOffset.Z.Value());
      }
      // Channels
      for (int i = 0; i < static_cast<int>((*it)->Channels.NumElements()); ++i)
      {
        // NOTE: The following code is inspired by Open3DMotion to be able to write a C3D file
        // Probably safe to use hardware ID here (assuming analog channels start at 1 and are sequential) but do full remapping just in case
        int hardwareID = (*it)->Channels[i];
        for (int analogindex_zerobased = 0 ; analogindex_zerobased < static_cast<int>(o3dm_analogs.size()) ; ++analogindex_zerobased)
        {
          if (o3dm_analogs[analogindex_zerobased]->HardwareID.IsSet() && o3dm_analogs[analogindex_zerobased]->HardwareID == hardwareID)
          {
            channelData[inc*numChannelPerPlatform + i] = static_cast<int16_t>(analogindex_zerobased + 1);
            Analog::Pointer ch = output->GetAnalog(analogindex_zerobased);
            // Open3DMotion stores platform' forces and not their reactions.
            ch->GetValues() *= -1.0;
            // By default Open3DMotion set force's label to Force1, Force2, etc.
            // They are rewritten to be compatible with other file formats available in BTK.
            switch (i)
            {
            case 0:
              ch->SetLabel((kfp ? "FX12" : "FX") + suffix);
              ch->SetUnit("N");
              break;
            case 1:
              ch->SetLabel((kfp ? "FX34" : "FY") + suffix);
              ch->SetUnit("N");
              break;
            case 2:
              ch->SetLabel((kfp ? "FY14" : "FZ") + suffix);
              ch->SetUnit("N");
              break;
            case 3:
              ch->SetLabel((kfp ? "FY23" : "MX") + suffix);
              ch->SetUnit(kfp ? "N" : "N" + output->GetPointUnit());
              break;
            case 4:
              ch->SetLabel((kfp ? "FZ1" : "MY") + suffix);
              ch->SetUnit(kfp ? "N" : "N" + output->GetPointUnit());
              break;
            case 5:
              ch->SetLabel((kfp ? "FZ2" : "MZ") + suffix);
              ch->SetUnit(kfp ? "N" : "N" + output->GetPointUnit());
              break;
            case 6:
              ch->SetLabel("FZ3" + suffix);
              ch->SetUnit("N");
              break;
            case 7:
              ch->SetLabel("FZ4" + suffix);
              ch->SetUnit("N");
              break;
            }
            break;
          }
        }
      }
      // Calibration matrix (Only AMTI type 4)
      if (hasCalibrationMatrix && (typeData[inc] == 4))
      {
        for (size_t row = 0; row < 6; ++row)
          for (size_t col = 0; col < 6; ++col)
            calmatrixData[inc*numChannelPerPlatform*numChannelPerPlatform + numChannelPerPlatform*row + col] = static_cast<float>((*it)->Calibration[6*col + row]);
      }
      ++inc;
    }
    std::vector<uint8_t> cornersDims = std::vector<uint8_t>(3, 3); cornersDims[1] = 4; cornersDims[2] = static_cast<uint8_t>(numPlatforms);
    std::vector<uint8_t> originDims = std::vector<uint8_t>(2, 3); originDims[1] = static_cast<uint8_t>(numPlatforms);
    std::vector<uint8_t> channelDims = std::vector<uint8_t>(2, numChannelPerPlatform); channelDims[1] = static_cast<uint8_t>(numPlatforms);
    MetaData::Pointer forcePlatform = MetaData::New("FORCE_PLATFORM");
    forcePlatform->AppendChild(MetaData::New("USED", numPlatforms));
    forcePlatform->AppendChild(MetaData::New("TYPE", typeData));
    forcePlatform->AppendChild(MetaData::New("ZERO", std::vector<int16_t>(2,0)));
    forcePlatform->AppendChild(MetaData::New("CORNERS", cornersDims, cornersData));
    forcePlatform->AppendChild(MetaData::New("ORIGIN", originDims, originData));
    forcePlatform->AppendChild(MetaData::New("CHANNEL", channelDims, channelData));
    if (hasCalibrationMatrix)
    {
      std::vector<uint8_t> calmatrixDims = std::vector<uint8_t>(3, static_cast<int8_t>(numChannelPerPlatform)); calmatrixDims[2] = static_cast<uint8_t>(numPlatforms);
      forcePlatform->AppendChild(MetaData::New("CAL_MATRIX", calmatrixDims, calmatrixData));
    }
    output->GetMetaData()->AppendChild(forcePlatform);
    
    // Finalize the acquisition
    output->Resize(points->GetItemNumber(), numPointFrames, analogs->GetItemNumber(), numAnalogSamplesPerFrame);
    output->SetPointFrequency(pointFrequency);
    output->SetFirstFrame(firstFrame);
  };
};
