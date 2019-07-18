#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_pushButton_clicked()
{
    // MAIN ACTION
    try{
        imageHandler first(filename_first);
        first.readStringFromImage();
        QString filename = filename_first;
        filename.resize(filename.size() - 4);
        filename += "_out.png";
        first.writeToImageFromString(filename);
        QPixmap map(filename);
        ui->result_img_box->setPixmap(map);
    } catch (QException& e){
        ui->log->setText(e.what());
    }
}

void MainWindow::on_button_first_img_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Select Image"), "\\" ,tr("Images (*.png *.bmp)"));
    if (not filename.isEmpty()){
    ui->statusBar->showMessage(filename + " loaded");
    ui->origin_img_box->setPixmap(QPixmap(filename));
    filename_first = filename;
    if (filename_second != "") ui->pushButton->setEnabled(1);
    }
}

void MainWindow::on_button_second_img_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Select Image"), "\\" ,tr("Images (*.png *.bmp)"));
    if (not filename.isEmpty()){
    ui->statusBar->showMessage(filename + " loaded");
    ui->final_img_box->setPixmap(QPixmap(filename));
    filename_second = filename;
    if (filename_first != "") ui->pushButton->setEnabled(1);
    }
}

void MainWindow::on_button_def_clicked()
{
    mode = 0;
}

void MainWindow::on_button_back_clicked()
{
    mode = 1;
}

void MainWindow::on_button_reverse_clicked()
{
    mode = 2;
}

void MainWindow::on_horizontalSlider_sliderMoved(int position)
{
    iteration = position;
    ui->it_counter->setText(QString::number(iteration));
}
