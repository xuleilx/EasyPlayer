#include "MyPixel.h"
#include<stdlib.h>
#include<stdio.h>
#include <string.h>
#include <math.h>

MyPixel::MyPixel()
{

}
MyPixel::~MyPixel()
{

}

int MyPixel::yuv420_split(char *url, int w, int h,int num){
    FILE *fp=fopen(url,"rb+");
    FILE *fp1=fopen("output_420_y.y","wb+");
    FILE *fp2=fopen("output_420_u.y","wb+");
    FILE *fp3=fopen("output_420_v.y","wb+");

    unsigned char *pic=(unsigned char *)malloc(w*h*3/2);
    int i;
    for(i=0;i<num;i++){

        fread(pic,1,w*h*3/2,fp);
        //Y
        fwrite(pic,1,w*h,fp1);
        //U
        fwrite(pic+w*h,1,w*h/4,fp2);
        //V
        fwrite(pic+w*h*5/4,1,w*h/4,fp3);
    }

    free(pic);
    fclose(fp);
    fclose(fp1);
    fclose(fp2);
    fclose(fp3);

    return 0;
}

int MyPixel::yuv444_split(char *url, int w, int h,int num){
    FILE *fp=fopen(url,"rb+");
    FILE *fp1=fopen("output_444_y.y","wb+");
    FILE *fp2=fopen("output_444_u.y","wb+");
    FILE *fp3=fopen("output_444_v.y","wb+");
    unsigned char *pic=(unsigned char *)malloc(w*h*3);

    for(int i=0;i<num;i++){
        fread(pic,1,w*h*3,fp);
        //Y
        fwrite(pic,1,w*h,fp1);
        //U
        fwrite(pic+w*h,1,w*h,fp2);
        //V
        fwrite(pic+w*h*2,1,w*h,fp3);
    }

    free(pic);
    fclose(fp);
    fclose(fp1);
    fclose(fp2);
    fclose(fp3);

    return 0;
}

int MyPixel::yuv420_gray(char *url, int w, int h,int num){
    FILE *fp=fopen(url,"rb+");
    FILE *fp1=fopen("output_gray.yuv","wb+");
    unsigned char *pic=(unsigned char *)malloc(w*h*3/2);

    for(int i=0;i<num;i++){
        fread(pic,1,w*h*3/2,fp);
        //Gray
        memset(pic+w*h,128,w*h/2);
        fwrite(pic,1,w*h*3/2,fp1);
    }

    free(pic);
    fclose(fp);
    fclose(fp1);
    return 0;
}

int MyPixel::yuv420_halfy(char *url, int w, int h,int num){
    FILE *fp=fopen(url,"rb+");
    FILE *fp1=fopen("output_half.yuv","wb+");

    unsigned char *pic=(unsigned char *)malloc(w*h*3/2);

    for(int i=0;i<num;i++){
        fread(pic,1,w*h*3/2,fp);
        //Half
        for(int j=0;j<w*h;j++){
            unsigned char temp=pic[j]/2;
            //printf("%d,\n",temp);
            pic[j]=temp;
        }
        fwrite(pic,1,w*h*3/2,fp1);
    }

    free(pic);
    fclose(fp);
    fclose(fp1);

    return 0;
}

int MyPixel::yuv420_border(char *url, int w, int h,int border,int num){
    FILE *fp=fopen(url,"rb+");
    FILE *fp1=fopen("output_border.yuv","wb+");

    unsigned char *pic=(unsigned char *)malloc(w*h*3/2);

    for(int i=0;i<num;i++){
        fread(pic,1,w*h*3/2,fp);
        //Y
        for(int j=0;j<h;j++){
            for(int k=0;k<w;k++){
                if(k<border||k>(w-border)||j<border||j>(h-border)){
                    pic[j*w+k]=255;
                    //pic[j*w+k]=0;
                }
            }
        }
        fwrite(pic,1,w*h*3/2,fp1);
    }

    free(pic);
    fclose(fp);
    fclose(fp1);

    return 0;
}

int MyPixel::yuv420_graybar(int width, int height,int ymin,int ymax,int barnum,char *url_out){
    int barwidth;
    float lum_inc;
    unsigned char lum_temp;
    int uv_width,uv_height;
    FILE *fp=NULL;
    unsigned char *data_y=NULL;
    unsigned char *data_u=NULL;
    unsigned char *data_v=NULL;
    int t=0,i=0,j=0;

    barwidth=width/barnum;
    lum_inc=((float)(ymax-ymin))/((float)(barnum-1));
    uv_width=width/2;
    uv_height=height/2;

    data_y=(unsigned char *)malloc(width*height);
    data_u=(unsigned char *)malloc(uv_width*uv_height);
    data_v=(unsigned char *)malloc(uv_width*uv_height);

    if((fp=fopen(url_out,"wb+"))==NULL){
        printf("Error: Cannot create file!");
        return -1;
    }

    //Output Info
    printf("Y, U, V value from picture's left to right:\n");
    for(t=0;t<(width/barwidth);t++){
        lum_temp=ymin+(char)(t*lum_inc);
        printf("%3d, 128, 128\n",lum_temp);
    }
    //Gen Data
    for(j=0;j<height;j++){
        for(i=0;i<width;i++){
            t=i/barwidth;
            lum_temp=ymin+(char)(t*lum_inc);
            data_y[j*width+i]=lum_temp;
        }
    }
    for(j=0;j<uv_height;j++){
        for(i=0;i<uv_width;i++){
            data_u[j*uv_width+i]=128;
        }
    }
    for(j=0;j<uv_height;j++){
        for(i=0;i<uv_width;i++){
            data_v[j*uv_width+i]=128;
        }
    }
    fwrite(data_y,width*height,1,fp);
    fwrite(data_u,uv_width*uv_height,1,fp);
    fwrite(data_v,uv_width*uv_height,1,fp);
    fclose(fp);
    free(data_y);
    free(data_u);
    free(data_v);
    return 0;
}

double MyPixel::yuv420_psnr(char *url1,char *url2,int w,int h,int num){
    double psnr=0;
    FILE *fp1=fopen(url1,"rb+");
    FILE *fp2=fopen(url2,"rb+");
    unsigned char *pic1=(unsigned char *)malloc(w*h);
    unsigned char *pic2=(unsigned char *)malloc(w*h);

    for(int i=0;i<num;i++){
        fread(pic1,1,w*h,fp1);
        fread(pic2,1,w*h,fp2);

        double mse_sum=0,mse=0;
        for(int j=0;j<w*h;j++){
            mse_sum+=pow((double)(pic1[j]-pic2[j]),2);
        }
        mse=mse_sum/(w*h);
        psnr=10*log10(255.0*255.0/mse);
        printf("%5.3f\n",psnr);

        fseek(fp1,w*h/2,SEEK_CUR);
        fseek(fp2,w*h/2,SEEK_CUR);

    }

    free(pic1);
    free(pic2);
    fclose(fp1);
    fclose(fp2);
    return psnr;
}

int MyPixel::rgb24_split(char *url, int w, int h,int num){
    FILE *fp=fopen(url,"rb+");
    FILE *fp1=fopen("output_r.y","wb+");
    FILE *fp2=fopen("output_g.y","wb+");
    FILE *fp3=fopen("output_b.y","wb+");

    unsigned char *pic=(unsigned char *)malloc(w*h*3);

    for(int i=0;i<num;i++){

        fread(pic,1,w*h*3,fp);

        for(int j=0;j<w*h*3;j=j+3){
            //R
            fwrite(pic+j,1,1,fp1);
            //G
            fwrite(pic+j+1,1,1,fp2);
            //B
            fwrite(pic+j+2,1,1,fp3);
        }
    }

    free(pic);
    fclose(fp);
    fclose(fp1);
    fclose(fp2);
    fclose(fp3);

    return 0;
}

int MyPixel::rgb24_to_bmp(const char *rgb24path,int width,int height,const char *bmppath){
    typedef struct {
        int imageSize;
        int blank;
        int startPosition;
    }BmpHead;

    typedef struct {
        int Length;
        int width;
        int height;
        unsigned short colorPlane;
        unsigned short bitColor;
        int zipFormat;
        int realSize;
        int xPels;
        int yPels;
        int colorUse;
        int colorImportant;
    }InfoHead;

    int i=0,j=0;
    BmpHead m_BMPHeader;
    InfoHead  m_BMPInfoHeader;
    memset(&m_BMPHeader,0,sizeof(BmpHead));
    memset(&m_BMPInfoHeader,0,sizeof(InfoHead));
    char bfType[2]={'B','M'};
    int header_size=sizeof(bfType)+sizeof(BmpHead)+sizeof(InfoHead);
    unsigned char *rgb24_buffer=NULL;
    FILE *fp_rgb24=NULL,*fp_bmp=NULL;

    if((fp_rgb24=fopen(rgb24path,"rb"))==NULL){
        printf("Error: Cannot open input RGB24 file.\n");
        return -1;
    }
    if((fp_bmp=fopen(bmppath,"wb"))==NULL){
        printf("Error: Cannot open output BMP file.\n");
        return -1;
    }

    rgb24_buffer=(unsigned char *)malloc(width*height*3);
    fread(rgb24_buffer,1,width*height*3,fp_rgb24);

    m_BMPHeader.imageSize=3*width*height+header_size;
    m_BMPHeader.startPosition=header_size;

    m_BMPInfoHeader.Length=sizeof(InfoHead);
    m_BMPInfoHeader.width=width;
    //BMP storage pixel data in opposite direction of Y-axis (from bottom to top).
    m_BMPInfoHeader.height=-height;
    m_BMPInfoHeader.colorPlane=1;
    m_BMPInfoHeader.bitColor=24;
    m_BMPInfoHeader.realSize=3*width*height;

    fwrite(bfType,1,sizeof(bfType),fp_bmp);
    fwrite(&m_BMPHeader,1,sizeof(m_BMPHeader),fp_bmp);
    fwrite(&m_BMPInfoHeader,1,sizeof(m_BMPInfoHeader),fp_bmp);

    //BMP save R1|G1|B1,R2|G2|B2 as B1|G1|R1,B2|G2|R2
    //It saves pixel data in Little Endian
    //So we change 'R' and 'B'
    for(j =0;j<height;j++){
        for(i=0;i<width;i++){
            char temp=rgb24_buffer[(j*width+i)*3+2];
            rgb24_buffer[(j*width+i)*3+2]=rgb24_buffer[(j*width+i)*3+0];
            rgb24_buffer[(j*width+i)*3+0]=temp;
        }
    }
    fwrite(rgb24_buffer,3*width*height,1,fp_bmp);
    fclose(fp_rgb24);
    fclose(fp_bmp);
    free(rgb24_buffer);
    printf("Finish generate %s!\n",bmppath);
    return 0;
}

unsigned char clip_value(unsigned char x,unsigned char min_val,unsigned char  max_val){
    if(x>max_val){
        return max_val;
    }else if(x<min_val){
        return min_val;
    }else{
        return x;
    }
}

//RGB to YUV420
bool RGB24_TO_YUV420(unsigned char *RgbBuf,int w,int h,unsigned char *yuvBuf)
{
    unsigned char*ptrY, *ptrU, *ptrV, *ptrRGB;
    memset(yuvBuf,0,w*h*3/2);
    ptrY = yuvBuf;
    ptrU = yuvBuf + w*h;
    ptrV = ptrU + (w*h*1/4);
    unsigned char y, u, v, r, g, b;
    for (int j = 0; j<h;j++){
        ptrRGB = RgbBuf + w*j*3 ;
        for (int i = 0;i<w;i++){

            r = *(ptrRGB++);
            g = *(ptrRGB++);
            b = *(ptrRGB++);
            y = (unsigned char)( ( 66 * r + 129 * g +  25 * b + 128) >> 8) + 16  ;
            u = (unsigned char)( ( -38 * r -  74 * g + 112 * b + 128) >> 8) + 128 ;
            v = (unsigned char)( ( 112 * r -  94 * g -  18 * b + 128) >> 8) + 128 ;
            *(ptrY++) = clip_value(y,0,255);
            if (j%2==0&&i%2 ==0){
                *(ptrU++) =clip_value(u,0,255);
            }
            else{
                if (i%2==0){
                    *(ptrV++) =clip_value(v,0,255);
                }
            }
        }
    }
    return true;
}

int MyPixel::rgb24_to_yuv420(char *url_in, int w, int h,int num,char *url_out){
    FILE *fp=fopen(url_in,"rb+");
    FILE *fp1=fopen(url_out,"wb+");

    unsigned char *pic_rgb24=(unsigned char *)malloc(w*h*3);
    unsigned char *pic_yuv420=(unsigned char *)malloc(w*h*3/2);

    for(int i=0;i<num;i++){
        fread(pic_rgb24,1,w*h*3,fp);
        RGB24_TO_YUV420(pic_rgb24,w,h,pic_yuv420);
        fwrite(pic_yuv420,1,w*h*3/2,fp1);
    }

    free(pic_rgb24);
    free(pic_yuv420);
    fclose(fp);
    fclose(fp1);

    return 0;
}

int MyPixel::rgb24_colorbar(int width, int height,char *url_out){

    unsigned char *data=NULL;
    int barwidth;
    FILE *fp=NULL;
    int i=0,j=0;

    data=(unsigned char *)malloc(width*height*3);
    barwidth=width/8;

    if((fp=fopen(url_out,"wb+"))==NULL){
        printf("Error: Cannot create file!");
        return -1;
    }

    for(j=0;j<height;j++){
        for(i=0;i<width;i++){
            int barnum=i/barwidth;
            switch(barnum){
            case 0:{
                data[(j*width+i)*3+0]=255;
                data[(j*width+i)*3+1]=255;
                data[(j*width+i)*3+2]=255;
                break;
            }
            case 1:{
                data[(j*width+i)*3+0]=255;
                data[(j*width+i)*3+1]=255;
                data[(j*width+i)*3+2]=0;
                break;
            }
            case 2:{
                data[(j*width+i)*3+0]=0;
                data[(j*width+i)*3+1]=255;
                data[(j*width+i)*3+2]=255;
                break;
            }
            case 3:{
                data[(j*width+i)*3+0]=0;
                data[(j*width+i)*3+1]=255;
                data[(j*width+i)*3+2]=0;
                break;
            }
            case 4:{
                data[(j*width+i)*3+0]=255;
                data[(j*width+i)*3+1]=0;
                data[(j*width+i)*3+2]=255;
                break;
            }
            case 5:{
                data[(j*width+i)*3+0]=255;
                data[(j*width+i)*3+1]=0;
                data[(j*width+i)*3+2]=0;
                break;
            }
            case 6:{
                data[(j*width+i)*3+0]=0;
                data[(j*width+i)*3+1]=0;
                data[(j*width+i)*3+2]=255;

                break;
            }
            case 7:{
                data[(j*width+i)*3+0]=0;
                data[(j*width+i)*3+1]=0;
                data[(j*width+i)*3+2]=0;
                break;
            }
            }

        }
    }
    fwrite(data,width*height*3,1,fp);
    fclose(fp);
    free(data);

    return 0;
}
