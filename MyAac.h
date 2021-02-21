#ifndef MYAAC_H
#define MYAAC_H


class MyAac
{
public:
    MyAac();

    static int aac_parser(char *url,char* output_url);
};

#endif // MYAAC_H
