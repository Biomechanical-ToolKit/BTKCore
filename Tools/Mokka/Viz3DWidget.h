#ifndef Viz3DWidget_h
#define Viz3DWidget_h

#include <QVTKWidget.h>
#include <vtkRenderer.h>

class Viz3DWidget : public QVTKWidget
{
  Q_OBJECT
  
public:
  Viz3DWidget(QWidget* parent = 0);
  ~Viz3DWidget();
  
public slots:
  
signals:
  void fileDropped(const QString& filename);
  
protected:
  void dragEnterEvent(QDragEnterEvent *event);
  void dropEvent(QDropEvent *event);
  
private:
};

#endif // Viz3DWidget_h