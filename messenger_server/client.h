#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QWebSocket>
#include <QString>

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QWebSocket *socket, QObject *parent = nullptr);

    void sendMessage(const QString &msg);
    void disconnectFromServer();

signals:
    void messageReceived(const QString &message);
    void disconnected();

private:
    QWebSocket *socket;
};

#endif // CLIENT_H
