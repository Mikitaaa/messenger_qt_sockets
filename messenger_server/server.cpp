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
    for (Client *client : clients) {
            client->disconnectFromServer();
            client->deleteLater();
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

    Client *client = new Client(clientSocket, this);
    clients.append(client);

    connect(client, &Client::messageReceived, this, &Server::readMessage);
    connect(client, &Client::disconnected, this, &Server::handleClientDisconnection);
    emit ThrowlogMessage("New client connected.");
}

void Server::handleClientDisconnection() {
    Client *client = qobject_cast<Client*>(sender());
    if (!client) { return; }

    clients.removeOne(client);
    client->deleteLater();

    emit ThrowlogMessage("Client disconnected.");
}

void Server::readMessage(QString message) {
    Client *client = qobject_cast<Client*>(sender());
    if (!client) { return; }

    QString clientMessage = QString("Client %1: %2").arg(clients.indexOf(client)).arg(message);
    sendMessageToAll(clientMessage);
}

void Server::sendMessageToAll(const QString &msg) {
    for (Client *client : clients) {
        client->sendMessage(msg);
    }
}
