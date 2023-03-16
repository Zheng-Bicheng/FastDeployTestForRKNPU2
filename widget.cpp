#include "widget.h"
#include "./ui_widget.h"

Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget) {
  ui->setupUi(this);
  _detection = new Detection(this);
  ui->tabWidget->addTab(_detection, "Detection");
}

Widget::~Widget() { delete ui; }
