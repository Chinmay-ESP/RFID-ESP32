# ESP32-RFID Reading & Publishing Using Micro-ROS in ESP-IDF

This repository demonstrates how to interface an RFID reader with an ESP32-S3 microcontroller and publish the RFID data to ROS2 using Micro-ROS.

---

## **RFID Connection with ESP32-S3**

| RFID Pin | ESP32-S3 Pin |
|----------|--------------|
| SDA      | GPIO 35      |
| SCLK     | GPIO 36      |
| MOSI     | GPIO 38      |
| MISO     | GPIO 37      |

---

## **Functionality**

1. The ESP32-S3 reads the UID from the RFID module and logs it to the serial monitor.
2. The UID data is published to ROS2 using Micro-ROS with an `int32` publisher.

---

## **Publishing to ROS2 with Micro-ROS**

To enable communication between the ESP32-S3 and ROS2, Micro-ROS is used. The following command runs the Micro-ROS agent in a Docker container:

```bash
docker run -it --rm --ipc host --network host --privileged microros/micro-ros-agent:humble serial -b 12000000 --dev /dev/ttyACM1
```

### **Steps to Publish Data:**
1. Connect the ESP32-S3 to your computer.
2. Ensure the Micro-ROS agent is running using the command above.
3. Monitor the published data in ROS2.

---

## **Requirements**

- **ESP-IDF Framework**: Install and configure the ESP-IDF for ESP32-S3 development.
- **Micro-ROS**: Ensure the Micro-ROS library is integrated into the ESP-IDF project.
- **Docker**: Required to run the Micro-ROS agent.
- **ROS2**: Installed on the host system (Humble Hawksbill).

---

## **Usage**

1. Clone the repository:
   ```bash
   git clone <repository-url>
   ```

2. Initialize and update submodules (if any):
   ```bash
   git submodule update --init --recursive
   ```

3. Build and flash the ESP32-S3 project:
   ```bash
   idf.py build
   idf.py flash
   idf.py monitor
   ```

4. Run the Micro-ROS agent on the host system (see command above).

---

## **Features**

- Real-time RFID data acquisition.
- Seamless integration with ROS2 through Micro-ROS.
- Lightweight and efficient implementation.

---
