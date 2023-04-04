#include "facedetection.h"
using namespace fastdeploy::vision::facedet;
using namespace fastdeploy::vision::utils;
FaceDetection::FaceDetection(QWidget *parent) : BaseWidget(parent) {
  ui = get_ui();
  ui->comboBoxModel->addItem("SCRFD");
}

FaceDetection::~FaceDetection() {}

void FaceDetection::on_pushButtonStart_clicked() {
  QString file_path = QFileDialog::getOpenFileName(
      this, tr("Select execute file"), QDir::currentPath(),
      "Image files (*.jpg *.png *.jpeg);;All files(*.*)");
  cv::Mat src = read_image(file_path);
  if (src.empty()) {
    qDebug() << "file_path is none.";
    return;
  }
  set_image_to_label(src, ui->labelBeforeLabel);
  resize_image_label(ui->labelBeforeLabel);
  resize_main_widget();
  predict_image(src);
}

void FaceDetection::predict_image(cv::Mat &src) {
  fastdeploy::vision::FaceDetectionResult res;
  std::string model_file, params_file, config_file;
  auto option = fastdeploy::RuntimeOption();
  option.UseRKNPU2();
  if (ui->comboBoxOption->currentText() == "CPU") {
    QMessageBox::information(this, tr("提示信息"),
                             tr("暂时不支持CPU推理，请使用NPU进行推理"));
    return;
  }

  if (ui->comboBoxModel->currentText() == "SCRFD") {
    QString path = QDir::cleanPath(QString(MODEL_FOLDER) + QDir::separator() +
                                   QString(SCRFD_MODEL_PATH));
    model_file = path.toStdString();
    auto model = SCRFD(model_file, params_file, option, fastdeploy::RKNN);
    if (!model.Initialized()) {
      std::cerr << "Failed to initialize." << std::endl;
      return;
    }
    model.DisableNormalize();
    model.DisablePermute();
    if (!model.Predict(&src, &res)) {
      std::cerr << "Failed to predict." << std::endl;
      return;
    }
  } else {
    return;
  }
  res.boxes.resize(1);
  res.landmarks.resize(1);
  res.scores.resize(1);
  auto vis_im_list = AlignFaceWithFivePoints(src, res);
  set_image_to_label(vis_im_list[0], ui->labelAfterLabel);
  resize_image_label(ui->labelAfterLabel);
  resize_main_widget();
  QApplication::processEvents(QEventLoop::AllEvents, 100); //防止阻塞界面
}
