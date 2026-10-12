#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include <QTableWidgetItem>
#include "provide.h"
#include <QHeaderView>
#include <QPushButton>
#include <QLayoutItem>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mysystem.generatemockdata();//debug假数据
    initialtable();//8888888888
    //同样对mysystem处理
    //apparently connect;
    initialgridlayout();
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

    ui->tabWidget->setTabText(0, QString("Parking Tab"));
    ui->tabWidget->setTabText(1, QString("Waiting Tab"));
    ui->tabWidget->setTabText(2, QString("History Tab"));
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
        ui->table_parking->setItem(table_row, 1, new QTableWidgetItem(QString::number(car.spotposition)));
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
    refreshgridlayout();
}
void MainWindow::spotbtnclick(int spotposition) {
    for (const auto& car : mysystem.getactivecar()) {
        if (car.spotposition == spotposition) {
            int cost = mysystem.anticipatesettle(car.intime, QDateTime());
            QMessageBox::information(this, "车位详情",
                                     QString("车位：%1\n车牌：%2\n入场：%3\n预估费用：%4 元")
                                         .arg(spotposition + 1)
                                         .arg(car.plate)
                                         .arg(car.intime.toString("yyyy-MM-dd HH:mm:ss"))
                                         .arg(cost));
            ui->plate->setText(car.plate);
            return;
        }
    }
    QMessageBox::information(this, "车位详情", "该车位空闲");
    //ui->messege->setText(QString("spotposition %1").arg(spotposition + 1));
}
void MainWindow::initialgridlayout() {
    int parknum = mysystem.parkcfg.cfg_parknum();
    int cols = 5;
    QLayoutItem* item;
    while((item = ui->gridLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }
    m_spotButtons.clear();

    for(int i = 0; i < parknum; i++) {
        QPushButton* btn = new QPushButton(QString("车位 %1").arg(i + 1), this);
        btn->setFixedSize(80, 60);

        int row = i / cols;
        int col = i % cols;
        ui->gridLayout->addWidget(btn, row, col);
        connect(btn, &QPushButton::clicked, this, [this, i](){
            spotbtnclick(i);
        });
        m_spotButtons.append(btn);
    }
}

void MainWindow::refreshgridlayout() {
    QHash<int, parkingsystem::Car> spotmap;
    for (const auto& car : mysystem.getactivecar()) {
        if(car.spotposition >= 0) {
            spotmap[car.spotposition] = car;
        }
    }
    for (int i = 0; i < m_spotButtons.size(); i++) {
        QPushButton* btn = m_spotButtons[i];
        if (spotmap.contains(i)) {
            btn->setText(spotmap[i].plate);
            btn->setStyleSheet("background-color: #d4f4dd; color: #1e7e34;");
        } else {
            btn->setText(QString("车位 %1").arg(i + 1));
            btn->setStyleSheet("background-color: #f8d7da; color: #721c24");
        }
    }
}

