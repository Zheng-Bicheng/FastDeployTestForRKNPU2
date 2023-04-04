#ifndef SEGMENTATION_H
#define SEGMENTATION_H
#include "basewidget.h"
#include <QDebug>
#include <QFileDialog>
#include <QLabel>
#include <QMessageBox>
#include <QString>
#include <QWidget>
#if (RKNPU2_SOC == 0)
#else
#define PPHUMANSEG_MODEL_PATH                                                  \
  "Portrait_PP_HumanSegV2_Lite_256x144_infer/"                                 \
  "Portrait_PP_HumanSegV2_Lite_256x144_infer_rk3588_unquantized.rknn"
#define PPHUMANSEG_CONFIG_PATH                                                 \
  "Portrait_PP_HumanSegV2_Lite_256x144_infer/deploy.yaml"
#endif
class Segmentation : public BaseWidget {
  Q_OBJECT

public:
  explicit Segmentation(QWidget *parent = nullptr);
  ~Segmentation();

private slots:
  void on_pushButtonStart_clicked();

private:
  Ui::BaseWidget *ui;
  void predict_image(const cv::Mat &src);
};

#endif // SEGMENTATION_H
