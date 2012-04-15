import unittest

import AcquisitionUnitConverterTest
import ForcePlatformsExtractorTest
import GroundReactionWrenchFilterTest
import SeparateKnownVirtualMarkersFilterTest

def suite():
    suite = unittest.TestSuite()
    suite.addTest(unittest.makeSuite(AcquisitionUnitConverterTest.AcquisitionUnitConverterTest))
    suite.addTest(unittest.makeSuite(ForcePlatformsExtractorTest.ForcePlatformsExtractorTest))
    suite.addTest(unittest.makeSuite(GroundReactionWrenchFilterTest.GroundReactionWrenchFilterTest))
    suite.addTest(unittest.makeSuite(SeparateKnownVirtualMarkersFilterTest.SeparateKnownVirtualMarkersFilterTest))
    return suite
