#ifndef MYFFMPEG_H
#define MYFFMPEG_H
#include <QString>
extern "C"
{
#include <libavformat/avformat.h>
#include "libavutil/avstring.h"
}


class MyFFmpeg
{
public:
    MyFFmpeg(QString url);
    ~MyFFmpeg();
    void openMedia();

    QString getTitle();
    QString getArtist();
    QString getAlbum();

private:
    QString getDataByKey(QString key);
    QString getDataByKeyPriv(const AVDictionary *m,QString key);
private:
    AVFormatContext *mAvFormatCtx;
    QString mUrl;
};

#endif // MYFFMPEG_H
