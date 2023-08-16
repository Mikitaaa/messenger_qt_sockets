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

    void connectToServer();
    void disconnectFromServer();

private:
    Client *client;
    QTextEdit *chatTextEdit;
    QLabel *errorLabel;

    QPushButton *sendButton;
    QPushButton *connectButton;
    QPushButton *disconnectButton;

    QLineEdit *ipLineEdit;
    QLineEdit *portLineEdit;
    QLineEdit *messageLineEdit;

    void updateUIState();

};
#endif // CLIENTWINDOW_H
