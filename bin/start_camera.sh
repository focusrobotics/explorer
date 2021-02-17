#!/bin/sh

# mount a ramdisk on /mnt/ramdisk. This assumes you already did a mkdir /mnt/ramdisk.
sudo mount -t tmpfs none /mnt/ramdisk

# run raspistill in timelapse mode so that a new image is created every 200 ms for 600 seconds
raspistill -vf -hf -w 640 -h 480 -o /mnt/ramdisk/snapshot.jpg -tl 200 -t 600000

