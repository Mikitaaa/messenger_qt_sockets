#ifndef CLIENT_H
#define CLIENT_H

#include <QtWidgets>
#include <QtNetwork>

class Client : public QObject {
    Q_OBJECT

public:
    Client(QObject *parent = nullptr);

    void setServerAddress(const QString &address);
    void setServerPort(quint16 port);

    void sendMessage(const QString &message);

    void connectToServer();
    void disconnectFromServer();
    bool isConnected() const;

private slots:
    void connected();
    void disconnected();
    void readyRead();

private:
    QTcpSocket *serverSocket;
    QString serverAddress;
    quint16 serverPort;

signals:
    void ThrowActionResult(const QString &errorMessage);
    void ThrowMessageFromServer(const QString &Message);

    void connectedToServer();
    void disconnectedFromServer();

};

#endif // CLIENT_H
