import btk
import unittest

class MetaDataTest(unittest.TestCase):
    def test_SimpleCtor(self):
        test = btk.btkMetaData('POINT')
        self.assertEqual(test.GetLabel(), 'POINT')
        self.assertEqual(test.GetDescription(), '')
        self.assertEqual(test.GetUnlockState(), True)
        self.assertEqual(test.HasInfo(), False)
  
    def test_CtorWithValue(self):
        test = btk.btkMetaData('SCALE', 0.145, 'point scale')
        self.assertEqual(test.GetLabel(), 'SCALE')
        self.assertEqual(test.GetDescription(), 'point scale')
        self.assertEqual(test.GetUnlockState(), True)
        self.assertEqual(test.HasInfo(), True)
        val = test.GetInfo()
        values = val.ToString()
        self.assertEqual(values[0], '0.145')
  
    def test_CtorAndChild(self):
        point = btk.btkMetaData('POINT')
        pointUsed = btk.btkMetaData('USED', 16)
        pointScale = btk.btkMetaData('SCALE', -0.0833)
        point.AppendChild(pointUsed)
        point.AppendChild(pointScale)
        self.assertEqual(point.GetChildNumber(), 2)
  
    def test_Clone(self):
        point = btk.btkMetaData('POINT')
        pointUsed = btk.btkMetaData('USED', 16)
        pointScale = btk.btkMetaData('SCALE', -0.0833)
        point.AppendChild(pointUsed)
        point.AppendChild(pointScale)
        point2 = point.Clone()
        point2.GetChild(0).SetLabel('FRAMES')
        self.assertEqual(point.GetChild(0).GetLabel(), 'USED')
        self.assertEqual(point2.GetChild(0).GetLabel(), 'FRAMES')
        point2.GetChild(0).GetInfo().SetValues(1300)
        values = point.GetChild(0).GetInfo().ToString()
        self.assertEqual(values[0], '16')
        values = point2.GetChild(0).GetInfo().ToString()
        self.assertEqual(values[0], '1300')
  
    def test_Equality(self):
        point = btk.btkMetaData('POINT')
        pointUsed = btk.btkMetaData('USED', 16)
        pointScale = btk.btkMetaData('SCALE', -0.0833)
        point.AppendChild(pointUsed)
        point.AppendChild(pointScale)
        self.assertTrue(point == point)
        self.assertTrue(point != pointUsed)
        point2 = point.Clone()
        self.assertTrue(point == point2)
        point2.GetChild('SCALE').GetInfo().SetValue(0, 10.0)
        self.assertTrue(point != point2)
        point2.GetChild('SCALE').GetInfo().SetValue(0, -0.0833)
        self.assertTrue(point == point2)
        point2.GetChild('USED').GetInfo().SetValue(0, 10)
        self.assertTrue(point != point2)
  
    def test_Find(self):
        point = btk.btkMetaData('POINT')
        pointUsed = btk.btkMetaData('USED', 16)
        pointScale = btk.btkMetaData('SCALE', -0.0833)
        point.AppendChild(pointUsed)
        point.AppendChild(pointScale)
        self.assertEqual(point.FindChild('USED').value(), pointUsed)
  
    def test_TakeChild(self):
        point = btk.btkMetaData('POINT')
        pointUsed = btk.btkMetaData('USED', 16)
        pointScale = btk.btkMetaData('SCALE', -0.0833)
        point.AppendChild(pointUsed)
        point.AppendChild(pointScale)
        pointScale2 = point.TakeChild(1)
        self.assertEqual(point.GetChildNumber(), 1)
        self.assertEqual(point.GetChild(0), pointUsed)
        pointScale2.SetLabel('FRAMES')
        self.assertEqual(pointScale.GetLabel(), 'FRAMES')
  
    def test_InsertAndSetChildren(self):
        point = btk.btkMetaData('POINT')
        pointUsed = btk.btkMetaData('USED', 0)
        pointScale = btk.btkMetaData('SCALE', -0.0833)
        pointLabels = btk.btkMetaData('LABELS', [32, 0], [])
        pointFrames = btk.btkMetaData('FRAMES', 1200)
        pointOther = btk.btkMetaData('OTHER', 4)
        point.AppendChild(pointUsed)
        point.AppendChild(pointScale)
        self.assertEqual(point.InsertChild(0, pointLabels), True)
        self.assertEqual(point.GetChildNumber(), 3)
        self.assertEqual(point.GetChild(0), pointLabels)
        self.assertEqual(point.InsertChild(15, pointLabels), False)
        point.SetChild(0, pointLabels)
        point.SetChild(2, pointOther)
        self.assertEqual(point.GetChild(2), pointOther)
        self.assertEqual(point.GetChildNumber(), 3)

    def test_UtilsCreateChild(self):
        root = btk.btkMetaData('ROOT')
        point = btk.btkMetaData('POINT')
        point.SetDescription('point group')
        point.SetUnlockState(False)
        root.AppendChild(point)
        self.assertEqual(root.GetChildNumber(), 1)
        self.assertEqual(point.GetLabel(), 'POINT')
        self.assertEqual(len(point.GetDescription()), 11)
        self.assertEqual(point.GetUnlockState(), False)
        point2 = btk.btkMetaDataCreateChild(root, 'POINT')
        self.assertEqual(root.GetChildNumber(), 1)
        self.assertEqual(point.GetLabel(), 'POINT')
        self.assertEqual(len(point.GetDescription()), 0)
        self.assertEqual(point2.GetUnlockState(), True)
        self.assertEqual(point2.GetLabel(), 'POINT')
        self.assertEqual(len(point2.GetDescription()), 0)
        self.assertEqual(point2.GetUnlockState(), True)

    def test_UtilsCreateChildFloat1Value(self):
        analog = btk.btkMetaData('ANALOG')
        btk.btkMetaDataCreateChild(analog, 'SCALE', 0.08333)
        scale = analog.GetChild('SCALE')
        self.assertEqual(analog.GetChildNumber(), 1)
        self.assertEqual(scale.GetLabel(), 'SCALE')
        self.assertEqual(len(scale.GetDescription()), 0)
        self.assertAlmostEqual(scale.GetInfo().ToDouble()[0], 0.08333, 5)

    def test_UtilsCreateChildFloat12Values(self):
        analog = btk.btkMetaData('ANALOG')
        btk.btkMetaDataCreateChild(analog, 'SCALE', btk.btkDoubleArray(12, 0.08333))
        scale = analog.GetChild('SCALE')        
        self.assertEqual(analog.GetChildNumber(), 1)
        self.assertEqual(scale.GetLabel(), 'SCALE')
        self.assertEqual(len(scale.GetDescription()), 0)
        self.assertEqual(scale.GetInfo().GetValueNumber(), 12)

    def test_UtilsCreateChildFloat300Values(self):
        analog = btk.btkMetaData('ANALOG')
        btk.btkMetaDataCreateChild(analog, 'SCALE', btk.btkDoubleArray(300, 0.08333))
        scale = analog.GetChild('SCALE')        
        self.assertEqual(analog.GetChildNumber(), 2)
        self.assertEqual(scale.GetLabel(), 'SCALE')
        self.assertEqual(len(scale.GetDescription()), 0)
        self.assertEqual(scale.GetInfo().GetValueNumber(), 255)
        scale2 = analog.GetChild('SCALE2')
        self.assertEqual(scale2.GetLabel(), 'SCALE2')
        self.assertEqual(len(scale2.GetDescription()), 0)
        self.assertEqual(scale2.GetInfo().GetValueNumber(), 45)

    def test_UtilsMetaDataCollapseString(self):
        point = btk.btkMetaData('POINT')
        btk.btkMetaDataCreateChild(point, 'LABELS', btk.btkStringArray(1120, 'TESTS'))
        self.assertEqual(point.GetChildNumber(), 5)        
        labels = point.GetChild('LABELS')        
        self.assertEqual(labels.GetInfo().GetValueNumber(), 255)
        labels2 = point.GetChild('LABELS2')
        self.assertEqual(labels2.GetInfo().GetValueNumber(), 255)
        labels3 = point.GetChild('LABELS3')
        self.assertEqual(labels3.GetInfo().GetValueNumber(), 255)
        labels4 = point.GetChild('LABELS4')
        self.assertEqual(labels4.GetInfo().GetValueNumber(), 255)
        labels5 = point.GetChild('LABELS5')
        self.assertEqual(labels5.GetInfo().GetValueNumber(), 100)
        values = btk.btkMetaDataCollapseChildrenValuesString(point, 'LABELS')
        self.assertEqual(len(values), 1120)

    def test_UtilsMetaDataCollapseStringAndBlank(self):
        point = btk.btkMetaData('POINT')
        btk.btkMetaDataCreateChild(point, 'LABELS', btk.btkStringArray(125, 'TESTS'))
        values = btk.btkMetaDataCollapseChildrenValuesString(point, 'LABELS', 150, 'uname*')
        self.assertEqual(len(values), 150)
        for i in range(125,150):
            self.assertEqual(values[i], 'uname*' + str(i+1))

    def test_UtilsMetaDataCollapseInteger(self):
        point = btk.btkMetaData('POINT')
        btk.btkMetaDataCreateChild(point, 'LABELS', btk.btkIntArray(1120, 54))
        self.assertEqual(point.GetChildNumber(), 5)        
        labels = point.GetChild('LABELS')        
        self.assertEqual(labels.GetInfo().GetValueNumber(), 255)
        labels2 = point.GetChild('LABELS2')
        self.assertEqual(labels2.GetInfo().GetValueNumber(), 255)
        labels3 = point.GetChild('LABELS3')
        self.assertEqual(labels3.GetInfo().GetValueNumber(), 255)
        labels4 = point.GetChild('LABELS4')
        self.assertEqual(labels4.GetInfo().GetValueNumber(), 255)
        labels5 = point.GetChild('LABELS5')
        self.assertEqual(labels5.GetInfo().GetValueNumber(), 100)
        values = btk.btkMetaDataCollapseChildrenValuesInt(point, 'LABELS')
        self.assertEqual(len(values), 1120)

    def test_UtilsMetaDataCollapseIntegerAndBlank(self):
        point = btk.btkMetaData('POINT')
        btk.btkMetaDataCreateChild(point, 'LABELS', btk.btkIntArray(125, 54))
        values = btk.btkMetaDataCollapseChildrenValuesInt(point, 'LABELS', 150)
        self.assertEqual(len(values), 150)
        for i in range(125,150):
            self.assertEqual(values[i], 0)
            
    def test_SetInfo(self):
        test = btk.btkMetaData('Channel')
        test.SetInfo(btk.btkMetaDataInfo(8))
        self.assertEqual(test.HasInfo(), True)
        self.assertEqual(test.GetInfo().ToInt(0), 8)
        
    def test_ExtractChildInfo(self):
        point = btk.btkMetaData('POINT')
        pointUsed = btk.btkMetaData('USED', 16)
        pointScale = btk.btkMetaData('SCALE', -0.0833)
        point.AppendChild(pointUsed)
        point.AppendChild(pointScale)
        