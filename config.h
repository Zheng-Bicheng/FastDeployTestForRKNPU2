#ifndef CONFIG_H
#define CONFIG_H

#include <QString>
#include <QTime>

/* define model path */
#if (RK_SOC == 0)
#else
#define PPHUMANSEG_MODEL_PATH                                                  \
  "Portrait_PP_HumanSegV2_Lite_256x144_infer/"                                 \
  "Portrait_PP_HumanSegV2_Lite_256x144_infer_rk3588_unquantized.rknn"
#define PPHUMANSEG_CONFIG_PATH                                                 \
  "Portrait_PP_HumanSegV2_Lite_256x144_infer/deploy.yaml"

#define SCRFD_MODEL_PATH                                                       \
  "scrfd_500m_bnkps_shape640x640/"                                             \
  "scrfd_500m_bnkps_shape640x640_rk3588_quantized.rknn"

QString get_info(QString info);
#endif
#endif // CONFIG_H
