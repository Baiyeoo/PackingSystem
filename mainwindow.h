#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "provide.h"
#include <QVector>
#include <QPushButton>
#include <QCloseEvent>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;


protected:
    void closeEvent(QCloseEvent* event) override;
private slots:
       void on_botton_clicked();
    void spotbtnclick(int spotposition);
private:
    Ui::MainWindow *ui;

    parkingsystem mysystem;//ui与业务沟通的桥梁
    QVector<QPushButton*> m_spotButtons;
    QString m_datafile;

    void initialtable();
    void refreshtable();
    void initialgridlayout();
    void refreshgridlayout();


};
#endif // MAINWINDOW_H
