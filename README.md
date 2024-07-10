### ExternalWebRadar Documentation

#### Overview
**ExternalWebRadar** is a simple localhost HTTP web radar tool designed to provide basic radar-like functionalities for web applications. This project is implemented using C++ and provides a web interface for visualization.

#### Table of Contents
1. [Getting Started](#getting-started)
2. [Installation](#installation)
3. [Usage](#usage)
4. [Configuration](#configuration)
5. [Features](#features)
6. [Contribution](#contribution)
7. [License](#license)
8. [Contact](#contact)

#### Getting Started
To get started with the **ExternalWebRadar** project, you need to clone the repository and set up the development environment.

##### Prerequisites
- C++ Compiler (supporting C++17 or later)
- CMake (version 3.10 or later)
- Git
- Web Browser (for accessing the radar interface)

##### Clone the Repository
```bash
git clone https://github.com/MMVR1337/ExternalWebRadar.git
cd ExternalWebRadar
```

#### Installation
Follow these steps to build and run the project:

1. **Generate Build Files**
    ```bash
    cmake -S . -B build
    ```

2. **Build the Project**
    ```bash
    cmake --build build
    ```

3. **Run the Executable**
    ```bash
    ./build/ExternalWebRadar
    ```

#### Usage
Upon running the executable, the web radar interface will be available on `http://localhost:8080`. Open your web browser and navigate to this address to access the radar interface.

##### Key Functionalities
- **Real-time Data**: Visualize data in real-time on the radar interface.
- **Customization**: Adjust radar parameters through the web interface.

#### Configuration
Configuration options are available in the `config.json` file located in the root directory of the project. You can customize the following parameters:

- **port**: The port on which the web server runs (default: 8080).
- **refresh_rate**: The rate at which the radar data is refreshed (default: 1 second).

Example `config.json`:
```json
{
    "port": 8080,
    "refresh_rate": 1
}
```

#### Features
- **Simple Setup**: Easily set up and run the radar on your local machine.
- **Web Interface**: Intuitive web interface for visualizing radar data.
- **Configurable**: Customize settings through a configuration file.

#### Contribution
Contributions are welcome! Please follow these steps to contribute:

1. **Fork the Repository**
2. **Create a Feature Branch**
    ```bash
    git checkout -b feature/YourFeature
    ```
3. **Commit Your Changes**
    ```bash
    git commit -m "Add some feature"
    ```
4. **Push to the Branch**
    ```bash
    git push origin feature/YourFeature
    ```
5. **Open a Pull Request**

#### License
This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for more information.

#### Contact
For any questions or suggestions, feel free to open an issue or contact the repository owner at [MMVR1337](https://github.com/MMVR1337).

---
