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

int MainWindow::min_of_three(int first, int second, int third){
    if (first < second){
        if (first < third) return first;
        else return third;
    } else if (second < third) return second;
    else return third;
}

QPair<QString, int> MainWindow::editDistance(QVector<int> source, QVector<int> target){
    int** matrix = new int*[source.size() + 1];
    char** edit = new char*[source.size() + 1];
    for (int i = 0; i < source.size() + 1; i++){
        matrix[i] = new int[target.size() + 1];
        edit[i] = new char[target.size() + 1];
    }

    matrix[0][0] = 0;
    edit[0][0] = 'M';
    for (int iterator = 1; iterator <= target.size(); iterator++){
        matrix[0][iterator] = iterator;
        edit[0][iterator] = 'I';
    }
    for (int iterator = 1; iterator <= source.size(); iterator++){
        matrix[iterator][0] = iterator;
        edit[iterator][0] = 'D';
        for (int counter = 1; counter <= target.size(); counter++){
            if (source[iterator - 1] != target[counter - 1]){
            //matrix[iterator][counter] = min_of_three(matrix[iterator - 1][counter], matrix[iterator][counter - 1], matrix[iterator - 1][counter - 1]) + 1;
            if (matrix[iterator][counter - 1] == min_of_three(matrix[iterator - 1][counter], matrix[iterator][counter - 1], matrix[iterator - 1][counter - 1])){
                matrix[iterator][counter] = matrix[iterator][counter - 1] + 1;
                edit[iterator][counter] = 'I';
            }
            else if (matrix[iterator - 1][counter] == min_of_three(matrix[iterator - 1][counter], matrix[iterator][counter - 1], matrix[iterator - 1][counter - 1])){
                matrix[iterator][counter] = matrix[iterator - 1][counter] + 1;
                edit[iterator][counter] = 'D';
            }
            else if (matrix[iterator - 1][counter - 1] == min_of_three(matrix[iterator - 1][counter], matrix[iterator][counter - 1], matrix[iterator - 1][counter - 1])){
                matrix[iterator][counter] = matrix[iterator - 1][counter - 1] + 1;
                edit[iterator][counter] = 'R';
                }
            }
            else
            {
                matrix[iterator][counter] = matrix[iterator - 1][counter - 1];
                edit[iterator][counter] = 'M';
            }
        }
    }

    if (matrix[source.size()][target.size()] == 0) throw equalChainCode();
    else{

    QVector<char> presc;
        int iterator = source.size(), counter = target.size();

        while ((iterator != 0) || (counter != 0)){
            char tmp = edit[iterator][counter];
            presc.push_back(tmp);
            if (tmp == 'R' || tmp == 'M'){
                iterator--; counter--;
            }
            else if (tmp == 'D'){
                iterator--;
            } else counter--;
        }
    std::reverse(presc.begin(), presc.end());

    delete[] matrix;
    delete[] edit;
    return QPair<QString,int>(presc.data(), matrix[source.size()][target.size()]);
    }
}

QVector<int> MainWindow::buidCodeByPrescription(QString presc, QVector<int> source, QVector<int> target){
    int j = 0;
    for (int i = 0; i < presc.size(); i++){
        if (presc[i] == 'R'){
            source[i - j] = target[i - j];
        } else if (presc[i] == 'I'){
            source.insert(i - j, target[i - j]);
        } else if (presc[i] == 'D'){
            source.remove(i - j, 1);
            j++;
        }
    }
    return source;
}

void MainWindow::morphing(int dist, QString presc, QString first, QString second){

}

void MainWindow::on_pushButton_clicked()
{
    // MAIN ACTION
    try{
        if (filename_first == filename_second) throw equalChainCode();
        imageHandler first(filename_first);
        imageHandler second(filename_second);

        first.readStringFromImage();
        second.readStringFromImage();

        QString filename = filename_first;
        //filename.resize(filename.size() - 4);
       //filename += "_out.png";


        // TEST SECTION


        //ui->log->setText(editDistance(first.getChaincode(), second.getChaincode()));
        ui->log->setText(editDistance(first.getChaincode(), second.getChaincode()).first + "\n" + first.writeChaincode() + "\n" + second.writeChaincode());

        first.setChaincode(buidCodeByPrescription(editDistance(first.getChaincode(), second.getChaincode()).first, first.getChaincode(), second.getChaincode()));
        //  END OF TEST SECTION

        first.writeToImageFromString("out.png");
        QPixmap map("out.png");
        ui->result_img_box->setPixmap(map);
        ui->statusBar->showMessage("Done");



    } catch (QException& e){
        ui->log->setText(e.what());
    }
}

void MainWindow::on_button_first_img_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Select Image"), "" ,tr("Images (*.png *.bmp)"));
    if (not filename.isEmpty()){
    ui->statusBar->showMessage(filename + " loaded");
    ui->origin_img_box->setPixmap(QPixmap(filename));
    filename_first = filename;
    if (filename_second != "") ui->pushButton->setEnabled(1);
    }
}

void MainWindow::on_button_second_img_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Select Image"), "" ,tr("Images (*.png *.bmp)"));
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
