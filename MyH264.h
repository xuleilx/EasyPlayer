#ifndef MYH264_H
#define MYH264_H
#include <stdio.h>

class MyH264
{
public:
    MyH264();
    /**
     * Analysis H.264 Bitstream
     * @param url    Location of input H.264 bitstream file.
     */
    static int h264_parser(char *url,char* output_url);
};

#endif // MYH264_H
