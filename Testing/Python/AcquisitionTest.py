import btk
import unittest

class AcquisitionTest(unittest.TestCase):
    def test_Constructor(self):
        test = btk.btkAcquisition()
        self.assertEqual(test.GetPointFrameNumber(), 0)
        self.assertEqual(test.GetAnalogFrameNumber(), 0)
        self.assertEqual(test.GetPointNumber(), 0)
        self.assertEqual(test.GetAnalogNumber(), 0)
        self.assertEqual(test.GetMetaData().GetLabel(), "ROOT")
    
    def test_SetDurationAndFrequency(self):
        test = btk.btkAcquisition()
        test.Init(1, 150, 1, 1)
        test.SetPointFrequency(100)
        self.assertEqual(test.GetPointFrequency(), 100)
        self.assertEqual(test.GetAnalogFrequency(), 100)
        self.assertEqual(test.GetPointFrameNumber(), 150)
        self.assertEqual(test.GetAnalogFrameNumber(), 150)
    
    def test_SetNumber(self):
        test = btk.btkAcquisition()
        test.Init(12, 150, 7, 1)
        test.SetPointFrequency(100)
        self.assertEqual(test.GetPointNumber(), 12)
        self.assertEqual(test.GetAnalogNumber(), 7)
        self.assertEqual(test.GetPointFrameNumber(), 150)
        self.assertEqual(test.GetAnalogFrameNumber(), 150)
        ptPoint = test.GetPoint(0)
        self.assertEqual(ptPoint.GetFrameNumber(), 150)
        ptAnalog = test.GetAnalog(0)
        self.assertEqual(ptAnalog.GetFrameNumber(), 150)
    
    def test_SetAnalogFrequencyOnly(self):
        test = btk.btkAcquisition()
        test.Init(0, 200, 1, 1)
        self.assertEqual(test.GetPointFrameNumber(), 200)
        self.assertEqual(test.GetAnalogFrameNumber(), 200)

    def test_InitPointAndAnalog(self):
        test = btk.btkAcquisition()
        test.Init(5, 200, 10, 1)
        self.assertEqual(test.GetPointNumber(), 5)
        self.assertEqual(test.GetAnalogNumber(), 10)
        self.assertEqual(test.GetPointFrameNumber(), 200)
        self.assertEqual(test.GetAnalogFrameNumber(), 200)

    def test_InitPointOnly(self):
        test = btk.btkAcquisition()
        test.Init(5, 200)
        self.assertEqual(test.GetPointNumber(), 5)
        self.assertEqual(test.GetAnalogNumber(), 0)
        self.assertEqual(test.GetPointFrameNumber(), 200)
        self.assertEqual(test.GetAnalogFrameNumber(), 200)

    def test_InitAnalogOnly(self):
        test = btk.btkAcquisition()
        test.Init(0, 200, 10)
        self.assertEqual(test.GetPointNumber(), 0)
        self.assertEqual(test.GetAnalogNumber(), 10)
        self.assertEqual(test.GetPointFrameNumber(), 200)
        self.assertEqual(test.GetAnalogFrameNumber(), 200)
    
    def test_ResizeFromEnd(self):
        test = btk.btkAcquisition()
        test.Init(2, 200, 10, 2)
        self.assertEqual(test.GetFirstFrame(), 1)
        self.assertEqual(test.GetPointNumber(), 2)
        self.assertEqual(test.GetAnalogNumber(), 10)
        self.assertEqual(test.GetPointFrameNumber(), 200)
        self.assertEqual(test.GetAnalogFrameNumber(), 400)
        test.ResizeFrameNumberFromEnd(50)
        self.assertEqual(test.GetFirstFrame(), 151)
        self.assertEqual(test.GetPointNumber(), 2)
        self.assertEqual(test.GetAnalogNumber(), 10)
        self.assertEqual(test.GetPointFrameNumber(), 50)
        self.assertEqual(test.GetAnalogFrameNumber(), 100)
    
    def test_RemoveLastPoint(self):
        test = btk.btkAcquisition()
        test.Init(10, 5)
        self.assertEqual(test.GetFirstFrame(), 1)
        self.assertEqual(test.GetPointNumber(), 10)
        self.assertEqual(test.GetPointFrameNumber(), 5)
        test.RemovePoint("uname*10")
        self.assertEqual(test.GetPointNumber(), 9)
        test.RemovePoint("uname*9")
        self.assertEqual(test.GetPointNumber(), 8)
        test.RemovePoint("uname*8")
        self.assertEqual(test.GetPointNumber(), 7)
        test.RemovePoint("uname*7")
        self.assertEqual(test.GetPointNumber(), 6)
        test.RemovePoint("uname*6")
        self.assertEqual(test.GetPointNumber(), 5)
        test.RemovePoint("uname*5")
        self.assertEqual(test.GetPointNumber(), 4)
        test.RemovePoint("uname*4")
        self.assertEqual(test.GetPointNumber(), 3)
        test.RemovePoint("uname*3")
        self.assertEqual(test.GetPointNumber(), 2)
        test.RemovePoint("uname*2")
        self.assertEqual(test.GetPointNumber(), 1)
        test.RemovePoint("uname*1")
        self.assertEqual(test.GetPointNumber(), 0)