# Release-0.0.1
This release introduces a client-side UI that monitors the 'openness' of the Oxygen valve and obtains thrust measurements from the load cell.

Load cell measurements are displayed using an ASCII chart, with a rate of one HTTP GET request per second, maintaining a record of the last 100 measurements.

## Known Bugs
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
-- Search engine for embeded development needs
- ESP32 Tutorials - https://esp32io.com/tutorials/esp32-button-toggle-led
