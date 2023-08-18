#include "server.h"
#include <QNetworkInterface>

Server::Server(QObject *parent) : QObject(parent) {
    webSocketServer = new QWebSocketServer(QStringLiteral("WebSocket Server"), QWebSocketServer::NonSecureMode, this);
    connect(webSocketServer, &QWebSocketServer::newConnection, this, &Server::handleNewConnection);
}

QString getLocalIpAddress() {
    QString localIpAddress;
    foreach (const QNetworkInterface &interface, QNetworkInterface::allInterfaces()) {
        foreach (const QNetworkAddressEntry &entry, interface.addressEntries()) {
            if (entry.ip().protocol() == QAbstractSocket::IPv4Protocol && !entry.ip().isLoopback()) {
                localIpAddress = entry.ip().toString();
                return localIpAddress;
            }
        }
    }

    return localIpAddress;
}

void Server::start() {
    if (!webSocketServer->listen(QHostAddress::Any, SERVER_PORT)) {
        emit ThrowlogMessage("Error: Server could not start!");
    } else {
        emit ThrowlogMessage("Server started with IP: " + getLocalIpAddress());
    }
}

void Server::stop() {
    for (QWebSocket *clientSocket : clients) {
            clientSocket->close();
        }

    webSocketServer->close();
    emit ThrowlogMessage("Server stopped.");
}

void Server::handleNewConnection() {
    QWebSocket *clientSocket = webSocketServer->nextPendingConnection();
    if (!clientSocket) {
        emit ThrowlogMessage("Error: Unable to get client connection.");
        return;
    }

    clients.append(clientSocket);
    connect(clientSocket, &QWebSocket::textMessageReceived, this, &Server::readMessage);
    connect(clientSocket, &QWebSocket::disconnected, this, &Server::handleClientDisconnection);
    emit ThrowlogMessage("New client connected.");
}

void Server::handleClientDisconnection() {
    QWebSocket *clientSocket = qobject_cast<QWebSocket*>(sender());
    if (!clientSocket) { return; }

    clients.removeOne(clientSocket);
    emit ThrowlogMessage("Client disconnected.");
    clientSocket->deleteLater();
}

void Server::readMessage(QString message) {
    QWebSocket *clientSocket = qobject_cast<QWebSocket*>(sender());
    if (!clientSocket) { return; }

    sendMessageToAll(message);
}

void Server::sendMessageToAll(const QString &msg) {
    for (QWebSocket *otherClient : clients) {
        otherClient->sendTextMessage(msg);
    }
}
