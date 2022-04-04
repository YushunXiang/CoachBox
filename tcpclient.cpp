#include "tcpclient.h"

TCPclient::TCPclient(QObject *parent) : QObject(parent)
{
    Client = new QTcpSocket(this);
    Client->abort();
    connect(Client,&QTcpSocket::readyRead,this,&TCPclient::ReadData);
    Config("255.255.255.255",0);
}
bool TCPclient::Connect()
{
    Client->connectToHost(ip,port);
    if(Client->waitForConnected(1000)) return true;
    else return false;
}
bool TCPclient::Disconnect()
{
    Client->disconnectFromHost();
    if (Client->state() == QAbstractSocket::UnconnectedState || Client->waitForDisconnected(1000))
    {
        return true;
    }
    else return false;
}
void TCPclient::ReadData()
{
    buffer = Client->readAll();
    emit RBinfoupdated();
}
void TCPclient::Config(QString ip, int port)
{
    this->ip=ip;
    this->port=port;
}
bool TCPclient::isReadable()
{
    return Client->isReadable();
}
bool TCPclient::isBufferEmpty()
{
    return buffer.isEmpty();
}
QByteArray TCPclient::getBuffer()
{
    return buffer;
}
QString TCPclient::getip()
{
    return ip;
}
int TCPclient::getport()
{
    return port;
}
