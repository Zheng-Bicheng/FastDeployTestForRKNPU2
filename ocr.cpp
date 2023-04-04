#include "ocr.h"
using namespace fastdeploy::vision::ocr;
using namespace fastdeploy::pipeline;
OCR::OCR(QWidget *parent) : BaseWidget(parent) {
  ui = get_ui();
  ui->comboBoxModel->addItem("PPOCRv3");
}

OCR::~OCR() {}
void OCR::on_pushButtonStart_clicked() {
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

void OCR::predict_image(const cv::Mat &src) {
  fastdeploy::vision::OCRResult result;
  std::string det_model_file, cls_model_file, rec_model_file, params_file,
      config_file, rec_label_file;
  auto det_option = fastdeploy::RuntimeOption();
  det_option.UseRKNPU2();
  auto cls_option = fastdeploy::RuntimeOption();
  cls_option.UseRKNPU2();
  auto rec_option = fastdeploy::RuntimeOption();
  rec_option.UseRKNPU2();
  auto format = fastdeploy::RKNN;
  if (ui->comboBoxOption->currentText() == "CPU") {
    QMessageBox::information(this, tr("提示信息"),
                             tr("暂时不支持CPU推理，请使用NPU进行推理"));
    return;
  }

  if (ui->comboBoxModel->currentText() == "PPOCRv3") {
    QString det_path =
        QDir::cleanPath(QString(MODEL_FOLDER) + QDir::separator() +
                        QString(PPOCRV3_DET_MODEL_PATH));
    det_model_file = det_path.toStdString();
    QString cls_path =
        QDir::cleanPath(QString(MODEL_FOLDER) + QDir::separator() +
                        QString(PPOCRV3_CLS_MODEL_PATH));
    cls_model_file = cls_path.toStdString();
    QString rec_path =
        QDir::cleanPath(QString(MODEL_FOLDER) + QDir::separator() +
                        QString(PPOCRV3_REC_MODEL_PATH));
    rec_model_file = rec_path.toStdString();
    QString label_path =
        QDir::cleanPath(QString(MODEL_FOLDER) + QDir::separator() +
                        QString(PPOCRV3_REC_LABEL_PATH));
    rec_label_file = label_path.toStdString();
    auto det_model =
        DBDetector(det_model_file, params_file, det_option, format);
    //    auto cls_model = fastdeploy::vision::ocr::Classifier(
    //        cls_model_file, params_file, cls_option, format);
    auto rec_model = Recognizer(rec_model_file, params_file, rec_label_file,
                                rec_option, format);

    //    cls_model.GetPreprocessor().DisableNormalize();
    //    cls_model.GetPreprocessor().DisablePermute();

    det_model.GetPreprocessor().DisableNormalize();
    det_model.GetPreprocessor().DisablePermute();

    rec_model.GetPreprocessor().DisableNormalize();
    rec_model.GetPreprocessor().DisablePermute();

    det_model.GetPreprocessor().SetStaticShapeInfer(true);
    rec_model.GetPreprocessor().SetStaticShapeInfer(true);

    assert(det_model.Initialized());
    //    assert(cls_model.Initialized());
    assert(rec_model.Initialized());

    auto ppocr_v3 = PPOCRv3(&det_model, nullptr, &rec_model);

    //    ppocr_v3.SetClsBatchSize(1);
    ppocr_v3.SetRecBatchSize(1);

    if (!ppocr_v3.Initialized()) {
      std::cerr << "Failed to initialize PP-OCR." << std::endl;
      return;
    }

    if (!ppocr_v3.Predict(src, &result)) {
      std::cerr << "Failed to predict." << std::endl;
      return;
    }
  } else {
    return;
  }

  auto vis_im = fastdeploy::vision::VisOcr(src, result);
  set_image_to_label(vis_im, ui->labelAfterLabel);
  resize_image_label(ui->labelAfterLabel);
  resize_main_widget();
  QApplication::processEvents(QEventLoop::AllEvents, 100); //防止阻塞界面
}
