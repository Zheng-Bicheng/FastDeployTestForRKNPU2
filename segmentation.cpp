#include "segmentation.h"
#include "ui_segmentation.h"

using namespace fastdeploy::vision::segmentation;

Segmentation::Segmentation(QWidget *parent)
    : QWidget(parent), ui(new Ui::Segmentation) {
  ui->setupUi(this);
  _parent_widget = parent;
}

Segmentation::~Segmentation() { delete ui; }

void Segmentation::on_comboBoxDevice_currentTextChanged(const QString &arg1) {
  if (arg1 == "Local Picture") {
    ui->pushButtonStart->setText("选择图片");
  } else {
    ui->pushButtonStart->setText("选择视频");
  }
}

void Segmentation::on_pushButtonStart_clicked() {
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

void Segmentation::resize_show_label() {
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

void Segmentation::set_show_label(const cv::Mat &show_data,
                                  QLabel *show_label) {
  QImage img = QImage((uchar *)show_data.data, show_data.cols, show_data.rows,
                      show_data.step, QImage::Format_RGB888);
  show_label->setPixmap(QPixmap::fromImage(img));
}

cv::Mat Segmentation::read_image() {
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

void Segmentation::predict_image(const cv::Mat &src) {
  fastdeploy::vision::SegmentationResult res;
  std::string model_file, params_file, config_file;
  auto format = fastdeploy::ModelFormat::RKNN;
  auto option = fastdeploy::RuntimeOption();
  option.UseRKNPU2();
  if (ui->comboBoxOption->currentText() == "CPU") {
    QMessageBox::information(this, tr("提示信息"),
                             tr("暂时不支持CPU推理，请使用NPU进行推理"));
    return;
  }

  if (ui->comboBoxModel->currentText() == "PPHumanseg") {
    QString model_path =
        QDir::cleanPath(QString(MODEL_FOLDER) + QDir::separator() +
                        QString(PPHUMANSEG_MODEL_PATH));
    model_file = model_path.toStdString();
    QString config_path =
        QDir::cleanPath(QString(MODEL_FOLDER) + QDir::separator() +
                        QString(PPHUMANSEG_CONFIG_PATH));
    config_file = config_path.toStdString();
    auto model =
        PaddleSegModel(model_file, params_file, config_file, option, format);
    if (!model.Initialized()) {
      std::cerr << "Failed to initialize." << std::endl;
      return;
    }
    model.GetPreprocessor().DisablePermute();
    model.GetPreprocessor().DisableNormalize();
    if (!model.Predict(src, &res)) {
      std::cerr << "Failed to predict." << std::endl;
      return;
    }
  } else {
    return;
  }
  auto vis_im = fastdeploy::vision::VisSegmentation(src, res);
  cv::Mat after_predict_image = change_mat_format(vis_im);
  set_show_label(after_predict_image, ui->labelAfterLabel);
  QApplication::processEvents(QEventLoop::AllEvents, 100); //防止阻塞界面
}
