import unittest

import EventTest
import AnalogTest
import PointTest
import ForcePlatformTypesTest
import EventCollectionTest
import PointCollectionTest
import MetaDataInfoTest
import MetaDataTest
import AcquisitionTest
import WrenchTest

def suite():
    suite = unittest.TestSuite()
    # suite.addTest(unittest.makeSuite(EventTest.EventTest))
    # suite.addTest(unittest.makeSuite(AnalogTest.AnalogTest))
    # suite.addTest(unittest.makeSuite(PointTest.PointTest))
    # suite.addTest(unittest.makeSuite(ForcePlatformTypesTest.ForcePlatformTypesTest))
    # suite.addTest(unittest.makeSuite(EventCollectionTest.EventCollectionTest))
    # suite.addTest(unittest.makeSuite(PointCollectionTest.PointCollectionTest))
    # suite.addTest(unittest.makeSuite(MetaDataInfoTest.MetaDataInfoTest))
    # suite.addTest(unittest.makeSuite(MetaDataTest.MetaDataTest))
    # suite.addTest(unittest.makeSuite(AcquisitionTest.AcquisitionTest))
    # suite.addTest(unittest.makeSuite(WrenchTest.WrenchTest))
    return suite