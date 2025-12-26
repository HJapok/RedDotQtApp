#include "mainwindow.h"
#include <QVBoxLayout>
#include <QFileDialog>
#include <QPixmap>
#include <QImage>
#include <opencv2/opencv.hpp>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), tcp("127.0.0.1", 2000)
{
    QWidget *c = new QWidget(this);
    setCentralWidget(c);

    QVBoxLayout *layout = new QVBoxLayout(c);

    imageLabel = new QLabel();
    imageLabel->setFixedSize(400,300);
    resultText = new QTextEdit();
    resultText->setReadOnly(true);

    uploadBtn = new QPushButton("Upload Image");
    inspectBtn = new QPushButton("Inspect");
    sendBtn = new QPushButton("Send TCP");

    layout->addWidget(imageLabel);
    layout->addWidget(resultText);
    layout->addWidget(uploadBtn);
    layout->addWidget(inspectBtn);
    layout->addWidget(sendBtn);

    connect(uploadBtn, &QPushButton::clicked, this, &MainWindow::onUpload);
    connect(inspectBtn, &QPushButton::clicked, this, &MainWindow::onInspect);
    connect(sendBtn, &QPushButton::clicked, this, &MainWindow::onSend);
}

MainWindow::~MainWindow(){}

// ----------------- UPLOAD IMAGE -----------------
void MainWindow::onUpload()
{
    imagePath = QFileDialog::getOpenFileName(this,"Select Image","","*.png *.jpg *.bmp");
    if(imagePath.isEmpty()) return;

    QPixmap pix(imagePath);
    imageLabel->setPixmap(pix.scaled(imageLabel->size(), Qt::KeepAspectRatio));
    resultText->clear();
}

// ----------------- INSPECT IMAGE -----------------
void MainWindow::onInspect()
{
    if(imagePath.isEmpty()) return;

    // Load image using OpenCV
    cv::Mat img = cv::imread(imagePath.toStdString());
    if(img.empty()) {
        resultText->setText("Failed to load image!");
        return;
    }

    // Detect red dots
    auto results = vision.detect(img);

    // Draw red dots and labels on image
    cv::Mat displayImg = img.clone();
    vision.drawAndSave(displayImg, results, "result"); // saves with timestamp

    // Convert OpenCV image to QPixmap for display
    cv::cvtColor(displayImg, displayImg, cv::COLOR_BGR2RGB);
    QImage qimg(displayImg.data, displayImg.cols, displayImg.rows, displayImg.step, QImage::Format_RGB888);
    imageLabel->setPixmap(QPixmap::fromImage(qimg).scaled(imageLabel->size(), Qt::KeepAspectRatio));

    // Show PASS/FAIL
    resultText->clear();
    if(results.empty())
        resultText->append("Result: FAIL");
    else
        resultText->append("Result: PASS");

    // Optionally show coordinates in QTextEdit
    for(auto &r : results)
        resultText->append(QString("ID:%1  X:%2  Y:%3  Pass:%4").arg(r.id).arg(r.x).arg(r.y).arg(r.pass));
}

// ----------------- SEND TCP -----------------
void MainWindow::onSend()
{
    if(imagePath.isEmpty()) return;

    cv::Mat img = cv::imread(imagePath.toStdString());
    if(img.empty()) return;

    auto dots = vision.detect(img);
    vision.drawAndSave(img, dots, "result");

        // -------- Send results via TCP --------
        for (const auto& d : dots) {
            VisionResult result{d.id, d.x, d.y};
            if (!tcp.sendResult(result)) {
                std::cerr << "Failed to send result. Trying to reconnect..." << std::endl;
                // Optional: reconnect logic can be added here
            }
    }
}


#include "mainwindow.moc"