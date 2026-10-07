#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include <QTableWidgetItem>
#include "provide.h"
#include <QHeaderView>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mysystem.generatemockdata();//debug假数据
    initialtable();//8888888888
    //同样对mysystem处理
    //apparently connect;
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::initialtable() {
    ui->table_parking->setRowCount(mysystem.parkcfg.cfg_parknum());//888888888
    ui->table_parking->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->table_deque->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->table_history->setEditTriggers(QAbstractItemView::NoEditTriggers);//禁止修改

    ui->table_parking->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->table_deque->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->table_history->setSelectionBehavior(QAbstractItemView::SelectRows);

    ui->table_parking->setAlternatingRowColors(true);
    ui->table_deque->setAlternatingRowColors(true);
    ui->table_history->setAlternatingRowColors(true);

    //ui->table_parking->verticalHeader()->setVisible(false);
    ui->table_parking->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->table_deque->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->table_history->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);//自动stretch

    refreshtable();
}
void MainWindow::refreshtable() {
    auto activecar = mysystem.getactivecar();
    auto waitingcar = mysystem.getwaitingcar();
    auto historycar = mysystem.gethistorycar();

    ui->table_parking->setRowCount(0);
    ui->table_parking->setRowCount(mysystem.parkcfg.cfg_parknum());
    ui->table_deque->setRowCount(0);
    ui->table_deque->setRowCount(waitingcar.size());
    ui->table_history->setRowCount(0);
    ui->table_history->setRowCount(historycar.size());

    int table_row = 0;
    for(auto car: activecar) {
        ui->table_parking->setItem(table_row, 0, new QTableWidgetItem(car.plate));
        ui->table_parking->setItem(table_row, 1, new QTableWidgetItem(QString::number(table_row)));
        //ui->table_parking->setItem(table_row, 1, new QTableWidgetItem(car.carplatenumber));
        ui->table_parking->setItem(table_row, 2, new QTableWidgetItem(car.intime.toString("yyyy-MM-dd HH-mm-ss")));
        ui->table_parking->setItem(table_row, 3, new QTableWidgetItem(QString::number(mysystem.anticipatesettle(car.intime, car.outtime))));
        table_row++;
    }
    table_row = 0;
    for(auto car: waitingcar) {
        ui->table_deque->setItem(table_row, 0, new QTableWidgetItem(car.plate));
        ui->table_deque->setItem(table_row, 1, new QTableWidgetItem(QString::number(car.dequenumber)));
        ui->table_deque->setItem(table_row, 2, new QTableWidgetItem(car.indequetime.toString("yyyy-MM-dd HH-mm-ss")));
        table_row++;
    }
    table_row = 0;
    for(auto car: historycar) {
        ui->table_history->setItem(table_row, 0, new QTableWidgetItem(car.plate));
        ui->table_history->setItem(table_row, 1, new QTableWidgetItem(car.intime.toString("yyyy-MM-dd HH-mm-ss")));
        ui->table_history->setItem(table_row, 2, new QTableWidgetItem(car.outtime.toString("yyyy-MM-dd HH-mm-ss")));
        ui->table_history->setItem(table_row, 3, new QTableWidgetItem(QString::number(mysystem.anticipatesettle(car.intime, car.outtime))));
        table_row++;
    }
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
            ui->messege->setText("success carin") : ui->messege->setText("invalid repeat");

    } else {
        mysystem.carout(plate) ?
            ui->messege->setText("success carout") : ui->messege->setText("invalid plate");
    }

    refreshtable();
}
