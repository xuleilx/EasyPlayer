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
#include "MyPcm.h"
#include "MyH264.h"
#include "MyAac.h"
#include "MyFlv.h"
#include "MyUdpRtp.h"
#include "RtmpRecvFlv.h"
#include "RtmpSendFlv.h"
#include "MyFFmpeg.h"

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
                                                    "/home/xuleilx/mywork/video",
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

QString MainWindow::getPcmFilePath(){
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    "/home/xuleilx/mywork/audio",
                                                    tr("Raw (*.pcm)"));
    return fileName;
}

QString MainWindow::getAacFilePath(){
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    "/home/xuleilx/mywork/audio",
                                                    tr("AAC (*.aac)"));
    return fileName;
}


QString MainWindow::getFlvFilePath(){
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    "/home/xuleilx/mywork/video",
                                                    tr("FLV (*.flv)"));
    return fileName;
}


QString MainWindow::getH264FilePath(){
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    "/home/xuleilx/mywork/video",
                                                    tr("Raw (*.h264)"));
    return fileName;
}

QString MainWindow::getAudioFilePath(){
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    "/home/xuleilx/mywork/audio",
                                                    tr("AAC (*.aac *.mp3 *.ogg)"));
    return fileName;
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

void MainWindow::on_actionRgb24_to_yuv420_triggered()
{
    VideoInfo videoInfo1 = getInfoFromFilename();
    if(videoInfo1.name.isEmpty() || videoInfo1.width == 0 || videoInfo1.height == 0){
        return;
    }
    MyPixel::rgb24_to_yuv420(videoInfo1.name.toLatin1().data(),
                             videoInfo1.width,
                             videoInfo1.height,
                             1,
                             "output.yuv");
}

void MainWindow::on_actionRgb24_colorbar_triggered()
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

        if(widthSpinbox->value() == 0 || heightSpinbox->value() == 0) return;

        QString fileName;
        fileName.append("Output_").
                append(QString::number(widthSpinbox->value())).
                append('_').
                append(QString::number(heightSpinbox->value())).
                append(".rgb");
        qDebug()<<fileName;
        MyPixel::rgb24_colorbar(widthSpinbox->value(),
                                heightSpinbox->value(),
                                fileName.toLatin1().data());
    }
}

void MainWindow::on_actionRgb24_to_bmp_triggered()
{
    VideoInfo videoInfo1 = getInfoFromFilename();
    if(videoInfo1.name.isEmpty() || videoInfo1.width == 0 || videoInfo1.height == 0){
        return;
    }
    MyPixel::rgb24_to_bmp(videoInfo1.name.toLatin1().data(),
                          videoInfo1.width,
                          videoInfo1.height,
                          "output.bmp"
                          );
}

void MainWindow::on_actionPcm16le_split_triggered()
{
    QString filePath = getPcmFilePath();
    if(filePath.isNull()){
        return;
    }
    qDebug()<<filePath;
    MyPcm::pcm16le_split(filePath.toLatin1().data());
}

void MainWindow::on_actionPcm16le_halfvolumeleft_triggered()
{
    QString filePath = getPcmFilePath();
    if(filePath.isNull()){
        return;
    }
    qDebug()<<filePath;
    MyPcm::pcm16le_halfvolumeleft(filePath.toLatin1().data());
}

void MainWindow::on_actionPcm16le_doublespeed_triggered()
{
    QString filePath = getPcmFilePath();
    if(filePath.isNull()){
        return;
    }
    qDebug()<<filePath;
    MyPcm::pcm16le_doublespeed(filePath.toLatin1().data());
}

void MainWindow::on_actionPcm16le_to_pcm8_triggered()
{
    QString filePath = getPcmFilePath();
    if(filePath.isNull()){
        return;
    }
    qDebug()<<filePath;
    MyPcm::pcm16le_to_pcm8(filePath.toLatin1().data());
}

void MainWindow::on_actionPcm16le_cut_singlechannel_triggered()
{
    // input file
    QString filePath = getPcmFilePath();
    if(filePath.isNull()){
        return;
    }
    qDebug()<<filePath;

    // input cut info
    QDialog dialog(this);
    QFormLayout form(&dialog);
    form.addRow(new QLabel("User input:"));
    // start num
    QString startNum = QString("start num: ");
    QSpinBox *startNumSpinbox = new QSpinBox(&dialog);
    startNumSpinbox->setMaximum(9999);
    form.addRow(startNum, startNumSpinbox);
    // dur num
    QString durNum = QString("dur num: ");
    QSpinBox *durNumSpinbox = new QSpinBox(&dialog);
    durNumSpinbox->setMaximum(9999);
    form.addRow(durNum, durNumSpinbox);
    // Add Cancel and OK button
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                               Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

    // Process when OK button is clicked
    if (dialog.exec() == QDialog::Accepted) {
        // Do something here
        qDebug()<<startNumSpinbox->value();
        qDebug()<<durNumSpinbox->value();

        if(durNumSpinbox->value() == 0) return;

        MyPcm::pcm16le_cut_singlechannel(filePath.toLatin1().data(),
                                         startNumSpinbox->value(),
                                         durNumSpinbox->value());
    }
}

void MainWindow::on_actionPcm16le_to_wave_triggered()
{
    // input file
    QString filePath = getPcmFilePath();
    if(filePath.isNull()){
        return;
    }
    qDebug()<<filePath;

    // input audio info
    QDialog dialog(this);
    QFormLayout form(&dialog);
    form.addRow(new QLabel("User input:"));
    // Sample Rate
    QString sampleRate = QString("Sample Rate: ");
    QSpinBox *sampleRateSpinbox = new QSpinBox(&dialog);
    sampleRateSpinbox->setMaximum(48000);
    form.addRow(sampleRate, sampleRateSpinbox);
    // Channels
    QString channels = QString("Channels: ");
    QSpinBox *channelsSpinbox = new QSpinBox(&dialog);
    channelsSpinbox->setMaximum(2);
    form.addRow(channels, channelsSpinbox);
    // Add Cancel and OK button
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                               Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

    // Process when OK button is clicked
    if (dialog.exec() == QDialog::Accepted) {
        // Do something here
        qDebug()<<sampleRateSpinbox->value();
        qDebug()<<channelsSpinbox->value();

        if(sampleRateSpinbox->value() ==0 || channelsSpinbox->value() == 0) return;

        MyPcm::pcm16le_to_wave(filePath.toLatin1().data(),
                               channelsSpinbox->value(),
                               sampleRateSpinbox->value(),
                               "output_nocturne.wav");
    }
}

void MainWindow::on_actionH264_parser_triggered()
{
    QString filePath = getH264FilePath();
    QString outputPath("output_log.txt");
    if(filePath.isNull()){
        return;
    }
    qDebug()<<filePath;
    MyH264::h264_parser(filePath.toLatin1().data(),outputPath.toLatin1().data());

    QFile file(outputPath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    ui->textBrowser->setText(file.readAll());
}

void MainWindow::on_actionAac_parser_triggered()
{
    QString filePath = getAacFilePath();
    QString outputPath("output_log.txt");
    if(filePath.isNull()){
        return;
    }
    qDebug()<<filePath;
    MyAac::aac_parser(filePath.toLatin1().data(),outputPath.toLatin1().data());

    QFile file(outputPath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    ui->textBrowser->setText(file.readAll());
}

void MainWindow::on_actionFlv_parser_triggered()
{
    QString filePath = getFlvFilePath();
    QString outputPath("output_log.txt");
    if(filePath.isNull()){
        return;
    }
    qDebug()<<filePath;
    MyFlv::flv_parser(filePath.toLatin1().data(),outputPath.toLatin1().data());

    QFile file(outputPath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    ui->textBrowser->setText(file.readAll());
}

void MainWindow::on_actionUdp_parser_triggered()
{
    // input audio info
    QDialog dialog(this);
    QFormLayout form(&dialog);
    form.addRow(new QLabel("User input:"));
    // Sample Rate
    QString port = QString("Port: ");
    QSpinBox *portRateSpinbox = new QSpinBox(&dialog);
    portRateSpinbox->setMaximum(99999);
    form.addRow(port, portRateSpinbox);
    // Add Cancel and OK button
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                               Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

    // Process when OK button is clicked
    if (dialog.exec() == QDialog::Accepted) {
        // Do something here
        qDebug()<<portRateSpinbox->value();

        if(portRateSpinbox->value() == 0) return;

        myUdpRtp = new MyUdpRtp(portRateSpinbox->value());
        myUdpRtp->start();
    }
}

void MainWindow::on_actionRecv_flv_triggered()
{
    QDialog dialog(this);
    QSize dialogSize(400,100);
    dialog.setFixedSize(dialogSize);
    // layout
    QFormLayout form(&dialog);
    form.addRow(new QLabel("User input:"));
    // url
    QString rtmpUrl = QString("RTMP URL: ");
    QLineEdit *rtmpUrlLineedit = new QLineEdit(&dialog);
    form.addRow(rtmpUrl, rtmpUrlLineedit);
    // Add Cancel and OK button
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                               Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

    // Process when OK button is clicked
    if (dialog.exec() == QDialog::Accepted) {
        // Do something here
        qDebug()<<rtmpUrlLineedit->text();

        if(rtmpUrlLineedit->text().isEmpty()) return;

        myRtmpRecvFlv = new RtmpRecvFlv(rtmpUrlLineedit->text());
        myRtmpRecvFlv->start();
        // ---------------------
        QMessageBox msgBox;
        msgBox.setText("Receiving data ... \n\nPress OK to save as receive.flv");
        if (msgBox.exec() == QMessageBox::Ok) {
            // Do something here
            qDebug()<<"Hi, I am OK!";
            myRtmpRecvFlv->stop();
        }
    }
}

void MainWindow::on_actionSend_flv_triggered()
{
    QDialog dialog(this);
    QSize dialogSize(400,130);
    dialog.setFixedSize(dialogSize);
    // layout
    QFormLayout form(&dialog);
    form.addRow(new QLabel("User input:"));
    // file url
    QString fileUrl = QString("File URL: ");
    QLineEdit *fileUrlLineedit = new QLineEdit(&dialog);
    form.addRow(fileUrl, fileUrlLineedit);
    // rtmp url
    QString rtmpUrl = QString("Rtmp URL: ");
    QLineEdit *rtmpUrlLineedit = new QLineEdit(&dialog);
    form.addRow(rtmpUrl, rtmpUrlLineedit);
    // Add Cancel and OK button
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                               Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

    // Process when OK button is clicked
    if (dialog.exec() == QDialog::Accepted) {
        // Do something here
        qDebug()<<fileUrlLineedit->text();
        qDebug()<<rtmpUrlLineedit->text();

        if(fileUrlLineedit->text().isEmpty() || rtmpUrlLineedit->text().isEmpty()) return;

        myRtmpSendFlv = new RtmpSendFlv(fileUrlLineedit->text(),rtmpUrlLineedit->text());
        myRtmpSendFlv->start();
        // ---------------------
        QMessageBox msgBox;
        msgBox.setText("Sending data ... \n\nPress OK to Stop!");
        if (msgBox.exec() == QMessageBox::Ok) {
            // Do something here
            qDebug()<<"Hi, I am OK!";
            myRtmpSendFlv->stop();
        }
    }
}

void MainWindow::on_actionSend_h264_triggered()
{
    ui->textBrowser->setText("Todo: incomplete");
}

void MainWindow::on_actionMedia_info_triggered()
{
    QString filePath = getAudioFilePath();
    if(filePath.isNull()){
        return;
    }
    qDebug()<<filePath;

    MyFFmpeg fg(filePath);
    fg.openMedia();
    QString mediaInfo;
    mediaInfo.append("Title\t: ").append(fg.getTitle()).append('\n');
    mediaInfo.append("Artist\t: ").append(fg.getArtist()).append('\n');
    mediaInfo.append("Album\t: ").append(fg.getAlbum());
    ui->textBrowser->setText(mediaInfo);
}
