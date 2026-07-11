# Mini Weather Station

An Arduino-based mini weather station that measures **humidity**, **temperature**, and **air pressure**, and forecasts upcoming weather using the **Zambretti algorithm**. Readings are shown on an OLED display and printed to the Serial monitor.

## Features

- Reads relative humidity and temperature from a **DHT11** sensor.
- Reads barometric pressure, temperature, and altitude from a **BMP085/BMP180** sensor.
- Tracks sea-level pressure over a rolling ~3-hour window to detect whether pressure is **rising**, **falling**, or **steady**.
- Applies the **Zambretti forecasting algorithm** to translate the pressure trend into one of 32 human-readable forecasts (e.g. *"Settled Fine"*, *"Rain at Times, Worse Later"*).
- Displays live readings and the forecast on a 128×64 **SSD1306 OLED** over I²C.

## Hardware

| Component | Purpose | Connection |
|-----------|---------|------------|
| Arduino (Uno/Nano) | Microcontroller | — |
| DHT11 | Humidity & temperature | Data → `A0` |
| BMP085 / BMP180 | Barometric pressure | I²C (`SDA`/`SCL`) |
| SSD1306 OLED (128×64) | Display | I²C, address `0x3C` |

## Libraries

Install these via the Arduino Library Manager:

- `Adafruit_BMP085`
- `DHT` (the `dht` library used by `DHT.read11()`)
- `Adafruit_GFX`
- `Adafruit_SSD1306`
- `Wire` and `SPI` (bundled with the Arduino IDE)

## Getting Started

1. Wire the sensors and display as described in the table above.
2. Open `Weather_Station.ino` in the Arduino IDE.
3. Install the required libraries.
4. Select your board and port, then upload.
5. Open the Serial Monitor at **9600 baud** to view live readings; the forecast also appears on the OLED.

## How the Forecast Works

The sketch stores sea-level pressure samples over time and compares the current pressure against earlier samples to classify the trend (rising / falling / steady). The Zambretti index `z` is then computed from the pressure and trend, and mapped to a descriptive forecast string. Note that Zambretti was designed for the Northern Hemisphere and assumes reasonably calibrated sea-level pressure, so treat the output as an indicative forecast rather than a precise prediction.

## Notes

This is a hobby/educational project. Pressure thresholds and the calibration constants in the sketch may need tuning for your specific sensor and location.
