#include "facedetection.h"
FaceDetection::FaceDetection(QWidget *parent) : BaseWidget(parent) {
  ui = get_ui();
  ui->comboBoxModel->addItem("SCRFD");

  std::string model_file, params_file, config_file;
  auto option = fastdeploy::RuntimeOption();
  option.UseRKNPU2();
  QString path = QDir::cleanPath(QString(MODEL_FOLDER) + QDir::separator() +
                                 QString(SCRFD_MODEL_PATH));
  model_file = path.toStdString();
  scrfd_ = new SCRFD(model_file, params_file, option, fastdeploy::RKNN);
}

FaceDetection::~FaceDetection() { delete scrfd_; }

void FaceDetection::on_pushButtonStart_clicked() {
  QString file_path = QFileDialog::getOpenFileName(
      nullptr, tr("Select execute file"), QDir::currentPath(),
      "Image files (*.jpg *.png *.jpeg);;All files(*.*)");
  cv::Mat src = read_image(file_path);
  if (src.empty()) {
    qDebug() << "file_path is none.";
    return;
  }
  predict_image(src);
}

void FaceDetection::on_pushButtonOpenCamera_clicked() {
  std::string url = "v4l2src device=/dev/video20 ! image/jpeg, width=640, "
                    "height=360, framerate=25/1 ! jpegparse ! mppjpegdec ! "
                    "videoconvert ! appsink sync=false";
  ui->pushButtonInputFace->setDisabled(false);
  current_video_.Open(url);
  current_video_.start();
  QTimer::singleShot(1, this, &FaceDetection::show_data);
}

void FaceDetection::on_pushButtonInputFace_clicked() {
  do_predict_ = !do_predict_;
}

void FaceDetection::show_data() {
  cv::Mat src = read_mat_from_video();
  if (!src.empty()) {
    set_image_to_label(src, ui->labelBeforeLabel);
    predict_image(src);
  }
  QTimer::singleShot(1, this, &FaceDetection::show_data);
}

void FaceDetection::predict_image(cv::Mat &src) {
  fastdeploy::vision::FaceDetectionResult res;
  if (ui->comboBoxOption->currentText() == "CPU") {
    QMessageBox::information(this, tr("提示信息"),
                             tr("暂时不支持CPU推理，请使用NPU进行推理"));
    return;
  }

  if (ui->comboBoxModel->currentText() == "SCRFD") {
    if (!scrfd_->Initialized()) {
      std::cerr << "Failed to initialize." << std::endl;
      return;
    }
    scrfd_->DisableNormalize();
    scrfd_->DisablePermute();
    if (!scrfd_->Predict(&src, &res)) {
      std::cerr << "Failed to predict." << std::endl;
      return;
    }
  } else {
    return;
  }
  if (res.scores.size() == 0) {
    return;
  }
  auto vis_im = fastdeploy::vision::VisFaceDetection(src, res);
  set_image_to_label(vis_im, ui->labelBeforeLabel);
  resize_image_label(ui->labelBeforeLabel);
  resize_main_widget();

  res.boxes.resize(1);
  res.landmarks.resize(1);
  res.scores.resize(1);
  auto vis_im_list = AlignFaceWithFivePoints(src, res);
  set_image_to_label(vis_im_list[0], ui->labelAfterLabel);
  resize_image_label(ui->labelAfterLabel);
  resize_main_widget();
  QApplication::processEvents(QEventLoop::AllEvents, 100); //防止阻塞界面
}
