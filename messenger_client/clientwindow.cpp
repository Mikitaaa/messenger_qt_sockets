#include "clientwindow.h"

ClientWindow::ClientWindow(QWidget *parent)
    : QMainWindow(parent), client(new Client(this)) {
    messageLineEdit = new QLineEdit(this);
    sendButton = new QPushButton("Send", this);
    chatTextEdit = new QTextEdit(this);
    chatTextEdit->setTextInteractionFlags(Qt::NoTextInteraction);
    chatTextEdit->setCursor(Qt::ArrowCursor);

    QApplication::setOverrideCursor(Qt::ArrowCursor);
    QCoreApplication::processEvents();
    errorLabel = new QLabel(this);

    connect(sendButton, &QPushButton::clicked, this, &ClientWindow::sendMessage);

    QGridLayout *layout = new QGridLayout;
        layout->addWidget(chatTextEdit, 1, 1, 1, 3);
        layout->addWidget(messageLineEdit, 2, 1, 1, 2);
        layout->addWidget(sendButton, 2, 3, 1, 1);
        layout->addWidget(errorLabel, 3, 1, 3, 1);

    QWidget *centralWidget = new QWidget;
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);

    client->setServerPort(5000);
    client->setServerAddress("192.168.0.110");
    client->connectToServer();
}

ClientWindow::~ClientWindow() { delete client; }

void ClientWindow::sendMessage() {
    QString message = messageLineEdit->text();
    client->sendMessage(message);
    messageLineEdit->clear();
}
