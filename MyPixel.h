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

    /**
     * Split Y, U, V planes in YUV444P file.
     * @param url  Location of YUV file.
     * @param w    Width of Input YUV file.
     * @param h    Height of Input YUV file.
     * @param num  Number of frames to process.
     *
     */
    static int yuv444_split(char *url, int w, int h,int num);

    /**
     * Convert YUV420P file to gray picture
     * @param url     Location of Input YUV file.
     * @param w       Width of Input YUV file.
     * @param h       Height of Input YUV file.
     * @param num     Number of frames to process.
     */
    static int yuv420_gray(char *url, int w, int h,int num);

    /**
     * Halve Y value of YUV420P file
     * @param url     Location of Input YUV file.
     * @param w       Width of Input YUV file.
     * @param h       Height of Input YUV file.
     * @param num     Number of frames to process.
     */
    static int yuv420_halfy(char *url, int w, int h,int num);

    /**
     * Add border for YUV420P file
     * @param url     Location of Input YUV file.
     * @param w       Width of Input YUV file.
     * @param h       Height of Input YUV file.
     * @param border  Width of Border.
     * @param num     Number of frames to process.
     */
    static int yuv420_border(char *url, int w, int h,int border,int num);

    /**
     * Generate YUV420P gray scale bar.
     * @param width    Width of Output YUV file.
     * @param height   Height of Output YUV file.
     * @param ymin     Max value of Y
     * @param ymax     Min value of Y
     * @param barnum   Number of bars
     * @param url_out  Location of Output YUV file.
     */
    static int yuv420_graybar(int width, int height,int ymin,int ymax,int barnum,char *url_out);

    /**
     * Calculate PSNR between 2 YUV420P file
     * @param url1     Location of first Input YUV file.
     * @param url2     Location of another Input YUV file.
     * @param w        Width of Input YUV file.
     * @param h        Height of Input YUV file.
     * @param num      Number of frames to process.
     */
     static double yuv420_psnr(char *url1,char *url2,int w,int h,int num);

     /**
      * Split R, G, B planes in RGB24 file.
      * @param url  Location of Input RGB file.
      * @param w    Width of Input RGB file.
      * @param h    Height of Input RGB file.
      * @param num  Number of frames to process.
      *
      */
     static int rgb24_split(char *url, int w, int h,int num);

     /**
      * Convert RGB24 file to BMP file
      * @param rgb24path    Location of input RGB file.
      * @param width        Width of input RGB file.
      * @param height       Height of input RGB file.
      * @param url_out      Location of Output BMP file.
      */
     static int rgb24_to_bmp(const char *rgb24path,int width,int height,const char *bmppath);

     /**
      * Convert RGB24 file to YUV420P file
      * @param url_in  Location of Input RGB file.
      * @param w       Width of Input RGB file.
      * @param h       Height of Input RGB file.
      * @param num     Number of frames to process.
      * @param url_out Location of Output YUV file.
      */
     static int rgb24_to_yuv420(char *url_in, int w, int h,int num,char *url_out);

     /**
      * Generate RGB24 colorbar.
      * @param width    Width of Output RGB file.
      * @param height   Height of Output RGB file.
      * @param url_out  Location of Output RGB file.
      */
     static int rgb24_colorbar(int width, int height,char *url_out);

};

#endif // MYPIXEL_H
