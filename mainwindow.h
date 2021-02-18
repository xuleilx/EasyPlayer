#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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

private slots:
    void on_actionYuv444_split_triggered();

    void on_actionYuv420_split_triggered();

    void on_actionYuv420_gray_triggered();

    void on_actionYuv420_halfy_triggered();

    void on_actionYuv420_border_triggered();

    void on_actionYuv420_graybar_triggered();

    void on_actionYuv420_psnr_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
