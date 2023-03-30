#include "widget.h"
#include "./ui_widget.h"

Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget) {
  ui->setupUi(this);
  _detection = new Detection(this);
  ui->tabWidget->addTab(_detection, "Detection");

  _ocr = new OCR(this);
  ui->tabWidget->addTab(_ocr, "OCR");

  _segmentation = new Segmentation(this);
  ui->tabWidget->addTab(_segmentation, "Segmentation");
}

Widget::~Widget() { delete ui; }
