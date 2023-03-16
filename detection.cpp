#include "detection.h"
#include "ui_detection.h"
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
using namespace fastdeploy::vision::detection;

Detection::Detection(QWidget *parent) : QWidget(parent), ui(new Ui::Detection) {
  ui->setupUi(this);
  _parent_widget = parent;
}

Detection::~Detection() { delete ui; }

void Detection::on_comboBoxDevice_currentTextChanged(const QString &arg1) {
  if (arg1 == "Local Picture") {
    ui->pushButtonStart->setText("选择图片");
  } else {
    ui->pushButtonStart->setText("选择视频");
  }
}

void Detection::on_pushButtonStart_clicked() {
  if (ui->pushButtonStart->text() == "选择图片") {
    cv::Mat src = read_image();
    if (src.empty()) {
      return;
    }
    cv::Mat before_predict_image = change_mat_format(src);
    set_show_label(before_predict_image, ui->labelBeforeLabel);
    resize_show_label();
    predict_image(src);
  } else {
  }
}

void Detection::resize_show_label() {
  // 计算距离
  int distance_width = this->size().rwidth();
  distance_width -= 2 * ui->labelBeforeLabel->size().rwidth();
  distance_width += 10;
  int distance_height = this->size().rheight();
  distance_height -= 2 * ui->labelBeforeLabel->size().rheight();
  distance_height += 20;

  // resize显示部分
  ui->labelBeforeLabel->resize(ui->labelBeforeLabel->pixmap()->size());
  ui->labelAfterLabel->resize(ui->labelBeforeLabel->pixmap()->size());

  // 计算主界面resize后的距离
  int resize_w = distance_width + 2 * ui->labelBeforeLabel->size().rwidth();
  int resize_h = distance_height + 2 * ui->labelBeforeLabel->size().rheight();
  _parent_widget->resize(resize_w, resize_h);
}

cv::Mat Detection::change_mat_format(const cv::Mat &src) {
  // 转换为QT显示
  cv::Mat temp;
  cv::cvtColor(src, temp, cv::COLOR_BGR2RGB);
  return temp;
}

void Detection::set_show_label(const cv::Mat &show_data, QLabel *show_label) {
  QImage img = QImage((uchar *)show_data.data, show_data.cols, show_data.rows,
                      show_data.step, QImage::Format_RGB888);
  show_label->setPixmap(QPixmap::fromImage(img));
}

cv::Mat Detection::read_image() {
  QString file_path = QFileDialog::getOpenFileName(
      this, tr("Select execute file"), QDir::currentPath(),
      "Image files (*.jpg *.png *.jpeg);;All files(*.*)");
  if (!QFile::exists(file_path)) {
    return cv::Mat();
  }
  ui->lineEditInput->setText(file_path);

  // 读取图像
  cv::Mat src = cv::imread(file_path.toLatin1().data());
  if (src.empty()) {
    qDebug() << "图像不存在或出现了未知错误";
    return cv::Mat();
  }
  return src;
}

void Detection::predict_image(const cv::Mat &src) {
  fastdeploy::vision::DetectionResult res;
  std::string model_file, params_file, config_file;
  auto option = fastdeploy::RuntimeOption();
  RKYOLO *npu_model;
  if (ui->comboBoxOption->currentText() == "CPU") {
    QMessageBox::information(this, "提示信息", "用户确认信息");
    return;
  }
  if (ui->comboBoxModel->currentText() == "YOLOv5-s") {
    npu_model = new RKYOLOV5(model_file, option);
  }

  if (!npu_model->Initialized()) {
    std::cerr << "Failed to initialize." << std::endl;
    return;
  }
  if (!npu_model->Predict(src, &res)) {
    std::cerr << "Failed to predict." << std::endl;
    return;
  }
  auto vis_im = fastdeploy::vision::VisDetection(src, res, 0.5);
  cv::Mat after_predict_image = change_mat_format(vis_im);
  set_show_label(after_predict_image, ui->labelAfterLabel);
  QApplication::processEvents(QEventLoop::AllEvents, 100); //防止阻塞界面
}
