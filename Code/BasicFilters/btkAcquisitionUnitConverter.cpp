/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009, Arnaud Barré
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

#include "btkAcquisitionUnitConverter.h"
#include "btkMetaData.h"
#include "btkConvert.h"

namespace btk
{
  /**
   * @class AcquisitionUnitConverter
   * @brief Convert the units of the given input for points and analog channels.
   *
   * This filter can convert units for trajectory, force, moment, angle and power.
   * The known unit for the trajectory (length) are:
   *  - meter (m);
   *  - decimeter (dm);
   *  - centimeter (cm)
   *  - millimeter (mm)
   *  - inch (in)
   * The known unit for the force are:
   *  - newton (N);
   * The known unit for the moment are:
   *  - newton by meter (Nm);
   *  - newton by decimeter (Ndm);
   *  - newton by centimeter (Ncm);
   *  - newton by millimeter (Nmm);
   *  - newton by inch (Nin);
   * The known unit for the angle are:
   *  - radian (rad);
   *  - degree (deg);
   * The known unit for the power are:
   *  - watt (W);
   * Moreover, the metadata known to be influenced by these unit (FORCE_PLATFORM, 
   * SEG) are also converted. The metadata POINT:*UNITS are not updated as they are not
   * used in BTK (instead, you can use the methods btk::Acquisition::GetPointUnit()).
   *
   * Use the method AcquisitionUnitConverter::SetUnit() to selected the wanted units.
   *
   * By default, the data will be converted in millimeter, newton, newton by millimeter, 
   * degree and watt. Even if you could convert trajectories and moments with the desired 
   * units, it is recommended to use the same length unit (i.e. mm and Nmm or m with Nm).
   *
   * @ingroup BTKBasicFilters
   */
   
   const char* AcquisitionUnitConverter::LengthUnit[5] = {"m", "dm", "cm", "mm", "in"};
   const double AcquisitionUnitConverter::LengthScale[25] = {
     // m      dm     cm      mm    in
     1.0,   10.0, 100.0, 1000.0, 39.370079, // m
     0.1,    1.0,  10.0,  100.0,  3.9370079, // dm
     0.01,   0.1,   1.0,   10.0,  0.39370079, // cm
     0.001,  0.01,  0.1,    1.0,  0.039370079, // mm
     0.0254, 0.254, 2.54,  25.4,  1.0, // in
   };
   const char* AcquisitionUnitConverter::AngleUnit[2] = {"deg", "rad"};
   const double AcquisitionUnitConverter::AngleScale[4] = {
     // deg               rad
      1.0,               1.745329251994e-02, // deg
     57.295779513082322, 1.0, // rad
   };
   const char* AcquisitionUnitConverter::ForceUnit[1] = {"N"};
   const double AcquisitionUnitConverter::ForceScale[1] = {1.0};
   const char* AcquisitionUnitConverter::MomentUnit[5] = {"Nm", "Ndm", "Ncm", "Nmm", "Nin"};
   const double AcquisitionUnitConverter::MomentScale[25] = {
     // Nm      Ndm     Ncm      Nmm    Nin
     1.0,   10.0, 100.0, 1000.0, 39.370079, // Nm
     0.1,    1.0,  10.0,  100.0,  3.9370079, // Ndm
     0.01,   0.1,   1.0,   10.0,  0.39370079, // Ncm
     0.001,  0.01,  0.1,    1.0,  0.039370079, // Nmm
     0.0254, 0.254, 2.54,  25.4,  1.0, // Nin
   };
   const char* AcquisitionUnitConverter::PowerUnit[1] = {"W"};
   const double AcquisitionUnitConverter::PowerScale[1] = {1.0};
   const char* AcquisitionUnitConverter::VoltageUnit[2] = {"V", "mV"};
   const double AcquisitionUnitConverter::VoltageScale[4] = {
     // V                mV
     1.0,               1000.0, // V
     0.001,             1.0, // mV
   };
   
  /**
   * @enum AcquisitionUnitConverter::Unit
   * Enums used to specify the point's type.
   */
  /**
   * @var AcquisitionUnitConverter::Type AcquisitionUnitConverter::Length
   * Enum value which represent the length unit.
   */
  /**
   * @var AcquisitionUnitConverter::Type AcquisitionUnitConverter::Angle
   * Enum value which represent the angle unit.
   */
  /**
   * @var AcquisitionUnitConverter::Type AcquisitionUnitConverter::Force
   * Enum value which represent the force unit.
   */
  /**
   * @var AcquisitionUnitConverter::Type AcquisitionUnitConverter::Moment
   * Enum value which represent the moment unit.
   */
  /**
   * @var AcquisitionUnitConverter::Type AcquisitionUnitConverter::Power
   * Enum value which represent the power unit.
   */
  
  /**
   * @typedef AcquisitionUnitConverter::Pointer
   * Smart pointer associated with a AcquisitionUnitConverter object.
   */
  
  /**
   * @typedef AcquisitionUnitConverter::ConstPointer
   * Smart pointer associated with a const AcquisitionUnitConverter object.
   */
    
  /**
   * @fn static Pointer AcquisitionUnitConverter::New();
   * Creates a smart pointer associated with a AcquisitionUnitConverter object.
   */

  /**
   * @fn Acquisition::Pointer AcquisitionUnitConverter::GetInput()
   * Gets the input registered with this process.
   */
  
  /**
   * @fn void AcquisitionUnitConverter::SetInput(Acquisition::Pointer input)
   * Sets the input required with this process.
   */
  
  /**
   * @fn Acquisition::Pointer AcquisitionUnitConverter::GetOutput()
   * Gets the output created with this process.
   */
   
  /**
   * Set the unit @a u with the string @a s.
   */
  void AcquisitionUnitConverter::SetUnit(Unit u, const std::string& s)
  {
    if (this->m_Units[u].compare(s) == 0)
      return;
    this->m_Units[u] = s;
    this->Modified();
  };
  
  /**
   * @fn const std::string& AcquisitionUnitConverter::GetUnit(Unit u)
   * Return the string corresponding to the unit @a u.
   */
  
  /**
   * Constructor. Sets the number of inputs to 2 and outputs to 1.
   */
  AcquisitionUnitConverter::AcquisitionUnitConverter()
  : ProcessObject(), m_Units(std::vector<std::string>(6,""))
  {
    this->SetInputNumber(1);
    this->SetOutputNumber(1);
    
    this->m_Units[Length] = "mm";
    this->m_Units[Angle] = "deg";
    this->m_Units[Force] = "N";
    this->m_Units[Moment] = "Nmm";
    this->m_Units[Power] = "W";
    //this->m_Units[Scalar] = "";
  };
  
  /**
   * @fn Acquisition::Pointer AcquisitionUnitConverter::GetInput(int idx)
   * Returns the input at the index @a idx.
   */
  
  /**
   * @fn Acquisition::Pointer AcquisitionUnitConverter::GetOutput(int idx)
   * Gets the output created with this process at the index @a idx.
   */
  
  /**
   * Creates a Acquisition::Pointer object and return it as a DataObject::Pointer.
   */
  DataObject::Pointer AcquisitionUnitConverter::MakeOutput(int /* idx */)
  {
    return Acquisition::New();
  };
  
  /**
   * Generates the outputs' data.
   */
  void AcquisitionUnitConverter::GenerateData()
  {
    Acquisition::Pointer output = this->GetOutput();
    output->Reset();
    
    output->SetPointUnit(Point::Marker, this->m_Units[Point::Marker]);
    output->SetPointUnit(Point::Angle, this->m_Units[Point::Angle]);
    output->SetPointUnit(Point::Force, this->m_Units[Point::Force]);
    output->SetPointUnit(Point::Moment, this->m_Units[Point::Moment]);
    output->SetPointUnit(Point::Power, this->m_Units[Point::Power]);
    
    Acquisition::Pointer input = this->GetInput();
    if (input.get() == 0)
      return;
    
    int indexesOut[5]; // Not necessary for Scalar.
    double scales[6] = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
    
    const int numLength = sizeof(AcquisitionUnitConverter::LengthUnit) / sizeof(char*);
    const int numAngle = sizeof(AcquisitionUnitConverter::AngleUnit) / sizeof(char*);
    const int numForce = sizeof(AcquisitionUnitConverter::ForceUnit) / sizeof(char*);
    const int numMoment = sizeof(AcquisitionUnitConverter::MomentUnit) / sizeof(char*);
    const int numPower = sizeof(AcquisitionUnitConverter::PowerUnit) / sizeof(char*);
    const int numVoltage = sizeof(AcquisitionUnitConverter::VoltageUnit) / sizeof(char*);

    // Marker
    if (!this->ConvertUnit(&scales[Length], 
                           numLength,
                           AcquisitionUnitConverter::LengthUnit,
                           AcquisitionUnitConverter::LengthScale,
                           output->GetPointUnit(Point::Marker),
                           input->GetPointUnit(Point::Marker)))
    {
      btkErrorMacro("Unknown unit for the markers.");
      return;
    }
    this->CheckUnit(&indexesOut[Length], numLength, AcquisitionUnitConverter::LengthUnit, this->m_Units[Length]);
    // Angle
    if (!this->ConvertUnit(&scales[Angle],
                           numAngle,
                           AcquisitionUnitConverter::AngleUnit,
                           AcquisitionUnitConverter::AngleScale, 
                           output->GetPointUnit(Point::Angle), 
                           input->GetPointUnit(Point::Angle)))
    {
      btkErrorMacro("Unknown unit for the angles.");
      return;
    }
    this->CheckUnit(&indexesOut[Angle], numLength, AcquisitionUnitConverter::AngleUnit, this->m_Units[Angle]);
    // Force
    if (!this->ConvertUnit(&scales[Force], 
                           numForce,
                           AcquisitionUnitConverter::ForceUnit, 
                           AcquisitionUnitConverter::ForceScale, 
                           output->GetPointUnit(Point::Force), 
                           input->GetPointUnit(Point::Force)))
    {
      btkErrorMacro("Unknown unit for the forces.");
      return;
    }
    this->CheckUnit(&indexesOut[Force], numForce, AcquisitionUnitConverter::ForceUnit, this->m_Units[Force]);
    // Moment
    if (!this->ConvertUnit(&scales[Moment],
                           numMoment,
                           AcquisitionUnitConverter::MomentUnit,
                           AcquisitionUnitConverter::MomentScale,
                           output->GetPointUnit(Point::Moment),
                           input->GetPointUnit(Point::Moment)))
    {
      btkErrorMacro("Unknown unit for the moments.");
      return;
    }
    this->CheckUnit(&(indexesOut[Moment]), numMoment, AcquisitionUnitConverter::MomentUnit, this->m_Units[Moment]);
    // Power
    if (!this->ConvertUnit(&scales[Power], 
                           numPower,
                           AcquisitionUnitConverter::PowerUnit,
                           AcquisitionUnitConverter::PowerScale,
                           output->GetPointUnit(Point::Power), 
                           input->GetPointUnit(Point::Power)))
    {
      btkErrorMacro("Unknown unit for the powers.");
      return;
    }
    this->CheckUnit(&indexesOut[Power], numPower, AcquisitionUnitConverter::PowerUnit, this->m_Units[Power]);
    // Scalar
    int idxIn;
    std::string scalarUnit = input->GetPointUnit(Point::Scalar);
    if (this->CheckUnit(&idxIn, numLength, AcquisitionUnitConverter::LengthUnit, scalarUnit))
    {
      scales[5] = AcquisitionUnitConverter::LengthScale[idxIn * numLength + indexesOut[Length]];
      output->SetPointUnit(Point::Scalar, this->m_Units[Length]);
    }
    else if (this->CheckUnit(&idxIn, numAngle, AcquisitionUnitConverter::AngleUnit, scalarUnit))
    {
      scales[5] = AcquisitionUnitConverter::AngleScale[idxIn * numAngle + indexesOut[Angle]];
      output->SetPointUnit(Point::Scalar, this->m_Units[Angle]);
    }
    else if (this->CheckUnit(&idxIn, numForce, AcquisitionUnitConverter::ForceUnit, scalarUnit))
    {
      scales[5] = AcquisitionUnitConverter::ForceScale[idxIn * numForce + indexesOut[Force]];
      output->SetPointUnit(Point::Scalar, this->m_Units[Force]);
    }
    else if (this->CheckUnit(&idxIn, numMoment, AcquisitionUnitConverter::MomentUnit, scalarUnit))
    {
      scales[5] = AcquisitionUnitConverter::MomentScale[idxIn * numMoment + indexesOut[Moment]];
      output->SetPointUnit(Point::Scalar, this->m_Units[Moment]);
    }
    else if (this->CheckUnit(&idxIn, numPower, AcquisitionUnitConverter::PowerUnit, scalarUnit))
    {
      scales[5] = AcquisitionUnitConverter::PowerScale[idxIn * numPower + indexesOut[Power]];
      output->SetPointUnit(Point::Scalar, this->m_Units[Power]);
    }
    else
      output->SetPointUnit(Point::Scalar, scalarUnit);
    
    // Point conversion
    for (Acquisition::PointConstIterator it = input->BeginPoint() ; it != input->EndPoint() ; ++it)
    {
      btk::Point::Pointer p = (*it)->Clone();
      if (p->GetType() < 6)
        p->GetValues() *= scales[p->GetType()];
      else if (p->GetType() == 6) // Reaction: Force, Moment and Position
      {
        std::string suffix = p->GetLabel().substr(2, p->GetLabel().length()-2);
        if (suffix.compare(".F") == 0)
          p->GetValues() *= scales[Force];
        else if (suffix.compare(".M") == 0)
          p->GetValues() *= scales[Moment];
        else
          p->GetValues() *= scales[Length];
      }
      output->AppendPoint(p);
    }
    
    // Metadata + FP
    std::vector<int> channelsToNoScale;
    output->SetMetaData(input->GetMetaData()->Clone());
    MetaData::Pointer md = output->GetMetaData();
    MetaData::Iterator groupIt;
    MetaData::Iterator parameterIt;
    if ((scales[Length] != 1.0) || (scales[Force] != 1.0) || (scales[Moment] != 1.0))
    {
      // Convert known metdata which can influence on the computation of other parameters:
      //  - SEG:MARKER_DIAMETER
      //  - FORCE_PLATFORM:CORNERS
      //  - FORCE_PLATFORM:ORIGIN
      //  - FORCE_PLATFORM:CAL_MATRIX
      // All the units are also updated.
      // Metadata POINT:POWER_UNITS, POINT:FORCE_UNITS, etc. are not updated as they are not used
      // in BTK (instead, you can use btk::Acquisition::GetPointUnit() methods).
      groupIt = md->FindChild("SEG");
      if (groupIt != md->End())
      {
        parameterIt = (*groupIt)->FindChild("MARKER_DIAMETER");
        if (parameterIt != (*groupIt)->End())
          (*parameterIt)->GetInfo()->SetValues(static_cast<float>((*parameterIt)->GetInfo()->ToDouble(0) * scales[Length]));
      }
      groupIt = md->FindChild("FORCE_PLATFORM");
      if (groupIt != md->End())
      {
        std::vector<float> values;
        parameterIt = (*groupIt)->FindChild("CORNERS");
        if (parameterIt != (*groupIt)->End())
        {
          (*parameterIt)->GetInfo()->ToFloat(values);
          for (int i = 0 ; i < static_cast<int>(values.size()) ; ++i)
            values[i] = static_cast<float>(values[i] * scales[Length]);
          (*parameterIt)->GetInfo()->SetValues((*parameterIt)->GetInfo()->GetDimensions(), values);
        }
        parameterIt = (*groupIt)->FindChild("ORIGIN");
        if (parameterIt != (*groupIt)->End())
        {
          (*parameterIt)->GetInfo()->ToFloat(values);
          for (int i = 0 ; i < static_cast<int>(values.size()) ; ++i)
            values[i] = static_cast<float>(values[i] * scales[Length]);
          (*parameterIt)->GetInfo()->SetValues((*parameterIt)->GetInfo()->GetDimensions(), values);
        }
        parameterIt = (*groupIt)->FindChild("CAL_MATRIX");
        if (parameterIt != (*groupIt)->End())
        {
          MetaData::Iterator usedIt = (*groupIt)->FindChild("USED");
          MetaData::Iterator typeIt = (*groupIt)->FindChild("TYPE");
          MetaData::Iterator channelIt = (*groupIt)->FindChild("CHANNEL");
          if ((usedIt != (*groupIt)->End()) && (typeIt != (*groupIt)->End()) && (channelIt != (*groupIt)->End()))
          {
            (*parameterIt)->GetInfo()->ToFloat(values);
            std::vector<uint8_t> dims = (*parameterIt)->GetInfo()->GetDimensions();
            int total = (*parameterIt)->GetInfo()->GetDimensionsProduct();
            int used = (*usedIt)->GetInfo()->ToInt(0);
            std::vector<int> types, channels;
            (*typeIt)->GetInfo()->ToInt(types);
            (*channelIt)->GetInfo()->ToInt(channels);
            int num = 0;
            if (dims.size() <= 1)
            {
              btkErrorMacro("The metadata FORCE_PLATFORM:CAL_MATRIX is corrupted. Not enough dimension.");
            }
            else
            {
              if (dims.size() < 3)
                num = 1;
              else
                num = dims[2];
              if (num > static_cast<int>(types.size()))
                num = types.size();
              int valuesStep = (*parameterIt)->GetInfo()->GetDimension(0) * (*parameterIt)->GetInfo()->GetDimension(1);
              int columnsStep = (*parameterIt)->GetInfo()->GetDimension(0);
              bool corrupted = false;
              for (int i = 0 ; i < num ; ++i)
              {
                if ((types[i] != 1) && (types[i] != 2) && (types[i] != 3))
                {
                  std::vector<int>::iterator start = channels.begin();
                  std::advance(start, i * columnsStep);
                  std::vector<int>::iterator stop = channels.begin();
                  std::advance(stop, (i+1) * columnsStep);
                  channelsToNoScale.insert(channelsToNoScale.end(), start, stop);
                }
                switch (types[i])
                {
                case 1:
                  {
                  const double sf[6] = {scales[Force], scales[Force], scales[Force], scales[Length], scales[Length], scales[Moment]};
                  corrupted = !this->ConvertCalMatrix(values, i*valuesStep, total, columnsStep, 6, 6, sf);
                  }
                  break;
                case 2:
                case 4:
                  {
                  const double sf[6] = {scales[Force], scales[Force], scales[Force], scales[Moment], scales[Moment], scales[Moment]};
                  corrupted = !this->ConvertCalMatrix(values, i*valuesStep, total, columnsStep, 6, 6, sf);
                  }
                  break;
                case 5:
                  {
                  const double sf[6] = {scales[Force], scales[Force], scales[Force], scales[Moment], scales[Moment], scales[Moment]};
                  corrupted = !this->ConvertCalMatrix(values, i*valuesStep, total, columnsStep, 6, 8, sf);
                  }
                  break;
                case 3:
                case 7:
                  {
                  const double sf[8] = {scales[Force], scales[Force], scales[Force], scales[Force], scales[Force], scales[Force], scales[Force], scales[Force]};
                  corrupted = !this->ConvertCalMatrix(values, i*valuesStep, total, columnsStep, 8, 8, sf);
                  }
                  break;
                case 6:
                  {
                  const double sf[12] = {scales[Force], scales[Force], scales[Force], scales[Force], scales[Force], scales[Force], scales[Force], scales[Force], scales[Force], scales[Force], scales[Force], scales[Force]};
                  corrupted = !this->ConvertCalMatrix(values, i*valuesStep, total, columnsStep, 12, 12, sf);
                  }
                case 11:
                  btkErrorMacro("Force Platform type 11 is not yet supported. Please, report this to the developers");
                  break;
                case 12:
                  btkErrorMacro("Force Platform type 12 is not yet supported. Please, report this to the developers");
                  break;
                case 21:
                  btkErrorMacro("Force Platform type 21 is not yet supported. Please, report this to the developers");
                  break;
                }
              }
              if (corrupted)
              {
                btkErrorMacro("Not enough values for the metadata FORCE_PLATFORM:CAL_MATRIX. Impossible to scale the data correctly.");
              }
              else
                (*parameterIt)->GetInfo()->SetValues((*parameterIt)->GetInfo()->GetDimensions(), values);
            }
          }
        }
      }
    }
    
    // Analog conversion
    int idxChannel = 0;
    for (Acquisition::AnalogConstIterator it = input->BeginAnalog() ; it != input->EndAnalog() ; ++it)
    {
      double s = 1.0;
      btk::Analog::Pointer ac = (*it)->Clone();
      // Check the unit to discover the type
      // int idxIn; // Already declared for the scalar.
      if (this->CheckUnit(&idxIn, numLength, AcquisitionUnitConverter::LengthUnit, (*it)->GetUnit()))
      {
        s = AcquisitionUnitConverter::LengthScale[idxIn * numLength + indexesOut[Length]];
        ac->SetUnit(this->m_Units[Length]);
      }
      else if (this->CheckUnit(&idxIn, numAngle, AcquisitionUnitConverter::AngleUnit, (*it)->GetUnit()))
      {
        s = AcquisitionUnitConverter::AngleScale[idxIn * numAngle + indexesOut[Angle]];
        ac->SetUnit(this->m_Units[Angle]);
      }
      else if (this->CheckUnit(&idxIn, numForce, AcquisitionUnitConverter::ForceUnit, (*it)->GetUnit()))
      {
        // Check if this channel belongs to a force platform using a calibration matrix.
        bool noScale = false;
        for (int i = 0 ; i < static_cast<int>(channelsToNoScale.size()) ; ++i)
        {
          if (channelsToNoScale[i] == idxChannel)
          {
            noScale = true;
            break;
          }
        }
        if (noScale)
          s = 1.0;
        else
          s = AcquisitionUnitConverter::ForceScale[idxIn * numForce + indexesOut[Force]];
        ac->SetUnit(this->m_Units[Force]);
      }
      else if (this->CheckUnit(&idxIn, numMoment, AcquisitionUnitConverter::MomentUnit, (*it)->GetUnit()))
      {
        // Check if this channel belongs to a force platform using a calibration matrix.
        bool noScale = false;
        for (int i = 0 ; i < static_cast<int>(channelsToNoScale.size()) ; ++i)
        {
          if (channelsToNoScale[i] == idxChannel)
          {
            noScale = true;
            break;
          }
        }
        if (noScale)
          s = 1.0;
        else
          s = AcquisitionUnitConverter::MomentScale[idxIn * numMoment + indexesOut[Moment]];
        ac->SetUnit(this->m_Units[Moment]);
      }
      else if (this->CheckUnit(&idxIn, numPower, AcquisitionUnitConverter::PowerUnit, (*it)->GetUnit()))
      {
        s = AcquisitionUnitConverter::PowerScale[idxIn * numPower + indexesOut[Power]];
        ac->SetUnit(this->m_Units[Power]);
      }
      else if (this->CheckUnit(&idxIn, numVoltage, AcquisitionUnitConverter::VoltageUnit, (*it)->GetUnit()))
      {
        s = 1.0;
      }
      else
        btkErrorMacro("Unknown analog channel's unit: '"+ (*it)->GetUnit() + "'. Impossible to scale its data.");
      ac->GetValues() *= s;
      ac->SetScale(ac->GetScale() * s);
      output->AppendAnalog(ac);
      ++idxChannel;
    }
    
    output->SetFirstFrame(input->GetFirstFrame());
    output->SetAnalogResolution(input->GetAnalogResolution());
    output->SetPointFrequency(input->GetPointFrequency());
    output->Resize(input->GetPointNumber(), input->GetPointFrameNumber(), input->GetAnalogNumber(), input->GetNumberAnalogSamplePerFrame());
    
    // No conversion for the events
    output->SetEvents(input->GetEvents()->Clone());
  };
  
  bool AcquisitionUnitConverter::ConvertUnit(double* scale, int num, const char** units, const double* scales, const std::string& out, const std::string& in)
  {
    int idxIn = -1, idxOut = -1;
    if ((!this->CheckUnit(&idxIn, num, units, in)) || (!this->CheckUnit(&idxOut, num, units, out)))
      return false;
    *scale = scales[idxIn * num + idxOut];
    return true;
  };
  
  bool AcquisitionUnitConverter::CheckUnit(int* idx, int num, const char** units, const std::string& unit)
  {
    *idx = -1;
    for (int i = 0 ; i < num ; ++i)
    {
      if (unit.compare(units[i]) == 0)
      {
        *idx = i;
        return true;
      }
    }
    return false;
  };
};
