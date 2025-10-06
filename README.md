# Linux Final Project — System Monitoring

**Author:** Phan Vu Huynh Tuan  
**Version:** 1.0  
**Date:** 04.10.2025  

---

## 📖 Project Overview
<p align="center">
<img src="00_Common\SystemPicture.jpg" alt="System Architecture">
</p>

This project implements a **real-time system monitoring and analysis application** for embedded Linux devices such as Raspberry Pi or BeagleBone.  
It monitors key system metrics like **CPU usage**, **memory consumption**, **temperature**, and **network bandwidth**, displaying them through a lightweight **Qt-based GUI**.

### **Hardware Setup**
- **Development board:** Raspberry Pi Zero 2W  
- **Display:** 2.8” LCD TFT (ILI9341, no touch)  
- **Other components:** Custom case, mini breadboard  
- **Operating system:** Raspberry Pi OS (Kernel 6.12.25)  
- **Qt Framework:** Version 6.4.2  

---

## 🧩 System Architecture

<p align="center">
<img src="01_ArchitectureDesign\SystemArchitecture.png" alt="System Architecture">
</p>
The system consists of two main components:

1. **Driver** — A Linux framebuffer driver enabling user-space communication with the LCD through SPI and GPIO.  
2. **Application** — A Qt-based GUI for real-time display of system metrics.

- **SPI** handles data transfer.  
- **GPIO** controls reset and mode switching (command/data).  
- The driver exposes `/dev/fb1` for the application to render directly.  

---

## 📂 Project Folder Structure

The repository is organized for easy navigation and modular development.
```
LinuxFinalProject  
├── .vscode                  # VS Code configuration for FTP and remote deployment to Raspberry Pi  
├── 00_Common                # Common resources such as images, templates, and shared documents  
├── 01_ArchitectureDesign    # System architecture diagrams and design specifications  
├── 02_Documents             # Project documents, reports, and development notes  
├── 03_Code                  # Main source code of the project  
│   ├── 00_Driver            # Linux driver source code handling hardware interfaces  
│   ├── 01_Application       # Application code including testing modules and Qt GUI  
│   │   └── QTProject        # Qt-based GUI project  
│   │       ├── inc          # Header files (.h)  
│   │       ├── qrc          # Qt resource files (.qrc)  
│   │       │   └── img      # Image files used by the Qt resources  
│   │       ├── src          # C++ source files (.cpp)  
│   │       └── ui           # UI design files (.ui) created with Qt Designer  
│   └── 02_DeviceTree        # Device tree files for Raspberry Pi hardware configuration  
└── 04_RPi0_Case             # 3D drawings and models of the Raspberry Pi Zero case
```

### **Folder Descriptions**

- **.vscode/** – VS Code workspace and FTP deployment setup for Raspberry Pi.  
- **00_Common/** – Shared images, templates, and resources.  
- **01_ArchitectureDesign/** – Architecture diagrams and design specifications.  
- **02_Documents/** – Reports, guides, and test results.  
- **03_Code/** – Source code for drivers, applications, and device trees.  
- **04_RPi0_Case/** – Mechanical and 3D case design files.  

---

## ⚙️ Driver Specification

### **Driver Architecture**
The driver implements a **framebuffer interface** for LCD communication via SPI.  
It has two main components:
- **Framebuffer Handling** — Manages virtual memory and deferred I/O for efficient data transfer.  
- **Data Transfer Handling** — Configures and transmits data over SPI.

**Configuration summary:**
- SPI speed: 36 MHz  
- SPI mode: 0  
- DMA: Not used  
- Deferred I/O delay: 20 ms  
- Virtual memory size: 320 × 240 × 2 bytes (rounded to page size)

**Operation flow:**
- Probe the SPI device.  
- Initialize and configure the LCD.  
- Create `/dev/fb1` framebuffer interface.  
- Application writes pixel data directly to `/dev/fb1`.

### **Device Tree**
Defines the LCD and GPIO setup:
- **fragment@0 (SPI):**  
  - Compatible: `"customer,ili9341"`  
  - SPI0 bus, max frequency: 40 MHz  
  - GPIO24 (DC) and GPIO25 (RST) pins  
- **fragment@1 (GPIO):**  
  - Configures GPIO24 and GPIO25 as outputs for LCD control  

---

## 🖥️ Application Specification

### **Architecture**
The Qt application includes:
- **Main Thread:** Handles UI and window management  
- **Data Thread:** Gathers CPU, RAM, temperature, and network data  

Communication between threads uses **Qt signals and slots** for real-time updates.

### **Development Workflow**
1. Design and test UI on Windows using simulated data.  
2. Implement real-time features for Raspberry Pi.  
3. Optimize code for performance and readability.  

**Framework:** Qt Widgets (lightweight, no GPU acceleration required)

---

## 🪟 Application Windows

### **Main Window**
Displays essential system information:
- Device IP address  
- CPU and RAM usage meters  
- CPU temperature  
- Date, time, and network bandwidth  

Clickable elements allow navigation to other detailed windows.

### **CPU Window**
Shows detailed CPU information:
- Per-core usage graph  
- Total CPU usage, load average, and temperature  
- “Back” button to return to Main Window  

### **Memory Window**
Displays:
- RAM and storage usage (custom widgets)  
- Detailed RAM and swap data  
- Storage info (device, type, capacity)  
- “Back to Home” button  

### **Time Window**
Shows:
- Date and time  
- CPU and RAM usage bars  
- Network bandwidth  
- “Back to Home” button  

All windows stay active in memory for quick reopening and are only destroyed when the app closes.

---

## 🧠 Summary

This project provides a complete embedded Linux solution that:
- Monitors real-time system performance  
- Visualizes resource data through a lightweight Qt interface  
- Integrates a custom framebuffer driver for LCD output  
- Runs efficiently on limited hardware (Raspberry Pi Zero 2W)  

---

