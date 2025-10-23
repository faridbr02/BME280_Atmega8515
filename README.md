<h1 align="center">Environmental Data Monitoring System using BME280</h1>

<p align="center">
A bare-metal embedded system project using <b>ATmega8515</b> and <b>BME280</b> environmental sensor, featuring <b>SPI communication</b> and <b>UART serial interface</b>.
</p>

<p align="center">
  <a href="#"><img alt="Microcontroller Badge" src="https://img.shields.io/badge/Microcontroller-ATmega8515-blue?style=flat-square&logo=microchip"></a>
  <a href="#"><img alt="Language Badge" src="https://img.shields.io/badge/Language-C%20Embedded-green?style=flat-square&logo=c"></a>
  <a href="#"><img alt="Simulation Badge" src="https://img.shields.io/badge/Simulation-Proteus-orange?style=flat-square&logo=proteus"></a>
  <a href="#"><img alt="SPI Badge" src="https://img.shields.io/badge/Protocol-SPI-yellow?style=flat-square&logo=spi"></a>
  <a href="#"><img alt="UART Badge" src="https://img.shields.io/badge/Interface-UART-lightgrey?style=flat-square&logo=serialstudios"></a>
</p>

<br>


<pre><code>
## System Architecture
Project Structure:
├── Drivers/          # Hardware abstraction layer
│   ├── SPI.h/c       # SPI communication
│   ├── BME280.h/c    # BME sensor driver
│   └── UART.h/c      # UART serial communication
└── main.c            # Application with state machine

</code></pre>

<b> Current Implementation</b>

- <b>SPI Driver**: Master mode communication with BME280 sensor
- <b>UART Interface**: Serial output at 9600 baud for monitoring
- <b>BME280 Driver**: Direct datasheet implementation without third-party libraries
- <b>Calibration Algorithms**: Official Bosch compensation formulas for accurate measurements

<b> Features</b>

- <b>Triple Environmental Monitoring</b> Temperature, pressure, and humidity sensing
- <b>Bare-Metal Implementation</b> Complete low-level control without external libraries
- <b>SPI Communication</b> High-speed synchronous data transfer
- <b>Serial Data Output</b> Real-time monitoring through UART
- <b>Modular Driver Architecture</b> Reusable and maintainable code structure
- <b>Error Handling</b> Basic error detection and reporting
- <b>Proteus Simulation Compatible</b> Test without physical hardware

<b> Core Technologies</b>

- <b>UART</b> Serial communication protocol for debugging and data output
- <b>SPI</b> High-speed synchronous serial protocol for sensor communication
- <b>BME280 Calibration</b> Mathematical compensation algorithms converting raw sensor data to accurate environmental readings

<b> Setup Instructions</b>

1. Open project in MPLAB X IDE
2. Add all files to your project (include Drivers folder in include paths)
3. Build for ATmega8515 microcontroller (8MHz clock)
4. Load HEX file to Proteus simulation or hardware
5. Connect virtual terminal to UART (9600 baud, 8N1)

<b> Future Enhancements</b>

- <b>Data Logging</b> Environmental data history and statistics
- <b>LCD Display</b> Local visualization of measurements
- <b>Wireless Communication</b> Bluetooth or WiFi data transmission
- <b>Power Management</b> Low-power modes for battery operation

<b> Learning Objectives</b>

This educational project serves as a practical platform for:

- <b>Embedded C programming</b> for AVR microcontrollers
- <b>SPI protocol implementation</b> and timing control
- <b>UART communication</b> for system monitoring
- <b>Sensor driver development</b> from datasheet specifications
- <b>Hardware calibration algorithms</b> and signal processing
- <b>Bare-metal programming</b> without RTOS dependencies
- <b>Environmental sensing</b> and data interpretation

<b> Simulation</b>

A Proteus simulation file is included to test the system without physical hardware. The simulation demonstrates real-time environmental monitoring through a virtual terminal displaying temperature, pressure, and humidity readings.

---

<b>Note</b> This project is developed from scratch for educational purposes, implementing direct datasheet specifications without third-party libraries as part of my embedded systems learning journey. Feedback and suggestions for improvement are welcome!