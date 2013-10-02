import btk
import unittest
import numpy

class IMUTypesTest(unittest.TestCase):
    def test_IMUType1(self):
        imu = btk.btkIMUType1()
        self.assertEqual(imu.GetLabel(), 'IMU')
        self.assertEqual(imu.GetDescription(), '')
        self.assertEqual(imu.GetType(), 1)
        self.assertEqual(imu.GetFrequency(), 0.0)
        self.assertEqual(imu.GetFrameNumber(), 0)
        self.assertEqual(imu.GetChannelNumber(), 6)

    def test_IMUType2(self):
        imu = btk.btkIMUType2()
        self.assertEqual(imu.GetLabel(), 'IMU')
        self.assertEqual(imu.GetDescription(), '')
        self.assertEqual(imu.GetType(), 2)
        self.assertEqual(imu.GetFrequency(), 0.0)
        self.assertEqual(imu.GetFrameNumber(), 0)
        self.assertEqual(imu.GetChannelNumber(), 6)

    def test_IMUType1_noInit(self):
        imu = btk.btkIMUType1("LAD", "aayTER", False)
        self.assertEqual(imu.GetLabel(), 'LAD')
        self.assertEqual(imu.GetDescription(), 'aayTER')
        self.assertEqual(imu.GetType(), 1)
        self.assertEqual(imu.GetFrequency(), 0.0)
        self.assertEqual(imu.GetFrameNumber(), 0)
        self.assertEqual(imu.GetChannelNumber(), 0)

    def test_IMUType2_noInit(self):
        imu = btk.btkIMUType2("LAD", "aayTER", False)
        self.assertEqual(imu.GetLabel(), 'LAD')
        self.assertEqual(imu.GetDescription(), 'aayTER')
        self.assertEqual(imu.GetType(), 2)
        self.assertEqual(imu.GetFrequency(), 0.0)
        self.assertEqual(imu.GetFrameNumber(), 0)
        self.assertEqual(imu.GetChannelNumber(), 0)

    def test_IMUType1_Clone(self):
        imu_ = btk.btkIMUType1("MyIMU", "blabla")
        imu_.SetFrameNumber(10)
        imu_.SetFrequency(1.0)
        imu_.SetCalMatrix(numpy.eye(6))
        imu = imu_.Clone()
        self.assertEqual(imu.GetLabel(), 'MyIMU')
        self.assertEqual(imu.GetDescription(), 'blabla')
        self.assertEqual(imu.GetType(), 1)
        self.assertEqual(imu.GetFrequency(), 1.0)
        self.assertEqual(imu.GetFrameNumber(), 10)
        self.assertEqual(imu.GetChannelNumber(), 6)
        self.assertEqual(numpy.all(imu.GetCalMatrix() == numpy.eye(6)), True)

    def test_IMUType2_Clone(self):
        imu_ = btk.btkIMUType2("MyIMU", "blabla")
        imu_.SetFrameNumber(10)
        imu_.SetFrequency(1.0)
        imu_.SetCalMatrix(numpy.eye(6))
        imu = imu_.Clone()
        self.assertEqual(imu.GetLabel(), 'MyIMU')
        self.assertEqual(imu.GetDescription(), 'blabla')
        self.assertEqual(imu.GetType(), 2)
        self.assertEqual(imu.GetFrequency(), 1.0)
        self.assertEqual(imu.GetFrameNumber(), 10)
        self.assertEqual(imu.GetChannelNumber(), 6)
        self.assertEqual(numpy.all(imu.GetCalMatrix() == numpy.eye(6)), True)
        
    def test_IMU_Rotate(self):
        imu = btk.btkIMUType2()
        imu.SetFrameNumber(10)
        accX = numpy.random.rand(10,1)
        accY = numpy.random.rand(10,1)
        accZ = numpy.random.rand(10,1)
        gyroX = numpy.random.rand(10,1)
        gyroY = numpy.random.rand(10,1)
        gyroZ = numpy.random.rand(10,1)
        imu.GetAccelerometerX().SetValues(accX);
        imu.GetAccelerometerY().SetValues(accY);
        imu.GetAccelerometerZ().SetValues(accZ);
        imu.GetGyroscopeX().SetValues(gyroX);
        imu.GetGyroscopeY().SetValues(gyroY);
        imu.GetGyroscopeZ().SetValues(gyroZ);
        R = numpy.array([[0.,-1.,0.],[0.,0.,-1.],[-1.,0.,0.]])
        imu.Rotate(R)
        numpy.testing.assert_almost_equal(imu.GetAccelerometerX().GetValues(), -accY, 15)
        numpy.testing.assert_almost_equal(imu.GetAccelerometerY().GetValues(), -accZ, 15)
        numpy.testing.assert_almost_equal(imu.GetAccelerometerZ().GetValues(), -accX, 15)
        numpy.testing.assert_almost_equal(imu.GetGyroscopeX().GetValues(), -gyroY, 15)
        numpy.testing.assert_almost_equal(imu.GetGyroscopeY().GetValues(), -gyroZ, 15)
        numpy.testing.assert_almost_equal(imu.GetGyroscopeZ().GetValues(), -gyroX, 15)