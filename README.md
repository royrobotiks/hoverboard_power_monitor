# Hoverboard Power Monitor

The hoverboard power monitor is a volt-, current- and power-mete. The measurements are visualized as absolute numbers and in the form of an oscillogram.
The data can log data on an SD-card with a temporal resolution of ca. 10Hz.

The device is programmed with the Arduino IDE 1.8.11. An additional visualizer is programmed in Processing 3.5.4. The visualizer renders an image sequence with correct timing (e.g. 25 FPS) based on the logged data. The image sequence can be superimposed on a video in order to analyze the current flow / Watts at every moment. 

[![Watch the video](https://niklasroy.com/hoverhack/videos/power_monitor_overlay.mp4)]
