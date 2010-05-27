#ifndef Viz3DWidget_h
#define Viz3DWidget_h

#include <btkAcquisition.h>
#include <btkProcessObject.h>
#include <btkVTKAxesWidget.h>
#include <btkVTKForcePlatformsSource.h>
#include <btkVTKGRFsFramesSource.h>
#include <btkVTKMarkersFramesSource.h>

#include <QTableWidgetItem>

#include <map>

#include <QVTKWidget.h>
#include <vtkRenderer.h>
#include <vtkEventQtSlotConnect.h>
//#include <vtkObject.h>
#include <vtkMapperCollection.h>

class vtkStreamingDemandDrivenPipelineCollection;
class vtkProcessMap;

class Viz3DWidget : public QVTKWidget
{
  Q_OBJECT
  
public:
  Viz3DWidget(QWidget* parent = 0);
  ~Viz3DWidget();
  
  void initialize();
  vtkRenderer* renderer() const {return this->mp_Renderer;};
  
  void setMarkerRadius(int id, double r);
  double markerRadius(int id);
  void setMarkerColorIndex(int id, int idx);
  int markerColorIndex(int id);
  void setMarkerVisibility(int id, bool visible);
  bool markerVisibility(int id);
  double* markerColorValue(int c);
  bool appendNewMarkerColor(const QColor& color, int* idx);
  void setGroundOrientation(double x, double y, double z);
  void load(btk::Acquisition::Pointer acq);
  
public slots:
  // Qt / VTK
  void updateDisplayedMarkersList(vtkObject* caller, unsigned long vtk_event, void* client_data, void* call_data);
  void selectPickedMarker(vtkObject* caller, unsigned long vtk_event, void* client_data, void* call_data);
  void selectPickedMarkers(vtkObject* caller, unsigned long vtk_event, void* client_data, void* call_data);
  // Qt
  void clear();
  void circleSelectedMarkers(QList<QTableWidgetItem*> items);
  void updateDisplay();
  void updateDisplay(int frame);
  void showSelectedMarkers(const QList<QTableWidgetItem*>& items);
  void hideSelectedMarkers(const QList<QTableWidgetItem*>& items);
  void showAllMarkers();
  void hideAllMarkers();
  void updateMarkerVisibility(QTableWidgetItem* item);
  
signals:
  void fileDropped(const QString& filename);
  void visibleMarkersChanged(const QVector<int>& ids);
  void pickedMarkerChanged(int id);
  void pickedMarkersChanged(int id);
  
protected:
  void dragEnterEvent(QDragEnterEvent *event);
  void dropEvent(QDropEvent *event);
  
private:
  vtkRenderer* mp_Renderer;
  btk::VTKAxesWidget* mp_AxesWidget;
  vtkEventQtSlotConnect* mp_EventQtSlotConnections;
  vtkProcessMap* mp_VTKProc;
  vtkMapperCollection* mp_Mappers;
  vtkStreamingDemandDrivenPipelineCollection* mp_Syncro;
  std::map<int, btk::ProcessObject::Pointer> m_BTKProc;
  int m_FirstFrame;
};

#endif // Viz3DWidget_h