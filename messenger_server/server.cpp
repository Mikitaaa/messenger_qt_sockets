#include "server.h"
#include <QDebug>
#include <QNetworkInterface>

Server::Server(QObject *parent) : QObject(parent) {
    server = new QTcpServer(this);
    connect(server, &QTcpServer::newConnection, this, &Server::handleNewConnection);
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
    if (!server->listen(QHostAddress::Any, SERVER_PORT)) {
        qDebug() << "Server could not start!";
        qDebug() << "Error: " << server->errorString();
    } else {
        qDebug() << "Server started.";
        qDebug() << "Server IP Address: " << getLocalIpAddress();
    }
}

void Server::handleNewConnection() {
    QTcpSocket *clientSocket = server->nextPendingConnection();
    if (!clientSocket) {
            qDebug() << "Error: Unable to get client connection.";
            return;
        }

    clients.append(clientSocket);
    connect(clientSocket, &QTcpSocket::readyRead, this, &Server::readMessage);
    connect(clientSocket, &QTcpSocket::disconnected, this, &Server::handleClientDisconnection);
    qDebug() << "New client connected.";
}

void Server::handleClientDisconnection() {
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket*>(sender());
    if (!clientSocket) { return; }

    clients.removeOne(clientSocket);
    qDebug() << "Client disconnected.";
    clientSocket->deleteLater();
}

void Server::readMessage() {
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket*>(sender());
    if (!clientSocket) { return; }


    QString message = clientSocket->readAll();
        if (message.isEmpty()) {
            qDebug() << "Received empty message.";
        } else {
            qDebug() << "Received: " << message;
            sendMessageToAll(message);
        }
}

void Server::sendMessageToAll(QString msg) {
    for (QTcpSocket *otherClient : clients) {
          otherClient->write(msg.toUtf8());
          otherClient->flush();
    }
}
