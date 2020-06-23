#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
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


    void on_speedSend_clicked();

    void on_posSend_clicked();

    void on_deltaSpeendSend_clicked();

    void on_deltaPosSend_clicked();

private:
    Ui::MainWindow *ui;


};

#endif // MAINWINDOW_H
