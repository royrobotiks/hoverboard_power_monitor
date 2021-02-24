# Hoverboard Power Monitor

The **hoverboard power monitor** is a volt-, current- and power-meter. Measurements are visualized as absolute numbers and in the form of an oscillogram on a little TFT display. The device can log data on an SD-card with a temporal resolution of about 10Hz.

The power monitor is programmed with the Arduino IDE 1.8.11. An additional visualizer is programmed in Processing 3.5.4. The visualizer renders an image sequence with correct timing (e.g. 25 FPS) based on a logfile. The image sequence can be superimposed on a video in order to analyze the power consumption and battery behaviour (Voltage drops) at every moment of the ride. 

![alt text](https://github.com/royrobotiks/hoverboard_power_monitor/blob/main/images/hoverboard_power_monitor.gif "hoverboard ride with superimposed power data")

