#ifndef FACEDETECTION_H
#define FACEDETECTION_H

#include <QWidget>

namespace Ui {
class FaceDetection;
}

class FaceDetection : public QWidget
{
    Q_OBJECT

public:
    explicit FaceDetection(QWidget *parent = nullptr);
    ~FaceDetection();

private:
    Ui::FaceDetection *ui;
};

#endif // FACEDETECTION_H
