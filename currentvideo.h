#ifndef CURRENTVIDEO_H
#define CURRENTVIDEO_H

#include "opencv2/core.hpp"
#include "opencv2/videoio.hpp"
#include <QMutex>
#include <QQueue>
#include <QThread>

class CurrentVideo : public QThread {
public:
  explicit CurrentVideo() = default;
  void run();
  bool Open(std::string &gstreamer_url);
  cv::Mat Read();

private:
  cv::VideoCapture cap_;
  QQueue<cv::Mat> mat_queue_;
  void GetMatFromOpencv(void *class_point);
  bool init_ = false;
};

#endif // CURRENTVIDEO_H
