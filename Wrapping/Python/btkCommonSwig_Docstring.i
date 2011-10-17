/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009-2011, Arnaud Barré
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 
 *   * Redistributions of source code must retain the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials
 *     provided with the distribution.
 *   * Neither the name(s) of the copyright holders nor the names
 *     of its contributors may be used to endorse or promote products
 *     derived from this software without specific prior written
 *     permission.
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
//                                    Event                                  //
// ------------------------------------------------------------------------- //

%feature("docstring") btkEvent "
An event is defined as:
 - a label (string);
 - a time (double);
 - a frame (integer);
 - a context (string);
 - a detection flag (integer);
 - a description (string);
 - an ID (integer).
 
The detection flag has a limited range of possible values and are listed in the following enumeration:
 - btk.btkEvent.Unknown: Unknown method used to detect the event;
 - btk.btkEvent.Manual: Event detected manually;
 - btk.btkEvent.Automatic: Automatic method used to detect the event;
 - btk.btkEvent.FromForcePlatform: Method based on force platform data to detect the event.

Note: Even if there is no rule for the id value, these 3 next values are used in Vicon Workstation and Nexus to distinguish events' type:
 - The value 0 is for general event;
 - Value 1 corresponds to the foot strike event;
 - Value 2 for the foot off. Without the use of these values.
It is known that Workstation and Nexus put all events in general (default value is 0). 
For Polygon, one user reported that some points had their values set to 0.

For example, a Heel Strike event on the left leg or the subject John Doe at frame 143 can be set by
evt = btk.btkEvent('FootStrike', 143, 'Right', btk.btkEvent.Manual, 'John Doe', 'Heel strike event', 2)";
 
BTK_SWIG_AUTODOC_IMPL(Event, SetLabel, "SetLabel(self, string)");
BTK_SWIG_AUTODOC_IMPL(Event, SetDescription, "SetDescription(self, string)");
BTK_SWIG_AUTODOC_IMPL(Event, SetContext, "SetContext(self, string)");
BTK_SWIG_AUTODOC_IMPL(Event, SetSubject, "SetSubject(self, string)");
BTK_SWIG_AUTODOC_IMPL(Event, SetTime, "SetTime(self, double)");
BTK_SWIG_AUTODOC_IMPL(Event, SetFrame, "SetFrame(self, int)");
BTK_SWIG_AUTODOC_IMPL(Event, SetDetectionFlags, "SetDetectionFlags(self, int)");
BTK_SWIG_AUTODOC_IMPL(Event, SetId, "SetId(self, int)");

BTK_SWIG_DOCSTRING(Event, Clone, "Deep copy of the object");
BTK_SWIG_DOCSTRING_IMPL(Event, GetLabel, "Returns the event's label");
BTK_SWIG_DOCSTRING_IMPL(Event, SetLabel, "Sets the event's label");
BTK_SWIG_DOCSTRING_IMPL(Event, GetDescription, "Returns the event's description");
BTK_SWIG_DOCSTRING_IMPL(Event, SetDescription, "Sets the event's description");
BTK_SWIG_DOCSTRING_IMPL(Event, GetContext, "Returns the event's context");
BTK_SWIG_DOCSTRING_IMPL(Event, SetContext, "Sets the event's context");
BTK_SWIG_DOCSTRING_IMPL(Event, GetSubject, "Returns the event's subject");
BTK_SWIG_DOCSTRING_IMPL(Event, SetSubject, "Sets the event's subject");
BTK_SWIG_DOCSTRING_IMPL(Event, GetTime, "Returns the event's time");
BTK_SWIG_DOCSTRING_IMPL(Event, SetTime, "Sets the event's time");
BTK_SWIG_DOCSTRING_IMPL(Event, GetFrame, "Returns the event's frame");
BTK_SWIG_DOCSTRING_IMPL(Event, SetFrame, "Sets the event's frame");
BTK_SWIG_DOCSTRING_IMPL(Event, GetDetectionFlags, "Returns the event's detection flags");
BTK_SWIG_DOCSTRING_IMPL(Event, SetDetectionFlags, "Sets the event's detection flags");
BTK_SWIG_DOCSTRING_IMPL(Event, HasDetectionFlag, "Check if the event has the given detection flag");
BTK_SWIG_DOCSTRING_IMPL(Event, GetId, "Returns the event's ID");
BTK_SWIG_DOCSTRING_IMPL(Event, SetId, "Sets the event's ID");

// ------------------------------------------------------------------------- //
//                                  Analog                                   //
// ------------------------------------------------------------------------- //

%feature("docstring") btkAnalog "
An analog channel is defined as:
 - an array of double containing the samples (NumPy array);
 - a unit (string);
 - a scale (double);
 - an offset (integer);
 - a gain (integer).

The gain has a limited range of possible values and are listed in the following enumeration:
 - btk.btkAnalog.Unknown: unknwon gain;
 - btk.btkAnalog.PlusMinus10: gain for the range +/- 10 volts;
 - btk.btkAnalog.PlusMinus5: gain for the range +/- 5 volts;
 - btk.btkAnalog.PlusMinus2Dot5: gain for the range +/- 2.5 volts;
 - btk.btkAnalog.PlusMinus1Dot25: gain for the range +/- 1.25 volts;
 - btk.btkAnalog.PlusMinus1: gain for the range +/- 1 volts;.

Note: The informations associated (unit, scale, offset, gain) are useful for other classes/methods. 
For example, when you save an acquisition in a file, analog's data are generally unscaled and written as integer, 
then the scale and offset are important."

BTK_SWIG_AUTODOC_IMPL(Analog, SetLabel, "SetLabel(self, string)");
BTK_SWIG_AUTODOC_IMPL(Analog, SetDescription, "SetDescription(self, string)");
BTK_SWIG_AUTODOC(Analog, SetValue, "SetValue(self, int, double)");
BTK_SWIG_AUTODOC_IMPL(Analog, GetValues, "GetValues(self) -> array (NumPy)");
BTK_SWIG_AUTODOC_IMPL(Analog, SetValues, "SetValues(self, array)");
BTK_SWIG_AUTODOC_IMPL(Analog, SetFrameNumber, "SetFrameNumber(self, int)");
BTK_SWIG_AUTODOC_IMPL(Analog, SetUnit, "SetUnit(self, string)");
BTK_SWIG_AUTODOC_IMPL(Analog, SetGain, "SetGain(self, int)");

BTK_SWIG_DOCSTRING(Analog, Clone, "Deep copy of the object");
BTK_SWIG_DOCSTRING_IMPL(Analog, GetLabel, "Returns the analog's label");
BTK_SWIG_DOCSTRING_IMPL(Analog, SetLabel, "Sets the analog's label");
BTK_SWIG_DOCSTRING_IMPL(Analog, GetDescription, "Returns the analog's description");
BTK_SWIG_DOCSTRING_IMPL(Analog, SetDescription, "Sets the analog's description");
BTK_SWIG_DOCSTRING(Analog, GetValue, "Returns only one sample");
BTK_SWIG_DOCSTRING(Analog, SetValue, "Sets only one sample");
BTK_SWIG_DOCSTRING_IMPL(Analog, GetValues, "Returns the analog's samples");
BTK_SWIG_DOCSTRING_IMPL(Analog, SetValues, "Sets the analog's samples");
BTK_SWIG_DOCSTRING_IMPL(Analog, GetFrameNumber, "Returns the number of frames");
BTK_SWIG_DOCSTRING_IMPL(Analog, SetFrameNumber, "Sets the number of frames");
BTK_SWIG_DOCSTRING_IMPL(Analog, GetUnit, "Returns the analog's unit");
BTK_SWIG_DOCSTRING_IMPL(Analog, SetUnit, "Sets the analog's unit");
BTK_SWIG_DOCSTRING_IMPL(Analog, GetGain, "Returns the analog's gain");
BTK_SWIG_DOCSTRING_IMPL(Analog, SetGain, "Sets the analog's gain");

// ------------------------------------------------------------------------- //
//                                    Point                                  //
// ------------------------------------------------------------------------- //
%feature("docstring") btkPoint "
The coordinates of the point are generally measured by an acquisition system. For each frame, the
3D value has a residual and mask. They can provide informations on the quality of these 3D data 
(residual associated with the reconstruction of the marker by camera) as well as the part(s) of 
the system which measured the coordinates (the masks is used mainly to know which camera detect 
the markers). 

This class is also used to represent angles, forces, moments, powers and scalars. 
To know/set the type of the point, you can use the methods GetType() and SetType(). 

The type has a limited range of possible values and are listed in the following enumeration:
 - btk.btkPoint.Marker: Set the point object as a marker;
 - btk.btkPoint.Angle: Set the point object as an angle;
 - btk.btkPoint.Force: Set the point object as a force;
 - btk.btkPoint.Moment: Set the point object as a moment;
 - btk.btkPoint.Power: Set the point object as a power;
 - btk.btkPoint.Scalar: Set the point object as a scalar along the time;
 - btk.btkPoint.Reaction: Special type to indicate the point corresponds to one part of a wrench (not really used);

Note: In some case the values at specific frames are invalid (mainly due to marker's occlusion).
To detect if the frame is invalid, you can check the residual as well as the mask which will be 
set to the value -1. It is the same thing if you want to set the frame as invalid. You can set
the values of the frames to 0 and the associated residual and mask to -1.

Note: A residual with a value equal to 0 means that this frame has been post-processed (interpolation, filtering, etc.)."

BTK_SWIG_AUTODOC_IMPL(Point, SetLabel, "SetLabel(self, string)");
BTK_SWIG_AUTODOC_IMPL(Point, SetDescription, "SetDescription(self, string)");
BTK_SWIG_AUTODOC(Point, SetValue, "SetValue(self, int, double)");
BTK_SWIG_AUTODOC_IMPL(Point, GetValues, "GetValues(self) -> array (NumPy)");
BTK_SWIG_AUTODOC_IMPL(Point, SetValues, "SetValues(self, array)");
BTK_SWIG_AUTODOC(Point, SetResidual, "SetResidual(self, int, double)");
BTK_SWIG_AUTODOC_IMPL(Point, GetResiduals, "GetResiduals(self) -> array (NumPy)");
BTK_SWIG_AUTODOC_IMPL(Point, SetResiduals, "SetResiduals(self, array)");
BTK_SWIG_AUTODOC(Point, SetMask, "SetResidual(self, int, double)");
BTK_SWIG_AUTODOC_IMPL(Point, GetMasks, "GetMasks(self) -> array (NumPy)");
BTK_SWIG_AUTODOC_IMPL(Point, SetMasks, "SetResiduals(self, array)");
BTK_SWIG_AUTODOC_IMPL(Point, SetFrameNumber, "SetFrameNumber(self, int)");
BTK_SWIG_AUTODOC_IMPL(Point, SetType, "SetUnit(self, int)");

BTK_SWIG_DOCSTRING(Point, Clone, "Deep copy of the object");
BTK_SWIG_DOCSTRING_IMPL(Point, GetLabel, "Returns the point's label");
BTK_SWIG_DOCSTRING_IMPL(Point, SetLabel, "Sets the point's label");
BTK_SWIG_DOCSTRING_IMPL(Point, GetDescription, "Returns the point's description");
BTK_SWIG_DOCSTRING_IMPL(Point, SetDescription, "Sets the point's description");
BTK_SWIG_DOCSTRING(Point, GetValue, "Returns only one value for the given component and frame");
BTK_SWIG_DOCSTRING(Point, SetValue, "Sets only one value for the given component and frame");
BTK_SWIG_DOCSTRING_IMPL(Point, GetValues, "Returns the point's values");
BTK_SWIG_DOCSTRING_IMPL(Point, SetValues, "Sets the point's values");
BTK_SWIG_DOCSTRING(Point, GetResidual, "Returns only one residual for the given frame");
BTK_SWIG_DOCSTRING(Point, SetResidual, "Sets only one residual for the given frame");
BTK_SWIG_DOCSTRING_IMPL(Point, GetResiduals, "Returns the point's residuals");
BTK_SWIG_DOCSTRING_IMPL(Point, SetResiduals, "Sets the point's residuals");
BTK_SWIG_DOCSTRING(Point, GetMask, "Returns only one mask for the given frame");
BTK_SWIG_DOCSTRING(Point, SetMask, "Sets only one mask for the given frame");
BTK_SWIG_DOCSTRING_IMPL(Point, GetMasks, "Returns the point's masks");
BTK_SWIG_DOCSTRING_IMPL(Point, SetMasks, "Sets the point's masks");
BTK_SWIG_DOCSTRING_IMPL(Point, GetFrameNumber, "Returns the number of frames");
BTK_SWIG_DOCSTRING_IMPL(Point, SetFrameNumber, "Sets the number of frames");
BTK_SWIG_DOCSTRING_IMPL(Point, GetType, "Returns the point's type");
BTK_SWIG_DOCSTRING_IMPL(Point, SetType, "Sets the point's type");

// ------------------------------------------------------------------------- //
//                                 ForcePlatform                             //
// ------------------------------------------------------------------------- //
%feature("docstring") btkForcePlatform "
This class is generic and doesn't know its type, nor the number of analog channels used.
To use predefined force platform as proposed in the documentation of the C3D file format, check the following classes:
 - btk::ForcePlatformType1: 6 channels (FX, FY, FZ, PX, PY, MZ);
 - btk::ForcePlatformType2: 6 channels (FX, FY, FZ, MX, MY, MZ);
 - btk::ForcePlatformType3: 8 channels (FZ1, FZ2, FZ3, FZ4, FX12, FX34, FY14, FY23);
 - btk::ForcePlatformType4: Same as Type-2 + calibration matrix 6 (columns) by 6 (rows);
 - btk::ForcePlatformType5: Same as Type-3 + calibration matrix 6 (columns) by 8 (rows);
 - btk::ForcePlatformType6: 12 channels (FX[1,2,3,4], FY[1,2,3,4], FZ[1,2,3,4] + calibration matrix 12 by 12)."

BTK_SWIG_AUTODOC_IMPL(ForcePlatform, Begin, "Begin(self) -> btkAnalogIterator");
BTK_SWIG_AUTODOC_IMPL(ForcePlatform, End, "End(self) -> btkAnalogIterator");
BTK_SWIG_AUTODOC_IMPL(ForcePlatform, GetChannel, "GetChannel(self, int) -> btkAnalog)");
BTK_SWIG_AUTODOC_IMPL(ForcePlatform, SetChannel, "SetChannel(self, int , btkAnalog)");
BTK_SWIG_AUTODOC_IMPL(ForcePlatform, GetOrigin, "GetOrigin(self) -> array (NumPy)");
BTK_SWIG_AUTODOC_IMPL(ForcePlatform, SetOrigin(double , double , double ), "SetOrigin(self, double , double , double )");
BTK_SWIG_AUTODOC_IMPL(ForcePlatform, SetOrigin(const btk::ForcePlatform::Origin& ), "SetOrigin(self, array)");
BTK_SWIG_AUTODOC_IMPL(ForcePlatform, GetCorner, "GetCorner(self, int) -> array (NumPy)");
BTK_SWIG_AUTODOC_IMPL(ForcePlatform, SetCorner(int, int, double ), "SetCorner(self, int, int, double)");
BTK_SWIG_AUTODOC_IMPL(ForcePlatform, SetCorner(int, double, double, double ), "SetCorner(self, int, double, double, double )");
BTK_SWIG_AUTODOC_IMPL(ForcePlatform, SetCorner(int, const btk::ForcePlatform::Corner& ), "SetCorner(self, int, array)");
BTK_SWIG_AUTODOC_IMPL(ForcePlatform, GetCorners, "GetCorners(self) -> array (NumPy)");
BTK_SWIG_AUTODOC_IMPL(ForcePlatform, SetCorners, "SetCorners(self, array)");
BTK_SWIG_AUTODOC_IMPL(ForcePlatform, GetCalMatrix, "GetCalMatrix(self) -> array (NumPy)");
BTK_SWIG_AUTODOC_IMPL(ForcePlatform, SetCalMatrix, "SetCalMatrix(self, array)");

BTK_SWIG_DOCSTRING(ForcePlatform, Clone, "Deep copy of the object");
BTK_SWIG_DOCSTRING_IMPL(ForcePlatform, Begin, "Returns an iterator to the beginning of the list of items.");
BTK_SWIG_DOCSTRING_IMPL(ForcePlatform, End, "Returns an iterator just past the last item.");
BTK_SWIG_DOCSTRING_IMPL(ForcePlatform, GetChannelNumber, "Returns the number of analog channels used by the force platform");
BTK_SWIG_DOCSTRING_IMPL(ForcePlatform, GetChannel, "Returns the analog channel for the given index.");
BTK_SWIG_DOCSTRING_IMPL(ForcePlatform, SetChannel, "Sets the analog channel for the given index");
BTK_SWIG_DOCSTRING_IMPL(ForcePlatform, GetChannels, "Returns force platform's channels.");
BTK_SWIG_DOCSTRING_IMPL(ForcePlatform, GetOrigin, "Returns the origin of the force platform.");
BTK_SWIG_DOCSTRING_IMPL(ForcePlatform, SetOrigin, "Sets the origin.");
BTK_SWIG_DOCSTRING_IMPL(ForcePlatform, GetCorner, "Returns the corner for the given index.");
BTK_SWIG_DOCSTRING_IMPL(ForcePlatform, SetCorner, "Sets the corner for the given index.");
BTK_SWIG_DOCSTRING_IMPL(ForcePlatform, GetCorners, "Returns corners' coordinates.");
BTK_SWIG_DOCSTRING_IMPL(ForcePlatform, SetCorners, "Sets corners' coordinates.");
BTK_SWIG_DOCSTRING_IMPL(ForcePlatform, GetCalMatrix, "Returns the calibration matrix.");
BTK_SWIG_DOCSTRING_IMPL(ForcePlatform, SetCalMatrix, "Sets the calibration matrix.");
BTK_SWIG_DOCSTRING_IMPL(ForcePlatform, GetType, "Returns the type of the force platform.");

%feature("docstring") btkForcePlatformType1 "
Force platform composed of 6 channels:
 - FX: Horizontal forces on the axis X of the platform;
 - FY: Horizontal forces on the axis Y of the platform;
 - FZ: Vertical forces on the axis Z of the platform;
 - PX: Position of the centre of pressure (CoP) on the axis X of the platform;
 - PY: Position of the centre of pressure (CoP) on the axis Y of the platform;
 _ MZ: Vertical moment on the axis Z of the platform."

%feature("docstring") btkForcePlatformType2 "
Force platform composed of 6 channels:
 - FX: Horizontal forces on the axis X of the platform;
 - FY: Horizontal forces on the axis Y of the platform;
 - FZ: Vertical forces on the axis Z of the platform;
 - MX: Horizontal moment on the axis X of the platform;
 - MY: Horizontal moment on the axis Y of the platform;
 _ MZ: Vertical moment on the axis Z of the platform."

%feature("docstring") btkForcePlatformType3 "
Force platform composed of 8 channels:
 - FZ1: Vertical forces measured by the sensor on the corner 1;
 - FZ2: Vertical forces measured by the sensor on the corner 2;
 - FZ3: Vertical forces measured by the sensor on the corner 3;
 - FZ4: Vertical forces measured by the sensor on the corner 4;
 - FX12: Horizontal forces measured by the sensors 1 & 2;
 - FX34: Horizontal forces measured by the sensors 3 & 4;
 - FY14: Horizontal forces measured by the sensors 1 & 4;
 - FY23: Horizontal forces measured by the sensors 2 & 3.
"

%feature("docstring") btkForcePlatformType4 "
Force platform composed of 6 channels and a 6 columns by 6 rows calibration matrix. 
The definition of the channels are the same than the force platform Type 2."

%feature("docstring") btkForcePlatformType5 "
Force platform composed of 8 channels and a 8 columns by 6 rows calibration matrix. 
The definition of the channels are the same than the force platform Type 3."
 
%feature("docstring") btkForcePlatformType6 "
Force platform composed of 12 channels and a 12 columns by 12 rows calibration matrix:
 - FX1: Horizontal forces measured by the sensor on the corner 1;
 - FX2: Horizontal forces measured by the sensor on the corner 2;
 - FX3: Horizontal forces measured by the sensor on the corner 3;
 - FX4: Horizontal forces measured by the sensor on the corner 4;
 - FY1: Horizontal forces measured by the sensor on the corner 1;
 - FY2: Horizontal forces measured by the sensor on the corner 2;
 - FY3: Horizontal forces measured by the sensor on the corner 3;
 - FY4: Horizontal forces measured by the sensor on the corner 4;
 - FZ1: Vertical forces measured by the sensor on the corner 1;
 - FZ2: Vertical forces measured by the sensor on the corner 2;
 - FZ3: Vertical forces measured by the sensor on the corner 3;
 - FZ4: Vertical forces measured by the sensor on the corner 4."