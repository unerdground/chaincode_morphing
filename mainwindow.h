#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>

#include <imagehandler.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_button_first_img_clicked();

    void on_button_second_img_clicked();

    void on_button_back_clicked();

    void on_button_def_clicked();

    void on_button_reverse_clicked();

    void on_horizontalSlider_sliderMoved(int position);

private:
    Ui::MainWindow *ui;

    unsigned int mode = 0;
    int iteration = 5;
    QString filename_first = "";
    QString filename_second = "";

};

#endif // MAINWINDOW_H
