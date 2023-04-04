#include "widget.h"
#include "./ui_widget.h"

Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget) {
  ui->setupUi(this);

  base_widget_ = new BaseWidget(this);
  ui->tabWidget->addTab(base_widget_, "BaseWidget");

  _detection = new Detection(this);
  ui->tabWidget->addTab(_detection, "Detection");

  _ocr = new OCR(this);
  ui->tabWidget->addTab(_ocr, "OCR");

  _segmentation = new Segmentation(this);
  ui->tabWidget->addTab(_segmentation, "Segmentation");

  _face_detection = new FaceDetection(this);
  ui->tabWidget->addTab(_face_detection, "FaceDetection");
}

Widget::~Widget() { delete ui; }
