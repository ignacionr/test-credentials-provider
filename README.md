# Simple Credential Provider

A custom Windows Credential Provider that displays fields for a username and password, and authenticates the user against a local Windows account using the `KERB_INTERACTIVE_UNLOCK_LOGON` structure.

## Features

- Collects **Username** and **Password** on the Windows login screen.
- Authenticates users against local Windows accounts.
- Uses the `KERB_INTERACTIVE_UNLOCK_LOGON` for secure credential handling.
- Provides basic error feedback if login fails.
- Supports **Unicode** for wide character and internationalization support.

## Project Structure

```bash
project/
│
├── src/
│   ├── DllMain.cpp                  # Entry point for the DLL
│   ├── SimpleCredentialProvider.cpp  # Implements the Credential Provider interface
│   └── SimpleCredential.cpp          # Handles user input and serialization of credentials
│
├── include/                          # Optional: Directory for header files (if needed)
│
├── SimpleCredentialProvider.dll      # Output DLL generated after compilation
│
└── README.md                         # This readme file

Prerequisites

Visual Studio (2019 or later) or cl.exe compiler.

Windows SDK for Credential Provider support.


Compilation

Using cl.exe (Microsoft C++ Compiler)

1. Open Developer Command Prompt for Visual Studio.


2. Navigate to the project root directory:

cd path\to\project


3. Compile the project with the following command:

cl /LD /DUNICODE /D_UNICODE /I"include" src\DllMain.cpp src\SimpleCredentialProvider.cpp src\SimpleCredential.cpp /link /DLL /OUT:SimpleCredentialProvider.dll


4. The resulting SimpleCredentialProvider.dll will be created in the project directory.



Unicode Compilation

This project is natively Unicode. To ensure Unicode is used throughout the project, the /DUNICODE /D_UNICODE flags are used to compile it with wide character support. Ensure all string literals in the source code use the L prefix for Unicode strings (e.g., L"Example String").

Installation

1. Once you have the SimpleCredentialProvider.dll:

Register the DLL: Open a Command Prompt with Administrator privileges and run:

regsvr32 SimpleCredentialProvider.dll

Register in Windows Registry: You will need to add the Credential Provider to the Windows Registry so it shows up on the login screen:

[HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\Authentication\Credential Providers\{YOUR_PROVIDER_CLSID}]
@="Simple Credential Provider"

Replace {YOUR_PROVIDER_CLSID} with the actual CLSID of your Credential Provider.



2. Reboot or log off to see the Credential Provider on the Windows login screen.



Uninstallation

1. To unregister the Credential Provider, run the following command:

regsvr32 /u SimpleCredentialProvider.dll


2. Optionally, remove the registry key manually or via script:

reg delete "HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\Authentication\Credential Providers\{YOUR_PROVIDER_CLSID}" /f



Troubleshooting

Registration Issues: Ensure you are running regsvr32 with Administrator privileges to register the DLL.

Unicode Support: Make sure the /DUNICODE and /D_UNICODE flags are used to compile the project for full Unicode support.

Error Handling: If the login fails, the provider will display an error message on the login screen. Ensure your local account exists and the username/password are correct.


License

This project is licensed under the MIT License. See the LICENSE file for more details.

Contributing

Contributions are welcome! Please open an issue or submit a pull request to propose changes or improvements.


---

Author: Ignacio Rodríguez
