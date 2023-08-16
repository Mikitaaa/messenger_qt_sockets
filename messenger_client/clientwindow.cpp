#include "clientwindow.h"

ClientWindow::ClientWindow(QWidget *parent)
    : QMainWindow(parent), client(new Client(this)) {
    resize(600, 600);

    connect(client, &Client::ThrowActionResult, this, &ClientWindow::displayError);
    connect(client, &Client::ThrowMessageFromServer, this, &ClientWindow::displayMessage);

    messageLineEdit = new QLineEdit(this);
    sendButton = new QPushButton("Send", this);

    chatTextEdit = new QTextEdit(this);
    chatTextEdit->setTextInteractionFlags(Qt::NoTextInteraction);
    chatTextEdit->setCursor(Qt::ArrowCursor);

    ipLineEdit = new QLineEdit(this);
    portLineEdit = new QLineEdit(this);
    connectButton = new QPushButton("Connect", this);
    disconnectButton = new QPushButton("Disconnect", this);

    chatTextEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    ipLineEdit->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
    portLineEdit->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
    sendButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
    connectButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
    disconnectButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);

    connect(sendButton, &QPushButton::clicked, this, &ClientWindow::sendMessage);
    connect(connectButton, &QPushButton::clicked, this, &ClientWindow::connectToServer);
    connect(disconnectButton, &QPushButton::clicked, this, &ClientWindow::disconnectFromServer);

    QApplication::setOverrideCursor(Qt::ArrowCursor);
    QCoreApplication::processEvents();
    errorLabel = new QLabel(this);

    QGridLayout *layout = new QGridLayout;
        layout->addWidget(chatTextEdit, 1, 1, 1, 5);
        layout->addWidget(messageLineEdit, 2, 1, 1, 4);
        layout->addWidget(sendButton, 2, 5, 1, 1);

        layout->addWidget(portLineEdit, 3, 1, 1, 1);
        layout->addWidget(ipLineEdit, 3, 2, 1, 2);
        layout->addWidget(connectButton, 4, 1, 1, 1);
        layout->addWidget(disconnectButton, 4, 2, 1, 1);

        layout->addWidget(errorLabel, 5, 1, 1, 4);

    QWidget *centralWidget = new QWidget;
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);

    ipLineEdit->setText("192.168.0.110");
    portLineEdit->setText("5000");
    updateUIState();
}

void ClientWindow::updateUIState() {
    bool connected = client->isConnected();
    messageLineEdit->setEnabled(connected);
    sendButton->setEnabled(connected);
    ipLineEdit->setEnabled(!connected);
    portLineEdit->setEnabled(!connected);
    connectButton->setEnabled(!connected);
    disconnectButton->setEnabled(connected);
}

void ClientWindow::connectToServer() {
    QString ip = ipLineEdit->text();
    quint16 port = portLineEdit->text().toInt();
    client->setServerAddress(ip);
    client->setServerPort(port);
    client->connectToServer();
    updateUIState();
}

void ClientWindow::disconnectFromServer() {
    client->disconnectFromServer();
    updateUIState();
}

void ClientWindow::sendMessage() {
    QString message = messageLineEdit->text();
    client->sendMessage(message);
    messageLineEdit->clear();
}

ClientWindow::~ClientWindow() { delete client; }

void ClientWindow::displayError(const QString &errorMessage) {
    errorLabel->setText(errorMessage);
}

void ClientWindow::displayMessage(const QString &Message) {
    chatTextEdit->append(Message + "\n");
}

