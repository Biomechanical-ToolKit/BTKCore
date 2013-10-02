/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009-2013, Arnaud Barré
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
//                                 DataObject                                //
// ------------------------------------------------------------------------- //

%feature("docstring") btkDataObject "
Input and output entry for processes in pipelines, cannot be instanced."

BTK_SWIG_DOCSTRING_IMPL(DataObject, Update, "Updates the ProcessObject associated with this DataObject.");
BTK_SWIG_DOCSTRING_IMPL(DataObject, GetTimestamp, "Returns the timestamp of the object.");

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
BTK_SWIG_AUTODOC_IMPL(Event, HasDetectionFlag, "HasDetectionFlag(self, DetectionFlag) -> bool");
BTK_SWIG_AUTODOC_IMPL(Event, SetId, "SetId(self, int)");

BTK_SWIG_DOCSTRING(Event, Clone, "Deep copy of the object.");
BTK_SWIG_DOCSTRING_IMPL(Event, GetLabel, "Returns the event's label.");
BTK_SWIG_DOCSTRING_IMPL(Event, SetLabel, "Sets the event's label.");
BTK_SWIG_DOCSTRING_IMPL(Event, GetDescription, "Returns the event's description.");
BTK_SWIG_DOCSTRING_IMPL(Event, SetDescription, "Sets the event's description.");
BTK_SWIG_DOCSTRING_IMPL(Event, GetContext, "Returns the event's context.");
BTK_SWIG_DOCSTRING_IMPL(Event, SetContext, "Sets the event's context.");
BTK_SWIG_DOCSTRING_IMPL(Event, GetSubject, "Returns the event's subject.");
BTK_SWIG_DOCSTRING_IMPL(Event, SetSubject, "Sets the event's subject.");
BTK_SWIG_DOCSTRING_IMPL(Event, GetTime, "Returns the event's time.");
BTK_SWIG_DOCSTRING_IMPL(Event, SetTime, "Sets the event's time.");
BTK_SWIG_DOCSTRING_IMPL(Event, GetFrame, "Returns the event's frame.");
BTK_SWIG_DOCSTRING_IMPL(Event, SetFrame, "Sets the event's frame.");
BTK_SWIG_DOCSTRING_IMPL(Event, GetDetectionFlags, "Returns the event's detection flags.");
BTK_SWIG_DOCSTRING_IMPL(Event, SetDetectionFlags, "Sets the event's detection flags");
BTK_SWIG_DOCSTRING_IMPL(Event, HasDetectionFlag, "Check if the event has the given detection flag.");
BTK_SWIG_DOCSTRING_IMPL(Event, GetId, "Returns the event's ID.");
BTK_SWIG_DOCSTRING_IMPL(Event, SetId, "Sets the event's ID.");

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
BTK_SWIG_AUTODOC_IMPL(Analog, SetData, "SetData(self, btkAnalogData, parenting = true)");
BTK_SWIG_AUTODOC_IMPL(Analog, GetData, "GetData(self) -> btkAnalogData");

BTK_SWIG_DOCSTRING(Analog, Clone, "Deep copy of the object.");
BTK_SWIG_DOCSTRING_IMPL(Analog, GetLabel, "Returns the analog's label.");
BTK_SWIG_DOCSTRING_IMPL(Analog, SetLabel, "Sets the analog's label.");
BTK_SWIG_DOCSTRING_IMPL(Analog, GetDescription, "Returns the analog's description.");
BTK_SWIG_DOCSTRING_IMPL(Analog, SetDescription, "Sets the analog's description.");
BTK_SWIG_DOCSTRING(Analog, GetValue, "Returns only one sample.");
BTK_SWIG_DOCSTRING(Analog, SetValue, "Sets only one sample.");
BTK_SWIG_DOCSTRING_IMPL(Analog, GetValues, "Returns the analog's samples.\nWARNING:You cannot set values using this method. Use the methods SetValues of SetValue for that.");
BTK_SWIG_DOCSTRING_IMPL(Analog, SetValues, "Sets the analog's samples.");
BTK_SWIG_DOCSTRING_IMPL(Analog, GetFrameNumber, "Returns the number of frames.");
BTK_SWIG_DOCSTRING_IMPL(Analog, SetFrameNumber, "Sets the number of frames.");
BTK_SWIG_DOCSTRING_IMPL(Analog, GetUnit, "Returns the analog's unit.");
BTK_SWIG_DOCSTRING_IMPL(Analog, SetUnit, "Sets the analog's unit.");
BTK_SWIG_DOCSTRING_IMPL(Analog, GetGain, "Returns the analog's gain.");
BTK_SWIG_DOCSTRING_IMPL(Analog, SetGain, "Sets the analog's gain.");
BTK_SWIG_DOCSTRING_IMPL(Analog, SetData, "Sets the data of this analog channel. By default, this object will take the parent of the data. Setting 'parenting' to false will unparent the old data, assign the new one, but doesn't set the point as its parent.");
BTK_SWIG_DOCSTRING_IMPL(Analog, GetData, "Return the data of this analog channel");
BTK_SWIG_DOCSTRING_IMPL(Point, SetDataSlice, "Convenient method to set easily the given value with the given frame.\nWARNING: This function is not safe. There is no checking to determine if the frame is out of range or not. It has the advantage to be faster.");

// ------------------------------------------------------------------------- //
//                                 Analog::Data                              //
// ------------------------------------------------------------------------- //

%feature("docstring") btkAnalogData "
Class storing the measures for one analog channel.
"

BTK_SWIG_AUTODOC_IMPL(AnalogData, GetValues, "GetValues(self) -> array (NumPy)");
BTK_SWIG_AUTODOC_IMPL(AnalogData, SetValues, "SetValues(self, array)");
BTK_SWIG_AUTODOC(AnalogData, SetValue, "SetValue(self, int, double)");
BTK_SWIG_AUTODOC_IMPL(AnalogData, Resize, "Resize(self, int)");

BTK_SWIG_DOCSTRING(AnalogData, Clone, "Deep copy of the object.");
BTK_SWIG_DOCSTRING_IMPL(AnalogData, GetValues, "Returns the analog's values.\nWARNING:You cannot set values using this method.");
BTK_SWIG_DOCSTRING_IMPL(AnalogData, SetValues, "Sets the analog's values.");
BTK_SWIG_DOCSTRING(AnalogData, GetValue, "Returns only one sample.");
BTK_SWIG_DOCSTRING(AnalogData, SetValue, "Sets only one sample.");
BTK_SWIG_DOCSTRING_IMPL(AnalogData, Resize, "Resize the data to the given number of frames.");

// ------------------------------------------------------------------------- //
//                                    Point                                  //
// ------------------------------------------------------------------------- //

%feature("docstring") btkPoint "
The coordinates of the point are generally measured by an acquisition system. For each frame, the
3D value has a residual. It can provide informations on the quality of these 3D data 
(residual associated with the reconstruction of the marker). 

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
To detect if the frame is invalid, you can check the residual which will be 
set to the value -1. It is the same thing if you want to set the frame as invalid. You can set
the values of the frames to 0 and the associated residual to -1.

Note: A residual with a value equal to 0 means that this frame has been post-processed (interpolation, filtering, etc.)."

BTK_SWIG_AUTODOC_IMPL(Point, SetLabel, "SetLabel(self, string)");
BTK_SWIG_AUTODOC_IMPL(Point, SetDescription, "SetDescription(self, string)");
BTK_SWIG_AUTODOC(Point, SetValue, "SetValue(self, int, int, double)");
BTK_SWIG_AUTODOC_IMPL(Point, GetValues, "GetValues(self) -> array (NumPy)");
BTK_SWIG_AUTODOC_IMPL(Point, SetValues, "SetValues(self, array)");
BTK_SWIG_AUTODOC(Point, SetResidual, "SetResidual(self, int, double)");
BTK_SWIG_AUTODOC_IMPL(Point, GetResiduals, "GetResiduals(self) -> array (NumPy)");
BTK_SWIG_AUTODOC_IMPL(Point, SetResiduals, "SetResiduals(self, array)");
BTK_SWIG_AUTODOC_IMPL(Point, SetFrameNumber, "SetFrameNumber(self, int)");
BTK_SWIG_AUTODOC_IMPL(Point, SetType, "SetUnit(self, int)");
BTK_SWIG_AUTODOC_IMPL(Point, SetData, "SetData(self, btkPointData, parenting = true)");
BTK_SWIG_AUTODOC_IMPL(Point, GetData, "GetData(self) -> btkPointData");

BTK_SWIG_DOCSTRING(Point, Clone, "Deep copy of the object.");
BTK_SWIG_DOCSTRING_IMPL(Point, GetLabel, "Returns the point's label.");
BTK_SWIG_DOCSTRING_IMPL(Point, SetLabel, "Sets the point's label.");
BTK_SWIG_DOCSTRING_IMPL(Point, GetDescription, "Returns the point's description.");
BTK_SWIG_DOCSTRING_IMPL(Point, SetDescription, "Sets the point's description.");
BTK_SWIG_DOCSTRING(Point, GetValue, "Returns only one value for the given component and frame.");
BTK_SWIG_DOCSTRING(Point, SetValue, "Sets only one value for the given component and frame.");
BTK_SWIG_DOCSTRING_IMPL(Point, GetValues, "Returns the point's values.\nWARNING:You cannot set values using this method. Use the methods SetValues of SetValue for that.");
BTK_SWIG_DOCSTRING_IMPL(Point, SetValues, "Sets the point's values.");
BTK_SWIG_DOCSTRING(Point, GetResidual, "Returns only one residual for the given frame.");
BTK_SWIG_DOCSTRING(Point, SetResidual, "Sets only one residual for the given frame.");
BTK_SWIG_DOCSTRING_IMPL(Point, GetResiduals, "Returns the point's residuals.");
BTK_SWIG_DOCSTRING_IMPL(Point, SetResiduals, "Sets the point's residuals.");
BTK_SWIG_DOCSTRING_IMPL(Point, GetFrameNumber, "Returns the number of frames.");
BTK_SWIG_DOCSTRING_IMPL(Point, SetFrameNumber, "Sets the number of frames.");
BTK_SWIG_DOCSTRING_IMPL(Point, GetType, "Returns the point's type.");
BTK_SWIG_DOCSTRING_IMPL(Point, SetType, "Sets the point's type.");
BTK_SWIG_DOCSTRING_IMPL(Point, SetData, "Sets the data of this point. By default, this object will take the parent of the data. Setting 'parenting' to false will unparent the old data, assign the new one, but doesn't set the point as its parent.");
BTK_SWIG_DOCSTRING_IMPL(Point, GetData, "Return the data of this point");
BTK_SWIG_DOCSTRING_IMPL(Point, SetDataSlice, "Convenient method to easily set the coordinates x,y,z and the residual for the given frame.\nWARNING: This function is not safe. There is no checking to determine if the frame is out of range or not. It has the advantage to be faster.");

// ------------------------------------------------------------------------- //
//                                 Point::Data                               //
// ------------------------------------------------------------------------- //

%feature("docstring") btkPointData "
Class storing the measures for a point (3D values and residuals along the time).
"

BTK_SWIG_AUTODOC_IMPL(PointData, GetValues, "GetValues(self) -> array (NumPy)");
BTK_SWIG_AUTODOC_IMPL(PointData, SetValues, "SetValues(self, array)");
BTK_SWIG_AUTODOC(Point, SetValue, "SetValue(self, int, int, double)");
BTK_SWIG_AUTODOC_IMPL(PointData, GetResiduals, "GetResiduals(self) -> array (NumPy)");
BTK_SWIG_AUTODOC_IMPL(PointData, SetResiduals, "SetResiduals(self, array)");
BTK_SWIG_AUTODOC(Point, SetResidual, "SetResidual(self, int, double)");
BTK_SWIG_AUTODOC_IMPL(PointData, Resize, "Resize(self, int)");

BTK_SWIG_DOCSTRING(PointData, Clone, "Deep copy of the object.");
BTK_SWIG_DOCSTRING_IMPL(PointData, GetValues, "Returns the point's values.\nWARNING:You cannot set values using this method.");
BTK_SWIG_DOCSTRING_IMPL(PointData, SetValues, "Sets the point's values.");
BTK_SWIG_DOCSTRING(Point, GetValue, "Returns only one value for the given component and frame.");
BTK_SWIG_DOCSTRING(Point, SetValue, "Sets only one value for the given component and frame.");
BTK_SWIG_DOCSTRING_IMPL(PointData, GetResiduals, "Returns the point's residuals.");
BTK_SWIG_DOCSTRING_IMPL(PointData, SetResiduals, "Sets the point's residuals.");
BTK_SWIG_DOCSTRING(Point, GetResidual, "Returns only one residual for the given frame.");
BTK_SWIG_DOCSTRING(Point, SetResidual, "Sets only one residual for the given frame.");
BTK_SWIG_DOCSTRING_IMPL(PointData, Resize, "Resize the data to the given number of frames.");

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
BTK_SWIG_AUTODOC_IMPL(ForcePlatform, SetOrigin(double , double , double ), "SetOrigin(self, double, double, double)");
BTK_SWIG_AUTODOC_IMPL(ForcePlatform, SetOrigin(const btk::ForcePlatform::Origin& ), "SetOrigin(self, array)");
BTK_SWIG_AUTODOC_IMPL(ForcePlatform, GetCorner, "GetCorner(self, int) -> array (NumPy)");
BTK_SWIG_AUTODOC_IMPL(ForcePlatform, SetCorner(int, int, double ), "SetCorner(self, int, int, double)");
BTK_SWIG_AUTODOC_IMPL(ForcePlatform, SetCorner(int, double, double, double ), "SetCorner(self, int, double, double, double)");
BTK_SWIG_AUTODOC_IMPL(ForcePlatform, SetCorner(int, const btk::ForcePlatform::Corner& ), "SetCorner(self, int, array)");
BTK_SWIG_AUTODOC_IMPL(ForcePlatform, GetCorners, "GetCorners(self) -> array (NumPy)");
BTK_SWIG_AUTODOC_IMPL(ForcePlatform, SetCorners, "SetCorners(self, array)");
BTK_SWIG_AUTODOC_IMPL(ForcePlatform, GetCalMatrix, "GetCalMatrix(self) -> array (NumPy)");
BTK_SWIG_AUTODOC_IMPL(ForcePlatform, SetCalMatrix, "SetCalMatrix(self, array)");

BTK_SWIG_DOCSTRING(ForcePlatform, Clone, "Deep copy of the object.");
BTK_SWIG_DOCSTRING_IMPL(ForcePlatform, Begin, "Returns an iterator to the beginning of the list of items.");
BTK_SWIG_DOCSTRING_IMPL(ForcePlatform, End, "Returns an iterator just past the last item.");
BTK_SWIG_DOCSTRING_IMPL(ForcePlatform, GetChannelNumber, "Returns the number of analog channels used by the force platform.");
BTK_SWIG_DOCSTRING_IMPL(ForcePlatform, GetChannel, "Returns the analog channel for the given index.");
BTK_SWIG_DOCSTRING_IMPL(ForcePlatform, SetChannel, "Sets the analog channel for the given index.");
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
 - MZ: Vertical moment on the axis Z of the platform."

%feature("docstring") btkForcePlatformType2 "
Force platform composed of 6 channels:
 - FX: Horizontal forces on the axis X of the platform;
 - FY: Horizontal forces on the axis Y of the platform;
 - FZ: Vertical forces on the axis Z of the platform;
 - MX: Horizontal moment on the axis X of the platform;
 - MY: Horizontal moment on the axis Y of the platform;
 - MZ: Vertical moment on the axis Z of the platform."

%feature("docstring") btkForcePlatformType3 "
Force platform composed of 8 channels:
 - FZ1: Vertical forces measured by the sensor on the corner 1;
 - FZ2: Vertical forces measured by the sensor on the corner 2;
 - FZ3: Vertical forces measured by the sensor on the corner 3;
 - FZ4: Vertical forces measured by the sensor on the corner 4;
 - FX12: Horizontal forces measured by the sensors 1 & 2;
 - FX34: Horizontal forces measured by the sensors 3 & 4;
 - FY14: Horizontal forces measured by the sensors 1 & 4;
 - FY23: Horizontal forces measured by the sensors 2 & 3."

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
 
// ------------------------------------------------------------------------- //
//                                    Wrench                                 //
// ------------------------------------------------------------------------- //

%feature("docstring") btkWrench "
Represents a wrench as position, force and moment varying during the time.

This class is mainly used to have a compact way to store forces and moment acting (or reacting) at a given position.
The moment should be expressed at the position as it is defined in general mechanics.

By default, the label used for the forces and the moments corresponds to the given label followed by the suffix '.F' and '.M' respectively.

Warning: The call of the constructor btkWrench(label) must be followed by the use of the method btkWrench::SetFrameNumber as it creates a null matrix for the values."

BTK_SWIG_AUTODOC_IMPL(Wrench, SetPosition, "SetPosition(self, btkPoint)");
BTK_SWIG_AUTODOC_IMPL(Wrench, SetForce, "SetForce(self, btkPoint)");
BTK_SWIG_AUTODOC_IMPL(Wrench, SetMoment, "SetMoment(self, btkPoint)");
BTK_SWIG_AUTODOC_IMPL(Wrench, GetComponent, "GetComponent(self, int) -> btkPoint)");
BTK_SWIG_AUTODOC_IMPL(Wrench, SetFrameNumber, "SetFrameNumber(self, int)");

BTK_SWIG_DOCSTRING_IMPL(Wrench, GetPosition, "Returns the wrench's position.");
BTK_SWIG_DOCSTRING_IMPL(Wrench, SetPosition, "Sets the wrench's position.");
BTK_SWIG_DOCSTRING_IMPL(Wrench, GetForce, "Returns the wrench's force.");
BTK_SWIG_DOCSTRING_IMPL(Wrench, SetForce, "Sets the wrench's force.");
BTK_SWIG_DOCSTRING_IMPL(Wrench, GetMoment, "Returns the wrench's moment.");
BTK_SWIG_DOCSTRING_IMPL(Wrench, SetForce, "Sets the wrench's moment.");
BTK_SWIG_DOCSTRING_IMPL(Wrench, GetComponent, "Returns the component with the given index. The possible value for the index are: 0: Returns the position, 1: Returns the force, 2: Returns the moment. Any other value will trigger an exception.");
BTK_SWIG_DOCSTRING_IMPL(Wrench, SetFrameNumber, "Set the number of frames in the wrenches. The given number of frames must be greater than 0.");
BTK_SWIG_DOCSTRING(btkWrench, Clone, "Deep copy of the object.");

// ------------------------------------------------------------------------- //
//                                    IMU                                    //
// ------------------------------------------------------------------------- //

%feature("docstring") btkIMU "
Container of analog channels representing an inertial measurement unit (IMU).

This class proposes to store any number of analog channels but the 6 first channels could be 
used for 3 accelerometers and 3 gyroscopes as some convenient methods are proposed in this way
(see SetChannels(), GetAccelerometerX(), GetAccelerometerY(), GetAccelerometerZ(), GetGyroscopeX(), GetGyroscopeY(), GetGyroscopeZ()).

The storage of the channel is done using an ID and not an array index. You can then use the same ID for some kind of sensor even if other are missing (i.e. IMU with accelerometers disabled or configured with 1 acc, and 2 gyros., etc.).

Note: This class is still experimental and could be modified in a new release."

BTK_SWIG_AUTODOC_IMPL(IMU, SetChannels, "SetChannels(self, btkAnalog, btkAnalog, btkAnalog, btkAnalog, btkAnalog, btkAnalog)");
BTK_SWIG_AUTODOC_IMPL(IMU, GetChannels, "GetChannels(self) -> btkAnalogCollection");
BTK_SWIG_AUTODOC_IMPL(IMU, SetChannel, "SetChannel(self, id, btkAnalog)");
BTK_SWIG_AUTODOC_IMPL(IMU, GetChannel(int ), "GetChannel(self, id) -> btkAnalog");
BTK_SWIG_AUTODOC_IMPL(IMU, GetChannel(std::string ), "GetChannel(self, label) -> btkAnalog");
BTK_SWIG_AUTODOC_IMPL(IMU, GetAccelerometerX, "GetAccelerometerX(self) -> btkAnalog");
BTK_SWIG_AUTODOC_IMPL(IMU, GetAccelerometerY, "GetAccelerometerY(self) -> btkAnalog");
BTK_SWIG_AUTODOC_IMPL(IMU, GetAccelerometerZ, "GetAccelerometerZ(self) -> btkAnalog");
BTK_SWIG_AUTODOC_IMPL(IMU, GetGyroscopeX, "GetGyroscopeX(self) -> btkAnalog");
BTK_SWIG_AUTODOC_IMPL(IMU, GetGyroscopeY, "GetGyroscopeY(self) -> btkAnalog");
BTK_SWIG_AUTODOC_IMPL(IMU, GetGyroscopeZ, "GetGyroscopeZ(self) -> btkAnalog");
BTK_SWIG_AUTODOC_IMPL(IMU, GetCalMatrix, "GetCalMatrix(self) -> array (NumPy)");
BTK_SWIG_AUTODOC_IMPL(IMU, SetCalMatrix, "SetCalMatrix(self, array)");

BTK_SWIG_DOCSTRING_IMPL(IMU, SetChannels, "Sets the 6 first channels of the IMUs with the analog channels @a accX, @a accY, @a accZ, @a gyroX, @a gyroY, @a gyroZ.");
BTK_SWIG_DOCSTRING_IMPL(IMU, GetChannels, "Returns the analog channels of the IMU in a collection. The analog channels are pushed in the collection by using their ID.");
BTK_SWIG_DOCSTRING_IMPL(IMU, SetChannel, "Sets an analog channel to the given ID. If an analog channel is already set to this ID, then it is replaced.");
BTK_SWIG_DOCSTRING_IMPL(IMU, GetChannel, "Returns the analog channel with the given ID or label. If there is no analog channel with the given ID, then an exception is thrown.");
BTK_SWIG_DOCSTRING_IMPL(IMU, GetFrameNumber, "Returns the number of frames set in this IMU.");
BTK_SWIG_DOCSTRING_IMPL(IMU, SetFrameNumber, "Set the number of frames for the IMU and modify also the number of frames for the channels.");
BTK_SWIG_DOCSTRING_IMPL(IMU, GetFrequency, "Returns the frequency set to the IMU. (by default it is 0 Hz).");
BTK_SWIG_DOCSTRING_IMPL(IMU, SetFrequency, "Sets the acquisition sample rate associated to the IMU.");
BTK_SWIG_DOCSTRING_IMPL(IMU, GetAccelerometerX, "Convenient method to return the analog channel with the ID 0 (which should correspond to an accelerometer measuring data on the X axis of the IMU).");
BTK_SWIG_DOCSTRING_IMPL(IMU, GetAccelerometerY, "Convenient method to return the analog channel with the ID 1 (which should correspond to an accelerometer measuring data on the Y axis of the IMU).");
BTK_SWIG_DOCSTRING_IMPL(IMU, GetAccelerometerZ, "Convenient method to return the analog channel with the ID 2 (which should correspond to an accelerometer measuring data on the Z axis of the IMU).");
BTK_SWIG_DOCSTRING_IMPL(IMU, GetGyroscopeX, "Convenient method to return the analog channel with the ID 3 (which should correspond to a gyroscope measuring data on the X axis of the IMU).");
BTK_SWIG_DOCSTRING_IMPL(IMU, GetGyroscopeY, "Convenient method to return the analog channel with the ID 4 (which should correspond to a gyroscope measuring data on the Y axis of the IMU).");
BTK_SWIG_DOCSTRING_IMPL(IMU, GetGyroscopeZ, "Convenient method to return the analog channel with the ID 5 (which should correspond to a gyroscope measuring data on the Z axis of the IMU).");
BTK_SWIG_DOCSTRING_IMPL(IMU, Rotate, "Convenient method to rotate the analog channels with the IDs 0-5. This method handles the case where some of these analog channels are missing.");
BTK_SWIG_DOCSTRING_IMPL(IMU, GetChannelNumber, "Returns the number of analog channels associated to this IMU.");
BTK_SWIG_DOCSTRING_IMPL(IMU, GetType, "Returns the type of the IMU.");

%feature("docstring") btkIMUType1 "
IMU 6D (3 accelerometers and 3 gyroscopes)."

%feature("docstring") btkIMUType2 "
IMU 6D (same as Type 1) with a calibration matrix used to align sensors (accelerometers and gyroscopes) on the same inertial reference frame."

// ------------------------------------------------------------------------- //
//                              EventCollection                              //
// ------------------------------------------------------------------------- //

%feature("docstring") btkEventCollection "
Collection of events stored in a list."

BTK_SWIG_AUTODOC_IMPL(EventCollection, Begin, "Begin(self) -> btkEventIterator");
BTK_SWIG_AUTODOC_IMPL(EventCollection, End, "End(self) -> btkEventIterator");
BTK_SWIG_AUTODOC_IMPL(EventCollection, GetIndexOf, "GetIndexOf(self, int) -> btkEvent");
BTK_SWIG_AUTODOC_IMPL(EventCollection, GetItem, "GetItem(self, int) -> btkEvent");
BTK_SWIG_AUTODOC_IMPL(EventCollection, InsertItem(Iterator , ItemPointer ), "InsertItem(self, btkEventIterator, btkEvent) -> bool");
BTK_SWIG_AUTODOC_IMPL(EventCollection, InsertItem(int , ItemPointer ), "InsertItem(self, int, btkEvent) -> bool");
BTK_SWIG_AUTODOC_IMPL(EventCollection, InsertItem(ItemPointer ), "InsertItem(self, btkEvent) -> bool");
BTK_SWIG_AUTODOC_IMPL(EventCollection, RemoveItem(Iterator ), "RemoveItem(self, btkEventIterator) -> btkEventIterator");
BTK_SWIG_AUTODOC_IMPL(EventCollection, RemoveItem(int ), "RemoveItem(self, int)");
BTK_SWIG_AUTODOC_IMPL(EventCollection, TakeItem(Iterator ), "TakeItem(self, btkEventIterator) -> btkEvent");
BTK_SWIG_AUTODOC_IMPL(EventCollection, TakeItem(int ), "TakeItem(self, int) -> btkEvent");
BTK_SWIG_AUTODOC_IMPL(EventCollection, SetItem, "SetItem(self, int) -> btkEvent");

BTK_SWIG_DOCSTRING_IMPL(EventCollection, Begin, "Returns an iterator to the beginning of the list of items.");
BTK_SWIG_DOCSTRING_IMPL(EventCollection, End, "Returns an iterator just past the last item.");
BTK_SWIG_DOCSTRING_IMPL(EventCollection, GetIndexOf, "Returns the index of the given item.");
BTK_SWIG_DOCSTRING_IMPL(EventCollection, GetItemNumber, "Returns the number of events");
BTK_SWIG_DOCSTRING_IMPL(EventCollection, SetItemNumber, "Sets the number of events");
BTK_SWIG_DOCSTRING_IMPL(EventCollection, GetItem, "Returns the event for the given index.");
BTK_SWIG_DOCSTRING_IMPL(EventCollection, SetItem, "Sets the event for the given index");
BTK_SWIG_DOCSTRING_IMPL(EventCollection, InsertItem, "Inserts an event in the list.");
BTK_SWIG_DOCSTRING_IMPL(EventCollection, IsEmpty, "Returns true if the collection is empty.");
BTK_SWIG_DOCSTRING_IMPL(EventCollection, RemoveItem, "Removes an event.");
BTK_SWIG_DOCSTRING_IMPL(EventCollection, SetItem, "Sets an event.");
BTK_SWIG_DOCSTRING_IMPL(EventCollection, TakeItem, "Removes an event and return it.");
BTK_SWIG_DOCSTRING_IMPL(EventCollection, Clear, "Clear the contents of the collection.");
BTK_SWIG_DOCSTRING(EventCollection, Clone, "Deep copy of the object");

// ------------------------------------------------------------------------- //
//                              AnalogCollection                             //
// ------------------------------------------------------------------------- //

%feature("docstring") btkAnalogCollection "
Collection of analog channels stored in a list."

BTK_SWIG_AUTODOC_IMPL(AnalogCollection, Begin, "Begin(self) -> btkAnalogIterator");
BTK_SWIG_AUTODOC_IMPL(AnalogCollection, End, "End(self) -> btkAnalogIterator");
BTK_SWIG_AUTODOC_IMPL(AnalogCollection, GetIndexOf, "GetIndexOf(self, int) -> btkAnalog");
BTK_SWIG_AUTODOC_IMPL(AnalogCollection, GetItem, "GetItem(self, int) -> btkAnalog");
BTK_SWIG_AUTODOC_IMPL(AnalogCollection, InsertItem(Iterator , ItemPointer ), "InsertItem(self, btkAnalogIterator, btkAnalog) -> bool");
BTK_SWIG_AUTODOC_IMPL(AnalogCollection, InsertItem(int , ItemPointer ), "InsertItem(self, int, btkAnalog) -> bool");
BTK_SWIG_AUTODOC_IMPL(AnalogCollection, InsertItem(ItemPointer ), "InsertItem(self, btkAnalog) -> bool");
BTK_SWIG_AUTODOC_IMPL(AnalogCollection, RemoveItem(Iterator ), "RemoveItem(self, btkAnalogIterator) -> btkAnalogIterator");
BTK_SWIG_AUTODOC_IMPL(AnalogCollection, RemoveItem(int ), "RemoveItem(self, int)");
BTK_SWIG_AUTODOC_IMPL(AnalogCollection, TakeItem(Iterator ), "TakeItem(self, btkAnalogIterator) -> btkAnalog");
BTK_SWIG_AUTODOC_IMPL(AnalogCollection, TakeItem(int ), "TakeItem(self, int) -> btkAnalog");
BTK_SWIG_AUTODOC_IMPL(AnalogCollection, SetItem, "SetItem(self, int) -> btkAnalog");

BTK_SWIG_DOCSTRING_IMPL(AnalogCollection, Begin, "Returns an iterator to the beginning of the list of items.");
BTK_SWIG_DOCSTRING_IMPL(AnalogCollection, End, "Returns an iterator just past the last item.");
BTK_SWIG_DOCSTRING_IMPL(AnalogCollection, GetIndexOf, "Returns the index of the given item.");
BTK_SWIG_DOCSTRING_IMPL(AnalogCollection, GetItemNumber, "Returns the number of analog channels");
BTK_SWIG_DOCSTRING_IMPL(AnalogCollection, SetItemNumber, "Sets the number of analog channels");
BTK_SWIG_DOCSTRING_IMPL(AnalogCollection, GetItem, "Returns the analog channel for the given index.");
BTK_SWIG_DOCSTRING_IMPL(AnalogCollection, SetItem, "Sets the analog channel for the given index");
BTK_SWIG_DOCSTRING_IMPL(AnalogCollection, InsertItem, "Inserts an analog channel in the list.");
BTK_SWIG_DOCSTRING_IMPL(AnalogCollection, IsEmpty, "Returns true if the collection is empty.");
BTK_SWIG_DOCSTRING_IMPL(AnalogCollection, RemoveItem, "Removes an analog channel.");
BTK_SWIG_DOCSTRING_IMPL(AnalogCollection, SetItem, "Sets an analog channel.");
BTK_SWIG_DOCSTRING_IMPL(AnalogCollection, TakeItem, "Removes an analog channel and return it.");
BTK_SWIG_DOCSTRING_IMPL(AnalogCollection, Clear, "Clear the contents of the collection.");
BTK_SWIG_DOCSTRING(AnalogCollection, Clone, "Deep copy of the object");

// ------------------------------------------------------------------------- //
//                               PointCollection                             //
// ------------------------------------------------------------------------- //

%feature("docstring") btkPointCollection "
Collection of point stored in a list."

BTK_SWIG_AUTODOC_IMPL(PointCollection, Begin, "Begin(self) -> btkPointIterator");
BTK_SWIG_AUTODOC_IMPL(PointCollection, End, "End(self) -> btkPointIterator");
BTK_SWIG_AUTODOC_IMPL(PointCollection, GetIndexOf, "GetIndexOf(self, int) -> btkPoint");
BTK_SWIG_AUTODOC_IMPL(PointCollection, GetItem, "GetItem(self, int) -> btkPoint");
BTK_SWIG_AUTODOC_IMPL(PointCollection, InsertItem(Iterator , ItemPointer ), "InsertItem(self, btkPointIterator, btkPoint) -> bool");
BTK_SWIG_AUTODOC_IMPL(PointCollection, InsertItem(int , ItemPointer ), "InsertItem(self, int, btkPoint) -> bool");
BTK_SWIG_AUTODOC_IMPL(PointCollection, InsertItem(ItemPointer ), "InsertItem(self, btkPoint) -> bool");
BTK_SWIG_AUTODOC_IMPL(PointCollection, RemoveItem(Iterator ), "RemoveItem(self, btkPointIterator) -> btkPointIterator");
BTK_SWIG_AUTODOC_IMPL(PointCollection, RemoveItem(int ), "RemoveItem(self, int)");
BTK_SWIG_AUTODOC_IMPL(PointCollection, TakeItem(Iterator ), "TakeItem(self, btkPointIterator) -> btkPoint");
BTK_SWIG_AUTODOC_IMPL(PointCollection, TakeItem(int ), "TakeItem(self, int) -> btkPoint");
BTK_SWIG_AUTODOC_IMPL(PointCollection, SetItem, "SetItem(self, int) -> btkPoint");

BTK_SWIG_DOCSTRING_IMPL(PointCollection, Begin, "Returns an iterator to the beginning of the list of items.");
BTK_SWIG_DOCSTRING_IMPL(PointCollection, End, "Returns an iterator just past the last item.");
BTK_SWIG_DOCSTRING_IMPL(PointCollection, GetIndexOf, "Returns the index of the given item.");
BTK_SWIG_DOCSTRING_IMPL(PointCollection, GetItemNumber, "Returns the number of points");
BTK_SWIG_DOCSTRING_IMPL(PointCollection, SetItemNumber, "Sets the number of points");
BTK_SWIG_DOCSTRING_IMPL(PointCollection, GetItem, "Returns the point for the given index.");
BTK_SWIG_DOCSTRING_IMPL(PointCollection, SetItem, "Sets the point for the given index");
BTK_SWIG_DOCSTRING_IMPL(PointCollection, InsertItem, "Inserts a point in the list.");
BTK_SWIG_DOCSTRING_IMPL(PointCollection, IsEmpty, "Returns true if the collection is empty.");
BTK_SWIG_DOCSTRING_IMPL(PointCollection, RemoveItem, "Removes a point.");
BTK_SWIG_DOCSTRING_IMPL(PointCollection, SetItem, "Sets a point.");
BTK_SWIG_DOCSTRING_IMPL(PointCollection, TakeItem, "Removes a point and return it.");
BTK_SWIG_DOCSTRING_IMPL(PointCollection, Clear, "Clear the contents of the collection.");
BTK_SWIG_DOCSTRING(PointCollection, Clone, "Deep copy of the object");

// ------------------------------------------------------------------------- //
//                           ForcePlatformCollection                         //
// ------------------------------------------------------------------------- //

%feature("docstring") btkForcePlatformCollection "
Collection of force platforms stored in a list."

BTK_SWIG_AUTODOC_IMPL(ForcePlatformCollection, Begin, "Begin(self) -> btkForcePlatformIterator");
BTK_SWIG_AUTODOC_IMPL(ForcePlatformCollection, End, "End(self) -> btkForcePlatformIterator");
BTK_SWIG_AUTODOC_IMPL(ForcePlatformCollection, GetIndexOf, "GetIndexOf(self, int) -> btkForcePlatform");
BTK_SWIG_AUTODOC_IMPL(ForcePlatformCollection, GetItem, "GetItem(self, int) -> btkForcePlatform");
BTK_SWIG_AUTODOC_IMPL(ForcePlatformCollection, InsertItem(Iterator , ItemPointer ), "InsertItem(self, btkForcePlatformIterator, btkForcePlatform) -> bool");
BTK_SWIG_AUTODOC_IMPL(ForcePlatformCollection, InsertItem(int , ItemPointer ), "InsertItem(self, int, btkForcePlatform) -> bool");
BTK_SWIG_AUTODOC_IMPL(ForcePlatformCollection, InsertItem(ItemPointer ), "InsertItem(self, btkForcePlatform) -> bool");
BTK_SWIG_AUTODOC_IMPL(ForcePlatformCollection, RemoveItem(Iterator ), "RemoveItem(self, btkForcePlatformIterator) -> btkForcePlatformIterator");
BTK_SWIG_AUTODOC_IMPL(ForcePlatformCollection, RemoveItem(int ), "RemoveItem(self, int)");
BTK_SWIG_AUTODOC_IMPL(ForcePlatformCollection, TakeItem(Iterator ), "TakeItem(self, btkForcePlatformIterator) -> btkForcePlatform");
BTK_SWIG_AUTODOC_IMPL(ForcePlatformCollection, TakeItem(int ), "TakeItem(self, int) -> btkForcePlatform");
BTK_SWIG_AUTODOC_IMPL(ForcePlatformCollection, SetItem, "SetItem(self, int) -> btkForcePlatform");

BTK_SWIG_DOCSTRING_IMPL(ForcePlatformCollection, Begin, "Returns an iterator to the beginning of the list of items.");
BTK_SWIG_DOCSTRING_IMPL(ForcePlatformCollection, End, "Returns an iterator just past the last item.");
BTK_SWIG_DOCSTRING_IMPL(ForcePlatformCollection, GetIndexOf, "Returns the index of the given item.");
BTK_SWIG_DOCSTRING_IMPL(ForcePlatformCollection, GetItemNumber, "Returns the number of force platforms");
BTK_SWIG_DOCSTRING_IMPL(ForcePlatformCollection, SetItemNumber, "Sets the number of force platforms");
BTK_SWIG_DOCSTRING_IMPL(ForcePlatformCollection, GetItem, "Returns the force platform for the given index.");
BTK_SWIG_DOCSTRING_IMPL(ForcePlatformCollection, SetItem, "Sets the force platform for the given index");
BTK_SWIG_DOCSTRING_IMPL(ForcePlatformCollection, InsertItem, "Inserts a force platform in the list.");
BTK_SWIG_DOCSTRING_IMPL(ForcePlatformCollection, IsEmpty, "Returns true if the collection is empty.");
BTK_SWIG_DOCSTRING_IMPL(ForcePlatformCollection, RemoveItem, "Removes a force platform.");
BTK_SWIG_DOCSTRING_IMPL(ForcePlatformCollection, SetItem, "Sets a force platform.");
BTK_SWIG_DOCSTRING_IMPL(ForcePlatformCollection, TakeItem, "Removes a force platform and return it.");
BTK_SWIG_DOCSTRING_IMPL(ForcePlatformCollection, Clear, "Clear the contents of the collection.");
BTK_SWIG_DOCSTRING(ForcePlatformCollection, Clone, "Deep copy of the object");

// ------------------------------------------------------------------------- //
//                               WrenchCollection                            //
// ------------------------------------------------------------------------- //

%feature("docstring") btkWrenchCollection "
Collection of wrenchs stored in a list."

BTK_SWIG_AUTODOC_IMPL(WrenchCollection, Begin, "Begin(self) -> btkWrenchIterator");
BTK_SWIG_AUTODOC_IMPL(WrenchCollection, End, "End(self) -> btkWrenchIterator");
BTK_SWIG_AUTODOC_IMPL(WrenchCollection, GetIndexOf, "GetIndexOf(self, int) -> btkWrench");
BTK_SWIG_AUTODOC_IMPL(WrenchCollection, GetItem, "GetItem(self, int) -> btkWrench");
BTK_SWIG_AUTODOC_IMPL(WrenchCollection, InsertItem(Iterator , ItemPointer ), "InsertItem(self, btkWrenchIterator, btkWrench) -> bool");
BTK_SWIG_AUTODOC_IMPL(WrenchCollection, InsertItem(int , ItemPointer ), "InsertItem(self, int, btkWrench) -> bool");
BTK_SWIG_AUTODOC_IMPL(WrenchCollection, InsertItem(ItemPointer ), "InsertItem(self, btkWrench) -> bool");
BTK_SWIG_AUTODOC_IMPL(WrenchCollection, RemoveItem(Iterator ), "RemoveItem(self, btkWrenchIterator) -> btkWrenchIterator");
BTK_SWIG_AUTODOC_IMPL(WrenchCollection, RemoveItem(int ), "RemoveItem(self, int)");
BTK_SWIG_AUTODOC_IMPL(WrenchCollection, TakeItem(Iterator ), "TakeItem(self, btkWrenchIterator) -> btkWrench");
BTK_SWIG_AUTODOC_IMPL(WrenchCollection, TakeItem(int ), "TakeItem(self, int) -> btkWrench");
BTK_SWIG_AUTODOC_IMPL(WrenchCollection, SetItem, "SetItem(self, int) -> btkWrench");

BTK_SWIG_DOCSTRING_IMPL(WrenchCollection, Begin, "Returns an iterator to the beginning of the list of items.");
BTK_SWIG_DOCSTRING_IMPL(WrenchCollection, End, "Returns an iterator just past the last item.");
BTK_SWIG_DOCSTRING_IMPL(WrenchCollection, GetIndexOf, "Returns the index of the given item.");
BTK_SWIG_DOCSTRING_IMPL(WrenchCollection, GetItemNumber, "Returns the number of wrenches");
BTK_SWIG_DOCSTRING_IMPL(WrenchCollection, SetItemNumber, "Sets the number of wrenches");
BTK_SWIG_DOCSTRING_IMPL(WrenchCollection, GetItem, "Returns the wrench for the given index.");
BTK_SWIG_DOCSTRING_IMPL(WrenchCollection, SetItem, "Sets the wrench for the given index");
BTK_SWIG_DOCSTRING_IMPL(WrenchCollection, InsertItem, "Inserts a wrench in the list.");
BTK_SWIG_DOCSTRING_IMPL(WrenchCollection, IsEmpty, "Returns true if the collection is empty.");
BTK_SWIG_DOCSTRING_IMPL(WrenchCollection, RemoveItem, "Removes a wrench.");
BTK_SWIG_DOCSTRING_IMPL(WrenchCollection, SetItem, "Sets a wrench.");
BTK_SWIG_DOCSTRING_IMPL(WrenchCollection, TakeItem, "Removes a wrench and return it.");
BTK_SWIG_DOCSTRING_IMPL(WrenchCollection, Clear, "Clear the contents of the collection.");
BTK_SWIG_DOCSTRING(WrenchCollection, Clone, "Deep copy of the object");

// ------------------------------------------------------------------------- //
//                                 IMUCollection                             //
// ------------------------------------------------------------------------- //

%feature("docstring") btkIMUCollection "
Collection of IMUs stored in a list."

BTK_SWIG_AUTODOC_IMPL(IMUCollection, Begin, "Begin(self) -> btkIMUIterator");
BTK_SWIG_AUTODOC_IMPL(IMUCollection, End, "End(self) -> btkIMUIterator");
BTK_SWIG_AUTODOC_IMPL(IMUCollection, GetIndexOf, "GetIndexOf(self, int) -> btkIMU");
BTK_SWIG_AUTODOC_IMPL(IMUCollection, GetItem, "GetItem(self, int) -> btkIMU");
BTK_SWIG_AUTODOC_IMPL(IMUCollection, InsertItem(Iterator , ItemPointer ), "InsertItem(self, btkIMUIterator, btkIMU) -> bool");
BTK_SWIG_AUTODOC_IMPL(IMUCollection, InsertItem(int , ItemPointer ), "InsertItem(self, int, btkIMU) -> bool");
BTK_SWIG_AUTODOC_IMPL(IMUCollection, InsertItem(ItemPointer ), "InsertItem(self, btkIMU) -> bool");
BTK_SWIG_AUTODOC_IMPL(IMUCollection, RemoveItem(Iterator ), "RemoveItem(self, btkIMUIterator) -> btkIMUIterator");
BTK_SWIG_AUTODOC_IMPL(IMUCollection, RemoveItem(int ), "RemoveItem(self, int)");
BTK_SWIG_AUTODOC_IMPL(IMUCollection, TakeItem(Iterator ), "TakeItem(self, btkIMUIterator) -> btkIMU");
BTK_SWIG_AUTODOC_IMPL(IMUCollection, TakeItem(int ), "TakeItem(self, int) -> btkIMU");
BTK_SWIG_AUTODOC_IMPL(IMUCollection, SetItem, "SetItem(self, int) -> btkIMU");

BTK_SWIG_DOCSTRING_IMPL(IMUCollection, Begin, "Returns an iterator to the beginning of the list of items.");
BTK_SWIG_DOCSTRING_IMPL(IMUCollection, End, "Returns an iterator just past the last item.");
BTK_SWIG_DOCSTRING_IMPL(IMUCollection, GetIndexOf, "Returns the index of the given item.");
BTK_SWIG_DOCSTRING_IMPL(IMUCollection, GetItemNumber, "Returns the number of IMUs");
BTK_SWIG_DOCSTRING_IMPL(IMUCollection, SetItemNumber, "Sets the number of IMUs");
BTK_SWIG_DOCSTRING_IMPL(IMUCollection, GetItem, "Returns the IMU for the given index.");
BTK_SWIG_DOCSTRING_IMPL(IMUCollection, SetItem, "Sets the IMU for the given index");
BTK_SWIG_DOCSTRING_IMPL(IMUCollection, InsertItem, "Inserts an IMU in the list.");
BTK_SWIG_DOCSTRING_IMPL(IMUCollection, IsEmpty, "Returns true if the collection is empty.");
BTK_SWIG_DOCSTRING_IMPL(IMUCollection, RemoveItem, "Removes a IMU.");
BTK_SWIG_DOCSTRING_IMPL(IMUCollection, SetItem, "Sets an IMU.");
BTK_SWIG_DOCSTRING_IMPL(IMUCollection, TakeItem, "Removes an IMU and return it.");
BTK_SWIG_DOCSTRING_IMPL(IMUCollection, Clear, "Clear the contents of the collection.");
BTK_SWIG_DOCSTRING(IMUCollection, Clone, "Deep copy of the object");

// ------------------------------------------------------------------------- //
//                                     MetaData                              //
// ------------------------------------------------------------------------- //

%feature("docstring") btkMetaData "
Store data which cannot be embedded within timeseries data (calibration matrix, subject informations, ...).

Each object can store children metadata. Theoretically, the number of sub-level is infinite. However, the main common use of the metadata is to use 2 levels. This use was introduced in the C3D format.
The values in a metadata are stored in a MetaDataInfo object and are accessible using the method GetInfo().

The unlock member accessible using the methods GetUnlock() and SetUnlock() doesn't affect the setting of the values, but is used only to inform the user/developer."

BTK_SWIG_AUTODOC_IMPL(MetaData, ExtractChildInfo, "ExtractChildInfo(self, label, format, numdims, noPossibleEmptyValue = True) -> btkMetaDataInfo");
BTK_SWIG_AUTODOC_IMPL(MetaData, FindChild, "FindChild(self, label) -> btkMetaDataIterator");
BTK_SWIG_AUTODOC_IMPL(MetaData, GetChild, "GetChild(self, index) -> btkMetaData");
BTK_SWIG_AUTODOC_IMPL(MetaData, InsertChild(Iterator , btkMetaData ), "InsertChild(self, btkMetaDataIterator, btkMetaData) -> bool");
BTK_SWIG_AUTODOC_IMPL(MetaData, InsertChild(int , btkMetaData ), "InsertChild(self, int, btkMetaData) -> bool");
BTK_SWIG_AUTODOC_IMPL(MetaData, RemoveChild(Iterator ), "RemoveChild(self, btkMetaDataIterator) -> btkMetaDataIterator");
BTK_SWIG_AUTODOC_IMPL(MetaData, RemoveChild(int ), "RemoveChild(self, int)");
BTK_SWIG_AUTODOC_IMPL(MetaData, RemoveChild(const string& ), "RemoveChild(self, string)");
BTK_SWIG_AUTODOC_IMPL(MetaData, SetChild, "SetChild(self, int, btkMetaData)");
BTK_SWIG_AUTODOC_IMPL(MetaData, SetLabel, "SetLabel(self, string)");
BTK_SWIG_AUTODOC_IMPL(MetaData, SetDescription, "SetDescription(self, string)");
BTK_SWIG_AUTODOC_IMPL(MetaData, TakeChild(Iterator ), "TakeChild(self, btkMetaDataIterator) -> btkMetaData");
BTK_SWIG_AUTODOC_IMPL(MetaData, TakeChild(int ), "TakeChild(self, int) -> btkMetaData");
BTK_SWIG_AUTODOC_IMPL(MetaData, TakeChild(const std::string& ), "TakeChild(self, string) -> btkMetaData");

BTK_SWIG_DOCSTRING_IMPL(MetaData, SetLabel, "Sets the label of the entry. If label already exist in the parent's list, then an exception is thrown.");
BTK_SWIG_DOCSTRING_IMPL(MetaData, GetUnlockState, "Gets the unlock state of the entry.");
BTK_SWIG_DOCSTRING_IMPL(MetaData, SetUnlockState, "Sets the unlock state of the entry.");
BTK_SWIG_DOCSTRING_IMPL(MetaData, GetInfo, "Gets the btkMetaDataInfo associated to the entry.");
BTK_SWIG_DOCSTRING_IMPL(MetaData, SetInfo, "Sets the btkMetaDataInfo associated to the entry.");
BTK_SWIG_DOCSTRING_IMPL(MetaData, HasInfo, "Checks if a btkMetaDataInfo is associated with the entry.");
BTK_SWIG_DOCSTRING_IMPL(MetaData, ExtractChildInfo, "Check if a child exists with the given label and correspond to the given format plus the number of dimensions with or without values (noPossibleEmptyValue). The informations of the metadata are extracted if all the checks are ok. Otherwise, the methods return an empty pointer.");
BTK_SWIG_DOCSTRING_IMPL(MetaData, HasMetaDataAsParent, "Checks if this object has a MetaData object as parents.");
BTK_SWIG_DOCSTRING_IMPL(MetaData, Begin, "Returns an iterator to the beginning of the list of children.");
BTK_SWIG_DOCSTRING_IMPL(MetaData, End, "Returns an iterator just past the last child.");
BTK_SWIG_DOCSTRING_IMPL(MetaData, GetChild, "Gets the entry for the given index or label. An exception is thrown is the given input doesn't correspond to any entry.");
BTK_SWIG_DOCSTRING_IMPL(MetaData, AppendChild, "Append the child entry. If entry has a label which already exists in the parent's list, then it is not inserted. Returns True if child has been inserted or False.");
BTK_SWIG_DOCSTRING_IMPL(MetaData, InsertChild, "Insert the child entry for the given location. If the entry has a label which already exists in the parent's list, then it is not inserted. Returns True if child has been inserted or False.");
BTK_SWIG_DOCSTRING_IMPL(MetaData, SetChild, "Sets the child entry for the given index");
BTK_SWIG_DOCSTRING_IMPL(MetaData, TakeChild, "Removes the child entry which has the given index, label or corresponding iterator. If there is no child with the specified label, then an empty pointer is returned.");
BTK_SWIG_DOCSTRING_IMPL(MetaData, RemoveChild, "Removes the child entry which has the given index, label or corresponding iterator.");
BTK_SWIG_DOCSTRING_IMPL(MetaData, ClearChildren, "Removes every children.");
BTK_SWIG_DOCSTRING_IMPL(MetaData, HasChildren, "Checks if the entry has children ");
BTK_SWIG_DOCSTRING_IMPL(MetaData, GetChildNumber, "Returns the number of children ");
BTK_SWIG_DOCSTRING_IMPL(MetaData, FindChild, "Finds the children which has the given label and return it as an iterator.");
BTK_SWIG_DOCSTRING(MetaData, Clone, "Deep copy of the object");

%feature("docstring") btkMetaDataCreateChild "Function to create/set easily a metadata (with or without value) from a given label into a given parent.";
%feature("docstring") btkMetaDataCreateChild "Function to create/set easily a metadata containing a 2D array as value, from a given label into a given parent.";
%feature("docstring") btkMetaDataCollapseChildrenValuesDouble "Convenient method to extract the values of a metadata as an array of doubles.\nCollapse the parent children entries' values starting with the string 'baselabel' and incrementing (for example: LABELS, LABELS2, LABELS3).";
%feature("docstring") btkMetaDataCollapseChildrenValuesInt "Convenient method to extract the values of a metadata as an array of integers.\nCollapse the parent children entries' values starting with the string 'baselabel' and incrementing (for example: LABELS, LABELS2, LABELS3).";
%feature("docstring") btkMetaDataCollapseChildrenValuesString "Convenient method to extract the values of a metadata as an array of strings.\nCollapse the parent children entries' values starting with the string 'baselabel' and incrementing (for example: LABELS, LABELS2, LABELS3).";

%feature("autodoc", "btkMetaDataCreateChild(parent , label , int )") btkMetaDataCreateChild(btkMetaData , const std::string& , int );
%feature("autodoc", "btkMetaDataCreateChild(parent , label , double )") btkMetaDataCreateChild(btkMetaData , const std::string& , double );
%feature("autodoc", "btkMetaDataCreateChild(parent , label , string )") btkMetaDataCreateChild(btkMetaData , const std::string& , const std::string& );
%feature("autodoc", "btkMetaDataCreateChild(parent , label , btkIntArray)") btkMetaDataCreateChild(btkMetaData , const std::string& , const std::vector<int>&);
%feature("autodoc", "btkMetaDataCreateChild(parent , label , btkDoubleArray)") btkMetaDataCreateChild(btkMetaData , const std::string& , const std::vector<double>&);
%feature("autodoc", "btkMetaDataCreateChild(parent , label , btkStringArray)") btkMetaDataCreateChild(btkMetaData , const std::string& , const std::vector<std::string>&);
%feature("autodoc", "btkMetaDataCreateChild(parent , label , btkIntArray , numCol)") btkMetaDataCreateChild(btkMetaData , const std::string& , const std::vector<int>& , int numCol);
%feature("autodoc", "btkMetaDataCreateChild(parent , label , btkDoubleArray , numCol)") btkMetaDataCreateChild(btkMetaData , const std::string& , const std::vector<double>& , int numCol);
%feature("autodoc", "btkMetaDataCreateChild(parent , label , btkStringArray , numCol)") btkMetaDataCreateChild(btkMetaData , const std::string& , const std::vector<std::string>& , int numCol);

%feature("docstring") btkMetaDataInfo "
Container class to store data of a MetaData object.

You can store integer value(s), real or string. This class has also methods to convert the native type into other, like integer into string.

Even if the data extracted are set as native Python type, the internal type are different and set as:
 - btk.btkMetaDataInfo.Char: Type for the string;
 - btk.btkMetaDataInfo.Byte: Signed integer type stored only on 8 bit. Possible values between -127 and 128;
 - btk.btkMetaDataInfo.Integer: Signed integer type stored only on 16 bit. Possible values between -32767 and 32768;
 - btk.btkMetaDataInfo.Real: Float type. Precision limited to 1e-5.
 
Note: The format btk.btkMetaDataInfo.Byte cannot be set using the method SetValues() do the impossibility to differentiate an integer. You need to use explicitly the method SetFormat()."

BTK_SWIG_AUTODOC_IMPL(MetaDataInfo, SetFormat, "SetFormat(self, btk.btkMetaDataInfo.Format)");

BTK_SWIG_DOCSTRING_IMPL(MetaDataInfo, GetDimension, "Returns the dimension for the specified index or 0 if the index is out of range.");
BTK_SWIG_DOCSTRING_IMPL(MetaDataInfo, SetDimension, "Sets the given value as the new dimension for the specified index or do nothing if the index is out of range. This method modify the values' structure and set default values ('0' or a string with white spaces).");
BTK_SWIG_DOCSTRING_IMPL(MetaDataInfo, GetDimensions, "Returns the dimensions associated with the values.");
BTK_SWIG_DOCSTRING_IMPL(MetaDataInfo, SetDimensions, "Sets the given array as the new dimensions and adapt the values and their structure. This method can add default values ('0' or a string with white spaces).");
BTK_SWIG_DOCSTRING_IMPL(MetaDataInfo, GetValueNumber, "Returns the number of values.");
BTK_SWIG_DOCSTRING_IMPL(MetaDataInfo, SetValue, "Sets the given value for the given index. This method does nothing if the index is out of range.");
BTK_SWIG_DOCSTRING_IMPL(MetaDataInfo, GetFormat, "Returns the internal format of the values.");
BTK_SWIG_DOCSTRING_IMPL(MetaDataInfo, GetFormatAsString, "Returns the format of the values as a string.");
BTK_SWIG_DOCSTRING_IMPL(MetaDataInfo, SetFormat, "Sets the format of the values and adapt the values. This method erases the values and set default values ('0' or a string with white spaces).");
BTK_SWIG_DOCSTRING_IMPL(MetaDataInfo, GetDimensionsProduct, "Compute the product of the dimensions from the given start index.");
BTK_SWIG_DOCSTRING_IMPL(MetaDataInfo, ResizeDimensions, "Resize the dimensions and adapt the values and their structure. If the number of dimensions is greater, then the new dimensions are equal to 1. If the number of dimensions is lower, then the value are shrunken.");
BTK_SWIG_DOCSTRING_IMPL(MetaDataInfo, ToString, "Convert stored value(s) into string(s).");
BTK_SWIG_DOCSTRING_IMPL(MetaDataInfo, ToInt, "Convert stored value(s) into integer(s).");
BTK_SWIG_DOCSTRING_IMPL(MetaDataInfo, ToDouble, "Convert stored value(s) into double(s).");
BTK_SWIG_DOCSTRING(MetaDataInfo, Clone, "Deep copy of the object");

// ------------------------------------------------------------------------- //
//                                  Acquisition                              //
// ------------------------------------------------------------------------- //

%feature("docstring") btkAcquisition "
Contains the data related to a biomechanical acquisition.

A biomechanical acquisition can contains:
  - 3D data (makers, points, forces, moments, ...) ;
  - analog channels (force platform, emg, ...) ;
  - events
  - acquisition's setup ;
  - subject's informations.
  
By default, the first frame is set to 1, the analog resolution is set to 12-bit, the maximum interpolation gap to 10 frames and the units are:
 - Marker: millimeters ;
 - Angle: degrees ;
 - Force: newtons ;
 - Moment: millimeter-newtons ;
 - Power: watts ;
 - Scalar: millimeters ;
 - Reaction: (nothing).
 
After the creation of the acquisition, you should use the Init() method to populate the acquisition. 
 
The member used for the maximum interpolation gap is only for information and is not used in the acquisition. It could be used later in a filter to fill gap.

The analog resolution can be set to the following values:
 - btk.btkAcquisition.Bit8 ;
 - btk.btkAcquisition.Bit10 ;
 - btk.btkAcquisition.Bit12 ;
 - btk.btkAcquisition.Bit14 ;
 - btk.btkAcquisition.Bit16."
  
BTK_SWIG_AUTODOC_IMPL(Acquisition, AppendAnalog, "AppendAnalog(self, btkAnalog)");
BTK_SWIG_AUTODOC_IMPL(Acquisition, AppendEvent, "AppendEvent(self, btkEvent)");
BTK_SWIG_AUTODOC_IMPL(Acquisition, AppendPoint, "AppendPoint(self, btkPoint)");
BTK_SWIG_AUTODOC_IMPL(Acquisition, FindAnalog, "FindAnalog(self, string) -> btkAnalogIterator");
BTK_SWIG_AUTODOC_IMPL(Acquisition, FindEvent, "FindEvent(self, string) -> btkEventIterator");
BTK_SWIG_AUTODOC_IMPL(Acquisition, FindPoint, "FindPoint(self, string) -> btkPointIterator");
BTK_SWIG_AUTODOC_IMPL(Acquisition, GetAnalog(int ), "GetAnalog(self, int) -> btkAnalog");
BTK_SWIG_AUTODOC_IMPL(Acquisition, GetAnalog(const std::string& ), "GetAnalog(self, string) -> btkAnalog");
BTK_SWIG_AUTODOC_IMPL(Acquisition, GetAnalogResolution, "GetAnalogResolution(self) -> btk.btkAcquisition.AnalogResolution");
BTK_SWIG_AUTODOC_IMPL(Acquisition, GetEvent, "GetEvent(self, int) -> btkEvent");
BTK_SWIG_AUTODOC_IMPL(Acquisition, GetPoint(int ), "GetPoint(self, int) -> btkPoint");
BTK_SWIG_AUTODOC_IMPL(Acquisition, GetPoint(const std::string& ), "GetPoint(self, string) -> btkPoint");
BTK_SWIG_AUTODOC_IMPL(Acquisition, GetPointUnit(Point::Type ), "GetPointUnit(self, btk.btkPoint.Type) -> string");
BTK_SWIG_AUTODOC_IMPL(Acquisition, Init(int , int ), "Init(self, pointNumber, frameNumber)");
BTK_SWIG_AUTODOC_IMPL(Acquisition, Init(int , int , int), "Init(self, pointNumber, frameNumber, analogNumber = 0)");
BTK_SWIG_AUTODOC_IMPL(Acquisition, Init(int , int , int, int), "Init(self, pointNumber, frameNumber, analogNumber = 0, , analogSampleNumberPerPointFrame = 1)");
BTK_SWIG_AUTODOC_IMPL(Acquisition, RemoveAnalog(AnalogIterator ), "RemoveAnalog(self, btkAnalogCollectionIterator) -> btkAnalogCollectionIterator");
BTK_SWIG_AUTODOC_IMPL(Acquisition, RemoveAnalog(int ), "RemoveAnalog(self, int)");
BTK_SWIG_AUTODOC_IMPL(Acquisition, RemoveAnalog(const std::string& ), "RemoveAnalog(self, string)");
BTK_SWIG_AUTODOC_IMPL(Acquisition, RemoveEvent(EventIterator ), "RemoveEvent(self, btkEventCollectionIterator) -> btkEventCollectionIterator");
BTK_SWIG_AUTODOC_IMPL(Acquisition, RemoveEvent(int ), "RemoveAnalog(self, int)");
BTK_SWIG_AUTODOC_IMPL(Acquisition, RemovePoint(PointIterator ), "RemovePoint(self, btkPointCollectionIterator) -> btkPointCollectionIterator");
BTK_SWIG_AUTODOC_IMPL(Acquisition, RemovePoint(int ), "RemovePoint(self, int)");
BTK_SWIG_AUTODOC_IMPL(Acquisition, RemovePoint(const std::string& ), "RemovePoint(self, string)");
BTK_SWIG_AUTODOC_IMPL(Acquisition, Resize(int , int ), "Resize(self, pointNumber, frameNumber)");
BTK_SWIG_AUTODOC_IMPL(Acquisition, Resize(int , int , int), "Resize(self, pointNumber, frameNumber, analogNumber = 0)");
BTK_SWIG_AUTODOC_IMPL(Acquisition, Resize(int , int , int, int), "Resize(self, pointNumber, frameNumber, analogNumber = 0, , analogSampleNumberPerPointFrame = 1)");
BTK_SWIG_AUTODOC_IMPL(Acquisition, ResizeAnalogNumber, "ResizeAnalogNumber(self, int)");
BTK_SWIG_AUTODOC_IMPL(Acquisition, ResizeFrameNumber, "ResizeFrameNumber(self, int)");
BTK_SWIG_AUTODOC_IMPL(Acquisition, ResizeFrameNumberFromEnd, "ResizeFrameNumberFromEnd(self, int)");
BTK_SWIG_AUTODOC_IMPL(Acquisition, ResizePointNumber, "ResizePointNumber(self, int)");
BTK_SWIG_AUTODOC_IMPL(Acquisition, SetAnalog, "SetAnalog(self, int, btkAnalog)");
BTK_SWIG_AUTODOC_IMPL(Acquisition, SetAnalogNumber, "SetAnalogNumber(self, int)");
BTK_SWIG_AUTODOC_IMPL(Acquisition, SetAnalogResolution, "SetAnalogResolution(self, btk.btkAcquisition.AnalogResolution)");
BTK_SWIG_AUTODOC_IMPL(Acquisition, SetAnalogs, "SetAnalogs(self, btkAnalogCollection)");
BTK_SWIG_AUTODOC_IMPL(Acquisition, SetEvent, "SetEvent(self, int, btkEvent)");
BTK_SWIG_AUTODOC_IMPL(Acquisition, SetEventNumber, "SetEventNumber(self, int)");
BTK_SWIG_AUTODOC_IMPL(Acquisition, SetEvents, "SetEvents(self, btkEventCollection)");
BTK_SWIG_AUTODOC_IMPL(Acquisition, SetFirstFrame, "SetFirstFrame(self, int)");
BTK_SWIG_AUTODOC_IMPL(Acquisition, SetMaxInterpolationGap, "SetMaxInterpolationGap(self, int)");
BTK_SWIG_AUTODOC_IMPL(Acquisition, SetMetaData, "SetMetaData(self, btkMetaData)");
BTK_SWIG_AUTODOC_IMPL(Acquisition, SetPoint, "SetPoint(self, int, btkPoint)");
BTK_SWIG_AUTODOC_IMPL(Acquisition, SetPointFrequency, "SetPointFrequency(self, double)");
BTK_SWIG_AUTODOC_IMPL(Acquisition, SetPointNumber, "SetPointNumber(self, int)");
BTK_SWIG_AUTODOC_IMPL(Acquisition, SetPoints, "SetPoints(self, btkPointCollection)");
BTK_SWIG_AUTODOC_IMPL(Acquisition, GetPointUnit(Point::Type), "GetPointUnit(self, btk.btkPoint.Type) -> std::string const");
BTK_SWIG_AUTODOC_IMPL(Acquisition, SetPointUnit(Point::Type , std::string), "SetPointUnit(self, btk.btkPoint.Type, string)");

BTK_SWIG_DOCSTRING_IMPL(Acquisition, BeginMetaData, "Returns an iterator to the beginning of the list of metadata.");
BTK_SWIG_DOCSTRING_IMPL(Acquisition, EndMetaData, "Returns an iterator just past the last metadata.");
BTK_SWIG_DOCSTRING_IMPL(Acquisition, GetMetaData, "Returns the metadata.");
BTK_SWIG_DOCSTRING_IMPL(Acquisition, SetMetaData, "Sets the metadata of the acquisition.");
BTK_SWIG_DOCSTRING_IMPL(Acquisition, BeginEvent, "Returns an iterator to the beginning of the list of events.");
BTK_SWIG_DOCSTRING_IMPL(Acquisition, EndEvent, "Returns a const iterator just past the last event.");
BTK_SWIG_DOCSTRING_IMPL(Acquisition, GetEvent, "Gets the event at the given index.");
BTK_SWIG_DOCSTRING_IMPL(Acquisition, SetEvent, "Sets the content of event at the given index.");
BTK_SWIG_DOCSTRING_IMPL(Acquisition, GetEvents, "Returns the collection of events.");
BTK_SWIG_DOCSTRING_IMPL(Acquisition, SetEvents, "Sets events for this acquisition.");
BTK_SWIG_DOCSTRING_IMPL(Acquisition, IsEmptyEvent, "Checks if the events' list is empty.");
BTK_SWIG_DOCSTRING_IMPL(Acquisition, GetEventNumber, "Returns the number of events.");
BTK_SWIG_DOCSTRING_IMPL(Acquisition, SetEventNumber, "Sets the number of events.");
BTK_SWIG_DOCSTRING_IMPL(Acquisition, ClearEvents, "Clear events.");
BTK_SWIG_DOCSTRING_IMPL(Acquisition, FindEvent, "Finds the event with the proposed label and returns the iterator associated with it.\nIf no event has the given label, an iterator pointing to the end of the collection is returned.");
BTK_SWIG_DOCSTRING_IMPL(Acquisition, AppendEvent, "Appends the given event in the acquisition.");
BTK_SWIG_DOCSTRING_IMPL(Acquisition, RemoveEvent, "Removes the event associated with the given index or the given iterator.");
BTK_SWIG_DOCSTRING_IMPL(Acquisition, BeginPoint, "Returns an iterator to the beginning of the list of points.");
BTK_SWIG_DOCSTRING_IMPL(Acquisition, EndPoint, "Returns an iterator just past the last point.");
BTK_SWIG_DOCSTRING_IMPL(Acquisition, GetPoint, "Gets the point at the given index or label. If no Point exists, then an exception is thrown.");
BTK_SWIG_DOCSTRING_IMPL(Acquisition, SetPoint, "Sets the content of a point at the given index.");
BTK_SWIG_DOCSTRING_IMPL(Acquisition, GetPoints, "Returns the collection of points.");
BTK_SWIG_DOCSTRING_IMPL(Acquisition, SetPoints, "Sets points for this acquisition.");
BTK_SWIG_DOCSTRING_IMPL(Acquisition, IsEmptyPoint, "Checks if the points' list is empty.");
BTK_SWIG_DOCSTRING_IMPL(Acquisition, GetPointNumber, "Returns the number of points.");
BTK_SWIG_DOCSTRING_IMPL(Acquisition, SetPointNumber, "Sets the number of points.");
BTK_SWIG_DOCSTRING_IMPL(Acquisition, ClearPoints, "Clear points.");
BTK_SWIG_DOCSTRING_IMPL(Acquisition, FindPoint, "Finds the point with the proposed label and returns the iterator associated with it.\nIf no point has the given label, an iterator pointing to the end of the collection is returned.");
BTK_SWIG_DOCSTRING_IMPL(Acquisition, AppendPoint, "Convenient method to append a point in the acquisition. This method also resizes the frame number of the inserted point if necessary.");
BTK_SWIG_DOCSTRING_IMPL(Acquisition, RemovePoint, "Convenient method to delete a point from the acquisition.");
BTK_SWIG_DOCSTRING_IMPL(Acquisition, BeginAnalog, "Returns an iterator to the beginning of the list of analog channels.");
BTK_SWIG_DOCSTRING_IMPL(Acquisition, EndAnalog, "Returns an iterator just past the last analog channel.");
BTK_SWIG_DOCSTRING_IMPL(Acquisition, IsEmptyAnalog, "Checks if the analogs' list is empty.");
BTK_SWIG_DOCSTRING_IMPL(Acquisition, GetAnalogNumber, "Returns the number of analog channels.");
BTK_SWIG_DOCSTRING_IMPL(Acquisition, SetAnalogNumber, "Sets the number of analog channels.");
BTK_SWIG_DOCSTRING_IMPL(Acquisition, GetAnalog, "Gets the analog channel at the given index or label. If no Analog exists, then an exception is thrown.");
BTK_SWIG_DOCSTRING_IMPL(Acquisition, SetAnalog, "Sets the analog channel at the given index by the content of the given analog channel.");
BTK_SWIG_DOCSTRING_IMPL(Acquisition, GetAnalogs, "Returns the collection of analog channels.");
BTK_SWIG_DOCSTRING_IMPL(Acquisition, SetAnalogs, "Sets analog channels for this acquisition.");
BTK_SWIG_DOCSTRING_IMPL(Acquisition, ClearAnalogs, "Clear analogs channels.");
BTK_SWIG_DOCSTRING_IMPL(Acquisition, FindAnalog, "Finds the analog channel with the proposed label and returns the iterator associated with it.\nIf no analog channel has the given label, an iterator pointing to the end of the collection is returned.");
BTK_SWIG_DOCSTRING_IMPL(Acquisition, AppendAnalog, "Append the analog channel ac in the acquisition This method also resizes the frame number of the inserted analog channel if necessary.");
BTK_SWIG_DOCSTRING_IMPL(Acquisition, RemoveAnalog, "Convenient method to delete an analog channel from the acquisition.");
BTK_SWIG_DOCSTRING_IMPL(Acquisition, Init, "Initialize the acquisition with the given number of point which have each the given number of frames.\nYou can set also the number of analog channels and their number of frames corresponds to the given integer factor multiplied by the number of frames for the points.\nThis method label added points and analog channels using the string 'uname*' concatenated with the index of the channel.");
BTK_SWIG_DOCSTRING_IMPL(Acquisition, Resize, "Resize the acquisition. Same as the method Init(), but does not label added points and analog channels");
BTK_SWIG_DOCSTRING_IMPL(Acquisition, ResizePointNumber, "Resize the number of points.");
BTK_SWIG_DOCSTRING_IMPL(Acquisition, ResizeAnalogNumber, "Resize the number of analog channels.");
BTK_SWIG_DOCSTRING_IMPL(Acquisition, ResizeFrameNumber, "Resize the number of frames.");
BTK_SWIG_DOCSTRING_IMPL(Acquisition, ResizeFrameNumberFromEnd, "Resize the number of frames by adding new (or removing) frames at the beginning of the acquisition and set automatically the new first frame index.");
BTK_SWIG_DOCSTRING_IMPL(Acquisition, Reset, "Resets the acquisition as an empty acquisition.\nTo re-populate this acquisition, you need to re-use the Init() method to set the point and analog number and their frame number.");
BTK_SWIG_DOCSTRING_IMPL(Acquisition, GetDuration, "Returns the duration of the acquisition. The duration is computed as the multiplication of the points' frequency with the points frame number.");
BTK_SWIG_DOCSTRING_IMPL(Acquisition, GetFirstFrame, "Return the first frame index of the acquisition.");
BTK_SWIG_DOCSTRING_IMPL(Acquisition, SetFirstFrame, "Sets the first frame index.");
BTK_SWIG_DOCSTRING_IMPL(Acquisition, GetLastFrame, "Returns the last frame index of the acquisition base on the first frame index and the frame number.");
BTK_SWIG_DOCSTRING_IMPL(Acquisition, GetPointUnit, "Returns the unit for points which have the given type.");
BTK_SWIG_DOCSTRING_IMPL(Acquisition, SetPointUnit, "Sets the point's unit for the given point's type. The type btk.btkPoint.Reaction cannot have any unit. You cannot set it.");
BTK_SWIG_DOCSTRING_IMPL(Acquisition, GetPointFrameNumber, "Returns the number of frames for the points.");
BTK_SWIG_DOCSTRING_IMPL(Acquisition, GetPointFrequency, "Returns the point's frequency.");
BTK_SWIG_DOCSTRING_IMPL(Acquisition, SetPointFrequency, "Sets the points sample rate.");
BTK_SWIG_DOCSTRING_IMPL(Acquisition, GetAnalogFrameNumber, "Returns the number of frames for the analog channels.");
BTK_SWIG_DOCSTRING_IMPL(Acquisition, GetNumberAnalogSamplePerFrame, "Returns the number of analog sample (acquired by each channel) per point frame.");
BTK_SWIG_DOCSTRING_IMPL(Acquisition, GetAnalogFrequency, "Returns the analogs sample rate.");
BTK_SWIG_DOCSTRING_IMPL(Acquisition, GetAnalogResolution, "Returns the analog resolution.");
BTK_SWIG_DOCSTRING_IMPL(Acquisition, SetAnalogResolution, "Sets the analog resolution.");
BTK_SWIG_DOCSTRING_IMPL(Acquisition, GetMaxInterpolationGap, "Gets the maximum gap length that any interpolation method would fill for the 3D point data.\nWarning: Only for information. Could be use later by a filter to fill gap 3D trajectories.");
BTK_SWIG_DOCSTRING_IMPL(Acquisition, SetMaxInterpolationGap, "Sets the maximum gap length that any interpolation method would fill for the 3D point data.\nWarning: Only for information. Could be use later by a filter to fill gap 3D trajectories.");
BTK_SWIG_DOCSTRING(Acquisition, Clone, "Deep copy of the object");

// ------------------------------------------------------------------------- //
//                               ProcessObject                               //
// ------------------------------------------------------------------------- //

%feature("docstring") btkProcessObject "
Interface to create a filter/process in a pipeline, cannot be instanced."

BTK_SWIG_DOCSTRING_IMPL(ProcessObject, GetInputNumber, "Returns the number of inputs.");
BTK_SWIG_DOCSTRING_IMPL(ProcessObject, GetValidInputNumber, "Returns the number of inputs which are valid (i.e. not null).");
BTK_SWIG_DOCSTRING_IMPL(ProcessObject, GetOutputNumber, "Returns the number of outputs.");
BTK_SWIG_DOCSTRING_IMPL(ProcessObject, ResetState, "Reset the state of the process. Usefull when an exception was thrown during the generation of the data.");
BTK_SWIG_DOCSTRING_IMPL(ProcessObject, Update, "Update the pipeline and then all the data inside. Recursive method which\n 1) determines the processes to update\n 2) generate the data by using the GenerateData() method.");
BTK_SWIG_DOCSTRING_IMPL(ProcessObject, GetTimestamp, "Returns the timestamp of the object.");