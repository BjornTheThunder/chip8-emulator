# CHIP-8 Emulator

A simple, lightweight CHIP-8 emulator written in **C++** using **SDL2** for graphics, timing, and input handling.

## Tech Stack

- **Language:** C++
- **Graphics/Input:** SDL2
- **Platform:** Windows (MinGW)

## Project Structure

```text
├── bin/          # Compiled executable and ROMs
├── headers/      # C++ header files
├── lib/          # SDL2 libraries
└── src/          # C++ source files
```

## Building

Compile the source files using GCC/MinGW on Windows:

```cmd
g++ -O3 -Wall -Wextra -Iheaders -Llib .\src\*.cpp -lmingw32 -lSDL2main -lSDL2 -o .\bin\main.exe
```

## Running

To run a ROM (for example, the included Tetris game), run the compiled binary with your parameters and ROM path:

```cmd
.\bin\main.exe 20 3 .\bin\tetris.ch8
```

### Command Usage

```cmd
.\bin\main.exe <clock_speed> <scale> <path_to_rom>
```

- `<clock_speed>`: Execution speed parameter.
- `<scale>`: Display window scaling factor.
- `<path_to_rom>`: Path to the `.ch8` ROM file.