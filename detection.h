#ifndef DETECTION_H
#define DETECTION_H
#include "basewidget.h"
#include <QDebug>
#include <QFileDialog>
#include <QLabel>
#include <QMessageBox>
#include <QString>
#include <QWidget>

#if (RKNPU2_SOC == 0)
#define YOLOV5_MODEL_PATH "yolov5-s-relu/yolov5s_relu_tk2_RK356X_i8.rknn"
#define YOLOV7_MODEL_PATH "yolov7-tiny/yolov7-tiny_tk2_RK356X_i8.rknn"
#define YOLOX_MODEL_PATH "yolox-s/yoloxs_tk2_RK356X_i8.rknn"
#else
#define YOLOV5_MODEL_PATH "yolov5-s-relu/yolov5s_relu_tk2_RK3588_i8.rknn"
#define YOLOV7_MODEL_PATH "yolov7-tiny/yolov7-tiny_tk2_RK3588_i8.rknn"
#define YOLOX_MODEL_PATH "yolox-s/yoloxs_tk2_RK3588_i8.rknn"
#endif

class Detection : public BaseWidget {
  Q_OBJECT

public:
  explicit Detection(QWidget *parent = nullptr);
  ~Detection();

private slots:
  void on_pushButtonStart_clicked();

private:
  Ui::BaseWidget *ui;
  void predict_image(const cv::Mat &src);
};

#endif // DETECTION_H
