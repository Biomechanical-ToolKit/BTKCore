import btk
import unittest
import _TDDConfigure

class C3DFileIOTest(unittest.TestCase):
    def test_CanReadFileEmpty(self):
        pt = btk.btkC3DFileIO()
        self.assertEqual(pt.CanReadFile(''), False);
    
    def test_CanReadFileEmptyFile(self):
        pt = btk.btkC3DFileIO()
        self.assertEqual(pt.CanReadFile(_TDDConfigure.C3DFilePathIN + 'others/Empty.c3d'), False);
    
    def test_CanReadFileFail(self):
        pt = btk.btkC3DFileIO()
        self.assertEqual(pt.CanReadFile(_TDDConfigure.C3DFilePathIN + 'others/Fail.c3d'), False);
    
    def test_CanReadFileOk(self):
        pt = btk.btkC3DFileIO()
        self.assertEqual(pt.CanReadFile(_TDDConfigure.C3DFilePathIN + 'sample01/Eb015pi.c3d'), True);
    
    def test_CanWriteFileEmpty(self):
        pt = btk.btkC3DFileIO()
        self.assertEqual(pt.CanWriteFile(''), False);
    
    def test_CanWriteFileFail(self):
        pt = btk.btkC3DFileIO()
        self.assertEqual(pt.CanWriteFile('test.jpeg'), False);
    
    def test_CanWriteFileOk(self):
        pt = btk.btkC3DFileIO()
        self.assertEqual(pt.CanWriteFile('test.c3d'), True);