#ifndef FACEDETECTION_H
#define FACEDETECTION_H

#include "config.h"
#include <QDebug>
#include <QFileDialog>
#include <QLabel>
#include <QMessageBox>
#include <QString>
#include <QWidget>
namespace Ui {
class FaceDetection;
}

class FaceDetection : public QWidget {
  Q_OBJECT

public:
  explicit FaceDetection(QWidget *parent = nullptr);
  ~FaceDetection();

private slots:
  void on_pushButtonStart_clicked();

  void on_comboBoxDevice_currentTextChanged(const QString &arg1);

private:
  QWidget *_parent_widget = nullptr;
  void resize_show_label();
  void set_show_label(const cv::Mat &show_data, QLabel *show_label);
  Ui::FaceDetection *ui;
  cv::Mat read_image();
  void predict_image(cv::Mat &src);
};

#endif // FACEDETECTION_H
