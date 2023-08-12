#ifndef SERVERWINDOW_H
#define SERVERWINDOW_H

#include <QMainWindow>

class ServerWindow : public QMainWindow
{
    Q_OBJECT

public:
    ServerWindow(QWidget *parent = nullptr);
    ~ServerWindow();
};
#endif // SERVERWINDOW_H
