import btk
import unittest
import numpy

class ForcePlatformTypesTest(unittest.TestCase):
    def test_ForcePlatformType1(self):
        pf = btk.btkForcePlatformType1()
        self.assertEqual(pf.GetType(), 1)
        self.assertEqual(numpy.all(pf.GetOrigin() == numpy.zeros((3,1))), True)
        self.assertEqual(pf.GetChannelNumber(), 6)
        pf.SetOrigin(numpy.array([[1.],[1.],[1.]]));
        self.assertEqual(numpy.all(pf.GetOrigin() == numpy.ones((3,1))), True)
        pf.SetOrigin(0.,0.,0.);
        self.assertEqual(numpy.all(pf.GetOrigin() == numpy.zeros((3,1))), True)

    def test_ForcePlatformType2(self):
        pf = btk.btkForcePlatformType2()
        self.assertEqual(pf.GetType(), 2)
        self.assertEqual(pf.GetChannelNumber(), 6)
        pf.SetCorner(2,2,2.);
        self.assertEqual(pf.GetCorner(2)[2], 2.)
        pf.SetCorner(1,numpy.array([[1.],[1.],[1.]]));
        self.assertEqual(numpy.all(pf.GetCorner(1) == numpy.ones((3,1))), True)
        pf.SetCorner(3,5.,6.,7.);
        c4 = numpy.array([[5.],[6.],[7.]]);
        self.assertEqual(numpy.all(pf.GetCorner(3) == c4), True)
        corners = numpy.array([[250.,1.,-250.,5.],[250.,1.,-250.,6.],[0.,1.,2.,7.]]);
        self.assertEqual(numpy.all(pf.GetCorners() == corners), True)

    def test_ForcePlatformType3(self):
        pf = btk.btkForcePlatformType3()
        self.assertEqual(pf.GetType(), 3)
        self.assertEqual(pf.GetChannelNumber(), 8)

    def test_ForcePlatformType4(self):
        pf = btk.btkForcePlatformType4()
        self.assertEqual(pf.GetType(), 4)
        self.assertEqual(pf.GetChannelNumber(), 6)
        self.assertEqual(numpy.all(pf.GetOrigin() == numpy.zeros((3,1))), True)
        self.assertEqual(numpy.all(pf.GetCalMatrix() == numpy.eye(6)), True)

    def test_ForcePlatformType5(self):
        pf = btk.btkForcePlatformType5()
        self.assertEqual(pf.GetType(), 5)
        self.assertEqual(pf.GetChannelNumber(), 6)
        self.assertEqual(numpy.all(pf.GetOrigin() == numpy.zeros((3,1))), True)
        self.assertEqual(numpy.all(pf.GetCalMatrix() == numpy.eye(8,6)), True)

    def test_ForcePlatformType6(self):
        pf = btk.btkForcePlatformType6()
        self.assertEqual(pf.GetType(), 6)
        self.assertEqual(pf.GetChannelNumber(), 12)
        self.assertEqual(numpy.all(pf.GetOrigin() == numpy.zeros((3,1))), True)
        self.assertEqual(numpy.all(pf.GetCalMatrix() == numpy.eye(12)), True)