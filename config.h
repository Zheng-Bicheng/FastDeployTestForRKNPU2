#ifndef CONFIG_H
#define CONFIG_H
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
#endif

#endif // CONFIG_H
