# LVGL Code for Wio Terminal to display data from SignalK via MQTT🖥️

This repository provides code to diaplay Engine data from SignalK via MQTT. Graphics is done via lvgl. This is provided as-is and should
be used as an example of how to display MQTT data using lvgl graphics widgets.

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
