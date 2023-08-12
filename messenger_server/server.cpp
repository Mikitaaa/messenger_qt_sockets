#include "server.h"
#include <QDebug>

Server::Server(QObject *parent) : QObject(parent)
{
    server = new QTcpServer(this);
    connect(server, &QTcpServer::newConnection, this, &Server::handleNewConnection);
}

void Server::start()
{
    if (!server->listen(QHostAddress::Any, SERVER_PORT)) {
        qDebug() << "Server could not start!";
        qDebug() << "Error: " << server->errorString();
    } else {
        qDebug() << "Server started.";
    }
}

void Server::handleNewConnection()
{
    QTcpSocket *clientSocket = server->nextPendingConnection();
    if (!clientSocket) {
            qDebug() << "Error: Unable to get client connection.";
            return;
        }

    clients.append(clientSocket);
    connect(clientSocket, &QTcpSocket::readyRead, this, &Server::readMessage);
    qDebug() << "New client connected.";
}

void Server::readMessage()
{
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket*>(sender());
    if (!clientSocket) { return; }


    QString message = clientSocket->readAll();
        if (message.isEmpty()) {
            qDebug() << "Received empty message.";
        } else {
            qDebug() << "Received: " << message;
        }
}
