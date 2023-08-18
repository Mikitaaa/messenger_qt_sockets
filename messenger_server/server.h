#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QWebSocketServer>
#include <QWebSocket>
#include <QList>

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = nullptr);
    void sendMessageToAll(const QString &msg);

public slots:
    void start();
    void stop();

private slots:
    void handleNewConnection();
    void handleClientDisconnection();
    void readMessage(QString message);

private:
    QWebSocketServer *webSocketServer;
    QList<QWebSocket*> clients;

    static const int SERVER_PORT = 5000;

signals:
    void ThrowlogMessage(const QString &message);
};

#endif // SERVER_H
