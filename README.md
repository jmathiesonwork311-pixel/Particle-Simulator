# ☄️ Particle Simulator
**A high-performance graphical engine designed for massive-scale collision physics.**

![Example animation](/Animation.gif)

## 📌 Overview
This simulator is architected to handle high volumes of particles while maintaining real-time performance. It utilizes **OpenGL** for hardware-accelerated rendering and **GCC** for optimized computational throughput on Linux systems.

### Key Features
* **Elastic Collisions:** Realistic momentum and kinetic energy conservation.
* **Massive Scale:** Optimized to handle thousands of entities simultaneously.
* **Hardware Accelerated:** Leverages GPU rendering for smooth frame rates.

---

## 🛠 Prerequisites
Ensure your Linux environment has the following dependencies installed:

* **GCC:** The GNU Compiler Collection.
* **OpenGL:** Development libraries (e.g., `libgl1-mesa-dev`).
* **Make:** Build automation tool.

### Quick Install (Debian/Ubuntu)
```bash
sudo apt-get update
sudo apt-get install build-essential libgl1-mesa-dev libglu1-mesa-dev freeglut3-dev
