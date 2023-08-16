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
            emit ThrowActionResult("Failed to send message.");
        }
        serverSocket->flush();
    } else {
        emit ThrowActionResult("Not connected to server. Message not sent.");
    }
}

void Client::connectToServer() {
    if (!serverAddress.isEmpty() && serverPort != 0) {
        serverSocket->connectToHost(serverAddress, serverPort);

        if (!serverSocket->waitForConnected()) {
            emit ThrowActionResult("Connection Failed.");
        }
    } else {
        emit ThrowActionResult("Server address or port isn't set.");
    }
}

void Client::disconnectFromServer() {  serverSocket->disconnectFromHost(); }

void Client::connected() { emit ThrowActionResult("Connected to server."); }

void Client::disconnected() { emit ThrowActionResult("Disconnected from server."); }

void Client::readyRead() {
    QByteArray data = serverSocket->readAll();
    emit ThrowMessageFromServer(QString(data));
}

void Client::setServerAddress(const QString &address) { serverAddress = address; }

void Client::setServerPort(quint16 port) { serverPort = port; }
