#include "facedetection.h"
#include "ui_facedetection.h"

FaceDetection::FaceDetection(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FaceDetection)
{
    ui->setupUi(this);
}

FaceDetection::~FaceDetection()
{
    delete ui;
}
