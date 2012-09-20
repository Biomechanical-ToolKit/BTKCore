import btk
import unittest
import _TDDConfigure

class AcquisitionUnitConverterTest(unittest.TestCase):
    def test_NoInputNoConversion(self):
        uc = btk.btkAcquisitionUnitConverter()
        uc.Update()
        output = uc.GetOutput()
        self.assertEqual(output.GetPointUnit(btk.btkPoint.Marker), 'mm')
        self.assertEqual(output.GetPointUnit(btk.btkPoint.Angle), 'deg')
        self.assertEqual(output.GetPointUnit(btk.btkPoint.Force), 'N')
        self.assertEqual(output.GetPointUnit(btk.btkPoint.Moment), 'Nmm')
        self.assertEqual(output.GetPointUnit(btk.btkPoint.Power), 'W')

    def test_NoInputConversionToMeter(self):
        uc = btk.btkAcquisitionUnitConverter()
        uc.SetUnit(btk.btkAcquisitionUnitConverter.Length, 'm')
        uc.SetUnit(btk.btkAcquisitionUnitConverter.Moment, 'Nm')
        uc.Update()
        output = uc.GetOutput()
        self.assertEqual(output.GetPointUnit(btk.btkPoint.Marker), 'mm')
        self.assertEqual(output.GetPointUnit(btk.btkPoint.Angle), 'deg')
        self.assertEqual(output.GetPointUnit(btk.btkPoint.Force), 'N')
        self.assertEqual(output.GetPointUnit(btk.btkPoint.Moment), 'Nmm')
        self.assertEqual(output.GetPointUnit(btk.btkPoint.Power), 'W')

    def test_ConversionToMeter(self):
        input = btk.btkAcquisition()
        input.Init(2,5)
        p1 = input.GetPoint(0)
        a = p1.GetValues()
        a[0,0] = 1234.56; a[1,0] = 34.65; a[2,0] = 98.08; a[3,0] = 987.12; a[4,0] = 654.12
        p1.SetValues(a)
        p2 = input.GetPoint(1)
        p2.SetType(btk.btkPoint.Moment)
        b = p2.GetValues()
        b[0,0] = 1234.56; b[1,0] = 1134.65; b[2,0] = 1000.54; b[3,0] = 987.12; b[4,0] = 1435.896
        p2.SetValues(b)
        
        uc = btk.btkAcquisitionUnitConverter()
        uc.SetInput(input)
        uc.SetUnit(btk.btkAcquisitionUnitConverter.Length, 'm')
        uc.SetUnit(btk.btkAcquisitionUnitConverter.Moment, 'Nm')
        uc.Update()
        output = uc.GetOutput()
        self.assertEqual(output.GetPointUnit(btk.btkPoint.Marker), 'm')
        self.assertEqual(output.GetPointUnit(btk.btkPoint.Angle), 'deg')
        self.assertEqual(output.GetPointUnit(btk.btkPoint.Force), 'N')
        self.assertEqual(output.GetPointUnit(btk.btkPoint.Moment), 'Nm')
        self.assertEqual(output.GetPointUnit(btk.btkPoint.Power), 'W')

        p1 = output.GetPoint(0)
        a = p1.GetValues()
        p2 = output.GetPoint(1)
        b = p2.GetValues()

        self.assertAlmostEqual(a[0,0], 1.23456, 10); 
        self.assertAlmostEqual(a[1,0], 0.03465, 10); 
        self.assertAlmostEqual(a[2,0], 0.09808, 10); 
        self.assertAlmostEqual(a[3,0], 0.98712, 10); 
        self.assertAlmostEqual(a[4,0], 0.65412, 10)

        self.assertAlmostEqual(b[0,0], 1.23456, 10)
        self.assertAlmostEqual(b[1,0], 1.13465, 10)
        self.assertAlmostEqual(b[2,0], 1.00054, 10)
        self.assertAlmostEqual(b[3,0], 0.98712, 10)
        self.assertAlmostEqual(b[4,0], 1.435896, 10)

    def test_ConversionForAnalog(self):
        input = btk.btkAcquisition()
        input.Init(0,5,2)
        a1 = input.GetAnalog(0)
        a1.SetUnit('Nmm')
        a = a1.GetValues()
        a[0,0] = 1234.56; a[1,0] = 34.65; a[2,0] = 98.08; a[3,0] = 987.12; a[4,0] = 654.12
        a1.SetValues(a)
        a2 = input.GetAnalog(1)
        b = a2.GetValues()
        b[0,0] = 1.56; b[1,0] = 1.65; b[2,0] = 1.54; b[3,0] = 0.98712; b[4,0] = 1.8964;
        a2.SetValues(b)

        uc = btk.btkAcquisitionUnitConverter()
        uc.SetInput(input)
        uc.SetUnit(btk.btkAcquisitionUnitConverter.Length, 'm')
        uc.SetUnit(btk.btkAcquisitionUnitConverter.Moment, 'Nm')
        uc.Update()
        output = uc.GetOutput()
        self.assertEqual(output.GetPointUnit(btk.btkPoint.Marker), 'm')
        self.assertEqual(output.GetPointUnit(btk.btkPoint.Angle), 'deg')
        self.assertEqual(output.GetPointUnit(btk.btkPoint.Force), 'N')
        self.assertEqual(output.GetPointUnit(btk.btkPoint.Moment), 'Nm')
        self.assertEqual(output.GetPointUnit(btk.btkPoint.Power), 'W')

        a1 = output.GetAnalog(0)
        a = a1.GetValues()
        a2 = output.GetAnalog(1)
        b = a2.GetValues()

        self.assertAlmostEqual(a[0], 1.23456, 10)
        self.assertAlmostEqual(a[1], 0.03465, 10)
        self.assertAlmostEqual(a[2], 0.09808, 10)
        self.assertAlmostEqual(a[3], 0.98712, 10)
        self.assertAlmostEqual(a[4], 0.65412, 10)

        self.assertAlmostEqual(b[0], 1.5600, 10)
        self.assertAlmostEqual(b[1], 1.6500, 10)
        self.assertAlmostEqual(b[2], 1.5400, 10)
        self.assertAlmostEqual(b[3], 0.98712, 10)
        self.assertAlmostEqual(b[4], 1.8964, 10)

    def test_ConversionFromFile(self):
        reader = btk.btkAcquisitionFileReader()
        reader.SetFilename(_TDDConfigure.C3DFilePathIN + 'sample09/PlugInC3D.c3d')
        acq = reader.GetOutput()

        uc = btk.btkAcquisitionUnitConverter()
        uc.SetInput(acq)
        uc.SetUnit(btk.btkAcquisitionUnitConverter.Length, 'm')
        uc.SetUnit(btk.btkAcquisitionUnitConverter.Angle, 'rad')
        uc.SetUnit(btk.btkAcquisitionUnitConverter.Moment, 'Nm')
        uc.Update()

        acq2 = uc.GetOutput()
        self.assertEqual(acq2.GetPointUnit(btk.btkPoint.Scalar), 'm')
        self.assertEqual(acq.GetFirstFrame(), acq2.GetFirstFrame())
        self.assertEqual(acq.GetPointFrequency(), acq2.GetPointFrequency())
        self.assertEqual(acq.GetPointNumber(), acq2.GetPointNumber())
        self.assertEqual(acq.GetPointFrameNumber(), acq2.GetPointFrameNumber())
        self.assertEqual(acq.GetAnalogFrequency(), acq2.GetAnalogFrequency())
        self.assertEqual(acq.GetAnalogNumber(), acq2.GetAnalogNumber())

        for j in range(0,acq.GetPointNumber()):
          s = 1.0
          t = acq.GetPoint(j).GetType()
          if (t == btk.btkPoint.Marker) or (t == btk.btkPoint.Scalar):
            s = 0.001
          elif (t == btk.btkPoint.Angle):
            s = 1.745329251994e-02
          elif (t == btk.btkPoint.Moment):
            s = 0.001
          for i in range(1,50):
            self.assertAlmostEqual(acq.GetPoint(j).GetValues()[i,0] * s, acq2.GetPoint(j).GetValues()[i,0], 10)
            self.assertAlmostEqual(acq.GetPoint(j).GetValues()[i,1] * s, acq2.GetPoint(j).GetValues()[i,1], 10)
            self.assertAlmostEqual(acq.GetPoint(j).GetValues()[i,2] * s, acq2.GetPoint(j).GetValues()[i,2], 10)
        for j in range(0,acq.GetAnalogNumber()):
          s = 1.0
          unit = acq.GetAnalog(j).GetUnit()
          if (unit == 'Nmm'):
            s = 0.001
          for i in range(1,50):
            self.assertAlmostEqual(acq.GetAnalog(j).GetValues()[i] * s, acq2.GetAnalog(j).GetValues()[i], 10)

        self.assertAlmostEqual(acq.GetMetaData().GetChild('SEG').GetChild('MARKER_DIAMETER').GetInfo().ToDouble(0) * 0.001,
                               acq2.GetMetaData().GetChild('SEG').GetChild('MARKER_DIAMETER').GetInfo().ToDouble(0), 5)
        
    