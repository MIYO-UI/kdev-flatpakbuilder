# KDevelop Flatpak Builder Plugin

A plugin for KDevelop IDE that integrates Flatpak building capabilities directly into your development workflow. Build, test, and deploy your applications as Flatpak packages without leaving your IDE.

[![KDE Frameworks 5](https://img.shields.io/badge/KDE%20Frameworks-5-blue.svg)](https://kde.org/products/frameworks/)
[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)

![Screenshot of KDevelop Flatpak Builder](doc/images/screenshot1.png)

## Features

- **Build Flatpak packages** directly from KDevelop
- **Install** your Flatpak packages locally for testing
- **Export** standalone Flatpak bundles (`.flatpak` files) for distribution
- **Create and edit** Flatpak manifests with smart templates
- **Syntax highlighting** for Flatpak manifest files
- **User-friendly output** with colorized build logs and error detection
- **Project integration** - detects existing Flatpak manifests automatically

## Requirements

- KDevelop 5.6 or later
- KDE Frameworks 5.44 or later
- Qt 5.12 or later
- Flatpak and flatpak-builder packages installed on your system

## Installation

### From Source

```bash
# Clone the repository
git clone https://github.com/yourusername/kdev-flatpakbuilder.git
cd kdev-flatpakbuilder

# Create build directory
mkdir build && cd build

# Configure and build
cmake -DCMAKE_BUILD_TYPE=Release ..
make

# Install
sudo make install
```

### Distribution Packages

#### Arch Linux
```bash
yay -S kdevelop-flatpakbuilder
```

#### Fedora
```bash
sudo dnf install kdevelop-flatpak-builder
```

#### Ubuntu/Debian
```bash
sudo apt install kdevelop-flatpak-builder
```

## Usage

### Enabling the Plugin

After installation, open KDevelop and go to:
1. Settings → Configure KDevelop...
2. Plugins
3. Find "Flatpak Builder" in the list and check the box
4. Click "Apply"

### Creating a Flatpak Manifest

1. Open your project in KDevelop
2. Go to "Project" → "Flatpak" → "Create Manifest"
3. The plugin will generate a default manifest based on your project structure
4. Edit the manifest to adjust settings as needed

### Building a Flatpak Package

1. Open your project in KDevelop
2. Go to "Project" → "Flatpak" → "Build Flatpak"
3. The build process will start and display progress in the output view
4. After successful build, you can install or export the package

### Installing and Testing

1. After building, go to "Project" → "Flatpak" → "Install Flatpak"
2. The package will be installed for your user account
3. You can now run and test the application

### Exporting a Bundle

1. After building, go to "Project" → "Flatpak" → "Export Bundle"
2. The plugin will create a `.flatpak` file that can be distributed and installed on other systems

## Configuration

The plugin can be configured through:

1. Settings → Configure KDevelop...
2. Project Configuration → Flatpak Builder

Available settings include:
- Path to flatpak-builder executable
- Path to flatpak executable
- Default build directory
- Custom build options

## Troubleshooting

### Common Issues

#### "flatpak-builder not found"
Make sure flatpak-builder is installed on your system:
```bash
# For Fedora/RHEL
sudo dnf install flatpak-builder

# For Ubuntu/Debian
sudo apt install flatpak-builder

# For Arch Linux
sudo pacman -S flatpak-builder
```

#### "Permission denied" errors
Make sure you have appropriate permissions to the build directory:
```bash
chmod -R 755 ~/.cache/flatpak-builder
```

#### "No Flatpak manifest found for this project"
Use the "Create Manifest" option to generate a manifest file for your project.

## Development

### Project Structure

```
kdev-flatpakbuilder/
├── CMakeLists.txt
├── kdevflatpakbuilder.desktop
├── kdevflatpakbuilder.json
├── kdevflatpakbuilder.rc
├── src/
│   ├── flatpakbuilderplugin.h/cpp
│   ├── flatpakbuilderconfig.h/cpp
│   ├── flatpakmanifestmanager.h/cpp
│   ├── flatpakbuildoutputparser.h/cpp
│   ├── flatpakbuilderjob.h/cpp
│   └── ui/
│       └── flatpakbuilderconfigwidget.h/cpp/ui
└── po/
    ├── en.po
    └── pl.po
```

### Contributing

Contributions are welcome! Please follow these steps:

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Make your changes
4. Run tests to ensure everything works
5. Commit your changes (`git commit -m 'Add amazing feature'`)
6. Push to the branch (`git push origin feature/amazing-feature`)
7. Open a Pull Request

Please follow the KDE Coding Style and make sure to add appropriate documentation.

## License

This project is licensed under the GNU General Public License v3.0 - see the [COPYING](COPYING) file for details.

## Acknowledgments

- KDE Community for the KDevelop platform
- Flatpak team for their excellent application containerization technology
- All contributors who have helped improve this plugin