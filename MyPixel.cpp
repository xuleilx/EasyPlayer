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
    FILE *fp1=fopen("./output_420_y.y","wb+");
    FILE *fp2=fopen("./output_420_u.y","wb+");
    FILE *fp3=fopen("./output_420_v.y","wb+");

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
