# Arduino-Powered Thermochromic Printer

![License](https://img.shields.io/badge/license-MIT-blue.svg)
![Arduino Compatible](https://img.shields.io/badge/compatible-Arduino%20UNO-green.svg)
![Version](https://img.shields.io/badge/version-1.0.0-brightgreen.svg)

## What It Does

The **Arduino-Powered Thermochromic Printer** reduces paper waste by using thermochromic ink that can be erased and reused. By precisely controlling heat, the printer erases existing text on thermochromic paper, allowing the sheets to revert to a clean state for future use.

## How It Works

- **Motor Control:** When the user enters the `start` command via the serial console, the Arduino activates a motor that mechanically interacts with the thermochromic paper to erase the text.
- **Temperature Adjustment:** Users can set the desired temperature by entering commands like `temp 100`. A PID controller ensures the heating element reaches and maintains this temperature accurately, enabling effective erasure of the thermochromic ink.
- **Eco-Friendly Operation:** By reusing paper instead of discarding it, the printer promotes sustainability and reduces environmental impact.

## Purpose

This project was developed for **BearHacks**, an eco-friendly hackathon, with the goal of creating a sustainable and low-cost alternative for everyday printing needs. By addressing paper waste, the Arduino-Powered Thermochromic Printer aligns with the hackathon's theme of promoting environmentally conscious technology solutions.

## Results

This project eneded up winning **🥇- 1st Place** 

---

*This project is licensed under the [MIT License](LICENSE).*
