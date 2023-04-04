#ifndef CONFIG_H
#define CONFIG_H

#include <QString>
#include <QTime>

/* define model path */
#if (RK_SOC == 0)
#else

QString get_info(QString info);
#endif
#endif // CONFIG_H
