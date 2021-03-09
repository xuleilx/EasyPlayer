#ifndef RECVFLV_H
#define RECVFLV_H
#include <QThread>

class RtmpRecvFlv : public QThread
{
protected:
    void run()
    {
        recv();
    }
public:
    RtmpRecvFlv(QString url);
    int recv();
    void stop()
    {
        mStop = true;
    }
private:
    //线程退出的标识量
    volatile bool mStop;
    QString mUrl;
};

#endif // RECVFLV_H
