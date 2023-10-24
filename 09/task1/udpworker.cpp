#include "udpworker.h"

UDPworker::UDPworker(QObject *parent) : QObject(parent){}

/*!
 * @brief Метод инициализирует UDP сервер
 */
void UDPworker::InitSocket(QHostAddress sendHost, int sendPort, int listenPort)
{
    sendToHost = sendHost;
    sendToPort = sendPort;

    serviceUdpSocket = new QUdpSocket(this);
    /*
     * Соединяем присваиваем адрес и порт серверу и соединяем функцию
     * обраотчик принятых пакетов с сокетом
     */
    serviceUdpSocket->bind(QHostAddress::LocalHost, listenPort);

    connect(serviceUdpSocket, &QUdpSocket::readyRead, this, &UDPworker::readPendingDatagrams);
}

/*!
 * @brief Метод осуществляет опередачу датаграммы
 */
void UDPworker::SendDatagram(const QByteArray& data)
{
    /*
     *  Отправляем данные
     */
    serviceUdpSocket->writeDatagram(data, sendToHost, sendToPort);
}

/*!
 * @brief Метод осуществляет чтение датаграм из сокета
 */
void UDPworker::readPendingDatagrams( void )
{
    /*
     *  Производим чтение принятых датаграмм
     */
    while(serviceUdpSocket->hasPendingDatagrams()){
            QNetworkDatagram datagram = serviceUdpSocket->receiveDatagram();
            emit sig_MessageReceived(datagram);
    }
}
