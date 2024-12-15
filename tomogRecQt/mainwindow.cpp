#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->graph->addGraph();
    ui->graph->graph(0)->valueAxis()->setLabel("Коэффициент поглощения");
    ui->graph->graph(0)->keyAxis()->setLabel("сдвиг t, пикс");
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Load File
void MainWindow::on_loadFileBtn_clicked()
{
    QFileDialog dialog;

    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setAcceptMode(QFileDialog::AcceptOpen);

    try
    {
        filename = dialog.getOpenFileName();
    }
    catch(...)
    {
        std::cout << "Some error!" << std::endl;
        return;
    }
    if (filename.size() < 3)
    {
        std::cout << "Empty string!" << std::endl;
        return;
    }
    std::cout << "String: " << filename.toStdString() << std::endl;

    arma::mat pic = gray_image_from_file<arma::mat>(filename.toStdString());
    object = create_scaled_Mat(pic, pic.n_rows, pic.n_cols);
    objectRot = object;
    objPic = QImage(object.n_rows, object.n_cols, QImage::Format_Grayscale8);
    create_QImage(object, objPic, object.n_rows, object.n_cols);

    y = QVector<double>(objectRot.n_rows);
    x = QVector<double>(objectRot.n_rows);
    for (auto i = 0; i < objectRot.n_rows; ++i)
    {
        x[i] = i;
    }

    ui->picture->setPixmap(QPixmap::fromImage(objPic));
    ui->result->setPixmap(QPixmap::fromImage(objPic));
    ui->sinogram->setPixmap(QPixmap::fromImage(objPic));

    return;
}

// Cur angle chenged
void MainWindow::on_curAngle_valueChanged(double val)
{
    // Rotate picture
    Rotate(object, objectRot, object.n_rows, object.n_cols, ui->curAngle->value());

    create_QImage(objectRot, objPic, objectRot.n_rows,objectRot.n_cols);
    ui->picture->setPixmap(QPixmap::fromImage(objPic));

    // Get projection
    get_projection(objectRot, objectRot.n_rows, objectRot.n_cols, y);

    auto g = ui->graph->graph(0);

    g->setData(x,y,true);
    g->rescaleAxes(true);
    ui->graph->replot();

    return;
}

void MainWindow::on_get_sinogram_clicked()
{
    double dA = ui->dAngle->value();
    double N = 180. / dA;
    std::cout << "N: " << N << std::endl;
    double angle = 0;

    arma::mat sinogram((int)N, (int)object.n_rows);

    for (int i = 0; i < (int)N; ++i)
    {
        Rotate(object, objectRot, object.n_rows, object.n_cols, angle);
        get_projection(objectRot, object.n_rows, object.n_cols, sinogram, i);
        angle +=dA;
        std::cout << "angle: " << angle << std::endl;
    }
    QImage sinPic(sinogram.n_rows, sinogram.n_cols, QImage::Format_Grayscale8);
    create_QImage(sinogram, sinPic, sinogram.n_rows, sinogram.n_cols);
    ui->sinogram->setPixmap(QPixmap::fromImage(sinPic));

    return;
}
