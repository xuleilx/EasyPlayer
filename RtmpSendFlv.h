#ifndef RTMPSENDFLV_H
#define RTMPSENDFLV_H
#include <QThread>

#define HTON16(x)  ((x>>8&0xff)|(x<<8&0xff00))
#define HTON24(x)  ((x>>16&0xff)|(x<<16&0xff0000)|(x&0xff00))
#define HTON32(x)  ((x>>24&0xff)|(x>>8&0xff00)|\
    (x<<8&0xff0000)|(x<<24&0xff000000))
#define HTONTIME(x) ((x>>16&0xff)|(x<<16&0xff0000)|(x&0xff00)|(x&0xff000000))

class RtmpSendFlv : public QThread
{
protected:
    void run()
    {
        send();
    }
public:
    RtmpSendFlv(QString fileUrl,QString rtmpUrl);
    int send(){
        publish_using_packet();
    }

    void stop()
    {
        mStop = true;
    }

private:
    /*read 1 byte*/
    int ReadU8(uint32_t *u8,FILE*fp){
        if(fread(u8,1,1,fp)!=1)
            return 0;
        return 1;
    }
    /*read 2 byte*/
    int ReadU16(uint32_t *u16,FILE*fp){
        if(fread(u16,2,1,fp)!=1)
            return 0;
        *u16=HTON16(*u16);
        return 1;
    }
    /*read 3 byte*/
    int ReadU24(uint32_t *u24,FILE*fp){
        if(fread(u24,3,1,fp)!=1)
            return 0;
        *u24=HTON24(*u24);
        return 1;
    }
    /*read 4 byte*/
    int ReadU32(uint32_t *u32,FILE*fp){
        if(fread(u32,4,1,fp)!=1)
            return 0;
        *u32=HTON32(*u32);
        return 1;
    }
    /*read 1 byte,and loopback 1 byte at once*/
    int PeekU8(uint32_t *u8,FILE*fp){
        if(fread(u8,1,1,fp)!=1)
            return 0;
        fseek(fp,-1,SEEK_CUR);
        return 1;
    }
    /*read 4 byte and convert to time format*/
    int ReadTime(uint32_t *utime,FILE*fp){
        if(fread(utime,4,1,fp)!=1)
            return 0;
        *utime=HTONTIME(*utime);
        return 1;
    }
    int publish_using_packet();
    int publish_using_write();
private:
    //线程退出的标识量
    volatile bool mStop;
    QString mFileUrl;
    QString mRtmpUrl;
};

#endif // RTMPSENDFLV_H
