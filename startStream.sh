#!/bin/sh
# manually starting a stream
mjpg_streamer -i 'input_raspicam.so -fps 30 -x 1080 -y 720 -ISO 3200' -o 'output_http.so -p 3000'
