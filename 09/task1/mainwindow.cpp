#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    udpWorkerTimer = new UDPworker(this);
    udpWorkerTimer->InitSocket(QHostAddress::LocalHost, SEND_TO_PORT1, LISTEN_PORT1);
    udpWorkerMessage = new UDPworker(this);
    udpWorkerMessage->InitSocket(QHostAddress::LocalHost, SEND_TO_PORT2, LISTEN_PORT2);

    connect(udpWorkerTimer, &UDPworker::sig_MessageReceived, this, &MainWindow::displayReceivedDateTimeData);
    connect(udpWorkerMessage, &UDPworker::sig_MessageReceived, this, &MainWindow::displayReceivedMessageData);
    connect(ui->pb_Send, &QPushButton::clicked, this, &MainWindow::sendMessage);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [&]{

        QDateTime dateTime = QDateTime::currentDateTime();

        QByteArray dataToSend;
        QDataStream outStr(&dataToSend, QIODevice::WriteOnly);

        outStr << dateTime;

        udpWorkerTimer->SendDatagram(dataToSend);
        timer->start(TIMER_DELAY);

    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::displayReceivedDateTimeData(QNetworkDatagram datagram)
{
    clearIfNeeded();
    QByteArray data;
    data = datagram.data();
    QDataStream inStr(&data, QIODevice::ReadOnly);
    QDateTime dateTime;
    inStr >> dateTime;
    ui->te_result->append("Текущее время: " + dateTime.toString() + ". "
                                                                    "Принято пакетов " + QString::number(counterPck));
}

void MainWindow::displayReceivedMessageData(QNetworkDatagram datagram)
{
    clearIfNeeded();
    auto str = QString("Принято сообщение от %1:%2, размер сообщения(байт) %3")
            .arg(datagram.senderAddress().toString())
            .arg(datagram.senderPort())
            .arg(datagram.data().size());
             ui->te_result->append(str);
}

void MainWindow::sendMessage()
{
    QByteArray dataToSend;
    QDataStream outStr(&dataToSend, QIODevice::WriteOnly);
    outStr << ui->le_TextToSend->text();
    udpWorkerMessage->SendDatagram(dataToSend);
}

void MainWindow::clearIfNeeded()
{
    counterPck++;
    if(counterPck % 20 == 0){
        ui->te_result->clear();
    }
}

void MainWindow::on_pb_Start_clicked()
{
    timer->start(TIMER_DELAY);
}

void MainWindow::on_pb_Stop_clicked()
{
    timer->stop();
}

