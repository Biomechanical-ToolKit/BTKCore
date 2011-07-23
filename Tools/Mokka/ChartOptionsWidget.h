#ifndef ChartOptionsWidget_h
#define ChartOptionsWidget_h

#include "ui_ChartOptionsWidget.h"

#include <QWidget>

class ChartOptionsWidget : public QWidget, public Ui::ChartOptionsWidget
{
  Q_OBJECT
  
public:
  ChartOptionsWidget(QWidget* parent = 0);
  
  void appendPlot(const QString& label, int color[3], double width);
  void clear();
  void setPlotOptionEnabled(bool enabled);
  
public slots:
  void displayPlotOption();
  void removePlot();
  void setLineColor();
  void setLineWidth(double value);
  
signals:
  void lineColorChanged(const QList<int>& indices, const QColor& color);
  void lineWidthChanged(const QList<int>& indices, double value);
  void plotRemoved(int);
  
protected:
  virtual void paintEvent(QPaintEvent* event);
  
private:
  QPixmap createLineIcon(const QColor& color, double width);
  void setLineColorButtonColor(const QColor& color);
  
  enum {LineColor = Qt::UserRole + 1, LineWidth};
};

#endif // ChartOptionsWidget_h