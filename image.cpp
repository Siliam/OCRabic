/*
 *  File    : Image.cpp
 *  Desc    :
 *  Author  : Ismail
 */

#include "Image.h"
#include <iostream>
#include <vector>
#include <QDebug>


Image::Image(cv::Mat original) : cv::Mat(original) { }

Image::Image(vector<double> v, int rows, int cols, int c, int i) : cv::Mat(rows, cols, CV_32FC1)
{
    for(int i = 0; i < rows; i++)
        for(int j = 0; j < cols; j++)
            this->at<float>(i, j) = v[j * rows + i];

    cv::normalize(*this, *this, 0, 255, cv::NORM_MINMAX, CV_32FC1, cv::Mat());
    cv::convertScaleAbs(*this, *this);
    _class = c;
    _index = i;
}

Image::Image(cv::Mat original, int c, int i) : cv::Mat(original)
{
    _class = c;
    _index = i;
}

double Image::val(int j)
{
    if(j % rows < rows && j / rows < cols)
        return (double) this->at<uchar>(j % rows, j / rows);
    else
    {
        qDebug() << "index out of range @Image::val";
        return 0.0;
    }
}

// Output the image in ASCII
void Image::print()
{
    std::cerr << "Image : " << _index << " (class " << _class << ")\n";

    uchar depth = this->type() & CV_MAT_DEPTH_MASK;
    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < cols; j++)
        {
            if(depth == CV_8U)
                std::cerr << (((int) this->at<uchar>(i, j) > 128) ? "." : "X");
            else
                std::cerr << (((int) this->at<float>(i, j) > 128) ? "." : "X");
        }
        std::cerr << "\n";
    }
}

void Image::show(string title)
{
    cv::Mat im(*this);
    cv::cvtColor(im, im, cv::COLOR_GRAY2RGB);
    cv::imshow(title, im);
}

QLabel* Image::getQLabel()
{
    QLabel* lbl = new QLabel();
    cv::Mat mat;
    cv::cvtColor(*this, mat, cv::COLOR_GRAY2RGB);
    QImage img((uchar *) mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
    lbl->setPixmap(QPixmap::fromImage(img));
    lbl->adjustSize();
    return lbl;
}
