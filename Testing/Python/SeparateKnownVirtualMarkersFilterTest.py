import btk
import unittest
import _TDDConfigure
import numpy

class SeparateKnownVirtualMarkersFilterTest(unittest.TestCase):
    def test_Constructor(self):
        skvm = btk.btkSeparateKnownVirtualMarkersFilter()
        labels = skvm.GetVirtualReferenceFrames()
        num = 19
        self.assertEqual(labels.size(), num)
        it = labels.begin()
        if (labels.size() >= num):
            # HED
            self.assertEqual(it.value().Origin, 'HEDO')
            self.assertEqual(it.value().Axis1, 'HEDA')
            self.assertEqual(it.value().Axis2, 'HEDL')
            self.assertEqual(it.value().Axis3, 'HEDP')
            it.incr()
            # LCL
            self.assertEqual(it.value().Origin, 'LCLO')
            self.assertEqual(it.value().Axis1, 'LCLA')
            self.assertEqual(it.value().Axis2, 'LCLL')
            self.assertEqual(it.value().Axis3, 'LCLP')
            it.incr()
            # LFE
            self.assertEqual(it.value().Origin, 'LFEO')
            self.assertEqual(it.value().Axis1, 'LFEA')
            self.assertEqual(it.value().Axis2, 'LFEL')
            self.assertEqual(it.value().Axis3, 'LFEP')
            it.incr()
            # LFO
            self.assertEqual(it.value().Origin, 'LFOO')
            self.assertEqual(it.value().Axis1, 'LFOA')
            self.assertEqual(it.value().Axis2, 'LFOL')
            self.assertEqual(it.value().Axis3, 'LFOP')
            it.incr()
            # LHN
            self.assertEqual(it.value().Origin, 'LHNO')
            self.assertEqual(it.value().Axis1, 'LHNA')
            self.assertEqual(it.value().Axis2, 'LHNL')
            self.assertEqual(it.value().Axis3, 'LHNP')
            it.incr()
            # LHU
            self.assertEqual(it.value().Origin, 'LHUO')
            self.assertEqual(it.value().Axis1, 'LHUA')
            self.assertEqual(it.value().Axis2, 'LHUL')
            self.assertEqual(it.value().Axis3, 'LHUP')
            it.incr()
            # LRA
            self.assertEqual(it.value().Origin, 'LRAO')
            self.assertEqual(it.value().Axis1, 'LRAA')
            self.assertEqual(it.value().Axis2, 'LRAL')
            self.assertEqual(it.value().Axis3, 'LRAP')
            it.incr()
            # LTI
            self.assertEqual(it.value().Origin, 'LTIO')
            self.assertEqual(it.value().Axis1, 'LTIA')
            self.assertEqual(it.value().Axis2, 'LTIL')
            self.assertEqual(it.value().Axis3, 'LTIP')
            it.incr()
            # LTO
            self.assertEqual(it.value().Origin, 'LTOO')
            self.assertEqual(it.value().Axis1, 'LTOA')
            self.assertEqual(it.value().Axis2, 'LTOL')
            self.assertEqual(it.value().Axis3, 'LTOP')
            it.incr()
            # PEL
            self.assertEqual(it.value().Origin, 'PELO')
            self.assertEqual(it.value().Axis1, 'PELA')
            self.assertEqual(it.value().Axis2, 'PELL')
            self.assertEqual(it.value().Axis3, 'PELP')
            it.incr()
            # RCL
            self.assertEqual(it.value().Origin, 'RCLO')
            self.assertEqual(it.value().Axis1, 'RCLA')
            self.assertEqual(it.value().Axis2, 'RCLL')
            self.assertEqual(it.value().Axis3, 'RCLP')
            it.incr()
            # RFE
            self.assertEqual(it.value().Origin, 'RFEO')
            self.assertEqual(it.value().Axis1, 'RFEA')
            self.assertEqual(it.value().Axis2, 'RFEL')
            self.assertEqual(it.value().Axis3, 'RFEP')
            it.incr()
            # RFO
            self.assertEqual(it.value().Origin, 'RFOO')
            self.assertEqual(it.value().Axis1, 'RFOA')
            self.assertEqual(it.value().Axis2, 'RFOL')
            self.assertEqual(it.value().Axis3, 'RFOP')
            it.incr()
            # RHN
            self.assertEqual(it.value().Origin, 'RHNO')
            self.assertEqual(it.value().Axis1, 'RHNA')
            self.assertEqual(it.value().Axis2, 'RHNL')
            self.assertEqual(it.value().Axis3, 'RHNP')
            it.incr()
            # RHU
            self.assertEqual(it.value().Origin, 'RHUO')
            self.assertEqual(it.value().Axis1, 'RHUA')
            self.assertEqual(it.value().Axis2, 'RHUL')
            self.assertEqual(it.value().Axis3, 'RHUP')
            it.incr()
            # RRA
            self.assertEqual(it.value().Origin, 'RRAO')
            self.assertEqual(it.value().Axis1, 'RRAA')
            self.assertEqual(it.value().Axis2, 'RRAL')
            self.assertEqual(it.value().Axis3, 'RRAP')
            it.incr()
            # RTI
            self.assertEqual(it.value().Origin, 'RTIO')
            self.assertEqual(it.value().Axis1, 'RTIA')
            self.assertEqual(it.value().Axis2, 'RTIL')
            self.assertEqual(it.value().Axis3, 'RTIP')
            it.incr()
            # RTO
            self.assertEqual(it.value().Origin, 'RTOO')
            self.assertEqual(it.value().Axis1, 'RTOA')
            self.assertEqual(it.value().Axis2, 'RTOL')
            self.assertEqual(it.value().Axis3, 'RTOP')
            it.incr()
            # TRX
            self.assertEqual(it.value().Origin, 'TRXO')
            self.assertEqual(it.value().Axis1, 'TRXA')
            self.assertEqual(it.value().Axis2, 'TRXL')
            self.assertEqual(it.value().Axis3, 'TRXP')
            it.incr()
        labels2 = skvm.GetVirtualMarkers()
        num = 2
        self.assertEqual(labels2.size(), num)
        it2 = labels2.begin()
        if (labels2.size() >= num):
            self.assertEqual(it2.value(), 'CentreOfMass')
            it2.incr()
            self.assertEqual(it2.value(), 'CentreOfMassFloor')

    def test_DefaultLabels(self):
        reader = btk.btkAcquisitionFileReader()
        reader.SetFilename(_TDDConfigure.C3DFilePathIN + 'sample09/PlugInC3D.c3d')
        reader.Update()
        skvm = btk.btkSeparateKnownVirtualMarkersFilter()
        skvm.SetInput(reader.GetOutput().GetPoints())
        skvm.Update()
         # markers
        points = skvm.GetOutput(0)
        self.assertEqual(points.GetItemNumber(), 18)
        inc = 0
        self.assertEqual(points.GetItem(inc).GetLabel(), 'RKNE'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'RTOE'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'RTIB'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'RASI'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'RTHI'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'RHEE'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'LKNE'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'RANK'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'RCLA'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'LTHI'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'LASI'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'C7'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'LTOE'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'LANK'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'SACR'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'LHEE'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'LCLA'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'LTIB');
        # virtual used for axes
        points = skvm.GetOutput(1)
        self.assertEqual(points.GetItemNumber(), 36)
        inc = 0
        self.assertEqual(points.GetItem(inc).GetLabel(), 'LFEO'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'LFEA'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'LFEL'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'LFEP'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'LFOO'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'LFOA'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'LFOL'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'LFOP'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'LTIO'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'LTIA'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'LTIL'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'LTIP'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'LTOO'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'LTOA'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'LTOL'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'LTOP'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'PELO'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'PELA'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'PELL'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'PELP'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'RFEO'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'RFEA'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'RFEL'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'RFEP'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'RFOO'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'RFOA'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'RFOL'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'RFOP'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'RTIO'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'RTIA'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'RTIL'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'RTIP'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'RTOO'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'RTOA'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'RTOL'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'RTOP')
        # other virtual markers
        points = skvm.GetOutput(2)
        self.assertEqual(points.GetItemNumber(), 0)
        # other type of points
        points = skvm.GetOutput(3)
        self.assertEqual(points.GetItemNumber(), 32)

    def test_DefaultLabelsAndPrefix(self):
        reader = btk.btkAcquisitionFileReader()
        reader.SetFilename(_TDDConfigure.C3DFilePathIN + 'sample04/sub_labels.c3d')
        reader.Update()
        skvm = btk.btkSeparateKnownVirtualMarkersFilter()
        skvm.SetInput(reader.GetOutput().GetPoints())
        skvm.SetLabelPrefix('Matt:')
        skvm.Update()
         # markers
        points = skvm.GetOutput(0)
        self.assertEqual(points.GetItemNumber(), 50)
        inc = 0
        # virtual used for axes
        points = skvm.GetOutput(1)
        self.assertEqual(points.GetItemNumber(), 36)
        inc = 0
        self.assertEqual(points.GetItem(inc).GetLabel(), 'Matt:LFEO'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'Matt:LFEA'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'Matt:LFEL'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'Matt:LFEP'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'Matt:LFOO'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'Matt:LFOA'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'Matt:LFOL'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'Matt:LFOP'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'Matt:LTIO'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'Matt:LTIA'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'Matt:LTIL'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'Matt:LTIP'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'Matt:LTOO'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'Matt:LTOA'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'Matt:LTOL'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'Matt:LTOP'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'Matt:PELO'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'Matt:PELA'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'Matt:PELL'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'Matt:PELP'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'Matt:RFEO'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'Matt:RFEA'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'Matt:RFEL'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'Matt:RFEP'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'Matt:RFOO'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'Matt:RFOA'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'Matt:RFOL'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'Matt:RFOP'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'Matt:RTIO'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'Matt:RTIA'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'Matt:RTIL'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'Matt:RTIP'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'Matt:RTOO'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'Matt:RTOA'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'Matt:RTOL'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'Matt:RTOP')
        # other virtual markers
        points = skvm.GetOutput(2)
        self.assertEqual(points.GetItemNumber(), 0)
        # other type of points
        points = skvm.GetOutput(3)
        self.assertEqual(points.GetItemNumber(), 28)
  
    def test_FromLabelsLists(self):
        # virtual markers for frame axes
        labels = ['LFE', 'LFO', 'LTI', 'LTO', 'RFE', 'RFO', 'RTI', 'RTO']
        virtualMarkerLabelsAxes = btk.btkStringAxesList()
        for i in range(0, len(labels)):
          label = labels[i]
          virtualMarkerLabelsAxes.push_back(btk.btkStringAxes(label + 'O', label + 'A', label + 'L', label + 'P'))  
        reader = btk.btkAcquisitionFileReader()
        reader.SetFilename(_TDDConfigure.C3DFilePathIN + 'sample04/sub_labels.c3d')
        reader.Update()
        skvm = btk.btkSeparateKnownVirtualMarkersFilter()
        skvm.SetInput(reader.GetOutput().GetPoints())
        skvm.SetLabelPrefix('Matt:')
        skvm.AppendVirtualMarker('LKNE')
        skvm.AppendVirtualMarker('RKNE')
        skvm.SetVirtualReferenceFrames(virtualMarkerLabelsAxes)
        skvm.Update()
        # markers
        points = skvm.GetOutput(0)
        self.assertEqual(points.GetItemNumber(), 52)
        inc = 0
        # virtual used for axes
        points = skvm.GetOutput(1)
        self.assertEqual(points.GetItemNumber(), 32)
        inc = 0
        self.assertEqual(points.GetItem(inc).GetLabel(), 'Matt:LFEO'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'Matt:LFEA'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'Matt:LFEL'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'Matt:LFEP'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'Matt:LFOO'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'Matt:LFOA'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'Matt:LFOL'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'Matt:LFOP'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'Matt:LTIO'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'Matt:LTIA'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'Matt:LTIL'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'Matt:LTIP'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'Matt:LTOO'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'Matt:LTOA'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'Matt:LTOL'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'Matt:LTOP'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'Matt:RFEO'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'Matt:RFEA'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'Matt:RFEL'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'Matt:RFEP'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'Matt:RFOO'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'Matt:RFOA'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'Matt:RFOL'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'Matt:RFOP'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'Matt:RTIO'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'Matt:RTIA'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'Matt:RTIL'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'Matt:RTIP'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'Matt:RTOO'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'Matt:RTOA'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'Matt:RTOL'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'Matt:RTOP')
        # other virtual markers
        points = skvm.GetOutput(2)
        self.assertEqual(points.GetItemNumber(), 2)
        inc = 0
        self.assertEqual(points.GetItem(inc).GetLabel(), 'Matt:LKNE'); inc +=1
        self.assertEqual(points.GetItem(inc).GetLabel(), 'Matt:RKNE')
        # other type of points
        points = skvm.GetOutput(3)
        self.assertEqual(points.GetItemNumber(), 28)

