#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <armadillo>
#include "qcustomplot.h"
#include "image_io.hpp"
#include "tomograph.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void on_loadFileBtn_clicked();
    void on_curAngle_valueChanged(double val);
    void on_get_sinogram_clicked();

private:  // functions
    // Create Q image
    template<typename Image>
    void create_QImage(Image img, QImage& image, unsigned w, unsigned h)
    {
        for (auto i = 0; i < w; ++i)
            for (auto j =0 ;j < h; ++j)
            {
                auto c = qRgb(img(i,j),img(i,j),img(i,j));
                image.setPixel(i,j,c);
            }

        return;
    }

    // Create scaled mat
    template<typename Image>
    Image create_scaled_Mat(Image img, unsigned w, unsigned h)
    {
        int width = w * sqrt(2) * 1.1;
        int height = h * sqrt(2) * 1.1;

        int dx = (width - w) / 2;
        int dy = (height - h) / 2;

        arma::mat image(width, height);
        for (auto i = 0; i < w; ++i)
            for (auto j =0 ;j < h; ++j)
            {
                image(i+ dx, j + dy) = img(i,j);
            }

        return image;
    }

private: //variables
    Ui::MainWindow *ui;
    arma::mat object;
    arma::mat objectRot;
    QString filename;
    QImage objPic;
    QVector<double> y;
    QVector<double> x;
};
#endif // MAINWINDOW_H
