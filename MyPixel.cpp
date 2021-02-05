#include "MyPixel.h"
#include<stdlib.h>
#include<stdio.h>

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
