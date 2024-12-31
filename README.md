# LVGL Code for Wio Terminal to display data from SignalK via MQTT🖥️

This repository provides code to diaplay Engine data from SignalK via MQTT. Graphics is done via lvgl. This is provided as-is and should
be used as an example of how to display MQTT data using lvgl graphics widgets.

This code is losely based on the following repository:
https://github.com/Ryan-py/lvgl-wio-terminal-starter.git
Thanks Ryan.

![IMG_20180719_034051](https://github.com/user-attachments/assets/3c1c7ae1-ac48-47a8-889a-a5204a65a27a)

## Prerequisites 🛠️

1. **Hardware**:
   - Seeed Wio Terminal

2. **Software**:
   - [Visual Studio Code](https://code.visualstudio.com/) with the [PlatformIO extension](https://platformio.org/platformio-ide) installed.

## Getting Started 🚀
   - git clone https://github.com/donmarlin/PIO_WioTerminal_Lvgl_Mqtt.git

2. **Open the Project in Visual Studio Code**:
- Launch Visual Studio Code and open the `PIO_WioTerminal_Lvgl_Mqtt` folder.

3. **Copy the `lv_conf.h` file**:
- The `lv_conf.h` file is required for configuring LVGL. You can find a sample `lv_conf.h` file in the LVGL repository.
- Copy the `lv_conf.h` file to the `.pio/libdeps/seeed_wio_terminal` directory in your project.

4. **Build and Upload the Code**:
- Connect your Wio Terminal to your computer.
- In the PlatformIO toolbar, click the "Build" and "Upload" buttons to compile and upload the code to your Wio Terminal.

Enjoy
