#include "currentvideo.h"
#include <QDebug>

void CurrentVideo::run() {
  if (!init_) {
    return;
  }
  while (true) {
    cv::Mat input_mat_{};
    if (cap_.isOpened()) {
      cap_ >> input_mat_;
      if (mat_queue_.empty()) {
        mat_queue_.enqueue(input_mat_);
      }
    }
  }
}

bool CurrentVideo::Open(std::string &gstreamer_url) {
  if (init_) {
    qDebug() << "Don't reopen the LZCurrentVideo";
    return init_;
  }

  qDebug() << QString::fromStdString(gstreamer_url);
  cap_ = cv::VideoCapture(gstreamer_url, cv::CAP_GSTREAMER);
  if (!cap_.isOpened()) {
    qDebug() << "Can't open gstreamer.";
    return init_;
  }

  init_ = true;
  return init_;
}

cv::Mat CurrentVideo::Read() {
  cv::Mat output_mat_;
  if (!init_) {
    qDebug() << "CurrentVideo has not been initialized!";
    return output_mat_;
  }

  if (mat_queue_.empty()) {
    qDebug() << "mat_queue_ is empty()!";
    return output_mat_;
  }

  output_mat_ = mat_queue_.front();
  mat_queue_.dequeue();
  return output_mat_;
}
