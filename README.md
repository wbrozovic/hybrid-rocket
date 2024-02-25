# Release-0.0.2
This release introduces the ability to log to an SD card along with some performance enhancements. Also in a continuous process of cleaning up code.

## Known Bugs
- Miscellaneous UI enhancements are required.
- Continuous readability improvements
### Resolved
- ESP32 Crash during thrust load measurement
  - When taking thrust measurements at 0.5s or 1s intervals while simultaneously rotating the Oxygen valve, the ESP32 becomes overloaded and crashes.
  - Potential fix:
    - Decrease the number of client-side requests by consolidating all sensor values into a single object on the server side, which can then be requested by the client side via HTTP GET. This should minimize the number of requests the ESP32 needs to process. Moreover, improve JavaScript code to manage promises effectively, and avoid requesting additional data until the current promise is resolved.
- Miscellaneous UI enhancements are required.

# hybrid-rocket
This is the code and documentation repo for the Hybrid Rocket project at the Dallas Maker Space for the Science Committe.

Please create a branch with the naming convention [your_name or nickname or initials]/[feature-name] 


# Resources

- Platform.io - https://platformio.org/
  - Search engine for embeded development needs
- Platformio for vscode - https://platformio.org/install/ide?install=vscode
  - Use this if you develop in vscode - it helps you compile and upload your code to the ESP32
- ESP32 Tutorials - https://esp32io.com/tutorials/esp32-button-toggle-led
- Using Multiple SPI busses & custom SPI Pins:  https://randomnerdtutorials.com/esp32-spi-communication-arduino/#custom-spi-pins
