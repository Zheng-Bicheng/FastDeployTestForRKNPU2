#ifndef DETECTION_H
#define DETECTION_H
#include "config.h"
#include <QDebug>
#include <QFileDialog>
#include <QLabel>
#include <QMessageBox>
#include <QString>
#include <QWidget>
namespace Ui {
class Detection;
}

class Detection : public QWidget {
  Q_OBJECT

public:
  explicit Detection(QWidget *parent = nullptr);
  ~Detection();

private slots:
  void on_comboBoxDevice_currentTextChanged(const QString &arg1);
  void on_pushButtonStart_clicked();

private:
  QWidget *_parent_widget = nullptr;
  Ui::Detection *ui;
  void resize_show_label();
  void set_show_label(const cv::Mat &show_data, QLabel *show_label);
  cv::Mat read_image();
  void predict_image(const cv::Mat &src);
};

#endif // DETECTION_H
