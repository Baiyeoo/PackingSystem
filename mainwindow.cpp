#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include <QTableWidgetItem>
#include "provide.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initialtable();//8888888888
    //同样对mysystem处理
    //apparently connect;
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::initialtable() {

}
void MainWindow::refreshtable() {

}
void MainWindow::on_botton_clicked() {
    QString plate = ui->plate->text();
    QString combobox = ui->combobox->currentText();
    //ui->messege->setText(plate);
    //ui->messege->setText(combobox);
    //mysystem;
    if (combobox == "入库") {
        //ui->messege->setText(combobox);
        mysystem.carin(plate) ?
            ui->messege->setText("success carin") : ui->messege->setText("fail carin");

    } else {
        mysystem.carout(plate) ?
            ui->messege->setText("success carout") : ui->messege->setText("fail carout");
    }

    refreshtable();
}

void initialtable() {

}
void refreshtable() {

}