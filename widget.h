#ifndef WIDGET_H
#define WIDGET_H

#include "detection.h"
#include "ocr.h"
#include "segmentation.h"
#include <QWidget>
QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget {
  Q_OBJECT

public:
  Widget(QWidget *parent = nullptr);
  ~Widget();

private:
  Ui::Widget *ui;
  Detection *_detection;
  Segmentation *_segmentation;
  OCR *_ocr;
};
#endif // WIDGET_H
