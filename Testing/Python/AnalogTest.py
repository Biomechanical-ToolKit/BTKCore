import btk
import unittest

class AnalogTest(unittest.TestCase):
    def test_default_ctor(self):
        a = btk.btkAnalog()
        self.assertEqual(a.GetLabel(), '')
        self.assertEqual(a.GetDescription(), '')
        self.assertEqual(a.GetFrameNumber(), 0)
        self.assertEqual(a.GetUnit(), 'V')
        self.assertEqual(a.GetGain(), btk.btkAnalog.Unknown)
        self.assertEqual(a.GetScale(), 1.0)
        
    def test_default_ctor2(self):
        a = btk.btkAnalog('F1X', 'Force platform 1, X-axis')
        self.assertEqual(a.GetLabel(), 'F1X')
        self.assertEqual(a.GetDescription(), 'Force platform 1, X-axis')
        self.assertEqual(a.GetFrameNumber(), 0)
        self.assertEqual(a.GetUnit(), 'V')
        self.assertEqual(a.GetGain(), btk.btkAnalog.Unknown)
        self.assertEqual(a.GetScale(), 1.0)
        
    def test_default_ctor3(self):
        a = btk.btkAnalog('F1X', 'Force platform 1, X-axis')
        a.SetFrameNumber(45);
        self.assertEqual(a.GetLabel(), 'F1X')
        self.assertEqual(a.GetDescription(), 'Force platform 1, X-axis')
        self.assertEqual(a.GetFrameNumber(), 45)
        self.assertEqual(a.GetUnit(), 'V')
        self.assertEqual(a.GetGain(), btk.btkAnalog.Unknown)
        self.assertEqual(a.GetScale(), 1.0)
        
    def test_ctor_frame(self):
        a = btk.btkAnalog(45)
        self.assertEqual(a.GetLabel(), '')
        self.assertEqual(a.GetDescription(), '')
        self.assertEqual(a.GetFrameNumber(), 45)
        self.assertEqual(a.GetUnit(), 'V')
        self.assertEqual(a.GetGain(), btk.btkAnalog.Unknown)
        self.assertEqual(a.GetScale(), 1.0)
        
    def test_ctor_label(self):
        a = btk.btkAnalog('F1X', 45)
        self.assertEqual(a.GetLabel(), 'F1X')
        self.assertEqual(a.GetDescription(), '')
        self.assertEqual(a.GetFrameNumber(), 45)
        self.assertEqual(a.GetUnit(), 'V')
        self.assertEqual(a.GetGain(), btk.btkAnalog.Unknown)
        self.assertEqual(a.GetScale(), 1.0)
        
    def test_modifiers(self):
        a = btk.btkAnalog()
        a.SetLabel('F1X')
        a.SetDescription('Force platform 1, X-axis')
        a.SetFrameNumber(30)
        a.SetUnit('N')
        a.SetGain(btk.btkAnalog.PlusMinus10)
        a.SetScale(-0.003458)
        self.assertEqual(a.GetLabel(), 'F1X')
        self.assertEqual(a.GetDescription(), 'Force platform 1, X-axis')
        self.assertEqual(a.GetFrameNumber(), 30)
        self.assertEqual(a.GetUnit(), 'N')
        self.assertEqual(a.GetGain(), btk.btkAnalog.PlusMinus10)
        self.assertEqual(a.GetScale(), -0.003458)
        
    def test_copy(self):
        a1 = btk.btkAnalog()
        a1.SetLabel('F1X')
        a1.SetDescription('Force platform 1, X-axis')
        a1.SetFrameNumber(30)
        a1.SetUnit('N')
        a1.SetGain(btk.btkAnalog.PlusMinus10)
        a1.SetScale(-0.003458)
        a2 = a1
        self.assertEqual(a2.GetLabel(), 'F1X')
        self.assertEqual(a2.GetDescription(), 'Force platform 1, X-axis')
        self.assertEqual(a2.GetFrameNumber(), 30)
        self.assertEqual(a2.GetUnit(), 'N')
        self.assertEqual(a2.GetGain(), btk.btkAnalog.PlusMinus10)
        self.assertEqual(a2.GetScale(), -0.003458)
        a2.SetLabel('Other')
        a2.SetDescription('Nothing')
        a2.SetFrameNumber(15)
        a2.SetUnit('mV')
        a2.SetGain(btk.btkAnalog.PlusMinus5)
        a2.SetScale(0.0054)
        self.assertEqual(a1.GetLabel(), a2.GetLabel())
        self.assertEqual(a1.GetDescription(), a2.GetDescription())
        self.assertEqual(a1.GetFrameNumber(), a2.GetFrameNumber())
        self.assertEqual(a1.GetUnit(), a2.GetUnit())
        self.assertEqual(a1.GetGain(), a2.GetGain())
        self.assertEqual(a1.GetScale(), a2.GetScale())
        
    def test_clone(self):
        a1 = btk.btkAnalog()
        a1.SetLabel('F1X')
        a1.SetDescription('Force platform 1, X-axis')
        a1.SetFrameNumber(30)
        a1.SetUnit('N')
        a1.SetGain(btk.btkAnalog.PlusMinus10)
        a1.SetScale(-0.003458)
        a2 = a1.Clone()
        self.assertEqual(a2.GetLabel(), 'F1X')
        self.assertEqual(a2.GetDescription(), 'Force platform 1, X-axis')
        self.assertEqual(a2.GetFrameNumber(), 30)
        self.assertEqual(a2.GetUnit(), 'N')
        self.assertEqual(a2.GetGain(), btk.btkAnalog.PlusMinus10)
        self.assertEqual(a2.GetScale(), -0.003458)
        a2.SetLabel('Other')
        a2.SetDescription('Nothing')
        a2.SetFrameNumber(15)
        a2.SetUnit('mV')
        a2.SetGain(btk.btkAnalog.PlusMinus5)
        a2.SetScale(0.0054)
        self.assertEqual(a1.GetLabel(), 'F1X')
        self.assertEqual(a1.GetDescription(), 'Force platform 1, X-axis')
        self.assertEqual(a1.GetFrameNumber(), 30)
        self.assertEqual(a1.GetUnit(), 'N')
        self.assertEqual(a1.GetGain(), btk.btkAnalog.PlusMinus10)
        self.assertEqual(a1.GetScale(), -0.003458)
        self.assertEqual(a2.GetLabel(), 'Other')
        self.assertEqual(a2.GetDescription(), 'Nothing')
        self.assertEqual(a2.GetFrameNumber(), 15)
        self.assertEqual(a2.GetUnit(), 'mV')
        self.assertEqual(a2.GetGain(), btk.btkAnalog.PlusMinus5)
        self.assertEqual(a2.GetScale(), 0.0054)
        
    def test_getset_label(self):
        a = btk.btkAnalog('F1X', 'Force platform 1, X-axis')
        s = a.GetLabel()
        s = 'Other'
        self.assertEqual(a.GetLabel(), 'F1X')
        a.SetLabel(s)
        self.assertEqual(a.GetLabel(), s)
    
    def test_values_getcoeff(self):
        a = btk.btkAnalog(5)
        v = a.GetValues()
        self.assertEqual(v.ndim, 2)
        self.assertEqual(v.size, 5)
        self.assertEqual(v.shape[0], 5)
        self.assertEqual(v[0], 0.0)
        self.assertEqual(v[1], 0.0)
        self.assertEqual(v[2], 0.0)
        self.assertEqual(v[3], 0.0)
        self.assertEqual(v[4], 0.0)
        
    def test_values_setcoeff(self):
        a = btk.btkAnalog(5)
        v = a.GetValues()
        self.assertEqual(v.ndim, 2)
        self.assertEqual(v.size, 5)
        self.assertEqual(a.GetValues()[0], 0.0)
        v[0] = 150.0;
        a.SetValues(v)
        v = a.GetValues()
        self.assertEqual(v.ndim, 2)
        self.assertEqual(v.size, 5)
        self.assertEqual(a.GetValues()[0], 150.0)

    def test_values_copy(self):
        a1 = btk.btkAnalog(5)
        a1.SetValue(0, 150.0)
        a2 = a1
        self.assertEqual(a2.GetValue(0), 150.0)
        a2.SetValue(1, 49.5);
        self.assertEqual(a1.GetValues()[1], 49.5)
        self.assertEqual(a2.GetValues()[1], 49.5)

    def test_values_clone(self):
        a1 = btk.btkAnalog(5)
        a1.SetValue(0, 150.0)
        a2 = a1.Clone()
        self.assertEqual(a2.GetValues()[0], 150.0)
        a2.SetValue(1, 49.5);
        self.assertEqual(a1.GetValues()[1], 0.0)
        self.assertEqual(a2.GetValues()[1], 49.5)