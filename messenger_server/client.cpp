#include "client.h"

Client::Client(QWebSocket *socket, QObject *parent) : QObject(parent), socket(socket)
{
    connect(socket, &QWebSocket::textMessageReceived, this, [this](const QString &message) {
            emit messageReceived(message);
        });

        connect(socket, &QWebSocket::disconnected, this, [this]() {
            emit disconnected();
        });
}

void Client::sendMessage(const QString &msg) { socket->sendTextMessage(msg); }

void Client::disconnectFromServer()
{
    if (socket) {
        socket->close();
    }
}
