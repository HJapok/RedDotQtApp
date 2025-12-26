#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QTextEdit>
#include "vision.h"
#include "tcp.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onUpload();
    void onInspect();
    void onSend();

private:
    QLabel *imageLabel;
    QTextEdit *resultText;
    QPushButton *uploadBtn;
    QPushButton *inspectBtn;
    QPushButton *sendBtn;

    QString imagePath;
    Vision vision;
    TCPClient tcp;
};

#endif