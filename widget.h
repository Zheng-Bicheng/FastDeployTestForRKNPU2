#ifndef WIDGET_H
#define WIDGET_H

#include "basewidget.h"
#include "detection.h"
//#include "facedetection.h"
#include "ocr.h"
//#include "segmentation.h"
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
  //  Segmentation *_segmentation;
  //  FaceDetection *_face_detection;
  OCR *_ocr;
  BaseWidget *base_widget_;
};
#endif // WIDGET_H
