# rov-main-computer

This is the main computer running on a Raspberry Pi 3b+ of my underwater remotely operated vehicule (ROV).
The server is implemented in C++ and it listens to sockets comming from an ethernet cable connected to a laptop.
The incoming bytes are used to control the ROV and the bytes sent contain the telemetry data comming from an Arduino connected over SPI.
A livestream from a Raspberry Pi Camera is also started once the connection with the client is established using MJPG-Streamer.
