# EasyPlayer
Learn Multimedia

```shell
# 生成yuv420p
gst-launch-1.0 -v videotestsrc ! video/x-raw,format=I420,width=800,height=480 ! filesink location=/tmp/test_800x480.yuv

# 生成rgb24
gst-launch-1.0 -v videotestsrc ! video/x-raw,format=RGB,width=800,height=480 ! filesink location=/tmp/test_800x480.rgb

# 生成pcm
gst-launch-1.0 -v audiotestsrc ! audio/x-raw,format=S16LE,rate=16000,channel=2 ! filesink location=/tmp/test_16k_2c.pcm
```
