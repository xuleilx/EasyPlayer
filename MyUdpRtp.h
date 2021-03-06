#ifndef MYUDPRTP_H
#define MYUDPRTP_H
#include <QThread>

class MyUdpRtp :public QThread
{
protected:
    void run()
    {
        udp_parser();
    }
public:
    MyUdpRtp(int port);
    int udp_parser();
    void stop()
    {
        mStop = true;
    }
private:
    //线程退出的标识量
    volatile bool mStop;
    int mPort;
};

#endif // MYUDPRTP_H
