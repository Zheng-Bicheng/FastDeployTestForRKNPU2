#ifndef CONFIG_H
#define CONFIG_H

#include "fastdeploy/vision.h"
#include <QString>
#include <QTime>

/* define where to save model */
#define MODEL_FOLDER "./model/"

/* define target soc */
// 0 for RK356X ; 1 for RK3588
#define RK_SOC 1

/* define model path */
#if (RK_SOC == 0)
#define YOLOV5_MODEL_PATH "yolov5-s-relu/yolov5s_relu_tk2_RK356X_i8.rknn"
#define YOLOV7_MODEL_PATH "yolov7-tiny/yolov7-tiny_tk2_RK356X_i8.rknn"
#define YOLOX_MODEL_PATH "yolox-s/yoloxs_tk2_RK356X_i8.rknn"
#else
#define YOLOV5_MODEL_PATH "yolov5-s-relu/yolov5s_relu_tk2_RK3588_i8.rknn"
#define YOLOV7_MODEL_PATH "yolov7-tiny/yolov7-tiny_tk2_RK3588_i8.rknn"
#define YOLOX_MODEL_PATH "yolox-s/yoloxs_tk2_RK3588_i8.rknn"

#define PPOCRV3_DET_MODEL_PATH                                                 \
  "PPOCRV3_det_QAT/PPOCRV3_det_QAT_rk3588_unquantized.rknn"
#define PPOCRV3_CLS_MODEL_PATH                                                 \
  "ch_ppocr_mobile_v2.0_cls_infer/"                                            \
  "ch_ppocr_mobile_v20_cls_infer_rk3588_quantized.rknn"
//#define PPOCRV3_REC_MODEL_PATH                                                 \
//  "ch_PP-OCRv3_rec_infer/ch_PP-OCRv3_rec_infer_rk3588_unquantized.rknn"
#define PPOCRV3_REC_MODEL_PATH                                                 \
  "PPOCRV3_rec_QAT/PPOCRV3_rec_QAT_rk3588_unquantized.rknn"
#define PPOCRV3_REC_LABEL_PATH "ch_PP-OCRv3_rec_infer/ppocr_keys_v1.txt"

#define PPHUMANSEG_MODEL_PATH                                                  \
  "Portrait_PP_HumanSegV2_Lite_256x144_infer/"                                 \
  "Portrait_PP_HumanSegV2_Lite_256x144_infer_rk3588_unquantized.rknn"
#define PPHUMANSEG_CONFIG_PATH                                                 \
  "Portrait_PP_HumanSegV2_Lite_256x144_infer/deploy.yaml"
#endif

QString get_info(QString info);
cv::Mat change_mat_format(const cv::Mat &src);
#endif // CONFIG_H
