#ifndef MYUDPRTP_H
#define MYUDPRTP_H
#include <QThread>

class MyUdpRtp :public QThread
{
protected:
    //线程退出的标识量
    volatile bool mStop;
    int mPort;
    void run()
    {
        udp_parser();
    }
public:
    void stop()
    {
        mStop = true;
    }
public:
    MyUdpRtp(int port);
    int udp_parser();
};

#endif // MYUDPRTP_H
