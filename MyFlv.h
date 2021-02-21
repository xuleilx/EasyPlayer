#ifndef MYFLV_H
#define MYFLV_H


class MyFlv
{
public:
    MyFlv();
    static int flv_parser(char *url,char* output_url);
};

#endif // MYFLV_H
