import btk
import unittest
import _TDDConfigure

class C3DFileReaderTest(unittest.TestCase):
    def test_NoFile(self):
        reader = btk.btkAcquisitionFileReader()
        self.assertRaises(RuntimeError, reader.Update)
        # self.assertRaisesRegexp(RuntimeError, 'Filename must be specified', reader.Update) # Python 2.7+
    
    def test_MisspelledFile(self):
        reader = btk.btkAcquisitionFileReader()
        reader.SetFilename('test.c3d')
        self.assertRaises(RuntimeError, reader.Update)
        # self.assertRaisesRegexp(RuntimeError, 'File doesn\'t exist\nFilename: test.c3d', reader.Update) # Python 2.7+
    
    def test_Sample01_Eb015pi(self):
        reader = btk.btkAcquisitionFileReader()
        reader.SetFilename(_TDDConfigure.C3DFilePathIN + 'sample01/Eb015pi.c3d')
        reader.Update()
        acq = reader.GetOutput()

        self.assertEqual(acq.GetMaxInterpolationGap(), 10)
        self.assertEqual(acq.GetPointFrequency(), 50)
        self.assertEqual(acq.GetPointNumber(), 26)
        self.assertEqual(acq.GetAnalogFrequency(), 200)
        self.assertEqual(acq.GetAnalogNumber(), 16)
        self.assertEqual(acq.GetPoint(0).GetLabel(), 'RFT1')
        self.assertEqual(acq.GetPoint(9).GetDescription(), 'DISTAL FOOT')
        self.assertEqual(acq.GetAnalog(15).GetLabel(), 'CH16')
        self.assertEqual(acq.GetAnalog(2).GetDescription(), 'FORCE Z-COMP 1')
        self.assertAlmostEqual(acq.GetPoint(0).GetValues()[8,0], 250.4166, 3)
        self.assertAlmostEqual(acq.GetPoint(17).GetValues()[16,0], 285, 3)
        self.assertAlmostEqual(acq.GetPoint(17).GetValues()[16,1], 222.4166, 3)
        self.assertAlmostEqual(acq.GetPoint(17).GetValues()[16,2], 564.4166, 3)
        self.assertAlmostEqual(acq.GetPoint(0).GetResiduals()[0], 1.3333, 3)
        self.assertAlmostEqual(acq.GetPoint(14).GetResiduals()[20], 1.91667, 3)
        self.assertAlmostEqual(acq.GetAnalog(0).GetValues()[0], -26.6599, 3)
        self.assertAlmostEqual(acq.GetAnalog(0).GetValues()[1], -25.8, 3)
        
        self.assertEqual(acq.GetEventNumber(), 3)
        self.assertEqual(acq.GetEvent(0).GetLabel(), 'RIC')
        self.assertAlmostEqual(acq.GetEvent(0).GetTime(), 2.72, 3)
        self.assertEqual(acq.GetEvent(1).GetLabel(), 'RHS')
        self.assertAlmostEqual(acq.GetEvent(1).GetTime(), 5.40, 3)
        self.assertEqual(acq.GetEvent(2).GetLabel(), 'RTO')
        self.assertAlmostEqual(acq.GetEvent(2).GetTime(), 7.32, 3)

        io = btk.btkC3DFileIO(reader.GetAcquisitionIO())        
        self.assertAlmostEqual(io.GetPointScale(), 0.08333, 3)
        
    def test_Sample01_Eb015si(self):
        reader = btk.btkAcquisitionFileReader()
        reader.SetFilename(_TDDConfigure.C3DFilePathIN + 'sample01/Eb015si.c3d')
        reader.Update()
        acq = reader.GetOutput()
        
        self.assertEqual(acq.GetMaxInterpolationGap(), 10)
        self.assertEqual(acq.GetPointFrequency(), 50)
        self.assertEqual(acq.GetPointNumber(), 26)
        self.assertEqual(acq.GetAnalogFrequency(), 200)
        self.assertEqual(acq.GetAnalogNumber(), 16)
        self.assertEqual(acq.GetPoint(0).GetLabel(), 'RFT1')
        self.assertAlmostEqual(acq.GetPoint(0).GetValue(8,0), 250.4166, 3)
        self.assertAlmostEqual(acq.GetPoint(17).GetValue(16,0), 285, 3)
        self.assertAlmostEqual(acq.GetPoint(17).GetValue(16,1), 222.4166, 3)
        self.assertAlmostEqual(acq.GetPoint(17).GetValue(16,2), 564.4166, 3)
        self.assertAlmostEqual(acq.GetPoint(0).GetResidual(0), 1.3333, 3)
        self.assertAlmostEqual(acq.GetPoint(14).GetResidual(20), 1.91667, 3)
        self.assertAlmostEqual(acq.GetAnalog(0).GetValue(0), -26.6599, 3)
        self.assertAlmostEqual(acq.GetAnalog(0).GetValue(1), -25.8, 3)

        self.assertEqual(acq.GetEventNumber(), 3)
        self.assertEqual(acq.GetEvent(0).GetLabel(), 'RIC')
        self.assertAlmostEqual(acq.GetEvent(0).GetTime(), 2.72, 3)
        self.assertEqual(acq.GetEvent(1).GetLabel(), 'RHS')
        self.assertAlmostEqual(acq.GetEvent(1).GetTime(), 5.40, 3)
        self.assertEqual(acq.GetEvent(2).GetLabel(), 'RTO')
        self.assertAlmostEqual(acq.GetEvent(2).GetTime(), 7.32, 3)
    
    def test_Sample01_Eb015vi(self):
        reader = btk.btkAcquisitionFileReader()
        reader.SetFilename(_TDDConfigure.C3DFilePathIN + 'sample01/Eb015vi.c3d')
        reader.Update()
        acq = reader.GetOutput()
        
        self.assertEqual(acq.GetMaxInterpolationGap(), 10)
        self.assertEqual(acq.GetPointFrequency(), 50)
        self.assertEqual(acq.GetPointNumber(), 26)
        self.assertEqual(acq.GetAnalogFrequency(), 200)
        self.assertEqual(acq.GetAnalogNumber(), 16)
        self.assertEqual(acq.GetPoint(0).GetLabel(), 'RFT1')
        self.assertAlmostEqual(acq.GetPoint(0).GetValues()[8,0], 250.4166, 3)
        self.assertAlmostEqual(acq.GetPoint(17).GetValues()[16,0], 285, 3)
        self.assertAlmostEqual(acq.GetPoint(17).GetValues()[16,1], 222.4166, 3)
        self.assertAlmostEqual(acq.GetPoint(17).GetValues()[16,2], 564.4166, 3)
        self.assertAlmostEqual(acq.GetPoint(0).GetResiduals()[0], 1.3333, 3)
        self.assertAlmostEqual(acq.GetPoint(14).GetResiduals()[20], 1.91667, 3)
        self.assertAlmostEqual(acq.GetAnalog(0).GetValues()[0], -26.6599, 3)
        self.assertAlmostEqual(acq.GetAnalog(0).GetValues()[1], -25.8, 3)

        self.assertEqual(acq.GetEventNumber(), 3)
        self.assertEqual(acq.GetEvent(0).GetLabel(), 'RIC')
        self.assertAlmostEqual(acq.GetEvent(0).GetTime(), 2.72, 3)
        self.assertEqual(acq.GetEvent(1).GetLabel(), 'RHS')
        self.assertAlmostEqual(acq.GetEvent(1).GetTime(), 5.40, 3)
        self.assertEqual(acq.GetEvent(2).GetLabel(), 'RTO')
        self.assertAlmostEqual(acq.GetEvent(2).GetTime(), 7.32, 3)
    
    def test_Sample01_Eb015pr(self):
        reader = btk.btkAcquisitionFileReader()
        reader.SetFilename(_TDDConfigure.C3DFilePathIN + 'sample01/Eb015pr.c3d')
        reader.Update()
        acq = reader.GetOutput()
        
        self.assertEqual(acq.GetMaxInterpolationGap(), 10)
        self.assertEqual(acq.GetPointFrequency(), 50)
        self.assertEqual(acq.GetPointNumber(), 26)
        self.assertEqual(acq.GetAnalogFrequency(), 200)
        self.assertEqual(acq.GetAnalogNumber(), 16)
        self.assertEqual(acq.GetPoint(0).GetLabel(), 'RFT1')
        self.assertAlmostEqual(acq.GetPoint(0).GetValue(8,0), 250.4166, 3)
        self.assertAlmostEqual(acq.GetPoint(17).GetValue(16,0), 285, 3)
        self.assertAlmostEqual(acq.GetPoint(17).GetValue(16,1), 222.4166, 3)
        self.assertAlmostEqual(acq.GetPoint(17).GetValue(16,2), 564.4166, 3)
        self.assertAlmostEqual(acq.GetPoint(0).GetResidual(0), 1.3333, 3)
        self.assertAlmostEqual(acq.GetPoint(14).GetResidual(20), 1.91667, 3)
        self.assertAlmostEqual(acq.GetAnalog(0).GetValue(0), -26.6599, 3)
        self.assertAlmostEqual(acq.GetAnalog(0).GetValue(1), -25.8, 3)

        self.assertEqual(acq.GetEventNumber(), 3)
        self.assertEqual(acq.GetEvent(0).GetLabel(), 'RIC')
        self.assertAlmostEqual(acq.GetEvent(0).GetTime(), 2.72, 3)
        self.assertEqual(acq.GetEvent(1).GetLabel(), 'RHS')
        self.assertAlmostEqual(acq.GetEvent(1).GetTime(), 5.40, 3)
        self.assertEqual(acq.GetEvent(2).GetLabel(), 'RTO')
        self.assertAlmostEqual(acq.GetEvent(2).GetTime(), 7.32, 3)
    
    def test_Sample01_Eb015sr(self):
        reader = btk.btkAcquisitionFileReader()
        reader.SetFilename(_TDDConfigure.C3DFilePathIN + 'sample01/Eb015sr.c3d')
        reader.Update()
        acq = reader.GetOutput()
        
        self.assertEqual(acq.GetMaxInterpolationGap(), 10)
        self.assertEqual(acq.GetPointFrequency(), 50)
        self.assertEqual(acq.GetPointNumber(), 26)
        self.assertEqual(acq.GetAnalogFrequency(), 200)
        self.assertEqual(acq.GetAnalogNumber(), 16)
        self.assertEqual(acq.GetPoint(0).GetLabel(), 'RFT1')
        self.assertAlmostEqual(acq.GetPoint(0).GetValues()[8,0], 250.4166, 3)
        self.assertAlmostEqual(acq.GetPoint(17).GetValues()[16,0], 285, 3)
        self.assertAlmostEqual(acq.GetPoint(17).GetValues()[16,1], 222.4166, 3)
        self.assertAlmostEqual(acq.GetPoint(17).GetValues()[16,2], 564.4166, 3)
        self.assertAlmostEqual(acq.GetPoint(0).GetResiduals()[0], 1.3333, 3)
        self.assertAlmostEqual(acq.GetPoint(14).GetResiduals()[20], 1.91667, 3)
        self.assertAlmostEqual(acq.GetAnalog(0).GetValues()[0], -26.6599, 3)
        self.assertAlmostEqual(acq.GetAnalog(0).GetValues()[1], -25.8, 3)

        self.assertEqual(acq.GetEventNumber(), 3)
        self.assertEqual(acq.GetEvent(0).GetLabel(), 'RIC')
        self.assertAlmostEqual(acq.GetEvent(0).GetTime(), 2.72, 3)
        self.assertEqual(acq.GetEvent(1).GetLabel(), 'RHS')
        self.assertAlmostEqual(acq.GetEvent(1).GetTime(), 5.40, 3)
        self.assertEqual(acq.GetEvent(2).GetLabel(), 'RTO')
        self.assertAlmostEqual(acq.GetEvent(2).GetTime(), 7.32, 3)
    
    def test_Sample01_Eb015vr(self):
        reader = btk.btkAcquisitionFileReader()
        reader.SetFilename(_TDDConfigure.C3DFilePathIN + 'sample01/Eb015vr.c3d')
        reader.Update()
        acq = reader.GetOutput()
        
        self.assertEqual(acq.GetMaxInterpolationGap(), 10)
        self.assertEqual(acq.GetPointFrequency(), 50)
        self.assertEqual(acq.GetPointNumber(), 26)
        self.assertEqual(acq.GetAnalogFrequency(), 200)
        self.assertEqual(acq.GetAnalogNumber(), 16)
        self.assertEqual(acq.GetPoint(0).GetLabel(), 'RFT1')
        self.assertAlmostEqual(acq.GetPoint(0).GetValue(8,0), 250.4166, 3)
        self.assertAlmostEqual(acq.GetPoint(17).GetValue(16,0), 285, 3)
        self.assertAlmostEqual(acq.GetPoint(17).GetValue(16,1), 222.4166, 3)
        self.assertAlmostEqual(acq.GetPoint(17).GetValue(16,2), 564.4166, 3)
        self.assertAlmostEqual(acq.GetPoint(0).GetResidual(0), 1.3333, 3)
        self.assertAlmostEqual(acq.GetPoint(14).GetResidual(20), 1.91667, 3)
        self.assertAlmostEqual(acq.GetAnalog(0).GetValue(0), -26.6599, 3)
        self.assertAlmostEqual(acq.GetAnalog(0).GetValue(1), -25.8, 3)

        self.assertEqual(acq.GetEventNumber(), 3)
        self.assertEqual(acq.GetEvent(0).GetLabel(), 'RIC')
        self.assertAlmostEqual(acq.GetEvent(0).GetTime(), 2.72, 3)
        self.assertEqual(acq.GetEvent(1).GetLabel(), 'RHS')
        self.assertAlmostEqual(acq.GetEvent(1).GetTime(), 5.40, 3)
        self.assertEqual(acq.GetEvent(2).GetLabel(), 'RTO')
        self.assertAlmostEqual(acq.GetEvent(2).GetTime(), 7.32, 3)
    
    def test_Sample02_DEC_INT(self):
        reader = btk.btkAcquisitionFileReader()
        reader.SetFilename(_TDDConfigure.C3DFilePathIN + 'sample02/DEC_INT.C3D')
        reader.Update()
        acq = reader.GetOutput()

        self.assertEqual(acq.GetPointFrequency(), 50)
        self.assertEqual(acq.GetPointNumber(), 36)
        self.assertEqual(acq.GetPointFrameNumber(), 89)
        self.assertEqual(acq.GetAnalogFrequency(), 200)
        self.assertEqual(acq.GetAnalogNumber(), 16)
        self.assertEqual(acq.GetAnalogFrameNumber(), 89 * 4)

        self.assertEqual(acq.GetPoint(3).GetLabel(), 'RSK1')
        self.assertAlmostEqual(acq.GetPoint(3).GetValue(16,0), 384.0944, 3)
        self.assertAlmostEqual(acq.GetPoint(3).GetValue(16,1), 523.8418, 3)
        self.assertAlmostEqual(acq.GetPoint(3).GetValue(16,2), 311.2683, 3)

        self.assertEqual(acq.GetAnalog(2).GetLabel(), 'FZ1')
        self.assertAlmostEqual(acq.GetAnalog(2).GetValue(15), 8.9280, 3)
        self.assertAlmostEqual(acq.GetAnalog(2).GetValue(39), 5.952, 3)

    def test_Sample02_Dec_real(self):
        reader = btk.btkAcquisitionFileReader()
        reader.SetFilename(_TDDConfigure.C3DFilePathIN + 'sample02/Dec_real.c3d')
        reader.Update()
        acq = reader.GetOutput()

        self.assertEqual(acq.GetPointFrequency(), 50)
        self.assertEqual(acq.GetPointNumber(), 36)
        self.assertEqual(acq.GetPointFrameNumber(), 89)
        self.assertEqual(acq.GetAnalogFrequency(), 200)
        self.assertEqual(acq.GetAnalogNumber(), 16)
        self.assertEqual(acq.GetAnalogFrameNumber(), 89 * 4)

        self.assertEqual(acq.GetPoint(3).GetLabel(), 'RSK1')
        self.assertAlmostEqual(acq.GetPoint(3).GetValue(16,0), 384.0944, 3)
        self.assertAlmostEqual(acq.GetPoint(3).GetValue(16,1), 523.8417, 3)
        self.assertAlmostEqual(acq.GetPoint(3).GetValue(16,2), 311.2683, 3)

        self.assertEqual(acq.GetAnalog(2).GetLabel(), 'FZ1')
        self.assertAlmostEqual(acq.GetAnalog(2).GetValue(15), 8.9280, 3)
        self.assertAlmostEqual(acq.GetAnalog(2).GetValue(39), 5.952, 3)

    def test_Sample02_pc_int(self):
        reader = btk.btkAcquisitionFileReader()
        reader.SetFilename(_TDDConfigure.C3DFilePathIN + 'sample02/pc_int.c3d')
        reader.Update()
        acq = reader.GetOutput()

        self.assertEqual(acq.GetPointFrequency(), 50)
        self.assertEqual(acq.GetPointNumber(), 36)
        self.assertEqual(acq.GetPointFrameNumber(), 89)
        self.assertEqual(acq.GetAnalogFrequency(), 200)
        self.assertEqual(acq.GetAnalogNumber(), 16)
        self.assertEqual(acq.GetAnalogFrameNumber(), 89 * 4)

        self.assertEqual(acq.GetPoint(3).GetLabel(), 'RSK1')
        self.assertAlmostEqual(acq.GetPoint(3).GetValue(16,0), 384.0944, 3)
        self.assertAlmostEqual(acq.GetPoint(3).GetValue(16,1), 523.5606, 3)
        self.assertAlmostEqual(acq.GetPoint(3).GetValue(16,2), 311.2683, 3)

        self.assertEqual(acq.GetAnalog(2).GetLabel(), 'FZ1')
        self.assertAlmostEqual(acq.GetAnalog(2).GetValue(15), 8.9280, 3)
        self.assertAlmostEqual(acq.GetAnalog(2).GetValue(39), 5.952, 3)

    def test_Sample02_pc_real(self):
        reader = btk.btkAcquisitionFileReader()
        reader.SetFilename(_TDDConfigure.C3DFilePathIN + 'sample02/pc_real.c3d')
        reader.Update()
        acq = reader.GetOutput()

        self.assertEqual(acq.GetPointFrequency(), 50)
        self.assertEqual(acq.GetPointNumber(), 36)
        self.assertEqual(acq.GetPointFrameNumber(), 89)
        self.assertEqual(acq.GetAnalogFrequency(), 200)
        self.assertEqual(acq.GetAnalogNumber(), 16)
        self.assertEqual(acq.GetAnalogFrameNumber(), 89 * 4)

        self.assertEqual(acq.GetPoint(3).GetLabel(), 'RSK1')
        self.assertAlmostEqual(acq.GetPoint(3).GetValue(16,0), 384.0944, 3)
        self.assertAlmostEqual(acq.GetPoint(3).GetValue(16,1), 523.8417, 3)
        self.assertAlmostEqual(acq.GetPoint(3).GetValue(16,2), 311.2683, 3)

        self.assertEqual(acq.GetAnalog(2).GetLabel(), 'FZ1')
        self.assertAlmostEqual(acq.GetAnalog(2).GetValue(15), 8.9280, 3)
        self.assertAlmostEqual(acq.GetAnalog(2).GetValue(39), 5.952, 3)

    def test_Sample02_sgi_int(self):
        reader = btk.btkAcquisitionFileReader()
        reader.SetFilename(_TDDConfigure.C3DFilePathIN + 'sample02/sgi_int.c3d')
        reader.Update()
        acq = reader.GetOutput()

        self.assertEqual(acq.GetPointFrequency(), 50)
        self.assertEqual(acq.GetPointNumber(), 36)
        self.assertEqual(acq.GetPointFrameNumber(), 89)
        self.assertEqual(acq.GetAnalogFrequency(), 200)
        self.assertEqual(acq.GetAnalogNumber(), 16)
        self.assertEqual(acq.GetAnalogFrameNumber(), 89 * 4)

        self.assertEqual(acq.GetPoint(3).GetLabel(), 'RSK1')
        self.assertAlmostEqual(acq.GetPoint(3).GetValue(16,0), 384.0944, 3)
        self.assertAlmostEqual(acq.GetPoint(3).GetValue(16,1), 523.5606, 3)
        self.assertAlmostEqual(acq.GetPoint(3).GetValue(16,2), 311.2683, 3)

        self.assertEqual(acq.GetAnalog(2).GetLabel(), 'FZ1')
        self.assertAlmostEqual(acq.GetAnalog(2).GetValue(15), 8.9280, 3)
        self.assertAlmostEqual(acq.GetAnalog(2).GetValue(39), 5.952, 3)

    def test_Sample02_sgi_real(self):
        reader = btk.btkAcquisitionFileReader()
        reader.SetFilename(_TDDConfigure.C3DFilePathIN + 'sample02/sgi_real.c3d')
        reader.Update()
        acq = reader.GetOutput()

        self.assertEqual(acq.GetPointFrequency(), 50)
        self.assertEqual(acq.GetPointNumber(), 36)
        self.assertEqual(acq.GetPointFrameNumber(), 89)
        self.assertEqual(acq.GetAnalogFrequency(), 200)
        self.assertEqual(acq.GetAnalogNumber(), 16)
        self.assertEqual(acq.GetAnalogFrameNumber(), 89 * 4)

        self.assertEqual(acq.GetPoint(3).GetLabel(), 'RSK1')
        self.assertAlmostEqual(acq.GetPoint(3).GetValue(16,0), 384.0944, 3)
        self.assertAlmostEqual(acq.GetPoint(3).GetValue(16,1), 523.8417, 3)
        self.assertAlmostEqual(acq.GetPoint(3).GetValue(16,2), 311.2683, 3)

        self.assertEqual(acq.GetAnalog(2).GetLabel(), 'FZ1')
        self.assertAlmostEqual(acq.GetAnalog(2).GetValue(15), 8.9280, 3)
        self.assertAlmostEqual(acq.GetAnalog(2).GetValue(39), 5.952, 3)
  
    def test_Sample05_vicon512(self):
        reader = btk.btkAcquisitionFileReader()
        reader.SetFilename(_TDDConfigure.C3DFilePathIN + 'sample05/vicon512.c3d')
        reader.Update()
        acq = reader.GetOutput()

        self.assertEqual(acq.GetPointFrequency(), 120)
        self.assertEqual(acq.GetPointNumber(), 45)
        self.assertEqual(acq.GetPointFrameNumber(), 6492)
        self.assertEqual(acq.GetAnalogFrequency(), 1080)
        self.assertEqual(acq.GetAnalogNumber(), 23)
        self.assertEqual(acq.GetAnalogFrameNumber(), 6492 * 9)

    def test_Sample06_MACsample(self):
        reader = btk.btkAcquisitionFileReader()
        reader.SetFilename(_TDDConfigure.C3DFilePathIN + 'sample06/MACsample.c3d')
        reader.Update()


    def test_Sample08_TEST(self):
        reader = btk.btkAcquisitionFileReader()
        # TESTAPI
        reader.SetFilename(_TDDConfigure.C3DFilePathIN + 'sample08/TESTAPI.c3d')
        reader.Update()
        TESTAPI = reader.GetOutput()
        # TESTBPI
        reader.SetFilename(_TDDConfigure.C3DFilePathIN + 'sample08/TESTBPI.c3d')
        reader.Update()
        TESTBPI = reader.GetOutput()
        # TESTCPI
        reader.SetFilename(_TDDConfigure.C3DFilePathIN + 'sample08/TESTCPI.c3d')
        reader.Update()
        TESTCPI = reader.GetOutput()
        # TESTDPI
        reader.SetFilename(_TDDConfigure.C3DFilePathIN + 'sample08/TESTDPI.c3d')
        reader.Update()
        TESTDPI = reader.GetOutput()

    def test_Sample09_PlugInC3D(self):
        reader = btk.btkAcquisitionFileReader()
        reader.SetFilename(_TDDConfigure.C3DFilePathIN + 'sample09/PlugInC3D.c3d')
        reader.Update()
        acq = reader.GetOutput()
        self.assertEqual(acq.GetPoint(0).GetType(), btk.btkPoint.Marker)
        self.assertEqual(acq.GetPoint(18).GetType(), btk.btkPoint.Angle)
        self.assertEqual(acq.GetPoint(64).GetType(), btk.btkPoint.Force)
        self.assertEqual(acq.GetPoint(72).GetType(), btk.btkPoint.Moment)
        self.assertEqual(acq.GetPoint(80).GetType(), btk.btkPoint.Power)
        
        self.assertEqual(acq.GetAnalog(0).GetGain(), btk.btkAnalog.PlusMinus5)
        self.assertEqual(acq.GetAnalog(1).GetGain(), btk.btkAnalog.PlusMinus5)
        self.assertEqual(acq.GetAnalog(2).GetGain(), btk.btkAnalog.PlusMinus5)
        self.assertEqual(acq.GetAnalog(3).GetGain(), btk.btkAnalog.PlusMinus5)
        self.assertEqual(acq.GetAnalog(4).GetGain(), btk.btkAnalog.PlusMinus5)
        self.assertEqual(acq.GetAnalog(5).GetGain(), btk.btkAnalog.PlusMinus5)
        self.assertEqual(acq.GetAnalog(6).GetGain(), btk.btkAnalog.PlusMinus5)
        self.assertEqual(acq.GetAnalog(7).GetGain(), btk.btkAnalog.PlusMinus5)
        self.assertEqual(acq.GetAnalog(8).GetGain(), btk.btkAnalog.PlusMinus5)
        self.assertEqual(acq.GetAnalog(9).GetGain(), btk.btkAnalog.PlusMinus5)
        self.assertEqual(acq.GetAnalog(10).GetGain(), btk.btkAnalog.PlusMinus5)
        self.assertEqual(acq.GetAnalog(11).GetGain(), btk.btkAnalog.PlusMinus5)
        self.assertEqual(acq.GetAnalog(12).GetGain(), btk.btkAnalog.PlusMinus10)
        self.assertEqual(acq.GetAnalog(13).GetGain(), btk.btkAnalog.PlusMinus10)
        self.assertEqual(acq.GetAnalog(14).GetGain(), btk.btkAnalog.PlusMinus10)
        self.assertEqual(acq.GetAnalog(15).GetGain(), btk.btkAnalog.PlusMinus10)
        self.assertEqual(acq.GetAnalog(16).GetGain(), btk.btkAnalog.PlusMinus10)
        self.assertEqual(acq.GetAnalog(17).GetGain(), btk.btkAnalog.PlusMinus10)
        self.assertEqual(acq.GetAnalog(18).GetGain(), btk.btkAnalog.PlusMinus10)
        self.assertEqual(acq.GetAnalog(19).GetGain(), btk.btkAnalog.PlusMinus10)
        self.assertEqual(acq.GetAnalog(20).GetGain(), btk.btkAnalog.PlusMinus5)
        self.assertEqual(acq.GetAnalog(21).GetGain(), btk.btkAnalog.PlusMinus5)
        self.assertEqual(acq.GetAnalog(22).GetGain(), btk.btkAnalog.PlusMinus5)
        self.assertEqual(acq.GetAnalog(23).GetGain(), btk.btkAnalog.PlusMinus5)
        self.assertEqual(acq.GetAnalog(24).GetGain(), btk.btkAnalog.PlusMinus5)
        self.assertEqual(acq.GetAnalog(25).GetGain(), btk.btkAnalog.PlusMinus5)
        self.assertEqual(acq.GetAnalog(26).GetGain(), btk.btkAnalog.PlusMinus5)
        self.assertEqual(acq.GetAnalog(27).GetGain(), btk.btkAnalog.PlusMinus5)
        self.assertEqual(acq.GetAnalog(28).GetGain(), btk.btkAnalog.PlusMinus5)
        self.assertEqual(acq.GetAnalog(29).GetGain(), btk.btkAnalog.PlusMinus5)
        self.assertEqual(acq.GetAnalog(30).GetGain(), btk.btkAnalog.PlusMinus5)
        self.assertEqual(acq.GetAnalog(31).GetGain(), btk.btkAnalog.PlusMinus5)
        self.assertEqual(acq.GetAnalog(32).GetGain(), btk.btkAnalog.PlusMinus5)
        self.assertEqual(acq.GetAnalog(33).GetGain(), btk.btkAnalog.PlusMinus5)
        self.assertEqual(acq.GetAnalog(34).GetGain(), btk.btkAnalog.PlusMinus5)
        self.assertEqual(acq.GetAnalog(35).GetGain(), btk.btkAnalog.PlusMinus5)
        
        self.assertEqual(acq.GetPointFrameNumber(), acq.GetMetaData().GetChild('POINT').GetChild('FRAMES').GetInfo().ToInt(0))

    def test_Sample13_Dance(self):
        reader = btk.btkAcquisitionFileReader()
        reader.SetFilename(_TDDConfigure.C3DFilePathIN + 'sample13/Dance.c3d')
        reader.Update()
        acq = reader.GetOutput()

        self.assertAlmostEqual(acq.GetPointFrequency(), 65.05, 2)
        self.assertEqual(acq.GetPointNumber(), 40)
        self.assertEqual(acq.GetPointFrameNumber(), 499)
        self.assertAlmostEqual(acq.GetAnalogFrequency(), 65.05, 2)
        self.assertEqual(acq.GetAnalogNumber(), 8)
        self.assertEqual(acq.GetAnalogFrameNumber(), 499)

    def test_Sample13_golfswing(self):
        reader = btk.btkAcquisitionFileReader()
        reader.SetFilename(_TDDConfigure.C3DFilePathIN + 'sample13/golfswing.c3d')
        reader.Update()
        acq = reader.GetOutput()

        self.assertAlmostEqual(acq.GetPointFrequency(), 107.53, 2)
        self.assertEqual(acq.GetPointNumber(), 29)
        self.assertEqual(acq.GetPointFrameNumber(), 514)
        self.assertAlmostEqual(acq.GetAnalogFrequency(), 107.53, 2)
        self.assertEqual(acq.GetAnalogNumber(), 8)
        self.assertEqual(acq.GetAnalogFrameNumber(), 514)

    def test_Sample15_FP1(self):
        reader = btk.btkAcquisitionFileReader()
        reader.SetFilename(_TDDConfigure.C3DFilePathIN + 'sample15/FP1.C3D')
        reader.Update()
    
    def test_Sample15_FP2(self):
        reader = btk.btkAcquisitionFileReader()
        reader.SetFilename(_TDDConfigure.C3DFilePathIN + 'sample15/FP2.C3D')
        reader.Update()

    def test_Sample18_bad_parameter_section(self):
        reader = btk.btkAcquisitionFileReader()
        reader.SetFilename(_TDDConfigure.C3DFilePathIN + 'sample18/bad_parameter_section.c3d')

        self.assertRaises(RuntimeError, reader.Update)
        # self.assertRaisesRegexp(RuntimeError, 'Bad data first block', reader.Update) # Python 2.7+

    def test_Sample19_sample19(self):
        reader = btk.btkAcquisitionFileReader()
        reader.SetFilename(_TDDConfigure.C3DFilePathIN + 'sample19/sample19.c3d')
        reader.Update()
        acq = reader.GetOutput()

        self.assertEqual(acq.GetPointFrequency(), 60)
        self.assertEqual(acq.GetPointNumber(), 0)
        self.assertEqual(acq.GetPointFrameNumber(), 34672)
        self.assertEqual(acq.GetAnalogFrequency(), 1080)
        self.assertEqual(acq.GetAnalogNumber(), 2)
        self.assertEqual(acq.GetAnalogFrameNumber(), 18 * 34672)

    def test_Sample20_phasespace_sample(self):
        reader = btk.btkAcquisitionFileReader()
        reader.SetFilename(_TDDConfigure.C3DFilePathIN + 'sample20/phasespace_sample.c3d')
        reader.Update()
        acq = reader.GetOutput()

        self.assertEqual(acq.GetPointFrequency(), 30)
        self.assertEqual(acq.GetPointNumber(), 40)
        self.assertEqual(acq.GetPointFrameNumber(), 701)
        self.assertEqual(acq.GetAnalogFrequency(), 30)
        self.assertEqual(acq.GetAnalogNumber(), 0)
        self.assertEqual(acq.GetAnalogFrameNumber(), 701)

    def test_Sample21_sample21(self):
        reader = btk.btkAcquisitionFileReader()
        reader.SetFilename(_TDDConfigure.C3DFilePathIN + 'sample21/sample21.c3d')
        reader.Update()

    def test_Sample22_BKINtechnologies(self):
        reader = btk.btkAcquisitionFileReader()
        reader.SetFilename(_TDDConfigure.C3DFilePathIN + 'sample22/BKINtechnologies.c3d')
        reader.Update()
        acq = reader.GetOutput()

        self.assertEqual(acq.GetEventNumber(), 2)
        self.assertEqual(acq.GetEvent(0).GetLabel(), 'STAY_CENTRE')
        self.assertAlmostEqual(acq.GetEvent(0).GetTime(), 0.0, 3)
        self.assertEqual(acq.GetEvent(0).GetDescription(), 'subject must wait at centre starting now')
        self.assertEqual(acq.GetEvent(1).GetLabel(), 'TARGET_ON')
        self.assertAlmostEqual(acq.GetEvent(1).GetTime(), 1.6140, 3)
        self.assertEqual(acq.GetEvent(1).GetDescription(), 'Target light ON, start waiting for subject to leave centre target')

    def test_Sample28_dynamic(self):
        reader = btk.btkAcquisitionFileReader()
        reader.SetFilename(_TDDConfigure.C3DFilePathIN + 'sample28/dynamic.C3D')
        reader.Update()

    def test_Sample28_standing(self):
        reader = btk.btkAcquisitionFileReader()
        reader.SetFilename(_TDDConfigure.C3DFilePathIN + 'sample28/standing.C3D')
        reader.Update()

    def test_Sample28_type1(self):
        reader = btk.btkAcquisitionFileReader()
        reader.SetFilename(_TDDConfigure.C3DFilePathIN + 'sample28/type1.C3D')
        reader.Update()
    
    def test_ParameterOverflow(self):
        reader = btk.btkAcquisitionFileReader()
        reader.SetFilename(_TDDConfigure.C3DFilePathIN + 'others/parameterOverflow.C3D')
        reader.Update()
        
        acq = reader.GetOutput()
        itAnalysis = acq.GetMetaData().FindChild('ANALYSIS')
        self.assertTrue(itAnalysis != acq.GetMetaData().End())
        if (itAnalysis != acq.GetMetaData().End()):
            self.assertTrue(itAnalysis.value().FindChild('VALUES') != itAnalysis.value().End())