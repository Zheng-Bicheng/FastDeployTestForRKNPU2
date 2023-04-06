#ifndef BASEWIDGET_H
#define BASEWIDGET_H
/**********************************************************************/
/* define where to save model */
#define MODEL_FOLDER "./model/"

/* define target soc */
#define RKNPU2_SOC 1 // 0 for RK356X ; 1 for RK3588
/**********************************************************************/

#include "currentvideo.h"
#include "fastdeploy/vision.h"
#include "ui_basewidget.h"
#include <QLabel>
#include <QWidget>
namespace Ui {
class BaseWidget;
}

class BaseWidget : public QWidget {
  Q_OBJECT

public:
  explicit BaseWidget(QWidget *parent = nullptr);
  ~BaseWidget();

protected:
  cv::Mat read_image(QString image_path);
  bool set_image_to_label(cv::Mat bgr_image, QLabel *label);
  void resize_image_label(QLabel *label);
  void resize_image_label(QLabel *label, QSize size);
  void resize_main_widget();
  void resize_main_widget(QSize size);
  Ui::BaseWidget *get_ui();

private slots:
  virtual void on_pushButtonStart_clicked();
  virtual void on_comboBoxDevice_currentTextChanged(const QString &arg1);
  virtual void on_pushButtonOpenCamera_clicked();
  virtual void on_pushButtonInputFace_clicked();

private:
  Ui::BaseWidget *ui;
  QWidget *main_widget_;
  CurrentVideo current_video_;
  int distance_width_ = 0;
  int distance_height_ = 0;
};

#endif // BASEWIDGET_H
