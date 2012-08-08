import btk
import unittest

class DownsampleFilterTest(unittest.TestCase):
    def test_wrench_case1(self):
      wrench = btk.btkWrench(100)
      wdf = btk.btkWrenchDownsampleFilter();
      wdf.SetInput(wrench)
      wdf.SetUpDownRatio(10)
      wdf.Update()
      self.assertEqual(wdf.GetOutput().GetComponent(0).GetFrameNumber(), 10)
      self.assertEqual(wdf.GetOutput().GetComponent(1).GetFrameNumber(), 10)
      self.assertEqual(wdf.GetOutput().GetComponent(2).GetFrameNumber(), 10)
      
    def test_wrench_case2(self):
      wrench = btk.btkWrench(100)
      wdf = btk.btkWrenchDownsampleFilter();
      wdf.SetInput(wrench)
      wdf.SetUpDownRatio(5)
      wdf.Update()
      self.assertEqual(wdf.GetOutput().GetComponent(0).GetFrameNumber(), 20)
      self.assertEqual(wdf.GetOutput().GetComponent(1).GetFrameNumber(), 20)
      self.assertEqual(wdf.GetOutput().GetComponent(2).GetFrameNumber(), 20)
      
    def test_wrenchCollection_case1(self):
      collection = btk.btkWrenchCollection()
      collection.InsertItem(btk.btkWrench(100))
      collection.InsertItem(btk.btkWrench(200))
      collection.InsertItem(btk.btkWrench(300))
      wcdf = btk.btkWrenchCollectionDownsampleFilter();
      wcdf.SetInput(collection)
      wcdf.SetUpDownRatio(10)
      wcdf.Update()
      self.assertEqual(wcdf.GetOutput().GetItem(0).GetComponent(0).GetFrameNumber(), 10)
      self.assertEqual(wcdf.GetOutput().GetItem(0).GetComponent(1).GetFrameNumber(), 10)
      self.assertEqual(wcdf.GetOutput().GetItem(0).GetComponent(2).GetFrameNumber(), 10)
      self.assertEqual(wcdf.GetOutput().GetItem(1).GetComponent(0).GetFrameNumber(), 20)
      self.assertEqual(wcdf.GetOutput().GetItem(1).GetComponent(1).GetFrameNumber(), 20)
      self.assertEqual(wcdf.GetOutput().GetItem(1).GetComponent(2).GetFrameNumber(), 20)
      self.assertEqual(wcdf.GetOutput().GetItem(2).GetComponent(0).GetFrameNumber(), 30)
      self.assertEqual(wcdf.GetOutput().GetItem(2).GetComponent(1).GetFrameNumber(), 30)
      self.assertEqual(wcdf.GetOutput().GetItem(2).GetComponent(2).GetFrameNumber(), 30)