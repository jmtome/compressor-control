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
| 1    | **I²C Temp/Humidity sensor** (e.g. SHT31, AHT20) | Shares I²C bus with LCD                                     |
| 1    | **NTC Thermistor**                               | Simulated with potentiometer on *A0* for now                |
| 1    | **Vacuum sensor** (analog)                       | Simulated with potentiometer on *A1*                        |
| 2    | **LEDs + 220 Ω resistors**                       | Visual placeholders for compressor *START* and *ON* outputs |
| Misc | Breadboard & jumpers                             | For rapid prototyping                                       |

---

## Pin Map

| Function               | Arduino Pin | Direction      |
| ---------------------- | ----------- | -------------- |
| **I²C SDA**            | A4          | Bi‑directional |
| **I²C SCL**            | A5          | Bi‑directional |
| NTC (analog)           | A0          | Input          |
| Vacuum sensor (analog) | A1          | Input          |
| Compressor START LED   | D6          | Output         |
| Compressor ON LED      | D7          | Output         |

> *Digital D0/D1 (hardware Serial) and D13 (on‑board LED) are kept free for debugging and bootloader reliability.*

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

### Why `compressor_controller.ino`?

Arduino IDE expects the sketch file name to match its parent folder. Naming it `compressor_controller` keeps the purpose clear while avoiding spaces and special characters. If you change the directory name, rename the `.ino` to match.

---

## Commit Strategy

We follow **small, verifiable steps**. Suggested commit messages:

1. **chore: add base project structure** – libraries, constants, empty `setup()`/`loop()`
2. **feat: initialize I²C LCD** – `lcd.init()` & splash screen
3. **feat: configure compressor output pins** – set D6/D7 as outputs, default LOW
4. **feat: read mock analog sensors** – map A0/A1 to °C & vacuum units
5. **feat: display mock data on LCD** – show temperature, humidity placeholder, vacuum

> Push after each commit so you can test & revert easily.

---

## Quick‑Start Instructions

1. **Clone** or create the repo and open `src/compressor_controller.ino` in Arduino IDE.
2. **Select Board** → *Arduino Uno* & correct **Port**.
3. **Wire components** per *Pin Map* above (use pots for A0/A1).
4. **Upload** Commit #2 to verify the LCD boots and displays text.
5. Continue through commits, verifying each stage on hardware.

---

## Simulating Sensors

Until real sensors arrive:

- Connect a potentiometer between **+5 V ↔ A0 ↔ GND** to mimic the NTC.
- Another potentiometer between **+5 V ↔ A1 ↔ GND** mimics the vacuum sensor.

Turning the knobs will change the values displayed on the LCD, letting you validate the analog pipeline.

---

## Future Work / TODO

- Integrate actual I²C temp/humidity sensor driver and replace mock humidity.
- Implement compressor startup logic with appropriate delays and safety checks.
- Add EEPROM persistence for configuration thresholds.
- Implement error handling & alerts on the LCD.
