import btk
import unittest

class PointTest(unittest.TestCase):
    def test_Constructor(self):
        test = btk.btkPoint("HEEL_R", 200);
        self.assertEqual(test.GetLabel(), "HEEL_R");
        self.assertEqual(test.GetDescription(), "");
        self.assertEqual(test.GetValues().shape[0], 200);
        self.assertEqual(test.GetValues().shape[1], 3);
        self.assertEqual(test.GetResiduals().shape[0], 200);
        self.assertEqual(test.GetMasks().shape[0], 200);
        self.assertEqual(test.GetType(), btk.btkPoint.Marker);
    
    def test_FrameNumber(self):
        test = btk.btkPoint("HEEL_R", 200);
        test.SetFrameNumber(225);
        self.assertEqual(test.GetValues().shape[0], 225);
        self.assertEqual(test.GetResiduals().shape[0], 225);
        self.assertEqual(test.GetMasks().shape[0], 225);