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
        if (serverSocket->write(data) == -1) {
            qDebug() << "Failed to send message.";
        }
        serverSocket->flush();
    } else {
        qDebug() << "Not connected to server. Message not sent.";
    }
}

void Client::connectToServer() {
    if (!serverAddress.isEmpty() && serverPort != 0) {
        serverSocket->connectToHost(serverAddress, serverPort);

        if (!serverSocket->waitForConnected()) {
            qDebug() << "Failed to connect to server. Error code: " << serverSocket->error();
        }
    } else {
        qDebug() << "Server address or port isn't set.";
    }
}

void Client::disconnectFromServer() {  serverSocket->disconnectFromHost(); }

void Client::connected() { qDebug() << "Connected to server."; }

void Client::disconnected() { qDebug() << "Disconnected from server."; }

void Client::readyRead() {
    QByteArray data = serverSocket->readAll();
    qDebug() << "Received data: " << data;
}

void Client::setServerAddress(const QString &address) { serverAddress = address; }

void Client::setServerPort(quint16 port) { serverPort = port; }
