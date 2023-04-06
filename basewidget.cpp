#include "basewidget.h"

#include <QDebug>
#include <QDir>
#include <QFileDialog>
#include <QImage>
#include <QPixmap>
BaseWidget::BaseWidget(QWidget *parent)
    : QWidget(parent), ui(new Ui::BaseWidget) {
  ui->setupUi(this);
  main_widget_ = parent;

  // 计算距离
  QLabel *before_label = ui->labelBeforeLabel;
  QLabel *after_label = ui->labelAfterLabel;
  distance_width_ = this->size().rwidth();
  distance_width_ -= before_label->size().rwidth();
  distance_width_ -= after_label->size().rwidth();
  distance_width_ += 10;

  distance_height_ = this->size().rheight();
  distance_height_ -= before_label->size().rheight();
  distance_height_ -= after_label->size().rheight();
  distance_height_ += 10;

  // hide
  ui->pushButtonInputFace->hide();
  ui->pushButtonInputFace->setDisabled(true);
  ui->pushButtonOpenCamera->hide();

  ui->labelBeforeLabel->setAlignment(Qt::AlignCenter);
  ui->labelAfterLabel->setAlignment(Qt::AlignCenter);
}

BaseWidget::~BaseWidget() { delete ui; }

void BaseWidget::on_pushButtonStart_clicked() {
  QString file_path = QFileDialog::getOpenFileName(
      nullptr, tr("Select execute file"), QDir::currentPath(),
      "Image files (*.jpg *.png *.jpeg);;All files(*.*)");
  cv::Mat src = read_image(file_path);
  if (src.empty()) {
    return;
  }
  set_image_to_label(src, ui->labelBeforeLabel);
  resize_image_label(ui->labelBeforeLabel);
  resize_main_widget();
}

cv::Mat BaseWidget::read_image(QString image_path) {
  if (!QFile::exists(image_path)) {
    return cv::Mat();
  }
  ui->lineEditInput->setText(image_path);

  // 读取图像
  cv::Mat src = cv::imread(image_path.toLatin1().data());
  if (src.empty()) {
    qDebug() << "图像不存在或出现了未知错误";
    return cv::Mat();
  }
  return src;
}

bool BaseWidget::set_image_to_label(cv::Mat bgr_image, QLabel *label) {
  if (bgr_image.empty()) {
    qDebug() << "set_image_to_label: bgr_image is empty()";
    return false;
  }
  cv::Mat rgb_image;
  cv::cvtColor(bgr_image, rgb_image, cv::COLOR_BGR2RGB);
  QImage img = QImage((uchar *)rgb_image.data, rgb_image.cols, rgb_image.rows,
                      rgb_image.step, QImage::Format_RGB888);
  label->setPixmap(QPixmap::fromImage(img));
  return true;
}

void BaseWidget::resize_image_label(QLabel *label) {
  QSize size = label->pixmap()->size();
  resize_image_label(label, size);
}

void BaseWidget::resize_image_label(QLabel *label, QSize size) {
  label->resize(size);
}

void BaseWidget::resize_main_widget() {
  QLabel *before_label = ui->labelBeforeLabel;
  QLabel *after_label = ui->labelAfterLabel;
  int resize_w, resize_h;
  if (before_label->pixmap() == nullptr) {
    resize_w = distance_width_ + before_label->size().rwidth() +
               after_label->size().rwidth();
    resize_h = distance_height_ + before_label->size().rheight() +
               after_label->size().rheight();
  } else if ((before_label->pixmap() != nullptr) &&
             (after_label->pixmap() == nullptr)) {
    resize_w = distance_width_ + before_label->pixmap()->size().rwidth() +
               after_label->size().rwidth();
    resize_h = distance_height_ + before_label->pixmap()->size().rheight() +
               after_label->size().rheight();
  } else {
    resize_w = distance_width_ + before_label->pixmap()->size().rwidth() +
               after_label->pixmap()->size().rwidth();
    resize_h = distance_height_ + before_label->pixmap()->size().rheight() +
               after_label->pixmap()->size().rheight();
  }

  main_widget_->resize(resize_w, resize_h);
}

void BaseWidget::resize_main_widget(QSize size) { main_widget_->resize(size); }

Ui::BaseWidget *BaseWidget::get_ui() { return ui; }

void BaseWidget::on_comboBoxDevice_currentTextChanged(const QString &arg1) {
  if (arg1 == "Local Picture") {
    ui->pushButtonStart->show();
    ui->lineEditInput->show();
    ui->pushButtonOpenCamera->hide();
    ui->pushButtonInputFace->hide();
  } else {
    ui->pushButtonStart->hide();
    ui->lineEditInput->hide();
    ui->pushButtonOpenCamera->show();
    ui->pushButtonInputFace->show();
  }
}

void BaseWidget::on_pushButtonOpenCamera_clicked() {
  ui->pushButtonInputFace->setDisabled(false);
}

void BaseWidget::on_pushButtonInputFace_clicked() {}
