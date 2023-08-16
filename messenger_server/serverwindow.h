#ifndef SERVERWINDOW_H
#define SERVERWINDOW_H

#include <QMainWindow>
#include "server.h"

class QPushButton;
class QTextEdit;

class ServerWindow : public QMainWindow
{
    Q_OBJECT

public:
    ServerWindow(QWidget *parent = nullptr);
    ~ServerWindow();

private slots:
    void appendToLog(const QString &Message);

private:
    Server *server;

    QPushButton *startButton;
    QPushButton *stopButton;

    QTextEdit *Serverlog;
};
#endif // SERVERWINDOW_H
