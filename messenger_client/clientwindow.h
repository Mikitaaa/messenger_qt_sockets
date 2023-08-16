#ifndef CLIENTWINDOW_H
#define CLIENTWINDOW_H

#include <QMainWindow>
#include "client.h"

class QLineEdit;
class QPushButton;

class ClientWindow : public QMainWindow
{
    Q_OBJECT

public:
    ClientWindow(QWidget *parent = nullptr);
    ~ClientWindow();

private slots:
    void sendMessage();
    void displayError(const QString &errorMessage);
    void displayMessage(const QString &Message);

private:
    Client *client;
    QLineEdit *messageLineEdit;
    QPushButton *sendButton;
    QTextEdit *chatTextEdit;
    QLabel *errorLabel;
};
#endif // CLIENTWINDOW_H
