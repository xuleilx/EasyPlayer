#include "RtmpSendFlv.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "librtmp/rtmp.h"
#include "librtmp/log.h"

RtmpSendFlv::RtmpSendFlv(QString fileUrl,QString rtmpUrl):mFileUrl(fileUrl),mRtmpUrl(rtmpUrl)
{
    mStop = true;
}

//Publish using RTMP_SendPacket()
int RtmpSendFlv::publish_using_packet(){
         RTMP *rtmp=NULL;
         RTMPPacket *packet=NULL;
         uint32_t start_time=0;
         uint32_t now_time=0;
         //the timestamp of the previous frame
         long pre_frame_time=0;
         long lasttime=0;
         int bNextIsKey=1;
         uint32_t preTagsize=0;

         //packet attributes
         uint32_t type=0;
         uint32_t datalength=0;
         uint32_t timestamp=0;
         uint32_t streamid=0;

         FILE*fp=NULL;
         fp=fopen(mFileUrl.toLatin1().data(),"rb");
         if (!fp){
                   RTMP_LogPrintf("Open File Error.\n");
                   return -1;
         }

         /* set log level */
         //RTMP_LogLevel loglvl=RTMP_LOGDEBUG;
         //RTMP_LogSetLevel(loglvl);

         rtmp=RTMP_Alloc();
         RTMP_Init(rtmp);
         //set connection timeout,default 30s
         rtmp->Link.timeout=5;
         if(!RTMP_SetupURL(rtmp,mRtmpUrl.toLatin1().data()))
         {
                   RTMP_Log(RTMP_LOGERROR,"SetupURL Err\n");
                   RTMP_Free(rtmp);
                   return -1;
         }

         //if unable,the AMF command would be 'play' instead of 'publish'
         RTMP_EnableWrite(rtmp);

         if (!RTMP_Connect(rtmp,NULL)){
                   RTMP_Log(RTMP_LOGERROR,"Connect Err\n");
                   RTMP_Free(rtmp);
                   return -1;
         }

         if (!RTMP_ConnectStream(rtmp,0)){
                   RTMP_Log(RTMP_LOGERROR,"ConnectStream Err\n");
                   RTMP_Close(rtmp);
                   RTMP_Free(rtmp);
                   return -1;
         }

         packet=(RTMPPacket*)malloc(sizeof(RTMPPacket));
         RTMPPacket_Alloc(packet,1024*64);
         RTMPPacket_Reset(packet);

         packet->m_hasAbsTimestamp = 0;
         packet->m_nChannel = 0x04;
         packet->m_nInfoField2 = rtmp->m_stream_id;

         RTMP_LogPrintf("Start to send data ...\n");

         //jump over FLV Header
         fseek(fp,9,SEEK_SET);
         //jump over previousTagSizen
         fseek(fp,4,SEEK_CUR);
         start_time=RTMP_GetTime();
         mStop = false;
         while(!mStop)
         {
                   if((((now_time=RTMP_GetTime())-start_time)
                              <(pre_frame_time)) && bNextIsKey){
                            //wait for 1 sec if the send process is too fast
                            //this mechanism is not very good,need some improvement
                            if(pre_frame_time>lasttime){
                                     RTMP_LogPrintf("TimeStamp:%8lu ms\n",pre_frame_time);
                                     lasttime=pre_frame_time;
                            }
                            usleep(1000);
                            continue;
                   }

                   //not quite the same as FLV spec
                   if(!ReadU8(&type,fp))
                            break;
                   if(!ReadU24(&datalength,fp))
                            break;
                   if(!ReadTime(&timestamp,fp))
                            break;
                   if(!ReadU24(&streamid,fp))
                            break;

                   if (type!=0x08&&type!=0x09){
                            //jump over non_audio and non_video frame，
                            //jump over next previousTagSizen at the same time
                            fseek(fp,datalength+4,SEEK_CUR);
                            continue;
                   }

                   if(fread(packet->m_body,1,datalength,fp)!=datalength)
                            break;

                   packet->m_headerType = RTMP_PACKET_SIZE_LARGE;
                   packet->m_nTimeStamp = timestamp;
                   packet->m_packetType = type;
                   packet->m_nBodySize  = datalength;
                   pre_frame_time=timestamp;

                   if (!RTMP_IsConnected(rtmp)){
                            RTMP_Log(RTMP_LOGERROR,"rtmp is not connect\n");
                            break;
                   }
                   if (!RTMP_SendPacket(rtmp,packet,0)){
                            RTMP_Log(RTMP_LOGERROR,"Send Error\n");
                            break;
                   }

                   if(!ReadU32(&preTagsize,fp))
                            break;

                   if(!PeekU8(&type,fp))
                            break;
                   if(type==0x09){
                            if(fseek(fp,11,SEEK_CUR)!=0)
                                     break;
                            if(!PeekU8(&type,fp)){
                                     break;
                            }
                            if(type==0x17)
                                     bNextIsKey=1;
                            else
                                     bNextIsKey=0;

                            fseek(fp,-11,SEEK_CUR);
                   }
         }

         RTMP_LogPrintf("\nSend Data Over\n");

         if(fp)
                   fclose(fp);

         if (rtmp!=NULL){
                   RTMP_Close(rtmp);
                   RTMP_Free(rtmp);
                   rtmp=NULL;
         }
         if (packet!=NULL){
                   RTMPPacket_Free(packet);
                   packet=NULL;
         }

         return 0;
}

//Publish using RTMP_Write()
int RtmpSendFlv::publish_using_write(){
         uint32_t start_time=0;
         uint32_t now_time=0;
         uint32_t pre_frame_time=0;
         uint32_t lasttime=0;
         int bNextIsKey=0;
         char* pFileBuf=NULL;

         //read from tag header
         uint32_t type=0;
         uint32_t datalength=0;
         uint32_t timestamp=0;

         RTMP *rtmp=NULL;

         FILE*fp=NULL;
         fp=fopen(mFileUrl.toLatin1().data(),"rb");
         if (!fp){
                   RTMP_LogPrintf("Open File Error.\n");
                   return -1;
         }

         /* set log level */
         //RTMP_LogLevel loglvl=RTMP_LOGDEBUG;
         //RTMP_LogSetLevel(loglvl);

         rtmp=RTMP_Alloc();
         RTMP_Init(rtmp);
         //set connection timeout,default 30s
         rtmp->Link.timeout=5;
         if(!RTMP_SetupURL(rtmp,mRtmpUrl.toLatin1().data()))
         {
                   RTMP_Log(RTMP_LOGERROR,"SetupURL Err\n");
                   RTMP_Free(rtmp);
                   return -1;
         }

         RTMP_EnableWrite(rtmp);
         //1hour
         RTMP_SetBufferMS(rtmp, 3600*1000);
         if (!RTMP_Connect(rtmp,NULL)){
                   RTMP_Log(RTMP_LOGERROR,"Connect Err\n");
                   RTMP_Free(rtmp);
                   return -1;
         }

         if (!RTMP_ConnectStream(rtmp,0)){
                   RTMP_Log(RTMP_LOGERROR,"ConnectStream Err\n");
                   RTMP_Close(rtmp);
                   RTMP_Free(rtmp);
                   return -1;
         }

         printf("Start to send data ...\n");

         //jump over FLV Header
         fseek(fp,9,SEEK_SET);
         //jump over previousTagSizen
         fseek(fp,4,SEEK_CUR);
         start_time=RTMP_GetTime();
         mStop = false;
         while(!mStop)
         {
                   if((((now_time=RTMP_GetTime())-start_time)
                              <(pre_frame_time)) && bNextIsKey){
                            //wait for 1 sec if the send process is too fast
                            //this mechanism is not very good,need some improvement
                            if(pre_frame_time>lasttime){
                                     RTMP_LogPrintf("TimeStamp:%8lu ms\n",pre_frame_time);
                                     lasttime=pre_frame_time;
                            }
                            usleep(1000);
                            continue;
                   }

                   //jump over type
                   fseek(fp,1,SEEK_CUR);
                   if(!ReadU24(&datalength,fp))
                            break;
                   if(!ReadTime(&timestamp,fp))
                            break;
                   //jump back
                   fseek(fp,-8,SEEK_CUR);

                   pFileBuf=(char*)malloc(11+datalength+4);
                   memset(pFileBuf,0,11+datalength+4);
                   if(fread(pFileBuf,1,11+datalength+4,fp)!=(11+datalength+4))
                            break;

                   pre_frame_time=timestamp;

                   if (!RTMP_IsConnected(rtmp)){
                            RTMP_Log(RTMP_LOGERROR,"rtmp is not connect\n");
                            break;
                   }
                   if (!RTMP_Write(rtmp,pFileBuf,11+datalength+4)){
                            RTMP_Log(RTMP_LOGERROR,"Rtmp Write Error\n");
                            break;
                   }

                   free(pFileBuf);
                   pFileBuf=NULL;

                   if(!PeekU8(&type,fp))
                            break;
                   if(type==0x09){
                            if(fseek(fp,11,SEEK_CUR)!=0)
                                     break;
                            if(!PeekU8(&type,fp)){
                                     break;
                            }
                            if(type==0x17)
                                     bNextIsKey=1;
                            else
                                     bNextIsKey=0;
                            fseek(fp,-11,SEEK_CUR);
                   }
         }

         RTMP_LogPrintf("\nSend Data Over\n");

         if(fp)
                   fclose(fp);

         if (rtmp!=NULL){
                   RTMP_Close(rtmp);
                   RTMP_Free(rtmp);
                   rtmp=NULL;
         }

         if(pFileBuf){
                   free(pFileBuf);
                   pFileBuf=NULL;
         }

         return 0;
}
