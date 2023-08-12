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

public slots:
    void start();

private slots:
    void handleNewConnection();
    void readMessage();

private:
    QTcpServer *server;
    QList<QTcpSocket*> clients;

    static const int SERVER_PORT = 5000;
};

#endif // SERVER_H
