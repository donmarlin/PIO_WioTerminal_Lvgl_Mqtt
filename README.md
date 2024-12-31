# LVGL Starter Code for Wio Terminal 🖥️

This repository provides a starter code for using LVGL (Lightweight GUI Library) with the Wio Terminal, a versatile development board by Seeed Studio. The code demonstrates the basic configuration and setup required to get LVGL up and running on the Wio Terminal using the Arduino framework and PlatformIO.

## Prerequisites 🛠️

1. **Hardware**:
   - Seeed Wio Terminal

2. **Software**:
   - [Visual Studio Code](https://code.visualstudio.com/) with the [PlatformIO extension](https://platformio.org/platformio-ide) installed.

## Getting Started 🚀
   - git clone https://github.com/Ryan-py/lvgl-wio-terminal-starter.git

2. **Open the Project in Visual Studio Code**:
- Launch Visual Studio Code and open the `lvgl-wio-terminal-starter` folder.

3. **Copy the `lv_conf.h` file**:
- The `lv_conf.h` file is required for configuring LVGL. You can find a sample `lv_conf.h` file in the LVGL repository.
- Copy the `lv_conf.h` file to the `.pio/libdeps/seeed_wio_terminal` directory in your project.

4. **Build and Upload the Code**:
- Connect your Wio Terminal to your computer.
- In the PlatformIO toolbar, click the "Build" and "Upload" buttons to compile and upload the code to your Wio Terminal.

## Code Overview 📚

The provided code includes the following key components:

1. **LVGL Initialization** 🔧
- The `setup()` function initializes LVGL, sets up the display driver, and registers the display driver with LVGL.

2. **Display Flushing** 🖥️
- The `my_flush()` function is used as the display flush callback, allowing LVGL to update the display with the rendered graphics.

3. **Event Handling** 🕹️
- The `loop()` function calls `lv_timer_handler()` to let LVGL handle any events and update the display accordingly.

4. **Logging (optional)** 📝
- If `USE_LV_LOG` is enabled, the `my_print()` function is used as the log printing callback for LVGL.

## Next Steps �ahead

Now that you have the starter code set up, you can start building your LVGL-based user interface for the Wio Terminal. You can explore the LVGL documentation and examples to learn more about the library's features and how to create custom widgets and applications.

Happy coding! 💻
