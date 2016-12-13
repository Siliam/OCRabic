/*
 *  File    : Image.h
 *  Desc    : A container class to ease the switch between vectors and cv::Mats
 *            Implements handy operations for debugging
 *  Author  : Ismail
 */

#ifndef IMAGE_H
#define IMAGE_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <QImage>
#include <QLabel>

using namespace std;

class Image : public cv::Mat
{
private:
    int _class;
    int _index;


public:
    Image(cv::Mat original);
    Image(cv::Mat original, int c, int i);
    Image(std::vector<double> v, int rows, int cols, int c = -1, int i = -1);

    QLabel* getQLabel();
    double  val(int j);
    void    print();
    void    show(std::string title);

    void    setClass(int c)     { _class = c; }
    void    setIndex(int i)     { _index = i; }
    int     getClass()          { return _class; }
    int     getNumber()         { return _index; }

};

#endif // IMAGE_H
