#include <iostream>
#include <armadillo>
#include <QMainWindow>

#define Pi 3.14159265359

template<typename T>
void Rotate(const arma::mat& data_in, arma::mat& data_out, T h, T w, double angle)
{
     int i1, j1;
     int height = h / sqrt(2);
     int width  = w / sqrt(2);
     int dx = (w - width) / 2;
     int dy = (h - height) / 2;

     int c_h = h / 2;
     int c_w = w / 2;

     angle *= (Pi / 180.);

     double sin_a = sin(angle);
     double cos_a = cos(angle);

     for (int i = 0; i < h; ++i)
         for (int j = 0; j < w; ++j)
             data_out(i,j) = 0;

     for (int i = dy; i < h - dy; ++i)
     {
         for (int j = dx; j < w - dy; ++j)
         {
             i1 = cos_a * (i - c_h) - sin_a * (j - c_w);
             j1 = sin_a * (i - c_h) + cos_a * (j - c_w);

             //i1 *= 0.9;
             //j1 *= 0.9;

             // inverce translate
             i1 += c_h;
             j1 += c_w;

             data_out(i1, j1) = data_in(i, j);
         }
     }

     return;
}

template<typename T>
void get_projection(const arma::mat& object, T rows, T cols, QVector<double>& vec)
{
    double sum = 0;
    for (T i = 0; i < rows; ++i)
    {
        for (T j = 0; j < cols; ++j)
        {
            sum += (double)object(i,j);
        }
        vec[i] = sum;

        sum = 0;
    }
    return;
}

template<typename T>
void get_projection(const arma::mat& object, T rows, T cols, arma::mat& sinogram, int n)
{
    double sum = 0;
    for (T i = 0; i < rows; ++i)
    {
        for (T j = 0; j < cols; ++j)
        {
            sum += (double)object(i,j);
        }
        sinogram(n,i) = sum;

        sum = 0;
    }
    return;
}
