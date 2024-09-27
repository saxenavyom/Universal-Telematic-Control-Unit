# Universal Telematic Control Unit

The Universal Telematic Control Unit (TCU) is a comprehensive vehicle monitoring system that integrates with the OBD-II port of any vehicle. It uses an STM32 Bluepill microcontroller to collect real-time data, process it, and transmit it to a remote server for analysis. The system supports GPS tracking, motion sensing, and various wireless connectivity options, making it ideal for fleet management and vehicle performance analysis.

## Features

- **Universal Compatibility:** Integrates with any vehicle using the OBD-II port
- **Real-time Data Monitoring:** Captures vehicle data such as speed, engine performance, and location
- **GPS Tracking:** Provides real-time vehicle location tracking
- **Motion Sensing:** Uses MEMS sensors to monitor vehicle movements
- **Wireless Connectivity:** Supports WiFi, BLE, and LTE for flexible data transmission
- **Data Transmission:** Uses MQTT protocol for secure communication to a Node.js server
- **Data Storage:** Stores collected data in a MongoDB database for further analysis
- **User Interface:** Provides a React.js frontend for visualizing vehicle data and analytics

## Technologies Used

- **Microcontroller:** STM32 Bluepill
- **Sensors:** MEMS for motion sensing, GPS module for location tracking
- **Programming Languages:** C/C++ for microcontroller, JavaScript for frontend/backend
- **Backend:** Node.js server with MongoDB database
- **Frontend:** React.js for data visualization
- **Communication Protocol:** MQTT for data transmission

## Getting Started

### Prerequisites

- STM32 Bluepill microcontroller
- OBD-II cable and connector
- GPS module, MEMS sensors
- WiFi/BLE/LTE module for wireless connectivity
- Node.js, MongoDB, React.js environment for server and frontend setup

### Installation

1. Clone the repository:
   ```bash
   git clone https://github.com/saxenavyom/Universal-Telematic-Control-Unit.git
   ```
2. Set up the hardware by connecting the STM32 Bluepill to the OBD-II port, GPS, and MEMS sensors.
3. Flash the microcontroller code using STM32CubeIDE or Arduino IDE.
4. Install the Node.js server and MongoDB for backend data handling:
   ```bash
   cd server
   npm install
   npm start
   ```
5. Set up the React.js frontend:
   ```bash
   cd client
   npm install
   npm start
   ```

### Usage

- Once everything is set up, the TCU will start collecting vehicle data and transmit it to the server.
- Access the React.js frontend to view real-time data, analytics, and visualizations.

## Project Structure

- `microcontroller/`: Source code for the STM32 Bluepill
- `server/`: Node.js backend and MongoDB configuration
- `client/`: React.js frontend code

## Contributing

Contributions are welcome! Feel free to open an issue or submit a pull request if you have suggestions or improvements.


## Contact

For any questions or inquiries, reach out to me at vyom.saxena.77@gmail.com.
