#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "provide.h"


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


private slots:
       void on_botton_clicked();
private:
    Ui::MainWindow *ui;

    parkingsystem mysystem;//ui与业务沟通的桥梁
    void initialtable();
    void refreshtable();


};
#endif // MAINWINDOW_H
