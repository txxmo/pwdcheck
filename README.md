# pwdcheck

## Overview

`pwdcheck` is a password strength checker application designed to assess the strength of passwords based on various criteria. It provides a graphical user interface for users to input passwords and receive real-time feedback on their strength.

## Features

- **Password Strength Assessment**: Evaluate the strength of passwords based on length, character types, and additional criteria.
- **Real-time Feedback**: Receive instant feedback on the strength of passwords as they are entered.
- **MD5 and SHA-1 Hashing**: Generate MD5 and SHA-1 hashes for passwords for additional security verification.
- **Tray Icon Integration**: Minimize the application to the system tray for easy access and quick password checks.
- **Customizable Requirements**: Define custom password strength requirements to suit specific security policies.
- **Dependency on OpenSSL Binaries**: Utilizes OpenSSL binaries for cryptographic operations.

## Requirements

- **Operating System**: Compatible with Windows, macOS, and Linux.
- **OpenSSL Binaries**: Required for cryptographic operations.
- **Visual C++ Redistributable (Windows)**: Required for Windows systems if not statically linked.
- **Dependencies**: Ensure all required dependencies are installed and accessible on the system.

## Usage

1. **Installation**: Download and install the `pwdcheck` application.
2. **Run the Application**: Launch the `pwdcheck` executable.
3. **Input Passwords**: Enter passwords into the application window to assess their strength.
4. **Receive Feedback**: Receive instant feedback on password strength and cryptographic hashes.
5. **Minimize to Tray**: Optionally minimize the application to the system tray for easy access.

## Contributing

Contributions to `pwdcheck` are welcome! If you encounter any issues or have suggestions for improvements, please feel free to submit a pull request or open an issue on the GitHub repository.

## License

This project is licensed under the [MIT License](LICENSE).