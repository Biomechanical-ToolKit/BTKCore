import btk
import unittest

class WrenchTest(unittest.TestCase):
    def test_Constructor_default(self):
        test = btk.btkWrench()
        self.assertEqual(test.GetPosition().GetLabel(), "")
        self.assertEqual(test.GetPosition().GetType(), btk.btkPoint.Marker)
        self.assertEqual(test.GetForce().GetLabel(), ".F")
        self.assertEqual(test.GetForce().GetType(), btk.btkPoint.Force)
        self.assertEqual(test.GetMoment().GetLabel(), ".M")
        self.assertEqual(test.GetMoment().GetType(), btk.btkPoint.Moment)
        
    def test_Constructor_label(self):
        test = btk.btkWrench("KneeJoint")
        self.assertEqual(test.GetPosition().GetLabel(), "KneeJoint")
        self.assertEqual(test.GetPosition().GetType(), btk.btkPoint.Marker)
        self.assertEqual(test.GetForce().GetLabel(), "KneeJoint.F")
        self.assertEqual(test.GetForce().GetType(), btk.btkPoint.Force)
        self.assertEqual(test.GetMoment().GetLabel(), "KneeJoint.M")
        self.assertEqual(test.GetMoment().GetType(), btk.btkPoint.Moment)
    
    def test_Constructor_fn(self):
      test = btk.btkWrench(10)
      self.assertEqual(test.GetPosition().GetLabel(), "")
      self.assertEqual(test.GetPosition().GetValues().shape[0], 10)
      self.assertEqual(test.GetForce().GetLabel(), ".F")
      self.assertEqual(test.GetForce().GetValues().shape[0], 10)
      self.assertEqual(test.GetMoment().GetLabel(), ".M")
      self.assertEqual(test.GetMoment().GetValues().shape[0], 10)
      
    def test_Constructor_full(self):
      test = btk.btkWrench("KneeJoint", 10)
      self.assertEqual(test.GetPosition().GetLabel(), "KneeJoint")
      self.assertEqual(test.GetPosition().GetValues().shape[0], 10)
      self.assertEqual(test.GetForce().GetLabel(), "KneeJoint.F")
      self.assertEqual(test.GetForce().GetValues().shape[0], 10)
      self.assertEqual(test.GetMoment().GetLabel(), "KneeJoint.M")
      self.assertEqual(test.GetMoment().GetValues().shape[0], 10)
      
    def test_GetComponent(self):
      test = btk.btkWrench()
      self.assertEqual(test.GetComponent(0).GetLabel(), "")
      self.assertEqual(test.GetComponent(0).GetType(), btk.btkPoint.Marker)
      self.assertEqual(test.GetComponent(1).GetLabel(), ".F")
      self.assertEqual(test.GetComponent(1).GetType(), btk.btkPoint.Force)
      self.assertEqual(test.GetComponent(2).GetLabel(), ".M")
      self.assertEqual(test.GetComponent(2).GetType(), btk.btkPoint.Moment)
      
    def test_SetFrameNumber(self):
      test = btk.btkWrench(10)
      test.SetFrameNumber(20);
      self.assertEqual(test.GetPosition().GetValues().shape[0], 20)
      self.assertEqual(test.GetForce().GetValues().shape[0], 20)
      self.assertEqual(test.GetMoment().GetValues().shape[0], 20)
      
    def test_SetComponent(self):
      test = btk.btkWrench()
      p = btk.btkPoint("KneeJointCenter",1)
      f = btk.btkPoint("KneeJointForce",1)
      m = btk.btkPoint("KneeJointMoment",1)
      test.SetPosition(p);
      test.SetForce(f);
      test.SetMoment(m);
      test.SetFrameNumber(10);
      self.assertEqual(test.GetPosition().GetLabel(), "KneeJointCenter")
      self.assertEqual(test.GetPosition().GetType(), btk.btkPoint.Marker)
      self.assertEqual(test.GetPosition().GetValues().shape[0], 10)
      self.assertEqual(test.GetForce().GetLabel(), "KneeJointForce")
      self.assertEqual(test.GetForce().GetType(), btk.btkPoint.Force)
      self.assertEqual(test.GetForce().GetValues().shape[0], 10)
      self.assertEqual(test.GetMoment().GetLabel(), "KneeJointMoment")
      self.assertEqual(test.GetMoment().GetType(), btk.btkPoint.Moment)
      self.assertEqual(test.GetMoment().GetValues().shape[0], 10)
      self.assertEqual(p.GetType(), btk.btkPoint.Marker)
      self.assertEqual(p.GetValues().shape[0], 10)
      self.assertEqual(f.GetType(), btk.btkPoint.Force)
      self.assertEqual(f.GetValues().shape[0], 10)
      self.assertEqual(m.GetType(), btk.btkPoint.Moment)
      self.assertEqual(m.GetValues().shape[0], 10)