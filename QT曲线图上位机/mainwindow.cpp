#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qcustomplot.h"
#include <QTcpServer>
#include <QTcpSocket>
void sock(void);
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    sock();
    ui->setupUi(this);
    QVector<double> x(101), y(101); // initialize with entries 0..100
    for (int i=0; i<101; ++i)
    {
      x[i] = i/50.0 - 1; // x goes from -1 to 1
      y[i] = x[i]*x[i];  // let's plot a quadratic function
    }
    QTcpServer *tcpServer;
    QTcpSocket *tcpSocket;
    ui->widget->setInteractions(QCP::iRangeDrag);
    QBrush qbrush(QColor(255,255,255));
    ui->widget->xAxis->setLabel("Time Axis(s)");
    ui->widget->yAxis->setLabel("speed");
    ui->widget->xAxis->setRange(-10, 10);
    ui->widget->yAxis->setRange(-5, 21);

    ui->widget->addGraph();
    ui->widget->graph(0)->setData(x,y);
    ui->widget->graph(0)->addData(11,-1);

    tcpServer=new QTcpServer(this);
    tcpServer->listen(QHostAddress::Any,8888);
    connect(tcpServer,&QTcpServer::newConnection,
            [=]()
            {
                tcpSocket=tcpServer->nextPendingConnection();
                QString ip=tcpSocket->peerAddress().toString();
                int port=tcpSocket->peerPort();
                QString temp=QString("[%1:%2]：成功连接").arg(ip).arg(port);
                ui->textbox->setText(temp);
                connect(tcpSocket,&QTcpSocket::readyRead,
                        [=]()

                            {
                                QByteArray arry= tcpSocket->readAll();
                                ui->textbox->append(arry);
                            }
                );
            }
    );

}
void sock(void)
{


}
MainWindow::~MainWindow()
{
    delete ui;
}

//void MainWindow::on_pushButton_clicked()
//{
//    QString date=ui->spinBox->text();
//    tcpSocket->write(date.toUtf8().data());
//}

void MainWindow::on_speedSend_clicked()
{
    QString date="s"+ui->spinBox->text();
    tcpSocket->write(date.toUtf8().data());
}


void MainWindow::on_posSend_clicked()
{
    QString date="p"+ui->spinBox_5->text();
    tcpSocket->write(date.toUtf8().data());
}

void MainWindow::on_deltaSpeendSend_clicked()
{
    QString date="ds"+ui->spinBox_6->text();
    tcpSocket->write(date.toUtf8().data());
}

void MainWindow::on_deltaPosSend_clicked()
{
    QString date="dp"+ui->spinBox_7->text();
    tcpSocket->write(date.toUtf8().data());
}
