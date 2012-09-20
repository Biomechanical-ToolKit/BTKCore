import btk
import unittest
import _TDDConfigure

class SubAcquisitionFilterTest(unittest.TestCase):
  def test_TestAllNoEffect(self):
    acq = btk.btkAcquisition()
    acq.Init(10,25,5,2)
    acq.AppendEvent(btk.btkEvent())
    acq.SetFirstFrame(10)
    acq.SetPointFrequency(25.0)
    sub = btk.btkSubAcquisitionFilter()
    sub.SetInput(acq)
    sub.Update()
    output = sub.GetOutput()
    # self.assertEqual(output.GetPoints(), acq.GetPoints())
    # self.assertEqual(output.GetAnalogs(), acq.GetAnalogs())
    # self.assertEqual(output.GetEvents(), acq.GetEvents())
    # self.assertEqual(output.GetMetaData(), acq.GetMetaData())
    self.assertEqual(output.GetFirstFrame(), acq.GetFirstFrame())
    self.assertEqual(output.GetPointFrequency(), acq.GetPointFrequency())
    self.assertEqual(output.GetPointFrameNumber(), acq.GetPointFrameNumber())
    self.assertEqual(output.GetPointNumber(), acq.GetPointNumber())
    self.assertEqual(output.GetAnalogNumber(), acq.GetAnalogNumber())
    self.assertEqual(output.GetEventNumber(), 1)
    self.assertEqual(output.GetNumberAnalogSamplePerFrame(), acq.GetNumberAnalogSamplePerFrame())
  
  def test_TestAllSubFrame(self):
    acq = btk.btkAcquisition()
    acq.Init(10,25,5,2)
    acq.AppendEvent(btk.btkEvent("", 12))
    acq.SetFirstFrame(10)
    acq.SetPointFrequency(25.0)
    for i in range(0,10):
      acq.GetPoint(i).SetValue(0,0,10.0)
    for i in range(0,5):
      acq.GetAnalog(i).SetValue(2,10.0)
    sub = btk.btkSubAcquisitionFilter()
    sub.SetInput(acq)
    sub.SetFramesIndex(0,9) # Ten first frames
    sub.Update()
    output = sub.GetOutput()
    self.assertEqual(output.GetFirstFrame(), acq.GetFirstFrame())
    self.assertEqual(output.GetPointFrequency(), acq.GetPointFrequency())
    self.assertEqual(output.GetPointFrameNumber(), 10)
    self.assertEqual(output.GetNumberAnalogSamplePerFrame(), acq.GetNumberAnalogSamplePerFrame())
    self.assertEqual(output.GetEventNumber(), 1)
    self.assertEqual(output.GetPointNumber(), acq.GetPointNumber())
    self.assertEqual(output.GetAnalogNumber(), acq.GetAnalogNumber())
    # self.assertEqual(output.GetMetaData(), acq.GetMetaData())
    # self.assertEqual(output.GetEvent(0), acq.GetEvent(0))
    for i in range(0,10):
      self.assertEqual(output.GetPoint(i).GetValues()[0,0], 10.0)
    for i in range(0,5):
      self.assertEqual(output.GetAnalog(i).GetValues()[2], 10.0)
  
  def test_TestAllSubFrameBis(self):
    acq = btk.btkAcquisition()
    acq.Init(10,25,5,2)
    acq.AppendEvent(btk.btkEvent("", 10))
    acq.SetFirstFrame(10)
    acq.SetPointFrequency(25.0)
    for i in range(0,10):
      acq.GetPoint(i).SetValue(0,0,10.0)
    for i in range(0,5):
      acq.GetAnalog(i).SetValue(2,10.0)
    sub = btk.btkSubAcquisitionFilter()
    sub.SetInput(acq)
    sub.SetFramesIndex(1,9) # Nine first frames
    sub.Update()
    output = sub.GetOutput()
    self.assertEqual(output.GetFirstFrame(), acq.GetFirstFrame())
    self.assertEqual(output.GetPointFrequency(), acq.GetPointFrequency())
    self.assertEqual(output.GetPointFrameNumber(), 9)
    self.assertEqual(output.GetNumberAnalogSamplePerFrame(), acq.GetNumberAnalogSamplePerFrame())
    self.assertEqual(output.GetEventNumber(), 0)
    self.assertEqual(output.GetPointNumber(), acq.GetPointNumber())
    self.assertEqual(output.GetAnalogNumber(), acq.GetAnalogNumber())
    # self.assertEqual(output.GetMetaData(), acq.GetMetaData())
    for i in range(0,10):
      self.assertEqual(output.GetPoint(i).GetValues()[0,0], 0.0)
    for i in range(0,5):
      self.assertEqual(output.GetAnalog(i).GetValues()[0], 10.0)
  
  def test_TestOnlyPoints(self):
    acq = btk.btkAcquisition()
    acq.Init(10,25,5,2)
    acq.AppendEvent(btk.btkEvent("", 12))
    acq.SetFirstFrame(10)
    acq.SetPointFrequency(25.0)
    for i in range(0,10):
      acq.GetPoint(i).SetValue(0,0,10.0)
    for i in range(0,5):
      acq.GetAnalog(i).SetValue(2,10.0)
    sub = btk.btkSubAcquisitionFilter()
    sub.SetInput(acq)
    sub.SetFramesIndex(0,9)
    sub.SetExtractionOption(btk.btkSubAcquisitionFilter.PointsOnly)
    sub.Update()
    output = sub.GetOutput()
    self.assertEqual(output.GetFirstFrame(), acq.GetFirstFrame())
    self.assertEqual(output.GetPointFrequency(), acq.GetPointFrequency())
    self.assertEqual(output.GetPointFrameNumber(), 10)
    self.assertEqual(output.GetNumberAnalogSamplePerFrame(), acq.GetNumberAnalogSamplePerFrame())
    self.assertEqual(output.GetEventNumber(), 0)
    self.assertEqual(output.GetPointNumber(), acq.GetPointNumber())
    self.assertEqual(output.GetAnalogNumber(), 0)
    # self.assertEqual(output.GetMetaData(), acq.GetMetaData())
    for i in range(0,10):
      self.assertEqual(output.GetPoint(i).GetValues()[0,0], 10.0)
  
  def test_TestOnlyFivePoints(self):
    acq = btk.btkAcquisition()
    acq.Init(10,25,5,2)
    acq.AppendEvent(btk.btkEvent("", 12))
    acq.SetFirstFrame(10)
    acq.SetPointFrequency(25.0)
    for i in range(0,10):
      acq.GetPoint(i).SetValue(0,0,10.0)
    for i in range(0,5):
      acq.GetAnalog(i).SetValue(2,10.0)
    sub = btk.btkSubAcquisitionFilter()
    sub.SetInput(acq)
    sub.SetFramesIndex(0,9)
    sub.SetExtractionOption(btk.btkSubAcquisitionFilter.PointsOnly, [0,2,4,6,8])
    sub.Update()
    output = sub.GetOutput()
    self.assertEqual(output.GetFirstFrame(), acq.GetFirstFrame())
    self.assertEqual(output.GetPointFrequency(), acq.GetPointFrequency())
    self.assertEqual(output.GetPointFrameNumber(), 10)
    self.assertEqual(output.GetNumberAnalogSamplePerFrame(), acq.GetNumberAnalogSamplePerFrame())
    self.assertEqual(output.GetEventNumber(), 0)
    self.assertEqual(output.GetPointNumber(), 5)
    self.assertEqual(output.GetAnalogNumber(), 0)
    # self.assertEqual(output.GetMetaData(), acq.GetMetaData())
    self.assertEqual(output.GetPoint(0).GetLabel(), "uname*1")
    self.assertEqual(output.GetPoint(1).GetLabel(), "uname*3")
    self.assertEqual(output.GetPoint(2).GetLabel(), "uname*5")
    self.assertEqual(output.GetPoint(3).GetLabel(), "uname*7")
    self.assertEqual(output.GetPoint(4).GetLabel(), "uname*9")
    for i in range(0,5):
      self.assertEqual(output.GetPoint(i).GetValues()[0,0], 10.0)
  
  def test_TestOnlyAnalogs(self):
    acq = btk.btkAcquisition()
    acq.Init(10,25,5,2)
    acq.AppendEvent(btk.btkEvent("", 12))
    acq.SetFirstFrame(10)
    acq.SetPointFrequency(25.0)
    for i in range(0,10):
      acq.GetPoint(i).SetValue(0,0,10.0)
    for i in range(0,5):
      acq.GetAnalog(i).SetValue(2,10.0)
    sub = btk.btkSubAcquisitionFilter()
    sub.SetInput(acq)
    sub.SetFramesIndex(0,9)
    sub.SetExtractionOption(btk.btkSubAcquisitionFilter.AnalogsOnly)
    sub.Update()
    output = sub.GetOutput()
    self.assertEqual(output.GetFirstFrame(), acq.GetFirstFrame())
    self.assertEqual(output.GetPointFrequency(), acq.GetPointFrequency())
    self.assertEqual(output.GetPointFrameNumber(), 10)
    self.assertEqual(output.GetNumberAnalogSamplePerFrame(), acq.GetNumberAnalogSamplePerFrame())
    self.assertEqual(output.GetEventNumber(), 0)
    self.assertEqual(output.GetPointNumber(), 0)
    self.assertEqual(output.GetAnalogNumber(), 5)
    # self.assertEqual(output.GetMetaData(), acq.GetMetaData())
    for i in range(0,5):
      self.assertEqual(output.GetAnalog(i).GetValues()[2], 10.0)
  
  def test_TestOnlyTwoAnalogsSubFrame(self):
    acq = btk.btkAcquisition()
    acq.Init(10,25,5,2)
    acq.AppendEvent(btk.btkEvent("", 12))
    acq.SetFirstFrame(10)
    acq.SetPointFrequency(25.0)
    for i in range(0,10):
      acq.GetPoint(i).SetValue(0,0,10.0)
    for i in range(0,5):
      acq.GetAnalog(i).SetValue(2,10.0)
    sub = btk.btkSubAcquisitionFilter()
    sub.SetInput(acq)
    sub.SetFramesIndex(1,9)
    ids = [1,3]
    sub.SetExtractionOption(btk.btkSubAcquisitionFilter.AnalogsOnly, ids)
    sub.Update()
    output = sub.GetOutput()
    self.assertEqual(output.GetFirstFrame(), acq.GetFirstFrame())
    self.assertEqual(output.GetPointFrequency(), acq.GetPointFrequency())
    self.assertEqual(output.GetPointFrameNumber(), 9)
    self.assertEqual(output.GetNumberAnalogSamplePerFrame(), acq.GetNumberAnalogSamplePerFrame())
    self.assertEqual(output.GetEventNumber(), 0)
    self.assertEqual(output.GetPointNumber(), 0)
    self.assertEqual(output.GetAnalogNumber(), 2)
    # self.assertEqual(output.GetMetaData(), acq.GetMetaData())
    self.assertEqual(output.GetAnalog(0).GetLabel(), "uname*2")
    self.assertEqual(output.GetAnalog(1).GetLabel(), "uname*4")
    for i in range(0,2):
      self.assertEqual(output.GetAnalog(i).GetValues()[0], 10.0)
  
  def test_TestOnlyEventsSubFrame(self):
    acq = btk.btkAcquisition()
    acq.Init(10,25,5,2)
    acq.AppendEvent(btk.btkEvent("", 10))
    acq.AppendEvent(btk.btkEvent("", 12))
    acq.AppendEvent(btk.btkEvent("", 15))
    acq.AppendEvent(btk.btkEvent("", 20))
    acq.AppendEvent(btk.btkEvent("", 21))
    acq.AppendEvent(btk.btkEvent("", 25))
    acq.SetFirstFrame(10)
    acq.SetPointFrequency(25.0)
    for i in range(0,10):
      acq.GetPoint(i).SetValue(0,0,10.0)
    for i in range(0,5):
      acq.GetAnalog(i).SetValue(2,10.0)
    sub = btk.btkSubAcquisitionFilter()
    sub.SetInput(acq)
    sub.SetFramesIndex(1,9)
    sub.SetExtractionOption(btk.btkSubAcquisitionFilter.EventsOnly)
    sub.Update()
    output = sub.GetOutput()
    self.assertEqual(output.GetFirstFrame(), acq.GetFirstFrame())
    self.assertEqual(output.GetPointFrequency(), acq.GetPointFrequency())
    self.assertEqual(output.GetPointFrameNumber(), 9)
    self.assertEqual(output.GetNumberAnalogSamplePerFrame(), acq.GetNumberAnalogSamplePerFrame())
    self.assertEqual(output.GetEventNumber(), 2)
    self.assertEqual(output.GetPointNumber(), 0)
    self.assertEqual(output.GetAnalogNumber(), 0)
    # self.assertEqual(output.GetMetaData(), acq.GetMetaData())
    self.assertEqual(output.GetEvent(0).GetFrame(), 12)
    self.assertEqual(output.GetEvent(1).GetFrame(), 15)
  