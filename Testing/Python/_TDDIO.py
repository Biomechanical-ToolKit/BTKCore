import unittest

import C3DFileIOTest
import C3DFileReaderTest
import C3DFileWriterTest

def suite():
    suite = unittest.TestSuite()
    # suite.addTest(unittest.makeSuite(C3DFileIOTest.C3DFileIOTest))
    # suite.addTest(unittest.makeSuite(C3DFileReaderTest.C3DFileReaderTest))
    # suite.addTest(unittest.makeSuite(C3DFileWriterTest.C3DFileWriterTest))
    return suite