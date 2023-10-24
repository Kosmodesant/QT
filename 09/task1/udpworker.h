#ifndef UDPWORKER_H
#define UDPWORKER_H

#include <QUdpSocket>
#include <QNetworkDatagram>
#include <QDateTime>

class UDPworker : public QObject
{
    Q_OBJECT
public:
    explicit UDPworker(QObject *parent = nullptr);
    void InitSocket(QHostAddress sendToHost, int sendToPort, int listenToPort);
    void SendDatagram(const QByteArray& data);

private slots:
    void readPendingDatagrams(void);

private:
    QUdpSocket* serviceUdpSocket;
    QHostAddress sendToHost;
    int sendToPort;
    int listenToPort;
signals:
    void sig_MessageReceived(QNetworkDatagram datagram);
};

#endif // UDPWORKER_H