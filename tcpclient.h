#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include "rtdb/include/world_model/robot.h"

class TCPclient : public QObject
{
    Q_OBJECT
public:
    explicit TCPclient(QObject *parent = nullptr);
    bool Connect();
    bool Disconnect();
    void ReadData();
    void Config(QString ip, int port);
    bool isReadable();
    bool isBufferEmpty();
    QByteArray getBuffer();
    QString getip();
    int getport();
private:
    QTcpSocket* Client;
    QString ip;
    int port;
    QByteArray buffer;
signals:
    void RBinfoupdated();
};

#endif // TCPCLIENT_H
