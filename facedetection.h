#ifndef FACEDETECTION_H
#define FACEDETECTION_H

#include "basewidget.h"
#include <QDebug>
#include <QFileDialog>
#include <QLabel>
#include <QMessageBox>
#include <QString>
#include <QWidget>

#if (RKNPU2_SOC == 0)
#else
#define SCRFD_MODEL_PATH                                                       \
  "scrfd_500m_bnkps_shape640x640/"                                             \
  "scrfd_500m_bnkps_shape640x640_rk3588_quantized.rknn"
#endif

class FaceDetection : public BaseWidget {
  Q_OBJECT

public:
  explicit FaceDetection(QWidget *parent = nullptr);
  ~FaceDetection();

private slots:
  void on_pushButtonStart_clicked();

private:
  Ui::BaseWidget *ui;
  void predict_image(cv::Mat &src);
};

#endif // FACEDETECTION_H
