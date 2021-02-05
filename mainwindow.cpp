#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QLabel>
#include <QMessageBox>
#include <QFileDialog>
#include "MyPixel.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionYuv420_split_triggered()
{
     QString fileName = QFileDialog::getOpenFileName();

    ui->textBrowser->setTextColor(QColor(Qt::red));
    ui->textBrowser->setText("This is yuv420_split: \n"+fileName);

    QByteArray byteArray(fileName.toLatin1());
    char *fileUri = byteArray.data();
    MyPixel::yuv420_split(fileUri,256,256,1);
}

void MainWindow::on_actionYuv444_split_triggered()
{
    ui->textBrowser->setTextColor(QColor(Qt::blue));
    ui->textBrowser->setText("This is yuv444_split.\n");
}
