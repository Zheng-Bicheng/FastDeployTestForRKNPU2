#include "ocr.h"
#include "config.h"
#include "ui_ocr.h"
using namespace fastdeploy::vision::ocr;
using namespace fastdeploy::pipeline;
OCR::OCR(QWidget *parent) : QWidget(parent), ui(new Ui::OCR) {
  ui->setupUi(this);
  _parent_widget = parent;
}

OCR::~OCR() { delete ui; }

void OCR::on_comboBoxDevice_currentTextChanged(const QString &arg1) {
  if (arg1 == "Local Picture") {
    ui->pushButtonStart->setText("选择图片");
  } else {
    ui->pushButtonStart->setText("选择视频");
  }
}

void OCR::on_pushButtonStart_clicked() {
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

void OCR::resize_show_label() {
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

void OCR::set_show_label(const cv::Mat &show_data, QLabel *show_label) {
  QImage img = QImage((uchar *)show_data.data, show_data.cols, show_data.rows,
                      show_data.step, QImage::Format_RGB888);
  show_label->setPixmap(QPixmap::fromImage(img));
}

cv::Mat OCR::read_image() {
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

  if (ui->comboBoxModel->currentText() == "ppocrv3") {
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

  ui->textEditInfo->append(get_info(QString::fromStdString(result.Str())));

  auto vis_im = fastdeploy::vision::VisOcr(src, result);
  cv::Mat after_predict_image = change_mat_format(vis_im);
  QApplication::processEvents(QEventLoop::AllEvents, 100); //防止阻塞界面
  set_show_label(after_predict_image, ui->labelAfterLabel);
  QApplication::processEvents(QEventLoop::AllEvents, 100); //防止阻塞界面
}
