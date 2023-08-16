#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = nullptr);
    void sendMessageToAll(QString msg);

public slots:
    void start();
    void stop();

private slots:
    void handleNewConnection();
    void handleClientDisconnection();
    void readMessage();

private:
    QTcpServer *server;
    QList<QTcpSocket*> clients;

    static const int SERVER_PORT = 5000;

signals:
    void ThrowlogMessage(const QString &message);
};

#endif // SERVER_H
