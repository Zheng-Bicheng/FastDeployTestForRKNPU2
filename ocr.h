#ifndef OCR_H
#define OCR_H
#include "config.h"
#include <QDebug>
#include <QFileDialog>
#include <QLabel>
#include <QMessageBox>
#include <QString>
#include <QWidget>

namespace Ui {
class OCR;
}

class OCR : public QWidget {
  Q_OBJECT

public:
  explicit OCR(QWidget *parent = nullptr);
  ~OCR();
private slots:
  void on_comboBoxDevice_currentTextChanged(const QString &arg1);
  void on_pushButtonStart_clicked();

private:
  QWidget *_parent_widget = nullptr;
  Ui::OCR *ui;
  void resize_show_label();
  void set_show_label(const cv::Mat &show_data, QLabel *show_label);
  cv::Mat read_image();
  void predict_image(const cv::Mat &src);
};

#endif // OCR_H
