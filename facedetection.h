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

using namespace fastdeploy::vision::facedet;
using namespace fastdeploy::vision::faceid;
using namespace fastdeploy::vision::utils;

class FaceDetection : public BaseWidget {
  Q_OBJECT

public:
  explicit FaceDetection(QWidget *parent = nullptr);
  ~FaceDetection();

private slots:
  virtual void on_pushButtonStart_clicked();
  virtual void on_pushButtonOpenCamera_clicked();
  virtual void on_pushButtonInputFace_clicked();
  virtual void show_data();

private:
  Ui::BaseWidget *ui;
  void predict_image(cv::Mat &src);
  SCRFD *scrfd_;
  ArcFace *arcface_;
  bool do_predict_ = false;
};

#endif // FACEDETECTION_H
