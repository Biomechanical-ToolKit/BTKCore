#include "Viz3DWidget.h"

#include <QDragEnterEvent>
#include <QUrl>

#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkActor.h>
#include <vtkPlaneSource.h>
#include <vtkRenderWindow.h>
#include <vtkCamera.h>

Viz3DWidget::Viz3DWidget(QWidget* parent)
: QVTKWidget(parent)
{
  // Member
  //this->mp_Renderer = vtkRenderer::New();
}

Viz3DWidget::~Viz3DWidget()
{
  //this->mp_Renderer->Delete();
};


void Viz3DWidget::dragEnterEvent(QDragEnterEvent *event)
{
  if (event->mimeData()->hasFormat("text/uri-list"))
    event->acceptProposedAction();
};

void Viz3DWidget::dropEvent(QDropEvent *event)
{
  QList<QUrl> urls = event->mimeData()->urls();
  if (urls.isEmpty())
      return;

  QString filename = urls.first().toLocalFile();
  if (filename.isEmpty())
      return;
  
  event->acceptProposedAction();
  emit fileDropped(filename);
};