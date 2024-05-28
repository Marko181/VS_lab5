#!/bin/bash
gst-launch-1.0 v4l2src device=/dev/video0 ! \
video/x-raw,format=UYVY,width=1280,height=720,framerate=30/1 ! \
v4l2h264enc ! \
'video/x-h264,level=(string)4.2' ! \
h264parse ! queue ! \
filesink location=v_fifo