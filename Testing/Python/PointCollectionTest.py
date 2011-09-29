import btk
import unittest

class PointCollectionTest(unittest.TestCase):
    def test_Constructor(self):
        test = btk.btkPointCollection();
        self.assertEqual(test.GetItemNumber(), 0);
    
    def test_InsertItem(self):
        test = btk.btkPointCollection();
        m = btk.btkPoint("HEEL_R", 10);
        self.assertEqual(test.InsertItem(0, m), True);
        self.assertEqual(test.GetItemNumber(), 1);
        m.SetValue(0,0,1.0);
        self.assertEqual(test.GetItem(0).GetValue(0,0), 1.0);