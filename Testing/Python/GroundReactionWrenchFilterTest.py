import btk
import unittest
import _TDDConfigure
import numpy

class GroundReactionWrenchFilterTest(unittest.TestCase):
    def test_FileSample10Type4a(self):
        reader = btk.btkAcquisitionFileReader()
        reader.SetFilename(_TDDConfigure.C3DFilePathIN + "sample10/type-4a.c3d")
        pfe = btk.btkForcePlatformsExtractor()
        grwf = btk.btkGroundReactionWrenchFilter()
        pfe.SetInput(reader.GetOutput())
        grwf.SetInput(pfe.GetOutput())
        grwc = grwf.GetOutput()
        grwc.Update()
        self.assertEqual(grwc.GetItemNumber(), 2)
        grw1 = grwc.GetItem(0)
        self.assertEqual(grw1.GetPosition().GetFrameNumber(), 5760)
