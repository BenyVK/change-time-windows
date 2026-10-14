# System Time Changer

An animated, console-based tool for setting the Windows system date and time. It elevates itself to Administrator when needed and works on Windows XP SP3 through Windows 11.

> **Author:** [BenyVK](https://github.com/BenyVK) · **Website:** [Gharri.ir](https://Gharri.ir)
> **Project:** [github.com/BenyVK/change-time-windows](https://github.com/BenyVK/change-time-windows)

---

## Features

- **Wide compatibility**: targets Windows XP SP3, Vista, 7, 8/8.1, 10 and 11.
- **Accurate OS detection**: uses `RtlGetVersion` from `ntdll.dll`, so the real version is reported even under compatibility shims. It falls back to `GetVersionExW` if that fails.
- **Automatic UAC elevation**: on Vista and later, if the program isn't running as Administrator, it relaunches itself with the `runas` verb.
- **Windows XP handling**: XP has no UAC, so the program tells you to log in as an Administrator instead.
- **Privilege handling**: enables `SeSystemtimePrivilege` on the process token before changing the clock.
- **Input validation**: every field is range-checked, and non-numeric input is rejected.
- **Animated console UI**: ASCII banner, typewriter text, spinners, progress bars and colored status messages. It uses ANSI/VT sequences when the console supports them and falls back to `cls` otherwise.
- **Lightweight**: a single `main.cpp` with no external dependencies beyond the Windows API.

## How It Works

1. Shows the banner and detects the OS version.
2. Checks for Administrator rights and, if needed, requests elevation through UAC.
3. Enables the system-time privilege.
4. Displays the current system time.
5. Prompts for year, month, day, hour, minute and second.
6. Applies the new time with `SetLocalTime` and shows the result.

### Accepted input ranges

| Field  | Range       |
|--------|-------------|
| Year   | 1970 – 2099 |
| Month  | 1 – 12      |
| Day    | 1 – 31      |
| Hour   | 0 – 23      |
| Minute | 0 – 59      |
| Second | 0 – 59      |

> Day-of-month is not cross-checked against the month. For example, `31` for February will be rejected by Windows, and the program will print the error code returned by `GetLastError()`.

## Usage

1. Build the executable (see below) or download it from the project releases.
2. Run `Change-Time-Auto.exe`.
3. Approve the UAC prompt if one appears.
4. Enter the new date and time when prompted.

You can also right-click the executable and choose **Run as administrator** to skip the elevation step.

## Building

### Option 1: Dev-C++ (the original setup)

The project was created with **Embarcadero Dev-C++ 6.3** and **TDM-GCC 9.2.0 (64-bit)**.

1. Open `Change-Time-Auto.dev` in Dev-C++.
2. Press **F11** (Compile & Run), or use *Execute → Compile*.

The linker flags `-ladvapi32 -lshell32` are already set in the project file.

> `Makefile.win` contains hard-coded paths such as `D:/Dev-Cpp/...`. Edit them to match your installation, or just use the project file.

### Option 2: MinGW-w64 / TDM-GCC from the command line

```bash
g++ main.cpp -o Change-Time-Auto.exe -static-libgcc -ladvapi32 -lshell32
```

### Building for Windows XP

The provided makefile uses a **64-bit** toolchain, and 64-bit executables don't run on 32-bit Windows XP. To target XP, build with a **32-bit** MinGW toolchain, for example:

```bash
i686-w64-mingw32-g++ main.cpp -o Change-Time-Auto.exe -static -ladvapi32 -lshell32
```

The source already defines `_WIN32_WINNT` and `WINVER` as `0x0501` (Windows XP).

## Project Structure

```
.
├── main.cpp                  # Application source
├── Makefile.win              # Dev-C++ generated makefile
├── Change-Time-Auto.dev      # Dev-C++ project file
├── Change-Time-Auto.layout   # Dev-C++ editor layout (IDE state only)
└── main.o                    # Compiled object file (build artifact)
```

> `main.o` and `*.layout` are build/IDE artifacts. Consider adding them to a `.gitignore`.

## Code Overview

| Component           | Responsibility                                                                    |
|---------------------|-----------------------------------------------------------------------------------|
| `ConsoleAnim`       | Console helpers: colors, cursor, typewriter text, spinner, progress bar, banner   |
| `getRealOsVersion`  | OS detection via `RtlGetVersion`, with a `GetVersionExW` fallback                 |
| `isRunningAsAdmin`  | Checks Administrators group membership via `CheckTokenMembership`                 |
| `relaunchAsAdmin`   | Re-executes the program elevated through `ShellExecuteExW` with `runas`           |
| `SystemTimeManager` | Enables `SeSystemtimePrivilege`, sets the time, formats the current time          |
| `askInt`            | Validated numeric input prompt                                                    |

## Notes and Limitations

- **Administrator rights are required.** Windows only allows changing the system clock with the `SeSystemtimePrivilege`.
- **Windows Time may overwrite your change.** If automatic time synchronization is enabled (the `W32Time` service or "Set time automatically"), Windows can resync the clock shortly afterward. Disable it if you need the change to stick.
- **Local time is set, not UTC.** The tool calls `SetLocalTime`, so the result depends on the system's current time zone settings.
- Changing the system clock can affect scheduled tasks, log timestamps, certificate validation and licensing checks. Use it with care.

## Contributing

Issues and pull requests are welcome at [github.com/BenyVK/change-time-windows](https://github.com/BenyVK/change-time-windows).

## Author

**BenyVK**
- Website: [Gharri.ir](https://Gharri.ir)
- GitHub: [github.com/BenyVK](https://github.com/BenyVK)
