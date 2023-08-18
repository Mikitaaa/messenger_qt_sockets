#include "client.h"

Client::Client(QObject *parent)
    : QObject(parent), cllientSocket(new QWebSocket()) {

    serverPort = 0;
    serverAddress = "null";

    connect(cllientSocket, &QWebSocket::connected, this, &Client::connected);
    connect(cllientSocket, &QWebSocket::disconnected, this, &Client::disconnected);
    connect(cllientSocket, &QWebSocket::textMessageReceived, this, &Client::textMessageReceived);
}

void Client::sendMessage(const QString &message) {
    if (cllientSocket->state() == QAbstractSocket::ConnectedState) {
        cllientSocket->sendTextMessage(message);
    } else {
        emit ThrowActionResult("Not connected to server. Message not sent.");
    }
}

void Client::connectToServer() {
    if (!serverAddress.isEmpty() && serverPort != 0) {
        QString url = QString("ws://%1:%2").arg(serverAddress).arg(serverPort);
       // webSocket->setProperty(QAbstractSocket::QtSocketVerboseDebug, true);

        cllientSocket->open(QUrl(url));
    } else {
        emit ThrowActionResult("Server address or port isn't set.");
    }
}

bool Client::isConnected() const {
    return cllientSocket->state() == QAbstractSocket::ConnectedState;
}

void Client::textMessageReceived(const QString &message) { emit ThrowMessageFromServer(message); }

void Client::disconnectFromServer() { cllientSocket->close(); }

void Client::connected() {
    emit ThrowActionResult("Connected to server.");
    emit connectedSignal();
}

void Client::disconnected() {
    emit ThrowActionResult("Disconnected from server.");
    emit disconnectedSignal();
}

void Client::setServerAddress(const QString &address) { serverAddress = address; }

void Client::setServerPort(quint16 port) { serverPort = port; }
