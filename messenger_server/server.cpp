#include "server.h"
#include <QNetworkInterface>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>

Server::Server(QObject *parent) : QObject(parent) {
    actionHandlers = {
            {"message", &Server::handleMessage}
        };

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

    connect(client, &Client::messageReceived, this, &Server::handleAction);
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

void Server::handleAction(QString message) {
    Client *client = qobject_cast<Client*>(sender());
    if (!client) { return; }

    QJsonDocument jsonDocument = QJsonDocument::fromJson(message.toUtf8());
        if (jsonDocument.isNull()) {  return; }

    QJsonObject jsonObject = jsonDocument.object();
    QString action = jsonObject.value("action").toString();


    auto it = actionHandlers.find(action);
        if (it != actionHandlers.end()) {
            (this->*(it->second))(client, jsonObject);
        } else {
            // Обработка неизвестного действия пока что нету такого
        }
}

void Server::handleMessage(Client * client, const QJsonObject &jsonObject) {
    QString content = jsonObject.value("content").toString();
    QString clientMessage = QString("Client %1: %2").arg(clients.indexOf(client)).arg(content);
    sendMessageToAll(clientMessage);
}

void Server::sendMessageToAll(const QString &msg) {
    for (Client *client : clients) {
        client->sendMessage(msg);
    }
}
