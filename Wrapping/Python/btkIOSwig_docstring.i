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
 
// ------------------------------------------------------------------------- //
//                             AcquisitionFileIO                             //
// ------------------------------------------------------------------------- //

%feature("docstring") btkAcquisitionFileIO "
Interface to read/write acquisition's files, cannot be instanced."

BTK_SWIG_DOCSTRING_IMPL(AcquisitionFileIO, GetFileType, "Gets the type of the file.");
BTK_SWIG_DOCSTRING_IMPL(AcquisitionFileIO, GetByteOrder, "Gets the byte order of the file (only for binary file).");
BTK_SWIG_DOCSTRING_IMPL(AcquisitionFileIO, GetByteOrderAsString, "Gets the byte order as a string.");
BTK_SWIG_DOCSTRING_IMPL(AcquisitionFileIO, SetByteOrder, "Sets the byte order of the file.");
BTK_SWIG_DOCSTRING_IMPL(AcquisitionFileIO, GetStorageFormat, "Return the format used to store points and analog channels.");
BTK_SWIG_DOCSTRING_IMPL(AcquisitionFileIO, GetStorageFormatAsString, "Gets the storage format as a string.");
BTK_SWIG_DOCSTRING_IMPL(AcquisitionFileIO, SetStorageFormat, "Set the format used to store points and analog channels.");
BTK_SWIG_DOCSTRING_IMPL(AcquisitionFileIO, CanReadFile, "Checks if the given filename can be read by this AcquisitionFileIO. This methods should try to read the file header instead to check the file's suffix.");
BTK_SWIG_DOCSTRING_IMPL(AcquisitionFileIO, CanWriteFile, "Checks if the given filename can be write by this AcquisitionFileIO. \nThis method should try to determine if the file's suffix corresponds to one of the supported file's suffixes by this btkAcquisitionFileIO.");
BTK_SWIG_DOCSTRING_IMPL(AcquisitionFileIO, Read, "Read the file designated by the given filename and fill the given btkAcquisition");
BTK_SWIG_DOCSTRING_IMPL(AcquisitionFileIO, Write, "Write the file designated by the given filename with the content of the given btkAcquisition.");
BTK_SWIG_DOCSTRING_IMPL(AcquisitionFileIO, GetInternalsUpdateOptions, "Returns the option(s) used to update internals.");
BTK_SWIG_DOCSTRING_IMPL(AcquisitionFileIO, SetInternalsUpdateOptions, "Sets the option(s) used to update internals.");
BTK_SWIG_DOCSTRING_IMPL(AcquisitionFileIO, HasInternalsUpdateOption, "Returns true if the given @a option is used or false if not.");

// ------------------------------------------------------------------------- //
//                           AcquisitionFileReader                           //
// ------------------------------------------------------------------------- //

%feature("docstring") btkAcquisitionFileReader "
Reader for files which contain acquisition data (C3D, TRC, ...).

This class uses a btkAcquisitionFileIO object to read the data from the file corresponding to the given filename.
You have two ways to set this object.
First, you can set it manually by using the method btkAcquisitionFileReader::SetAcquisitionIO(),
Second, the correct object can be detected automatically, by checking in each registered btkAcquisitionFileIO if it can read or not the file.

The use of the manual setting should be only used if you want to read a file with a selected file format. To go back to the automatic mode 
from the manual mode, you only have to use the method btkAcquisitionFileReader::SetAcquisitionIO() without any argument."

BTK_SWIG_AUTODOC_IMPL(AcquisitionFileReader, SetFilename, "SetFilename(self, string)");
BTK_SWIG_AUTODOC_IMPL(AcquisitionFileReader, SetDisableFilenameExceptionState, "SetDisableFilenameExceptionState(self, bool)");

BTK_SWIG_DOCSTRING(AcquisitionFileReader, GetAcquisitionIO, "Returns the btkAcquisitionIO used to read the given file.");
BTK_SWIG_DOCSTRING_IMPL(AcquisitionFileReader, GetOutput, "Returns the btkAcquisition read from the btkAcquisitionIO helper class.");
BTK_SWIG_DOCSTRING_IMPL(AcquisitionFileReader, GetDisableFilenameExceptionState, "Returns state of the disabling of the exception for missing filename.");
BTK_SWIG_DOCSTRING_IMPL(AcquisitionFileReader, SetDisableFilenameExceptionState, "Enable/disable exception for the missing of the filename.");
BTK_SWIG_DOCSTRING_IMPL(AcquisitionFileReader, GetFilename, "Gets the filename of the file to read.");
BTK_SWIG_DOCSTRING_IMPL(AcquisitionFileReader, SetFilename, "Specifies the file to read. This is forwarded to the IO instance.");
BTK_SWIG_DOCSTRING_IMPL(AcquisitionFileReader, SetAcquisitionIO, "Set the btkAcquisitionIO helper class. This method forces the btkAcquisitionIO to be used to read a particular file.\nThe btkAcquisitionIO is ofently created via the object factory mechanism that determines whether a particular btkAcquisitionIO can read a certain file.");
BTK_SWIG_DOCSTRING_IMPL(AcquisitionFileReader, Update, "Update the reader (i.e. read the given file).");

// ------------------------------------------------------------------------- //
//                           AcquisitionFileWriter                           //
// ------------------------------------------------------------------------- //

%feature("docstring") btkAcquisitionFileWriter "
Writer of acquisition data into acquisition file (C3D, TRC, ...)."

BTK_SWIG_DOCSTRING(AcquisitionFileWriter, GetAcquisitionIO, "Returns the btkAcquisitionIO used to write the given acquisition");
BTK_SWIG_DOCSTRING_IMPL(AcquisitionFileWriter, GetInput, "Returns the btkAcquisition to write by a btkAcquisitionIO helper class");
BTK_SWIG_DOCSTRING_IMPL(AcquisitionFileWriter, SetInput, "Sets the btkAcquisition to write by a btkAcquisitionIO helper class");
BTK_SWIG_DOCSTRING_IMPL(AcquisitionFileWriter, GetFilename, "Gets the filename of the file to write.");
BTK_SWIG_DOCSTRING_IMPL(AcquisitionFileWriter, SetFilename, "Specifies the file to write. This is forwarded to the IO instance.");
BTK_SWIG_DOCSTRING_IMPL(AcquisitionFileWriter, SetAcquisitionIO, "Set the btkAcquisitionIO helper class. This method forces the btkAcquisitionIO to be used to write a particular file.\nThe btkAcquisitionIO is ofently created via the object factory mechanism that determines whether a particular btkAcquisitionIO can write a certain file");
BTK_SWIG_DOCSTRING_IMPL(AcquisitionFileWriter, Update, "Update the writer (i.e. write the given acquisition).");

// ------------------------------------------------------------------------- //
//                                  C3DFileIO                                //
// ------------------------------------------------------------------------- //

%feature("docstring") btkC3DFileIO "
Interface to read/write C3D files.

All the informations related to the acquisition are stored in the output object.
Few data, like the byte order, the points' scale, the analog universal scale, the integer analog storage format are 
available as member of this class. These informations are extracted when a C3D file is read or can be 
filled/modified to write a new C3D file. All these members can be accessed or modified using dedicated methods.

Compared to the C3DServer API, all the data in BTK extracted from a C3D file are already scaled and available in the 
children of the output Acquisition. You don't need to access to the groups/parameters for that. However, if you have 
some custom parameters, then you can access them from the metadata stored in the output Acquisition using the method 
btkAcquisition::GetMetadata().

Moreover, there are several options to create a C3D file from an acquisition:
 - DataBasedUpdate;
 - MetaDataBasedUpdate;
 - CompatibleVicon.
 
By default, the writer is set with the options ScalesFromDataUpdate, MetaDataFromDataUpdate and CompatibleVicon.
These options give you the possibility to create a C3D file from any kind of acquisition (created from raw or extracted from another file format).

To write a C3D file with a given processor architecture (called byte order in BTK), you have to use the method btkC3DFileIO::SetByteOrder().

For more informations on this file's format: http:://www.c3d.org"

BTK_SWIG_DOCSTRING_IMPL(C3DFileIO, GetPointScale, "Returns the scale for the points obtains from the POINT:SCALE parameter or from its determination for a created acquisition.");
BTK_SWIG_DOCSTRING_IMPL(C3DFileIO, SetPointScale, "Sets the scale for the points.");
BTK_SWIG_DOCSTRING_IMPL(C3DFileIO, GetAnalogIntegerFormat, "Return the integer format (signed/unsigned).");
BTK_SWIG_DOCSTRING_IMPL(C3DFileIO, SetAnalogIntegerFormat, "Sets the integer format used to store analog channels data.");
BTK_SWIG_DOCSTRING_IMPL(C3DFileIO, GetAnalogChannelScale, "Returns the vector of scales used for the analog channels' scaling (parameter ANALOG:SCALE).");
BTK_SWIG_DOCSTRING_IMPL(C3DFileIO, SetAnalogChannelScale, "Sets the vector of scales used for the analog channels' scaling.");
BTK_SWIG_DOCSTRING_IMPL(C3DFileIO, GetAnalogZeroOffset, "Returns the vector of offsets used for the analog channels' scaling (parameter ANALOG:OFFSET).");
BTK_SWIG_DOCSTRING_IMPL(C3DFileIO, SetAnalogZeroOffset, "Sets the vector of offsets used for the analog channels' scaling.");
BTK_SWIG_DOCSTRING_IMPL(C3DFileIO, GetAnalogUniversalScale, "Returns the universal scale factor used to scale analog channels (parameter ANALOG:GEN_SCALE).");
BTK_SWIG_DOCSTRING_IMPL(C3DFileIO, SetAnalogUniversalScale, "Sets Returns the universal scale factor used to scale analog channels.");

// ------------------------------------------------------------------------- //
//                                  ANBFileIO                                //
// ------------------------------------------------------------------------- //

%feature("docstring") btkANBFileIO "
Interface to read/write ANB files (Motion Analysis Corp.)."

// ------------------------------------------------------------------------- //
//                                  ANCFileIO                                //
// ------------------------------------------------------------------------- //

%feature("docstring") btkANCFileIO "
Interface to read/write ANC files (Motion Analysis Corp.)."

BTK_SWIG_DOCSTRING_IMPL(ANCFileIO, GetFileGeneration, "Returns the generation of the ANC file.");
BTK_SWIG_DOCSTRING_IMPL(ANCFileIO, SetFileGeneration, "Set the generation of the ANC file.");

// ------------------------------------------------------------------------- //
//                                  ANGFileIO                                //
// ------------------------------------------------------------------------- //

%feature("docstring") btkANGFileIO"
Interface to read ANG files (Elite - BTS Bioengineering)."

// ------------------------------------------------------------------------- //
//                             CALForcePlateFileIO                           //
// ------------------------------------------------------------------------- //

%feature("docstring") btkCALForcePlateFileIO"
Interface to read/write force platform calibration CAL files (Motion Analysis Corp.)."

// ------------------------------------------------------------------------- //
//                                  CLBFileIO                                //
// ------------------------------------------------------------------------- //

%feature("docstring") btkCLBFileIO"
Interface to read CLB files (Contec Inc.)."

// ------------------------------------------------------------------------- //
//                               DelsysEMGFileIO                             //
// ------------------------------------------------------------------------- //
%feature("docstring") btkDelsysEMGFileIO"
Interface to read EMG file (Delsys Inc.)."

// ------------------------------------------------------------------------- //
//                                  EMFFileIO                                //
// ------------------------------------------------------------------------- //

%feature("docstring") btkEMFFileIO"
Interface to read EMF files (Ascension Technology Corporation)."

// ------------------------------------------------------------------------- //
//                                  EMxFileIO                                //
// ------------------------------------------------------------------------- //

%feature("docstring") btkEMxFileIO"
Interface to read EMG files (Elite - BTS Bioengineering)."

// ------------------------------------------------------------------------- //
//                                  GRxFileIO                                //
// ------------------------------------------------------------------------- //

%feature("docstring") btkGRxFileIO"
Interface to read GR* (GR1, GR2, ...) files (Elite - BTS Bioengineering)."

// ------------------------------------------------------------------------- //
//                                  MDFFileIO                                //
// ------------------------------------------------------------------------- //

%feature("docstring") btkMDFFileIO"
Interface to read MDF files (Codamotion)."

// ------------------------------------------------------------------------- //
//                                  MOMFileIO                                //
// ------------------------------------------------------------------------- //

%feature("docstring") btkMOMFileIO"
Interface to read MOM files (Elite - BTS Bioengineering)."

// ------------------------------------------------------------------------- //
//                                  PWRFileIO                                //
// ------------------------------------------------------------------------- //

%feature("docstring") btkPWRFileIO"
Interface to read PWR files (Elite - BTS Bioengineering)."

// ------------------------------------------------------------------------- //
//                                  RAxFileIO                                //
// ------------------------------------------------------------------------- //

%feature("docstring") btkRAxFileIO"
Interface to read RAW, RAH files (Elite - BTS Bioengineering)."

// ------------------------------------------------------------------------- //
//                                  RICFileIO                                //
// ------------------------------------------------------------------------- //

%feature("docstring") btkRICFileIO"
Interface to read RIC files (Elite - BTS Bioengineering)."

// ------------------------------------------------------------------------- //
//                                  TRBFileIO                                //
// ------------------------------------------------------------------------- //

%feature("docstring") btkTRBFileIO"
Interface to read/write TRB files (Motion Analysis Corp.)."

// ------------------------------------------------------------------------- //
//                                  TRCFileIO                                //
// ------------------------------------------------------------------------- //

%feature("docstring") btkTRCFileIO"
Interface to read/write TRC files (Motion Analysis Corp.)."

// ------------------------------------------------------------------------- //
//                                  TDFFileIO                                //
// ------------------------------------------------------------------------- //

%feature("docstring") btkTDFFileIO"
Interface to read TDF files (BTS Bioengineering)."

// ------------------------------------------------------------------------- //
//                             XLSOrthoTrakFileIO                            //
// ------------------------------------------------------------------------- //

%feature("docstring") btkXLSOrthoTrakFileIO"
Interface to read XLS OrthoTrak files (Motion Analysis Corp.)."

// ------------------------------------------------------------------------- //
//                                 XMOVEFileIO                               //
// ------------------------------------------------------------------------- //

%feature("docstring") btkXMOVEFileIO"
Interface to read XMOVE files (Codamotion)."