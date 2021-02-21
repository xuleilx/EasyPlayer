# EasyPlayer
Learn Multimedia

```shell
# 生成yuv420p
gst-launch-1.0 -v videotestsrc ! video/x-raw,format=I420,width=800,height=480 ! filesink location=/tmp/test_800x480.yuv

# 生成rgb24
gst-launch-1.0 -v videotestsrc ! video/x-raw,format=RGB,width=800,height=480 ! filesink location=/tmp/test_800x480.rgb

# 生成pcm
gst-launch-1.0 -v audiotestsrc ! audio/x-raw,format=S16LE,rate=16000,channel=2 ! filesink location=/tmp/test_16k_2c.pcm

# 生成aac
gst-launch-1.0 audiotestsrc ! audio/x-raw,layout=interleaved,rate=44100,channel=2 ! audioconvert ! avenc_aac ! avmux_adts ! filesink location=/tmp/test_16k_2c.aac

# 生成flv
ffmpeg -i AVC_high_1280x720_2013.mp4 -ar 22050 test.flv

# 生成rtp
ffmpeg -re -i sintel.ts -f mpegts udp://127.0.0.1:8880
ffmpeg -re -i sintel.ts -f rtp_mpegts udp://127.0.0.1:8880
```
