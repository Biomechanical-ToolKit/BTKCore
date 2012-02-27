import unittest

import AcquisitionUnitConverterTest

def suite():
    suite = unittest.TestSuite()
    suite.addTest(unittest.makeSuite(AcquisitionUnitConverterTest.AcquisitionUnitConverterTest))
    return suite
