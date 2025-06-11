# Arduino Compressor Controller Skeleton

## Project Purpose

This repository contains the **bare‑bones firmware and wiring plan** for an Arduino Uno‑based controller that will ultimately monitor temperature, humidity and vacuum, and drive a two‑stage compressor.

*Current phase*: infrastructure only (no control logic).

---

## Hardware Bill of Materials

| Qty  | Component                                        | Notes                                                       |
| ---- | ------------------------------------------------ | ----------------------------------------------------------- |
| 1    | **Arduino Uno R3** (ATmega328P)                  | Main MCU board                                              |
| 1    | **16×2 LCD with I²C backpack**                   | Typical address `0x27` or `0x3F`                            |
| 1    | **I²C Temp/Humidity sensor** (e.g. SHT30/SHT31)  | Shares I²C bus with LCD                                     |
| 1    | **NTC Thermistor**                               | Simulated with potentiometer on *A0* for now                |
| 1    | **Vacuum sensor** (analog)                       | Simulated with potentiometer on *A1*                        |
| 2    | **LEDs + 220 Ω resistors**                       | Visual placeholders for compressor *START* and *ON* outputs |
| Misc | Breadboard & jumpers                             | For rapid prototyping                                       |

---

## Detailed Pin Map

| **Component**               | **Arduino Pin(s)**  | **Type**           | **Direction** | **Notes** |
|----------------------------|---------------------|--------------------|---------------|-----------|
| **NTC Thermistor**         | A0                  | Analog             | Input         | Simulated with potentiometer |
| **Vacuum Sensor**          | A1                  | Analog             | Input         | Simulated with potentiometer |
| **SHT30/31 Temp/Humidity** | A4 (SDA), A5 (SCL)   | I²C (digital)      | Input         | Shares I²C bus with LCD |
| **16×2 LCD Display**       | A4 (SDA), A5 (SCL)   | I²C (digital)      | Output        | I²C backpack required |
| **Compressor START**       | D6                  | Digital            | Output        | Connected to LED + resistor or relay |
| **Compressor ON**          | D7                  | Digital            | Output        | Connected to LED + resistor or relay |

> ✅ **I²C is a shared bus** — A4 and A5 are used by both the LCD and SHT30/31. No conflict occurs as long as I²C addresses are different (`0x27` for LCD, `0x44` or `0x45` for SHT30/31).

---

## Free and Reserved Pins

| **Pin(s)**     | **Status**   | **Suggested Use**         |
|----------------|--------------|----------------------------|
| D0–D1          | Reserved      | UART (Serial comm)         |
| D2–D5, D8–D13  | Free          | Available for expansion    |
| A2–A3          | Free          | Extra analog sensors       |
| A6–A7          | Not present   | Not on DIP Uno, SMD only   |

---

## Block Diagram

![Block Diagram](block_diagram.png)

---

## Repository Layout

```
.
├── src/
│   └── compressor_controller.ino   ← main firmware file
└── README.md                       ← this file
```


## Required Arduino Libraries

Before compiling, make sure to install the following library:

- **LiquidCrystal_I2C** by **Frank de Brabander**  
  - Open the Arduino IDE  
  - Go to **Tools > Manage Libraries...**  
  - Search for `LiquidCrystal_I2C`  
  - Install the version by **Frank de Brabander**

---

## Quick‑Start Instructions

1. **Clone or download** this repository to your local machine.  
2. Open `src/compressor_controller.ino` in the **Arduino IDE**.  
3. **Install required libraries** (see above).  
4. Go to **Tools > Board** and select **Arduino Uno**.  
5. Go to **Tools > Port** and select the correct serial port.  
6. **Wire components** according to the *Pin Map* section.  
   - Simulate NTC and Vacuum sensors using potentiometers on A0 and A1.  
7. Compile and upload the sketch.  
   - After Commit #2, you should see a splash screen on the LCD ("System Booting").  
8. Continue through each commit step to build functionality incrementally.


---

## Simulating Sensors

Until real sensors arrive:

- Use a potentiometer between **+5 V ↔ A0 ↔ GND** for NTC simulation
- Use a potentiometer between **+5 V ↔ A1 ↔ GND** for vacuum simulation

---

## I²C Address Reference

| Device          | Default Address |
|-----------------|------------------|
| 16x2 LCD        | `0x27` or `0x3F`  |
| SHT30/31 Sensor | `0x44` or `0x45`  |
