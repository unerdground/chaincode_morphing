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

QString MainWindow::editDistance(QVector<int> source, QVector<int> target){
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
    return presc.data();
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

void MainWindow::morphing(QString presc, imageHandler source_img, QVector<int> target){
   QVector<int> temp_presc;
   QVector<int> source = source_img.getChaincode();
    for (int i = 0; i < presc.size(); i++){
        if (presc[i] != 'M'){
            temp_presc.push_back(i);
        }
    }
    int j = 0;
    for (int i = 1; i < iteration + 1; i++){
        for (int k = (i - 1)*temp_presc.size()/iteration; k < (i*temp_presc.size()/iteration); k++){

            if (presc[temp_presc[k]] == 'R'){
                source[temp_presc[k] - j] = target[temp_presc[k] - j];
            } else if (presc[temp_presc[k]] == 'I'){
                source.insert(temp_presc[k] - j, target[temp_presc[k] - j]);
            } else if (presc[temp_presc[k]] == 'D'){
                source.remove(temp_presc[k] - j, 1);
                j++;
            }
        }
        source_img.setChaincode(source);
        source_img.writeToImageFromString("out_" + QString::number(i) + ".png");
    }
}

void MainWindow::delay(int ms){
    QTime dieTime = QTime::currentTime().addMSecs( ms );
        while( QTime::currentTime() < dieTime )
        {
            QCoreApplication::processEvents( QEventLoop::AllEvents, 100 );
        }
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

        first.writeToImageFromString("out.png");

        //QString filename = filename_first;
        //filename.resize(filename.size() - 4);
       //filename += "_out.png";


        // TEST SECTION


        //ui->log->setText(editDistance(first.getChaincode(), second.getChaincode()));
        ui->log->setText(editDistance(first.getChaincode(), second.getChaincode()) + "\n" + first.writeChaincode() + "\n" + second.writeChaincode());
        morphing(editDistance(first.getChaincode(), second.getChaincode()), first, second.getChaincode());

        //first.setChaincode(buidCodeByPrescription(editDistance(first.getChaincode(), second.getChaincode()), first.getChaincode(), second.getChaincode()));
        //  END OF TEST SECTION

        ui->result_button->setEnabled(1);

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
    ui->result_button->setEnabled(0);
}

void MainWindow::on_result_button_clicked()
{
    QPixmap map("out.png");
    ui->result_img_box->setPixmap(map);
    delay(130);
    //first.writeToImageFromString("out.png");
    for (int i = 1; i <= iteration; i++){
        QPixmap map("out_" + QString::number(i) + ".png");
        ui->result_img_box->setPixmap(map);
        delay(130);
    }
    if (mode == 1){
        for (int i = iteration; i > 0; i--){
            QPixmap map("out_" + QString::number(i) + ".png");
            ui->result_img_box->setPixmap(map);
            delay(130);
        }
        QPixmap map("out.png");
        ui->result_img_box->setPixmap(map);
        delay(130);
    }

}
