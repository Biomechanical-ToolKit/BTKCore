import unittest

import AcquisitionUnitConverterTest
import AnalogOffsetRemoverTest
import DownsampleFilterTest
import ForcePlatformsExtractorTest
import GroundReactionWrenchFilterTest
import IMUsExtractorTest
import MergeAcquisitionFilterTest
import SeparateKnownVirtualMarkersFilterTest
import SubAcquisitionFilterTest
import VerticalGroundReactionForceGaitEventDetectorTest

def suite():
    suite = unittest.TestSuite()
    suite.addTest(unittest.makeSuite(AcquisitionUnitConverterTest.AcquisitionUnitConverterTest))
    suite.addTest(unittest.makeSuite(AnalogOffsetRemoverTest.AnalogOffsetRemoverTest))
    suite.addTest(unittest.makeSuite(DownsampleFilterTest.DownsampleFilterTest))
    suite.addTest(unittest.makeSuite(ForcePlatformsExtractorTest.ForcePlatformsExtractorTest))
    suite.addTest(unittest.makeSuite(GroundReactionWrenchFilterTest.GroundReactionWrenchFilterTest))
    suite.addTest(unittest.makeSuite(IMUsExtractorTest.IMUsExtractorTest))
    suite.addTest(unittest.makeSuite(MergeAcquisitionFilterTest.MergeAcquisitionFilterTest))
    suite.addTest(unittest.makeSuite(SeparateKnownVirtualMarkersFilterTest.SeparateKnownVirtualMarkersFilterTest))
    suite.addTest(unittest.makeSuite(SubAcquisitionFilterTest.SubAcquisitionFilterTest))
    suite.addTest(unittest.makeSuite(VerticalGroundReactionForceGaitEventDetectorTest.VerticalGroundReactionForceGaitEventDetectorTest))
    return suite
