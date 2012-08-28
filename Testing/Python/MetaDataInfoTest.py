import btk
import unittest

class MetaDataInfoTest(unittest.TestCase):
    def test_ctor_int(self):
        i = btk.btkMetaDataInfo(5)
        self.assertEqual(i.GetFormat(), btk.btkMetaDataInfo.Integer)
        self.assertEqual(len(i.GetDimensions()), 0)
        self.assertEqual(i.GetDimensionsProduct(), 1)
        self.assertEqual(i.ToInt(0), 5)
        self.assertEqual(i.ToDouble(0), 5.0)
        self.assertEqual(i.ToString(0), '5')
        v = i.ToInt()
        self.assertEqual(len(v), 1)
        self.assertEqual(v[0], 5)
        v = i.ToDouble()
        self.assertEqual(len(v), 1)
        self.assertEqual(v[0], 5.0)
        v = i.ToString()
        self.assertEqual(len(v), 1)
        self.assertEqual(v[0], '5')
        
    def test_ctor_double(self):
        i = btk.btkMetaDataInfo(5.5)
        self.assertEqual(i.GetFormat(), btk.btkMetaDataInfo.Real)
        self.assertEqual(len(i.GetDimensions()), 0)
        self.assertEqual(i.GetDimensionsProduct(), 1)
        self.assertEqual(i.ToInt(0), 5)
        self.assertEqual(i.ToDouble(0), 5.5)
        self.assertEqual(i.ToString(0), '5.5')
        v = i.ToInt()
        self.assertEqual(len(v), 1)
        self.assertEqual(v[0], 5)
        v = i.ToDouble()
        self.assertEqual(len(v), 1)
        self.assertEqual(v[0], 5.5)
        v = i.ToString()
        self.assertEqual(len(v), 1)
        self.assertEqual(v[0], '5.5')
        
    def test_ctor_string(self):
        i = btk.btkMetaDataInfo('FOO')
        self.assertEqual(i.GetFormat(), btk.btkMetaDataInfo.Char)
        self.assertEqual(len(i.GetDimensions()), 1)
        self.assertEqual(i.GetDimension(0), 3)
        self.assertEqual(i.GetDimensionsProduct(), 3)
        self.assertEqual(i.ToInt(0), 0)
        self.assertEqual(i.ToDouble(0), 0)
        self.assertEqual(i.ToString(0), 'FOO')
        v = i.ToInt()
        self.assertEqual(len(v), 1)
        self.assertEqual(v[0], 0)
        v = i.ToDouble()
        self.assertEqual(len(v), 1)
        self.assertEqual(v[0], 0)
        v = i.ToString()
        self.assertEqual(len(v), 1)
        self.assertEqual(v[0], 'FOO')
        
    def test_ctor_string_bis(self):
        i = btk.btkMetaDataInfo('45.25')
        self.assertEqual(i.GetFormat(), btk.btkMetaDataInfo.Char)
        self.assertEqual(len(i.GetDimensions()), 1)
        self.assertEqual(i.GetDimension(0), 5)
        self.assertEqual(i.GetDimensionsProduct(), 5)
        self.assertEqual(i.ToInt(0), 45)
        self.assertEqual(i.ToDouble(0), 45.25)
        self.assertEqual(i.ToString(0), '45.25')
        v = i.ToInt()
        self.assertEqual(len(v), 1)
        self.assertEqual(v[0], 45)
        v = i.ToDouble()
        self.assertEqual(len(v), 1)
        self.assertEqual(v[0], 45.25)
        v = i.ToString()
        self.assertEqual(len(v), 1)
        self.assertEqual(v[0], '45.25')
        
    def test_ctor_int_vector1D(self):
        i = btk.btkMetaDataInfo([1,2,3,4])
        self.assertEqual(i.GetFormat(), btk.btkMetaDataInfo.Integer)
        self.assertEqual(len(i.GetDimensions()), 1)
        self.assertEqual(i.GetDimension(0), 4)
        self.assertEqual(i.GetDimensionsProduct(), 4)
        v1 = i.ToInt()
        v2 = i.ToDouble()
        v3 = i.ToString()
        self.assertEqual(len(v1), 4)
        self.assertEqual(len(v2), 4)
        self.assertEqual(len(v3), 4)
        for j in range(0,4):
            self.assertEqual(i.ToInt(j), j + 1)
            self.assertEqual(i.ToDouble(j), float(j + 1))
            self.assertEqual(i.ToString(j), str(j + 1))
            self.assertEqual(v1[j], j + 1)
            self.assertEqual(v2[j], float(j + 1))
            self.assertEqual(v3[j], str(j + 1))
            
    def test_ctor_double_vector1D(self):
        i = btk.btkMetaDataInfo([1.5,2.5,3.5,4.5])
        self.assertEqual(i.GetFormat(), btk.btkMetaDataInfo.Real)
        self.assertEqual(len(i.GetDimensions()), 1)
        self.assertEqual(i.GetDimension(0), 4)
        self.assertEqual(i.GetDimensionsProduct(), 4)
        v1 = i.ToInt()
        v2 = i.ToDouble()
        v3 = i.ToString()
        self.assertEqual(len(v1), 4)
        self.assertEqual(len(v2), 4)
        self.assertEqual(len(v3), 4)
        for j in range(0,4):
            self.assertEqual(i.ToInt(j), j + 1)
            self.assertEqual(i.ToDouble(j), float(j + 1.5))
            self.assertEqual(i.ToString(j), str(j + 1.5))
            self.assertEqual(v1[j], j + 1)
            self.assertEqual(v2[j], float(j + 1.5))
            self.assertEqual(v3[j], str(j + 1.5))
            
    def test_ctor_string_vector1D(self):
        i = btk.btkMetaDataInfo(['FOO', 'BAR', 'FOOBAR'])
        self.assertEqual(i.GetFormat(), btk.btkMetaDataInfo.Char)
        self.assertEqual(len(i.GetDimensions()), 2)
        self.assertEqual(i.GetDimension(0), 6)
        self.assertEqual(i.GetDimension(1), 3)
        self.assertEqual(i.GetDimensionsProduct(), 18)
        v1 = i.ToInt()
        v2 = i.ToDouble()
        v3 = i.ToString()
        self.assertEqual(len(v1), 3)
        self.assertEqual(len(v2), 3)
        self.assertEqual(len(v3), 3)
        for j in range(0,3):
            self.assertEqual(i.ToInt(j), 0)
            self.assertEqual(i.ToDouble(j), 0)
            self.assertEqual(v1[j], 0)
            self.assertEqual(v2[j], 0)
        self.assertEqual(i.ToString(0), 'FOO   ')
        self.assertEqual(v3[0], 'FOO   ')
        self.assertEqual(i.ToString(1), 'BAR   ')
        self.assertEqual(v3[1], 'BAR   ')
        self.assertEqual(i.ToString(2), 'FOOBAR')
        self.assertEqual(v3[2], 'FOOBAR')
        
    def test_ctor_int_vector2D(self):
        i = btk.btkMetaDataInfo([2,2], [1,2,3,4])
        self.assertEqual(i.GetFormat(), btk.btkMetaDataInfo.Integer)
        self.assertEqual(len(i.GetDimensions()), 2)
        self.assertEqual(i.GetDimension(0), 2)
        self.assertEqual(i.GetDimension(1), 2)
        self.assertEqual(i.GetDimensionsProduct(), 4)
        v1 = i.ToInt()
        v2 = i.ToDouble()
        v3 = i.ToString()
        self.assertEqual(len(v1), 4)
        self.assertEqual(len(v2), 4)
        self.assertEqual(len(v3), 4)
        for j in range(0,4):
            self.assertEqual(i.ToInt(j), j + 1)
            self.assertEqual(i.ToDouble(j), float(j + 1))
            self.assertEqual(i.ToString(j), str(j + 1))
            self.assertEqual(v1[j], j + 1)
            self.assertEqual(v2[j], float(j + 1))
            self.assertEqual(v3[j], str(j + 1))
        
    def test_ctor_double_vector2D(self):
        i = btk.btkMetaDataInfo([2,2], [1.5,2.5,3.5,4.5])
        self.assertEqual(i.GetFormat(), btk.btkMetaDataInfo.Real)
        self.assertEqual(len(i.GetDimensions()), 2)
        self.assertEqual(i.GetDimension(0), 2)
        self.assertEqual(i.GetDimensionsProduct(), 4)
        v1 = i.ToInt()
        v2 = i.ToDouble()
        v3 = i.ToString()
        self.assertEqual(len(v1), 4)
        self.assertEqual(len(v2), 4)
        self.assertEqual(len(v3), 4)
        for j in range(0,4):
            self.assertEqual(i.ToInt(j), j + 1)
            self.assertEqual(i.ToDouble(j), float(j + 1.5))
            self.assertEqual(i.ToString(j), str(j + 1.5))
            self.assertEqual(v1[j], j + 1)
            self.assertEqual(v2[j], float(j + 1.5))
            self.assertEqual(v3[j], str(j + 1.5))
        
    def test_setFormat_realToByte(self):
        i = btk.btkMetaDataInfo(0.5)
        self.assertEqual(i.GetFormat(), btk.btkMetaDataInfo.Real)
        i.SetFormat(btk.btkMetaDataInfo.Byte)
        self.assertEqual(i.GetFormat(), btk.btkMetaDataInfo.Byte)
        self.assertEqual(len(i.GetDimensions()), 0)
        self.assertEqual(i.GetDimensionsProduct(), 1)
        v1 = i.ToInt()
        v2 = i.ToDouble()
        v3 = i.ToString()
        self.assertEqual(len(v1), 1)
        self.assertEqual(len(v2), 1)
        self.assertEqual(len(v3), 1)
        self.assertEqual(i.ToInt(0), 0)
        self.assertEqual(i.ToDouble(0), 0.0)
        self.assertEqual(i.ToString(0), '0')
        self.assertEqual(v1[0], 0)
        self.assertEqual(v2[0], 0.0)
        self.assertEqual(v3[0], '0')
        
    def test_setFormat_charToByte(self):
        i = btk.btkMetaDataInfo('Hello')
        self.assertEqual(i.GetFormat(), btk.btkMetaDataInfo.Char)
        i.SetFormat(btk.btkMetaDataInfo.Byte)
        self.assertEqual(i.GetFormat(), btk.btkMetaDataInfo.Byte)
        self.assertEqual(len(i.GetDimensions()), 0)
        self.assertEqual(i.GetDimensionsProduct(), 1)
        v1 = i.ToInt()
        v2 = i.ToDouble()
        v3 = i.ToString()
        self.assertEqual(len(v1), 1)
        self.assertEqual(len(v2), 1)
        self.assertEqual(len(v3), 1)
        self.assertEqual(i.ToInt(0), 0)
        self.assertEqual(i.ToDouble(0), 0.0)
        self.assertEqual(i.ToString(0), '0')
        self.assertEqual(v1[0], 0)
        self.assertEqual(v2[0], 0.0)
        self.assertEqual(v3[0], '0')
        
    def test_setFormat_charToReal(self):
        i = btk.btkMetaDataInfo('45.45')
        self.assertEqual(i.GetFormat(), btk.btkMetaDataInfo.Char)
        i.SetFormat(btk.btkMetaDataInfo.Real)
        self.assertEqual(i.GetFormat(), btk.btkMetaDataInfo.Real)
        self.assertEqual(len(i.GetDimensions()), 0)
        self.assertEqual(i.GetDimensionsProduct(), 1)
        v1 = i.ToInt()
        v2 = i.ToDouble()
        v3 = i.ToString()
        self.assertEqual(len(v1), 1)
        self.assertEqual(len(v2), 1)
        self.assertEqual(len(v3), 1)
        self.assertEqual(i.ToInt(0), 45)
        self.assertAlmostEqual(i.ToDouble(0), 45.45, 5)
        self.assertEqual(i.ToString(0), '45.45')
        self.assertEqual(v1[0], 45)
        self.assertAlmostEqual(v2[0], 45.45, 5)
        self.assertEqual(v3[0], '45.45')
        
    def test_resize_lower(self):
        i = btk.btkMetaDataInfo([], ['test', 'test', 'test', 'test'])
        self.assertEqual(i.GetFormat(), btk.btkMetaDataInfo.Char)
        self.assertEqual(len(i.GetDimensions()), 0)
        self.assertEqual(i.GetDimensionsProduct(), 1)
        self.assertEqual(len(i.ToInt()), 1)
        self.assertEqual(i.ToString(0), 't')
        
    def test_resize_upper(self):
        i = btk.btkMetaDataInfo([5], ['test', 'test', 'test', 'test'])
        self.assertEqual(i.GetFormat(), btk.btkMetaDataInfo.Char)
        self.assertEqual(len(i.GetDimensions()), 1)
        self.assertEqual(i.GetDimensionsProduct(), 5)
        self.assertEqual(len(i.ToInt()), 1)
        self.assertEqual(i.ToString(0), 'test ')
        
    def test_ConstructorVectorCharResizeEmptyDim2(self):
        test = btk.btkMetaDataInfo([5,5],btk.btkStringArray())
        self.assertEqual(test.GetFormat(), btk.btkMetaDataInfo.Char)
        self.assertEqual(len(test.GetDimensions()), 2)
        self.assertEqual(test.GetValueNumber(), 5)
        for i in range(0,5):
            self.assertEqual(test.ToString(i), '     ')
    
    def test_ConstructorVectorCharResizeLowerDim2(self):
        dim = [2,2]
        val = ['test','test','test','test']
        test = btk.btkMetaDataInfo(dim,val)
        self.assertEqual(test.GetFormat(), btk.btkMetaDataInfo.Char)
        self.assertEqual(len(test.GetDimensions()), 2)
        self.assertEqual(test.GetValueNumber(), 2)
        self.assertEqual(test.ToString(0), 'te')
        self.assertEqual(test.ToString(1), 'te')
    
    def test_ConstructorVectorCharResizeUpperDim2(self):
        dim = [5,5]
        val = ['test','test','test','test']
        test = btk.btkMetaDataInfo(dim,val)
        self.assertEqual(test.GetFormat(), btk.btkMetaDataInfo.Char)
        self.assertEqual(len(test.GetDimensions()), 2)
        self.assertEqual(test.GetValueNumber(), 5)
        for i in range(0,4):
            self.assertEqual(test.ToString(i), 'test ')
        self.assertEqual(test.ToString(4), '     ')
    
    def test_ConstructorVectorCharPointLabels(self):
        test = btk.btkMetaDataInfo([0,0], str([]))
        self.assertEqual(test.GetFormat(), btk.btkMetaDataInfo.Char)
        self.assertEqual(len(test.GetDimensions()), 2)
        self.assertEqual(test.GetValueNumber(), 0)
    
    def test_SetValueCharInChar(self):
        test = btk.btkMetaDataInfo('FOO')
        test.SetValue(0, 'FOOBAR')
        self.assertEqual(test.GetFormat(), btk.btkMetaDataInfo.Char)
        self.assertEqual(len(test.GetDimensions()), 1)
        self.assertEqual(test.ToString(0), 'FOOBAR')
    
    def test_SetValueCharInInt8(self):
        test = btk.btkMetaDataInfo(5)
        test.SetFormat(btk.btkMetaDataInfo.Byte)
        test.SetValue(0, 'FOOBAR')
        self.assertEqual(test.GetFormat(), btk.btkMetaDataInfo.Byte)
        self.assertEqual(len(test.GetDimensions()), 0)
        self.assertEqual(test.ToInt(0), 0)
    
    def test_SetValueCharInInt16(self):
        test = btk.btkMetaDataInfo(5)
        test.SetValue(0, 'FOOBAR')
        self.assertEqual(test.GetFormat(), btk.btkMetaDataInfo.Integer)
        self.assertEqual(len(test.GetDimensions()), 0)
        self.assertEqual(test.ToInt(0), 0)
    
    def test_SetValueCharInFloat(self):
        test = btk.btkMetaDataInfo(5.1234)
        test.SetValue(0, 'FOOBAR')
        self.assertEqual(test.GetFormat(), btk.btkMetaDataInfo.Real)
        self.assertEqual(len(test.GetDimensions()), 0)
        self.assertEqual(test.ToDouble(0), 0.0)
    
    def test_SetValueCharInChar_Number(self):
        test = btk.btkMetaDataInfo('FOO')
        test.SetValue(0, '12345')
        self.assertEqual(test.GetFormat(), btk.btkMetaDataInfo.Char)
        self.assertEqual(len(test.GetDimensions()), 1)
        self.assertEqual(test.ToString(0), '12345')
    
    def test_SetValueCharInInt8_Number(self):
        test = btk.btkMetaDataInfo(5)
        test.SetFormat(btk.btkMetaDataInfo.Byte)
        test.SetValue(0, '45')
        self.assertEqual(test.GetFormat(), btk.btkMetaDataInfo.Byte)
        self.assertEqual(len(test.GetDimensions()), 0)
        self.assertEqual(test.ToInt(0), 45)
    
    def test_SetValueCharInInt16_Number(self):
        test = btk.btkMetaDataInfo(5)
        test.SetValue(0, '12345')
        self.assertEqual(test.GetFormat(), btk.btkMetaDataInfo.Integer)
        self.assertEqual(len(test.GetDimensions()), 0)
        self.assertEqual(test.ToInt(0), 12345)
    
    def test_SetValueCharInFloat_Number(self):
        test = btk.btkMetaDataInfo(5.1234)
        test.SetValue(0, '1.2345')
        self.assertEqual(test.GetFormat(), btk.btkMetaDataInfo.Real)
        self.assertEqual(len(test.GetDimensions()), 0)
        self.assertAlmostEqual(test.ToDouble(0), 1.2345, 5)
    
    def test_SetValueInt8InChar(self):
        test = btk.btkMetaDataInfo('FOO')
        test.SetValue(0, 15)
        self.assertEqual(test.GetFormat(), btk.btkMetaDataInfo.Char)
        self.assertEqual(len(test.GetDimensions()), 1)
        self.assertEqual(test.ToString(0), '15')
    
    def test_SetValueInt8InInt8(self):
        test = btk.btkMetaDataInfo(5)
        test.SetFormat(btk.btkMetaDataInfo.Byte)
        test.SetValue(0, 15)
        self.assertEqual(test.GetFormat(), btk.btkMetaDataInfo.Byte)
        self.assertEqual(len(test.GetDimensions()), 0)
        self.assertEqual(test.ToInt(0), 15)
    
    def test_SetValueInt8InInt16(self):
        test = btk.btkMetaDataInfo(5)
        test.SetValue(0, 15)
        self.assertEqual(test.GetFormat(), btk.btkMetaDataInfo.Integer)
        self.assertEqual(len(test.GetDimensions()), 0)
        self.assertEqual(test.ToInt(0), 15)
    
    def test_SetValueInt8InFloat(self):
        test = btk.btkMetaDataInfo(5.1234)
        test.SetValue(0, 15)
        self.assertEqual(test.GetFormat(), btk.btkMetaDataInfo.Real)
        self.assertEqual(len(test.GetDimensions()), 0)
        self.assertEqual(test.ToDouble(0), 15.0)
    
    def test_SetValueInt16InChar(self):
        test = btk.btkMetaDataInfo('FOO')
        test.SetValue(0, 1024)
        self.assertEqual(test.GetFormat(), btk.btkMetaDataInfo.Char)
        self.assertEqual(len(test.GetDimensions()), 1)
        self.assertEqual(test.ToString(0), '1024')
    
    def test_SetValueInt16InInt8(self):
        test = btk.btkMetaDataInfo(5)
        test.SetFormat(btk.btkMetaDataInfo.Byte)
        test.SetValue(0, 12456)
        self.assertEqual(test.GetFormat(), btk.btkMetaDataInfo.Byte)
        self.assertEqual(len(test.GetDimensions()), 0)
        #self.assertEqual(test.ToInt(0), 168) # Python seems use another cycling rounging (opposite?)
        self.assertEqual(test.ToInt(0), -88)
    
    def test_SetValueInt16InInt16(self):
        test = btk.btkMetaDataInfo(2048)
        test.SetValue(0, 12456)
        self.assertEqual(test.GetFormat(), btk.btkMetaDataInfo.Integer)
        self.assertEqual(len(test.GetDimensions()), 0)
        self.assertEqual(test.ToInt(0), 12456)
    
    def test_SetValueInt16InFloat(self):
        test = btk.btkMetaDataInfo(2.456)
        test.SetValue(0, 4000)
        self.assertEqual(test.GetFormat(), btk.btkMetaDataInfo.Real)
        self.assertEqual(len(test.GetDimensions()), 0)
        self.assertEqual(test.ToDouble(0), 4000.0)
    
    def test_SetValueFloatInChar(self):
        test = btk.btkMetaDataInfo('FOO')
        test.SetValue(0, 1.2345)
        self.assertEqual(test.GetFormat(), btk.btkMetaDataInfo.Char)
        self.assertEqual(len(test.GetDimensions()), 1)
        self.assertEqual(test.ToString(0), '1.2345')
    
    def test_SetValueFloatInInt8(self):
        test = btk.btkMetaDataInfo(5)
        test.SetFormat(btk.btkMetaDataInfo.Byte)
        test.SetValue(0, 1.2345)
        self.assertEqual(test.GetFormat(), btk.btkMetaDataInfo.Byte)
        self.assertEqual(len(test.GetDimensions()), 0)
        self.assertEqual(test.ToInt(0), 1)
    
    def test_SetValueFloatInInt16(self):
        test = btk.btkMetaDataInfo(2048)
        test.SetValue(0, 1.2345)
        self.assertEqual(test.GetFormat(), btk.btkMetaDataInfo.Integer)
        self.assertEqual(len(test.GetDimensions()), 0)
        self.assertEqual(test.ToInt(0), 1)
    
    def test_SetValueFloatInFloat(self):
        test = btk.btkMetaDataInfo(2.456)
        test.SetValue(0, 3.14)
        self.assertEqual(test.GetFormat(), btk.btkMetaDataInfo.Real)
        self.assertEqual(len(test.GetDimensions()), 0)
        self.assertAlmostEqual(test.ToDouble(0), 3.14, 5)
    
    """
    # SetValue(int) is casted to SetValue(int16_t) -> 1234567 is truncated
    def test_SetValueIntInChar(self):
        test = btk.btkMetaDataInfo('FOO')
        test.SetValue(0, 1234567)
        self.assertEqual(test.GetFormat(), btk.btkMetaDataInfo.Char)
        self.assertEqual(len(test.GetDimensions()), 1)
        self.assertEqual(test.ToString(0), '1234567')
    """
    
    def test_SetValueIntInInt8(self): # Modified as this value is casted to a float and then loss in precision
        test = btk.btkMetaDataInfo(5)
        test.SetFormat(btk.btkMetaDataInfo.Byte)
        test.SetValue(0, 1234567)
        self.assertEqual(test.GetFormat(), btk.btkMetaDataInfo.Byte)
        self.assertEqual(len(test.GetDimensions()), 0)
        # self.assertEqual(test.ToInt(0), 135)  # Python seems use another cycling roudning (opposite?)
        self.assertEqual(test.ToInt(0), -121)
    
    def test_SetValueIntInInt16(self):
        test = btk.btkMetaDataInfo(2048)
        test.SetValue(0, 1234567)
        self.assertEqual(test.GetFormat(), btk.btkMetaDataInfo.Integer)
        self.assertEqual(len(test.GetDimensions()), 0)
        self.assertEqual(test.ToInt(0), -10617)
    
    """
    # SetValue(int) is casted to SetValue(int16_t) -> 1234567 is truncated
    def test_SetValueIntInFloat(self):
        test = btk.btkMetaDataInfo(2.456)
        test.SetValue(0, 1234567)
        self.assertEqual(test.GetFormat(), btk.btkMetaDataInfo.Real)
        self.assertEqual(len(test.GetDimensions()), 0)
        self.assertEqual(test.ToDouble(0), 1234567.0)
    """
    
    def test_SetValueDoubleInChar(self): # Modified as this value is casted to a float and then loss in precision
        test = btk.btkMetaDataInfo('FOO')
        test.SetValue(0, 1.23456)
        self.assertEqual(test.GetFormat(), btk.btkMetaDataInfo.Char)
        self.assertEqual(len(test.GetDimensions()), 1)
        self.assertEqual(test.ToString(0), '1.23456')
    
    def test_SetValueDoubleInInt8(self):
        test = btk.btkMetaDataInfo(5)
        test.SetFormat(btk.btkMetaDataInfo.Byte)
        test.SetValue(0, 1.23456789)
        self.assertEqual(test.GetFormat(), btk.btkMetaDataInfo.Byte)
        self.assertEqual(len(test.GetDimensions()), 0)
        self.assertEqual(test.ToInt(0), 1)
    
    def test_SetValueDoubleInInt16(self):
        test = btk.btkMetaDataInfo(2048)
        test.SetValue(0, 1.23456789)
        self.assertEqual(test.GetFormat(), btk.btkMetaDataInfo.Integer)
        self.assertEqual(len(test.GetDimensions()), 0)
        self.assertEqual(test.ToInt(0), 1)
    
    def test_SetValueDoubleInFloat(self): # Modified as this value is casted to a float and then loss in precision
        test = btk.btkMetaDataInfo(2.456)
        test.SetValue(0, 1.23456)
        self.assertEqual(test.GetFormat(), btk.btkMetaDataInfo.Real)
        self.assertEqual(len(test.GetDimensions()), 0)
        self.assertAlmostEqual(test.ToDouble(0), 1.23456, 5)

    def test_SetValuesFromVectorString(self):
        test = btk.btkMetaDataInfo(['NAME', 'CALIBRATION', 'FULL_DESCRIPTION', 'SETUP', 'DATE'])
        self.assertEqual(test.GetFormat(), btk.btkMetaDataInfo.Char)
        self.assertEqual(len(test.GetDimensions()), 2)
        self.assertEqual(test.GetDimension(0), 16)
        self.assertEqual(test.GetDimension(1), 5)
        self.assertEqual(test.GetValueNumber(), 5)
        self.assertEqual(test.ToString(0), 'NAME            ')
        self.assertEqual(test.ToString(1), 'CALIBRATION     ')
        self.assertEqual(test.ToString(2), 'FULL_DESCRIPTION')
        self.assertEqual(test.ToString(3), 'SETUP           ')
        self.assertEqual(test.ToString(4), 'DATE            ')

    def test_SetValueFromString(self):
        test = btk.btkMetaDataInfo(['NAME', 'CALIBRATION', 'FULL_DESCRIPTION', 'SETUP', 'DATE'])
        test.SetValue(2,'SHORTER')
        self.assertEqual(test.GetFormat(), btk.btkMetaDataInfo.Char)
        self.assertEqual(len(test.GetDimensions()), 2)
        self.assertEqual(test.GetDimension(0), 16)
        self.assertEqual(test.GetDimension(1), 5)
        self.assertEqual(test.GetValueNumber(), 5)
        self.assertEqual(test.ToString(0), 'NAME            ')
        self.assertEqual(test.ToString(1), 'CALIBRATION     ')
        self.assertEqual(test.ToString(2), 'SHORTER         ')
        self.assertEqual(test.ToString(3), 'SETUP           ')
        self.assertEqual(test.ToString(4), 'DATE            ')
        test.SetValue(2,'BIGGERANDBIGGERANDBIGGER')
        self.assertEqual(test.GetDimension(0), 24)
        self.assertEqual(test.GetDimension(1), 5)
        self.assertEqual(test.GetValueNumber(), 5)
        self.assertEqual(test.ToString(0), 'NAME                    ')
        self.assertEqual(test.ToString(1), 'CALIBRATION             ')
        self.assertEqual(test.ToString(2), 'BIGGERANDBIGGERANDBIGGER')
        self.assertEqual(test.ToString(3), 'SETUP                   ')
        self.assertEqual(test.ToString(4), 'DATE                    ')
    
    def test_SetFormatChar2Integer(self):
        dim = btk.btkIntArray(2, 5)
        val = btk.btkStringArray(4, 'test')
        test = btk.btkMetaDataInfo(dim, val)
        self.assertEqual(test.GetFormat(), btk.btkMetaDataInfo.Char)
        test.SetFormat(btk.btkMetaDataInfo.Integer)
        self.assertEqual(test.GetFormat(), btk.btkMetaDataInfo.Integer)
        self.assertEqual(len(test.GetDimensions()), 1)
        self.assertEqual(test.GetValueNumber(), 5)
        for i in range(0,5):
            self.assertEqual(test.ToInt(i), 0)
    
    def test_SetFormatFloat2Integer(self):
        dim = btk.btkIntArray(2, 5)
        val = btk.btkDoubleArray(25, 1.950)
        test = btk.btkMetaDataInfo(dim, val)
        self.assertEqual(test.GetFormat(), btk.btkMetaDataInfo.Real)
        test.SetFormat(btk.btkMetaDataInfo.Integer)
        self.assertEqual(test.GetFormat(), btk.btkMetaDataInfo.Integer)
        self.assertEqual(len(test.GetDimensions()), 2)
        self.assertEqual(test.GetValueNumber(), 25)
        for i in range(0,25):
            self.assertEqual(test.ToInt(i), 1)
    
    def test_SetFormatFloat2CharDim1(self):
        test = btk.btkMetaDataInfo([], [1.95])
        self.assertEqual(test.GetFormat(), btk.btkMetaDataInfo.Real)
        test.SetFormat(btk.btkMetaDataInfo.Char)
        self.assertEqual(test.GetFormat(), btk.btkMetaDataInfo.Char)
        self.assertEqual(len(test.GetDimensions()), 1)
        self.assertEqual(test.GetDimensions()[0], 4)
        self.assertEqual(test.GetValueNumber(), 1)
        self.assertEqual(test.ToString(0), '1.95')

    def test_SetFormatFloat2CharDim2(self):
        dim = [5,5]
        val = btk.btkDoubleArray(25, 1.95)
        test = btk.btkMetaDataInfo(dim, val)
        self.assertEqual(test.GetFormat(), btk.btkMetaDataInfo.Real)
        test.SetFormat(btk.btkMetaDataInfo.Char)
        self.assertEqual(test.GetFormat(), btk.btkMetaDataInfo.Char)
        self.assertEqual(len(test.GetDimensions()), 3)
        self.assertEqual(test.GetDimensions()[0], 4)
        self.assertEqual(test.GetDimensions()[1], 5)
        self.assertEqual(test.GetDimensions()[2], 5)
        self.assertEqual(test.GetValueNumber(), 25)
        for i in range(0,25):
            self.assertEqual(test.ToString(i), '1.95')
    
    def test_SetDimensionForFloat(self):
        dim = [5,5]
        val = btk.btkDoubleArray(25, 1.950)
        test = btk.btkMetaDataInfo(dim, val)
        test.SetDimension(0, 6)
        self.assertEqual(len(test.GetDimensions()), 2)
        self.assertEqual(test.GetValueNumber(), 30)
        for i in range(0,5):
            self.assertEqual(test.ToDouble(i*6+5), 0.0)
        test.SetDimension(1, 6)
        self.assertEqual(test.GetValueNumber(), 36)
        for i in range(30,35):
            self.assertEqual(test.ToDouble(i), 0.0)
    
    def test_SetDimensionForChar(self):
        test = btk.btkMetaDataInfo(btk.btkIntArray(2,5), btk.btkStringArray(5, 'test '))
        test.SetDimension(0, 2)
        self.assertEqual(len(test.GetDimensions()), 2)
        self.assertEqual(test.GetValueNumber(), 5)
        for i in range(0,5):
            self.assertEqual(test.ToString(i), 'te')
    
    def test_ResizeDimensionsFrom0To1Byte(self):
        test = btk.btkMetaDataInfo(0)
        test.ResizeDimensions(1)
        self.assertEqual(len(test.GetDimensions()), 1)
        self.assertEqual(test.GetValueNumber(), 1)
    
    def test_ResizeDimensionsFrom3To1Float(self):
        dim = btk.btkIntArray(3, 3)
        val = btk.btkDoubleArray(27, 1.950)
        test = btk.btkMetaDataInfo(dim, val)
        test.ResizeDimensions(1)
        self.assertEqual(len(test.GetDimensions()), 1)
        self.assertEqual(test.GetValueNumber(), 3)
    
    def test_ResizeDimensionsFrom1To0Char(self):
        test = btk.btkMetaDataInfo('test')
        test.ResizeDimensions(0)
        self.assertEqual(len(test.GetDimensions()), 0)
        self.assertEqual(test.GetValueNumber(), 1)
        self.assertEqual(test.ToString(0), 't')
    
    def test_Clone(self):
        test = btk.btkMetaDataInfo(1.435)
        test2 = test.Clone()
        test2.SetValues('test')
        self.assertAlmostEqual(test.ToDouble(0), 1.435, 5)
        self.assertEqual(test.GetFormat(), btk.btkMetaDataInfo.Real)
        self.assertEqual(test2.ToString(0), 'test')
        self.assertEqual(test2.GetFormat(), btk.btkMetaDataInfo.Char)
    
    def test_Equality(self):
        test = btk.btkMetaDataInfo(1.435)
        test2 = btk.btkMetaDataInfo(1.435)
        self.assertTrue(test == test2)
        test2.SetValues('Allo')
        self.assertTrue(test != test2)
        test2.SetValues(1.435)
        self.assertTrue(test == test2)
        test2.SetValues(2.5)
        self.assertTrue(test != test2)
        test.SetValues(btk.btkIntArray(5,5))
        test2.SetValues(btk.btkIntArray(5,5))
        self.assertTrue(test == test2)
        test2.SetValue(0, 15)
        self.assertTrue(test != test2)

    def test_String2String(self):
        test = btk.btkMetaDataInfo(btk.btkStringArray(5,'test'))
        val = test.ToString()
        self.assertEqual(len(val), 5)
        for i in range(0,5):
            self.assertEqual(val[i], 'test')

    def test_Byte2String(self):
        test = btk.btkMetaDataInfo(5)
        test.SetFormat(btk.btkMetaDataInfo.Byte)
        val = test.ToString()
        self.assertEqual(len(val), 1)
        self.assertEqual(val[0], '5')

    def test_Integer2String(self):
        test = btk.btkMetaDataInfo(btk.btkIntArray(5, 379))
        val = test.ToString()
        self.assertEqual(len(val), 5)
        for i in range(0,5):
            self.assertEqual(val[i], '379')

    def test_Real2String(self):
        test = btk.btkMetaDataInfo(btk.btkDoubleArray(5, 0.83333))
        val = test.ToString()
        self.assertEqual(len(val), 5)
        for i in range(0,5):
            self.assertEqual(val[i], '0.83333')
            
    def test_String2Byte(self):
        test = btk.btkMetaDataInfo(btk.btkStringArray(5,'test'))
        val = test.ToInt()
        self.assertEqual(len(val), 5)
        for i in range(0,5):
            self.assertEqual(val[i], 0)

    def test_Byte2Byte(self):
        test = btk.btkMetaDataInfo(5)
        test.SetFormat(btk.btkMetaDataInfo.Byte)
        val = test.ToInt()
        self.assertEqual(len(val), 1)
        self.assertEqual(val[0], 5)

    def test_Integer2Byte(self):
        test = btk.btkMetaDataInfo(btk.btkIntArray(5, 379))
        test.SetFormat(btk.btkMetaDataInfo.Byte)
        val = test.ToInt()
        self.assertEqual(len(val), 5)
        for i in range(0,5):
            self.assertEqual(val[i], 123)

    def test_Real2Byte(self):
        test = btk.btkMetaDataInfo(btk.btkDoubleArray(5, 0.1))
        val = test.ToInt()
        self.assertEqual(len(val), 5)
        for i in range(0,5):
            self.assertEqual(val[i], 0)

    def test_String2Integer(self):
        test = btk.btkMetaDataInfo(btk.btkStringArray(5,'test'))
        val = test.ToInt()
        self.assertEqual(len(val), 5)
        for i in range(0,5):
            self.assertEqual(val[i], 0)

    def test_Byte2Integer(self):
        test = btk.btkMetaDataInfo(5)
        test.SetFormat(btk.btkMetaDataInfo.Byte)
        val = test.ToInt()
        self.assertEqual(len(val), 1)
        self.assertEqual(val[0], 5)

    def test_Integer2Integer(self):
        test = btk.btkMetaDataInfo(btk.btkIntArray(5, 379))
        val = test.ToInt()
        self.assertEqual(len(val), 5)
        for i in range(0,5):
            self.assertEqual(val[i], 379)

    def test_Real2Integer(self):
        test = btk.btkMetaDataInfo(btk.btkDoubleArray(5, 0.1))
        val = test.ToInt()
        self.assertEqual(len(val), 5)
        for i in range(0,5):
            self.assertEqual(val[i], 0)

    def test_String2Real(self):
        test = btk.btkMetaDataInfo(btk.btkStringArray(5,'test'))
        val = test.ToDouble()
        self.assertEqual(len(val), 5)
        for i in range(0,5):
            self.assertEqual(val[i], 0)

    def test_Byte2Real(self):
        test = btk.btkMetaDataInfo(5)
        test.SetFormat(btk.btkMetaDataInfo.Byte)
        val = test.ToDouble()
        self.assertEqual(len(val), 1)
        self.assertEqual(val[0], 5)

    def test_Integer2Real(self):
        test = btk.btkMetaDataInfo(btk.btkIntArray(5, 379))
        val = test.ToDouble()
        self.assertEqual(len(val), 5)
        for i in range(0,5):
            self.assertEqual(val[i], 379)

    def test_Real2Real(self):
        test = btk.btkMetaDataInfo(btk.btkDoubleArray(5, 0.1))
        val = test.ToDouble()
        self.assertEqual(len(val), 5)
        for i in range(0,5):
            self.assertAlmostEqual(val[i], 0.1, 5)

    def test_Float2Double(self):
        test = btk.btkMetaDataInfo(btk.btkDoubleArray(5, 0.1))
        val = test.ToDouble()
        self.assertEqual(len(val), 5)
        for i in range(0,5):
            self.assertAlmostEqual(val[i], 0.1, 5)

    def test_Int82Int(self):
        test = btk.btkMetaDataInfo(btk.btkIntArray(5, 45))
        val = test.ToInt()
        self.assertEqual(len(val), 5)
        for i in range(0,5):
            self.assertEqual(val[i], 45)

    def test_Int162Double(self):
        test = btk.btkMetaDataInfo(btk.btkIntArray(5, 369))
        val = test.ToDouble()
        self.assertEqual(len(val), 5)
        for i in range(0,5):
            self.assertEqual(val[i], 369)

    def test_String2String_Number(self):
        test = btk.btkMetaDataInfo(btk.btkStringArray(5, '12345'))
        val = test.ToString()
        self.assertEqual(len(val), 5)
        for i in range(0,5):
            self.assertEqual(val[i], '12345')
    
    def test_String2Byte_Number(self):
        test = btk.btkMetaDataInfo(btk.btkStringArray(5, '45'))
        val = test.ToInt()
        self.assertEqual(len(val), 5)
        for i in range(0,5):
            self.assertEqual(val[i], 45)
    
    def test_String2Integer_Number(self):
        test = btk.btkMetaDataInfo(btk.btkStringArray(5, '12345'))
        val = test.ToInt()
        self.assertEqual(len(val), 5)
        for i in range(0,5):
            self.assertEqual(val[i], 12345)
    
    def test_String2Real_Number(self):
        test = btk.btkMetaDataInfo(btk.btkStringArray(5, '1.2345'))
        val = test.ToDouble()
        self.assertEqual(len(val), 5)
        for i in range(0,5):
            self.assertEqual(val[i], 1.2345)
    
    def test_Real2String2Real_Number(self):
        test = btk.btkMetaDataInfo(btk.btkDoubleArray(5, 1.2345))
        self.assertEqual(len(test.GetDimensions()), 1)
        self.assertEqual(test.GetDimensions()[0], 5)
        test.SetFormat(btk.btkMetaDataInfo.Char)
        self.assertEqual(len(test.GetDimensions()), 2)
        self.assertEqual(test.GetDimensions()[0], 6)
        self.assertEqual(test.GetDimensions()[1], 5)
        val = test.ToString()
        self.assertEqual(len(val), 5)
        for i in range(0,5):
            self.assertEqual(val[i], '1.2345')
        test.SetFormat(btk.btkMetaDataInfo.Real)
        self.assertEqual(len(test.GetDimensions()), 1)
        self.assertEqual(test.GetDimensions()[0], 5)
        val2 = test.ToDouble()
        self.assertEqual(len(val2), 5)
        for i in range(0,5):
            self.assertAlmostEqual(val2[i], 1.2345, 5)