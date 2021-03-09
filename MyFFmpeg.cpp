#include "MyFFmpeg.h"
#include <iostream>

MyFFmpeg::MyFFmpeg(QString url):mUrl(url)
{

}
MyFFmpeg::~MyFFmpeg(){
    avformat_close_input(&mAvFormatCtx);
}

void MyFFmpeg::openMedia(){
    mAvFormatCtx = avformat_alloc_context();
    int ret = avformat_open_input(&mAvFormatCtx, mUrl.toLocal8Bit().data(), NULL, NULL);
    if (ret < 0){
        fprintf(stderr, "Could not open file\n");
        abort();
    }
    ret = avformat_find_stream_info(mAvFormatCtx, NULL);
    if (ret < 0) {
        fprintf(stderr, "Could not find stream information\n");
        abort();
    }
}

QString MyFFmpeg::getDataByKeyPriv(const AVDictionary *m,QString key)
{
    const AVDictionaryEntry *tag = NULL;

    tag = av_dict_get(m, key.toLocal8Bit().data(), tag, AV_DICT_IGNORE_SUFFIX);
    if (!tag) return NULL;

    const char *p = tag->value;
    char tmp[256];
    size_t len = strcspn(p, "\x8\xa\xb\xc\xd");
    av_strlcpy(tmp, p, FFMIN(sizeof(tmp), len+1));
    return tmp;
}

QString MyFFmpeg::getDataByKey(QString key){
    QString tmp;
    tmp = getDataByKeyPriv(mAvFormatCtx->metadata,key.toLocal8Bit().data());
    if(!tmp.isNull()) return tmp;

    int i=0;
    for (i = 0; i < mAvFormatCtx->nb_streams; i++){
        tmp = getDataByKeyPriv(mAvFormatCtx->streams[i]->metadata,key.toLocal8Bit().data());
    }
    return tmp;
}

QString MyFFmpeg::getTitle(){
    return getDataByKey("title");
}

QString MyFFmpeg::getArtist(){
    return getDataByKey("Artist");
}

QString MyFFmpeg::getAlbum(){
    return getDataByKey("ALBUM");
}
