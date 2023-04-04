#include "config.h"

QString get_info(QString info) {
  QTime current_time = QTime::currentTime();
  QString time = current_time.toString("hh:mm:ss t");
  QDate current_date = QDate::currentDate();
  QString date = current_date.toString("yyyy-MM-dd");
  return date + " " + time + ": " + info + "\n";
}
