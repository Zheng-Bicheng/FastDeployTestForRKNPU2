#ifndef OCR_H
#define OCR_H
#include "basewidget.h"
#include <QDebug>
#include <QFileDialog>
#include <QLabel>
#include <QMessageBox>
#include <QString>
#include <QWidget>

#if (RKNPU2_SOC == 0)
#else
#define PPOCRV3_DET_MODEL_PATH                                                 \
  "PPOCRV3_det_QAT/PPOCRV3_det_QAT_rk3588_unquantized.rknn"
#define PPOCRV3_CLS_MODEL_PATH                                                 \
  "ch_ppocr_mobile_v2.0_cls_infer/"                                            \
  "ch_ppocr_mobile_v20_cls_infer_rk3588_quantized.rknn"
#define PPOCRV3_REC_MODEL_PATH                                                 \
  "ch_PP-OCRv3_rec_infer/ch_PP-OCRv3_rec_infer_rk3588_unquantized.rknn"
//#define PPOCRV3_REC_MODEL_PATH                                                 \
//  "PPOCRV3_rec_QAT/PPOCRV3_rec_QAT_rk3588_unquantized.rknn"
#define PPOCRV3_REC_LABEL_PATH "ch_PP-OCRv3_rec_infer/ppocr_keys_v1.txt"
#endif

class OCR : public BaseWidget {
  Q_OBJECT

public:
  explicit OCR(QWidget *parent = nullptr);
  ~OCR();
private slots:
  void on_pushButtonStart_clicked();

private:
  Ui::BaseWidget *ui;
  void predict_image(const cv::Mat &src);
};

#endif // OCR_H
