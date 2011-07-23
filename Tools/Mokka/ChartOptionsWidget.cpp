#include "ChartOptionsWidget.h"

#include <QPainter>
#include <QHeaderView>
#include <QColorDialog>

ChartOptionsWidget::ChartOptionsWidget(QWidget* parent)
: QWidget(parent, Qt::Popup | Qt::FramelessWindowHint)
{
  this->setupUi(this);
  
  this->setAttribute(Qt::WA_TranslucentBackground);
  this->resize(150,250);
  
  // this->plotTable->setColumnCount(2);
  // this->plotTable->verticalHeader()->hide();
  // this->plotTable->horizontalHeader()->hide();
  // this->plotTable->setShowGrid(false);
  // this->plotTable->setFrameShape(QFrame::NoFrame);
  // this->plotTable->setSelectionBehavior(QAbstractItemView::SelectRows);
  // this->plotTable->setSelectionMode(QAbstractItemView::ExtendedSelection);
  QHeaderView* header = this->plotTable->horizontalHeader();
  header->setMovable(false);
  header->resizeSection(1, 25);
  header->setResizeMode(1, QHeaderView::Fixed);
  header->setResizeMode(0, QHeaderView::Stretch);
  
  this->gridLayout->setAlignment(Qt::AlignJustify | Qt::AlignVCenter);
  
#ifdef Q_OS_MAC
  QFont f = this->font();
  f.setPixelSize(10);
  this->plotTable->setFont(f);
  this->lineWidthSpinBox->setFont(f);
  this->labelLineWidth->setFont(f);
  this->labelLineColor->setFont(f);  
  this->plotTable->verticalHeader()->setDefaultSectionSize(20); 

  QLayout* layout = this->layout();
  layout->setSpacing(6);
  layout->setContentsMargins(6, 18, 6, 6);
  this->plotTable->setAttribute(Qt::WA_MacShowFocusRect, false);
  this->lineWidthSpinBox->setAttribute(Qt::WA_MacShowFocusRect, false);
#endif

  connect(this->plotTable, SIGNAL(itemSelectionChanged()), this, SLOT(displayPlotOption()));
  connect(this->lineWidthSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setLineWidth(double)));
  connect(this->lineColorButton, SIGNAL(clicked()), this, SLOT(setLineColor()));
  
  // int color[3] = {255, 0, 0};
  // this->appendPlot("F1X (N)", color, 0.75);
  // this->appendPlot("F2Z (N)", color, 0.75);
  // this->appendPlot("M1X (Nmm)", color, 0.75);
  
  layout->activate(); // Update the layout geometry (important for the first display)
};

void ChartOptionsWidget::appendPlot(const QString& label, int color[3], double width)
{
  int rowIdx = this->plotTable->rowCount();
  this->plotTable->insertRow(rowIdx);
  QColor c = QColor(color[0], color[1], color[2]);
  QTableWidgetItem* item = new QTableWidgetItem(this->createLineIcon(c, width), label);
  item->setData(LineColor, c);
  item->setData(LineWidth, width);
  this->plotTable->setItem(rowIdx, 0, item);
  QPushButton* button = new QPushButton("", this);
  button->setFlat(true);
  button->setProperty("plotIndex", rowIdx);
  button->setStyleSheet("QPushButton {image: url(:/Resources/Images/plot_delete.png);} QPushButton:pressed {image: url(:/Resources/Images/plot_delete-down.png);} QPushButton:flat {border: none;}");
  this->plotTable->setCellWidget(rowIdx, 1, button);
  
  connect(button, SIGNAL(clicked()), this, SLOT(removePlot()));
};

void ChartOptionsWidget::clear()
{
  this->plotTable->clear();
  this->setPlotOptionEnabled(false);
};

void ChartOptionsWidget::setPlotOptionEnabled(bool enabled)
{
  this->labelLineWidth->setEnabled(enabled);
  this->lineWidthSpinBox->setEnabled(enabled);
  this->labelLineColor->setEnabled(enabled);
  this->lineColorButton->setEnabled(enabled);
  if (!enabled)
  {
    this->lineWidthSpinBox->clear();
    this->setLineColorButtonColor(Qt::white);
  }
};

void ChartOptionsWidget::displayPlotOption()
{
  QList<QTableWidgetItem*> selectedItems = this->plotTable->selectedItems();
  if (selectedItems.isEmpty())
    this->setPlotOptionEnabled(false);
  else
  {
    QColor lineColor;
    double lineWidth = -1.0;
    QList<QTableWidgetItem*>::const_iterator it = selectedItems.begin();
    while (it != selectedItems.end())
    {
      if ((*it)->column() == 0)
      {
        lineColor = (*it)->data(LineColor).value<QColor>();
        lineWidth = (*it)->data(LineWidth).toDouble();
        break;
      }
      ++it;
    }
    while (it != selectedItems.end())
    {
      if ((*it)->column() == 0)
      {
        QColor color = (*it)->data(LineColor).value<QColor>();
        double width = (*it)->data(LineWidth).toDouble();
        
        if (lineColor != color)
          lineColor = QColor();
        if (lineWidth != width)
          lineWidth = -1.0;
      }
      
      if (!lineColor.isValid() && (lineWidth == -1.0))
        break;
      
      ++it;
    }
    
    if (!lineColor.isValid())
      this->setLineColorButtonColor(Qt::white);
    else
      this->setLineColorButtonColor(lineColor);
      
    if (lineWidth == -1.0)
      this->lineWidthSpinBox->clear();
    else
      this->lineWidthSpinBox->setValue(lineWidth);
    
    this->setPlotOptionEnabled(true);
  }
};

void ChartOptionsWidget::removePlot()
{
  QObject* obj = sender();
  int idx = obj->property("plotIndex").toInt();
  this->plotTable->removeRow(idx);
  // Update the other indices
  for (int i = idx ; i < this->plotTable->rowCount() ; ++i)
    this->plotTable->cellWidget(i,1)->setProperty("plotIndex", i);
  if (this->plotTable->rowCount() == 0)
    this->setPlotOptionEnabled(false);
  emit plotRemoved(idx);
};

void ChartOptionsWidget::setLineColor()
{
  QColor color = QColorDialog::getColor(Qt::white, this);
  QList<int> indices;
  if (color.isValid())
  {
    QList<QTableWidgetItem*> selectedItems = this->plotTable->selectedItems();
    for (QList<QTableWidgetItem*>::const_iterator it = selectedItems.begin() ; it != selectedItems.end() ; ++it)
    {
      if ((*it)->column() == 0)
      {
        (*it)->setIcon(this->createLineIcon(color, (*it)->data(LineWidth).toDouble()));
        (*it)->setData(LineColor, color);
        indices << (*it)->row();
      }
    }
    this->setLineColorButtonColor(color);
    emit lineColorChanged(indices, color);
  }
  // Force the widget to be shown as it disappear when the color dialog is shown
  this->setVisible(true);
};

void ChartOptionsWidget::setLineWidth(double value)
{
  QList<int> indices;
  QList<QTableWidgetItem*> selectedItems = this->plotTable->selectedItems();
  for (QList<QTableWidgetItem*>::const_iterator it = selectedItems.begin() ; it != selectedItems.end() ; ++it)
  {
    if ((*it)->column() == 0)
    {
      (*it)->setIcon(this->createLineIcon((*it)->data(LineColor).value<QColor>(), value));
      (*it)->setData(LineWidth, value);
      indices << (*it)->row();
    }
  }
  emit lineWidthChanged(indices, value);
};

void ChartOptionsWidget::paintEvent(QPaintEvent* event)
{
  Q_UNUSED(event)
  
  const double midX = (double)this->width()/2.0;
  const double penWidth = 0.5;
  static const QPointF points[3] = {
     QPointF(midX-12.0, 8.75),
     QPointF(midX, 0.0),
     QPointF(midX+12.0, 8.75),
  };
 
  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);
  painter.setBrush(Qt::white);
  
  painter.setPen(QPen(Qt::gray, penWidth));
  //painter.drawRoundedRect(QRectF(0.0, 20.0, this->width(), this->height()-20), 10.0, 10.0);
  painter.drawRect(QRectF(0.0, 8.0, this->width(), this->height()-8.0));
  
  painter.setPen(QPen(Qt::white, penWidth));
  painter.drawPolygon(points,3);
 
  painter.setPen(QPen(Qt::gray, penWidth));
  painter.drawPolyline(points,3);
};

QPixmap ChartOptionsWidget::createLineIcon(const QColor& color, double width)
{
  QPen p = color;
  QImage lineImage(16, 16, QImage::Format_ARGB32);
  QPainter painter(&lineImage);
  // painter.setRenderHint(QPainter::Antialiasing);
  painter.setBrush(Qt::NoBrush);
  painter.setPen(Qt::NoPen);
  painter.setCompositionMode(QPainter::CompositionMode_Source);
  painter.fillRect(lineImage.rect(), Qt::transparent);
  painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
  p.setWidthF(width); painter.setPen(p);
  painter.drawLine(QLineF(2.0, 8.0, 14.0, 8.0));
  return QPixmap::fromImage(lineImage);
};

void ChartOptionsWidget::setLineColorButtonColor(const QColor& color)
{
  this->lineColorButton->setStyleSheet("QPushButton {background-color: rgb(" + QString::number(color.red()) + "," + QString::number(color.green())  + "," + QString::number(color.blue()) + "); border: 1px solid lightgray;} QPushButton:pressed {border-color: gray;}");
};