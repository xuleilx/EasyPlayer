#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <QLabel>
#include <QMessageBox>
#include <QFileDialog>
#include <QInputDialog>
#include <QFormLayout>
#include <QSpinBox>
#include <QDialogButtonBox>
#include <QDebug>
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

VideoInfo MainWindow::getInfoFromFilename(){
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    "/home/xuleilx/Videos",
                                                    tr("Raw (*.yuv *.rgb)"));
    VideoInfo videoInfo={0,0,0};

    if(!fileName.isNull()){
        QFileInfo fi(fileName);
        QString str(fi.baseName());
        QStringList list;

        list = str.split('_');
        str=list.last();        // 800x480
        list = str.split(QRegExp("[xX*]"));  // ["800","480"]
        if(list.size()!=2){
            QMessageBox msgBox;
            msgBox.setText("File Name must be like xxx_800x480.yuv \n800:width\n480:height\n");
            msgBox.exec();
            return videoInfo;
        }


        qDebug()<<list.at(0).toInt();
        qDebug()<<list.at(1).toInt();

        //        QByteArray byteArray(fileName.toLatin1());
        //        char *fileUri = byteArray.data();

        videoInfo.name = fileName;
        videoInfo.width = list.at(0).toInt();
        videoInfo.height = list.at(1).toInt();
        return videoInfo;
    }
    return videoInfo;
}

void MainWindow::on_actionYuv420_split_triggered()
{
    VideoInfo videoInfo = getInfoFromFilename();
    if(videoInfo.name.isEmpty() || videoInfo.width == 0 || videoInfo.height == 0){
        return;
    }
    MyPixel::yuv420_split(videoInfo.name.toLatin1().data(),
                          videoInfo.width,
                          videoInfo.height,
                          1);
}

void MainWindow::on_actionYuv444_split_triggered()
{
    VideoInfo videoInfo = getInfoFromFilename();
    if(videoInfo.name.isEmpty() || videoInfo.width == 0 || videoInfo.height == 0){
        return;
    }
    MyPixel::yuv444_split(videoInfo.name.toLatin1().data(),
                          videoInfo.width,
                          videoInfo.height,
                          1);
}

void MainWindow::on_actionYuv420_gray_triggered()
{
    VideoInfo videoInfo = getInfoFromFilename();
    if(videoInfo.name.isEmpty() || videoInfo.width == 0 || videoInfo.height == 0){
        return;
    }
    MyPixel::yuv420_gray(videoInfo.name.toLatin1().data(),
                         videoInfo.width,
                         videoInfo.height,
                         1);
}

void MainWindow::on_actionYuv420_halfy_triggered()
{
    VideoInfo videoInfo = getInfoFromFilename();
    if(videoInfo.name.isEmpty() || videoInfo.width == 0 || videoInfo.height == 0){
        return;
    }
    MyPixel::yuv420_halfy(videoInfo.name.toLatin1().data(),
                          videoInfo.width,
                          videoInfo.height,
                          1);
}

void MainWindow::on_actionYuv420_border_triggered()
{
    VideoInfo videoInfo = getInfoFromFilename();
    if(videoInfo.name.isEmpty() || videoInfo.width == 0 || videoInfo.height == 0){
        return;
    }
    MyPixel::yuv420_border(videoInfo.name.toLatin1().data(),
                           videoInfo.width,
                           videoInfo.height,
                           20,
                           1);
}

void MainWindow::on_actionYuv420_graybar_triggered()
{
    QDialog dialog(this);
    QFormLayout form(&dialog);
    form.addRow(new QLabel("User input:"));
    // Width
    QString width = QString("Width: ");
    QSpinBox *widthSpinbox = new QSpinBox(&dialog);
    widthSpinbox->setMaximum(9999);
    form.addRow(width, widthSpinbox);
    // Height
    QString height = QString("Height: ");
    QSpinBox *heightSpinbox = new QSpinBox(&dialog);
    heightSpinbox->setMaximum(9999);
    form.addRow(height, heightSpinbox);
    // BarNum
    QString barNum = QString("BarNum: ");
    QSpinBox *barNumSpinbox = new QSpinBox(&dialog);
    form.addRow(barNum, barNumSpinbox);
    // FilePath
    QString fielPath = QString("FilePath: ");
    QLineEdit *fielPathLineedit = new QLineEdit(&dialog);
    form.addRow(fielPath, fielPathLineedit);
    // Add Cancel and OK button
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                               Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

    // Process when OK button is clicked
    if (dialog.exec() == QDialog::Accepted) {
        // Do something here
        qDebug()<<widthSpinbox->value();
        qDebug()<<heightSpinbox->value();
        qDebug()<<barNumSpinbox->value();
        qDebug()<<fielPathLineedit->text();

        if(widthSpinbox->value() == 0 ||
                heightSpinbox->value() == 0 ||
                barNumSpinbox->value() == 0 ||
                fielPathLineedit->text().isEmpty()) return;

        QByteArray byteArray(fielPathLineedit->text().toLatin1());
        MyPixel::yuv420_graybar(widthSpinbox->value(),
                                heightSpinbox->value(),
                                0,255,
                                barNumSpinbox->value(),
                                byteArray.data());
    }
}

void MainWindow::on_actionYuv420_psnr_triggered()
{
    VideoInfo videoInfo1 = getInfoFromFilename();
    if(videoInfo1.name.isEmpty() || videoInfo1.width == 0 || videoInfo1.height == 0){
        return;
    }
    qDebug()<<videoInfo1.name;
    VideoInfo videoInfo2 = getInfoFromFilename();
    if(videoInfo2.name.isEmpty() || videoInfo2.width == 0 || videoInfo2.height == 0){
        return;
    }

    if(videoInfo1.width != videoInfo2.width || videoInfo1.height != videoInfo2.height){
        return;
    }
    qDebug()<<videoInfo1.name;
    qDebug()<<videoInfo2.name;
    qDebug()<<videoInfo1.width;
    qDebug()<<videoInfo1.height;
    qDebug()<<MyPixel::yuv420_psnr(videoInfo1.name.toLatin1().data(),
                                   videoInfo2.name.toLatin1().data(),
                                   videoInfo1.width,
                                   videoInfo1.height,
                                   1);
}

// note: yuvplayer : 800x480, format: y
void MainWindow::on_actionRgb24_split_triggered()
{
    VideoInfo videoInfo = getInfoFromFilename();
    if(videoInfo.name.isEmpty() || videoInfo.width == 0 || videoInfo.height == 0){
        return;
    }
    MyPixel::rgb24_split(videoInfo.name.toLatin1().data(),
                         videoInfo.width,
                         videoInfo.height,
                         1);
}
