function h = btkWriteAcquisition(filename) %#ok
%BTKWRITEACQUISITION Write acquisition's data into file (C3D, TRC, ...)
% 
%  BTKWRITEACQUISITION(H, FILENAME, ...) exports acquisition's data represented 
%  as the handle H into the file FILENAME.  The choice of the file format is
%  based on the file extension in FILENAME (for example, '.trc' or '.c3d').
%  This handle H must be obtained by the use of a btk* function.  
%  
%  Some formats (for example C3D) have options to store an acquisition. In a 
%  binary format, it is possible to specify the byte order (see BYTEORDER 
%  option) and the storage format (see STORAGEFORMAT option). The options are 
%  given in BTKWRITEACQUISITION after the filename by first the name of the
%  option and its value. 
%
%  The BYTEORDER option can receive 3 values:
%   - IEEE_LittleEndian: write integer and float in x86 format (PC).
%   - IEEE_BigEndian: write integer and float in PPC/MIPS format (SGI).
%   - VAX_LittleEndian: write integer and float in VAX format (DEC).
%  The STORAGEFORMAT option can receive 2 values:
%   - Integer: Acquisition data are scaled and stored as 16 bits integer.
%   - Float: Acquisition data are stored as floating values.
%  The REGENERATEINTERNALS is an advanced option which can receive 2 values.
%   - DataBased: Internal file format configuration (if any) is regenerated based on
%     the acquisition data (i.e. points and analog channels plus events). This
%     option is the default one.
%   - MetadataBased: Internal file format configuration (if any) is regenerated based
%     on the metadata. WARNING: Use this value carefully as it could be easy to
%     corrupt file format internals with it.
%
%  Without any option, a C3D File is written using the machine byte order
%  and the float as storage format. Using these options on ASCII formats 
%  (for example TRC) have no effect on the file data.
%  The writing of an empty acquisition (no point and no analog channel) in
%  a C3D file will create a template C3D file. Only the metadata will be saved.
%
%  EXAMPLES:
%   - BTKWRITEACQUISITION(acq, 'filename.c3d', 'StorageFormat', 'Float'); 
%   - BTKWRITEACQUISITION(acq, 'filename.c3d', 'ByteOrder', 'VAX_LittleEndian'); 
%   - BTKWRITEACQUISITION(acq, 'filename.c3d', 'StorageFormat', 'Integer', 'ByteOrder', 'IEEE_LittleEndian'); 

%  Author: A. Barré
%  Copyright 2009-2013 Biomechanical ToolKit (BTK).

% The following comment, MATLAB compiler pragma, is necessary to avoid 
% compiling this M-file instead of linking against the MEX-file.  Don't remove.
%# mex

error(generatemsgid('NotSupported'),'MEX file for BTKWRITEACQUISITION not found');

% [EOF] btkWriteAcquisition.m
