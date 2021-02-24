# Hoverboard Power Monitor

I wanted to gain a better understanding of the power consumption of my hoverboard during different riding situations. For that reason, I built a **hoverboard power monitor**, which is a power meter and data logger. The device measures Amperes and Voltage and calculates the current power consumption in Watts. The measurements are shown as numbers and as oscillograms on a little TFT display. They can also be logged on an SD-card with a temporary resolution of about 10Hz. A built-in real-time clock makes it easy to match the data with a video clip of the ride.

The power monitor is plugged in between battery and hoverboard electronics with its XT60 connectors. It is itself powered by the hoverboard battery. Besides recording the power consumption of hoverboard or scooter rides, the device can also record the charging process, e.g. if you want to plot charging curves.

The hoverboard power monitor is built around an Arduino Nano and it is programmed with the Arduino IDE 1.8.11.

<img src="https://github.com/royrobotiks/hoverboard_power_monitor/blob/main/images/finished_device.jpg" alt="the finished device" width="32%" height="32%"> <img src="https://github.com/royrobotiks/hoverboard_power_monitor/blob/main/images/sd_slot.jpg" alt="SD card slot" width="32%" height="32%"> <img src="https://github.com/royrobotiks/hoverboard_power_monitor/blob/main/images/buttons.jpg" alt="button interface" width="32%" height="32%">

I've also programmed a visualizer in Processing 3.5.4. This program reads logfiles and creates a timing-correct sequence of png's, which can be superimposed onto a video clip. 

![alt text](https://github.com/royrobotiks/hoverboard_power_monitor/blob/main/images/hoverboard_power_monitor.gif "hoverboard ride with superimposed power data")
[Here's a longer version of the video above](https://niklasroy.com/hoverhack/videos/power_monitor_overlay.mp4)


## Building the power monitor

<img src="https://github.com/royrobotiks/hoverboard_power_monitor/blob/main/images/modules.jpg" alt="modules" width="64%" height="64%"> <img src="https://github.com/royrobotiks/hoverboard_power_monitor/blob/main/images/case.jpg" alt="case" width="32%" height="32%">



 


