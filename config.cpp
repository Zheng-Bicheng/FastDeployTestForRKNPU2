#include "config.h"

QString get_info(QString info) {
  QTime current_time = QTime::currentTime();
  QString time = current_time.toString("hh:mm:ss t");
  QDate current_date = QDate::currentDate();
  QString date = current_date.toString("yyyy-MM-dd");
  return date + " " + time + ": " + info + "\n";
}

cv::Mat change_mat_format(const cv::Mat &src) {
  // 转换为QT显示
  cv::Mat temp;
  cv::cvtColor(src, temp, cv::COLOR_BGR2RGB);
  return temp;
}
