#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class MyUdpRtp;
class RtmpRecvFlv;
class RtmpSendFlv;

namespace Ui {
class MainWindow;
}

typedef struct{
    QString name;
    int width;
    int height;
}VideoInfo;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    VideoInfo getInfoFromFilename();
    QString getPcmFilePath();
    QString getAacFilePath();
    QString getAudioFilePath();
    QString getFlvFilePath();
    QString getH264FilePath();

private slots:
    void on_actionYuv444_split_triggered();

    void on_actionYuv420_split_triggered();

    void on_actionYuv420_gray_triggered();

    void on_actionYuv420_halfy_triggered();

    void on_actionYuv420_border_triggered();

    void on_actionYuv420_graybar_triggered();

    void on_actionYuv420_psnr_triggered();

    void on_actionRgb24_split_triggered();

    void on_actionRgb24_to_yuv420_triggered();

    void on_actionRgb24_colorbar_triggered();

    void on_actionRgb24_to_bmp_triggered();

    void on_actionPcm16le_split_triggered();

    void on_actionPcm16le_halfvolumeleft_triggered();

    void on_actionPcm16le_doublespeed_triggered();

    void on_actionPcm16le_to_pcm8_triggered();

    void on_actionPcm16le_cut_singlechannel_triggered();

    void on_actionPcm16le_to_wave_triggered();

    void on_actionH264_parser_triggered();

    void on_actionAac_parser_triggered();

    void on_actionFlv_parser_triggered();

    void on_actionUdp_parser_triggered();

    void on_actionRecv_flv_triggered();

    void on_actionSend_flv_triggered();

    void on_actionSend_h264_triggered();

    void on_actionMedia_info_triggered();

private:
    Ui::MainWindow *ui;
    MyUdpRtp* myUdpRtp;
    RtmpRecvFlv* myRtmpRecvFlv;
    RtmpSendFlv* myRtmpSendFlv;
};

#endif // MAINWINDOW_H
