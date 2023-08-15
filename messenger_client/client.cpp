#include "client.h"

Client::Client(QObject *parent)
    : QObject(parent), serverSocket(new QTcpSocket(this)) {

    serverPort = 5000;
    serverAddress = "";

    connect(serverSocket, &QTcpSocket::connected, this, &Client::connected);
    connect(serverSocket, &QTcpSocket::disconnected, this, &Client::disconnected);
    connect(serverSocket, &QTcpSocket::readyRead, this, &Client::readyRead);
}

void Client::sendMessage(const QString &message) {
    if (serverSocket->state() == QAbstractSocket::ConnectedState) {
        QByteArray data = message.toUtf8();
        serverSocket->write(data);
        serverSocket->flush();
    }
}

void Client::connectToServer() {  serverSocket->connectToHost(serverAddress, serverPort); }

void Client::disconnectFromServer() {  serverSocket->disconnectFromHost(); }

void Client::connected() { qDebug() << "Connected to server."; }

void Client::disconnected() { qDebug() << "Disconnected from server."; }

void Client::readyRead() {
    QByteArray data = serverSocket->readAll();
    qDebug() << "Received data: " << data;
}

void Client::setServerAddress(const QString &address) { serverAddress = address; }

void Client::setServerPort(quint16 port) { serverPort = port; }
