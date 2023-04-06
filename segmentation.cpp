#include "segmentation.h"

using namespace fastdeploy::vision::segmentation;

Segmentation::Segmentation(QWidget *parent) : BaseWidget(parent) {
  ui = get_ui();
  ui->comboBoxModel->addItem("PPHumansegV2");
}

Segmentation::~Segmentation() {}

void Segmentation::on_pushButtonStart_clicked() {
  QString file_path = QFileDialog::getOpenFileName(
      nullptr, tr("Select execute file"), QDir::currentPath(),
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

  if (ui->comboBoxModel->currentText() == "PPHumansegV2") {
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
  set_image_to_label(vis_im, ui->labelAfterLabel);
  resize_image_label(ui->labelAfterLabel);
  resize_main_widget();
  QApplication::processEvents(QEventLoop::AllEvents, 100); //防止阻塞界面
}
