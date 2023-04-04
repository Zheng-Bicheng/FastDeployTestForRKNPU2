#include "detection.h"
using namespace fastdeploy::vision::detection;

Detection::Detection(QWidget *parent) : BaseWidget(parent) {
  ui = get_ui();
  ui->comboBoxModel->addItem("YOLOv5-s");
  ui->comboBoxModel->addItem("YOLOv7-tiny");
  ui->comboBoxModel->addItem("YOLOX-s");
}

Detection::~Detection() {}

void Detection::on_pushButtonStart_clicked() {
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

void Detection::predict_image(const cv::Mat &src) {
  fastdeploy::vision::DetectionResult res;
  std::string model_file, params_file, config_file;
  auto option = fastdeploy::RuntimeOption();
  option.UseRKNPU2();
  RKYOLO *npu_model = nullptr;
  if (ui->comboBoxOption->currentText() == "CPU") {
    QMessageBox::information(this, tr("提示信息"),
                             tr("暂时不支持CPU推理，请使用NPU进行推理"));
    return;
  }
  if (ui->comboBoxModel->currentText() == "YOLOv5-s") {
    QString path = QDir::cleanPath(QString(MODEL_FOLDER) + QDir::separator() +
                                   QString(YOLOV5_MODEL_PATH));
    model_file = path.toStdString();
    npu_model = new RKYOLOV5(model_file, option);
  } else if (ui->comboBoxModel->currentText() == "YOLOv7-tiny") {
    QString path = QDir::cleanPath(QString(MODEL_FOLDER) + QDir::separator() +
                                   QString(YOLOV7_MODEL_PATH));
    model_file = path.toStdString();
    npu_model = new RKYOLOV7(model_file, option);
  } else if (ui->comboBoxModel->currentText() == "YOLOX-s") {
    QString path = QDir::cleanPath(QString(MODEL_FOLDER) + QDir::separator() +
                                   QString(YOLOX_MODEL_PATH));
    model_file = path.toStdString();
    npu_model = new RKYOLOX(model_file, option);
  } else {
    return;
  }

  if (!npu_model->Initialized()) {
    std::cerr << "Failed to initialize." << std::endl;
    return;
  }

  if (!npu_model->Predict(src, &res)) {
    std::cerr << "Failed to predict." << std::endl;
    return;
  }
  int vis_size = res.boxes.size();
  ui->textEditInfo->append(QString::number(vis_size));
  auto vis_im = fastdeploy::vision::VisDetection(
      src, res, ui->lineEditInputConf->text().toFloat());
  set_image_to_label(vis_im, ui->labelAfterLabel);
  resize_image_label(ui->labelAfterLabel);
  resize_main_widget();
  QApplication::processEvents(QEventLoop::AllEvents, 100); //防止阻塞界面
  delete npu_model;
}
