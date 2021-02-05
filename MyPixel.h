#ifndef MYPIXEL_H
#define MYPIXEL_H


class MyPixel
{
public:
    MyPixel();
    ~MyPixel();
    /**
     * Split Y, U, V planes in YUV420P file.
     * @param url  Location of Input YUV file.
     * @param w    Width of Input YUV file.
     * @param h    Height of Input YUV file.
     * @param num  Number of frames to process.
     *
     */
    static int yuv420_split(char *url, int w, int h,int num);
};

#endif // MYPIXEL_H
