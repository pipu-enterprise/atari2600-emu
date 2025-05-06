# atari2600-emu 

atari2600-emu is a strangely named nes emulator written in C.

## Installation

To compile atari2600-emu, use the provided `Makefile`. Ensure you have `gcc` or another compatible C compiler installed.

```bash
make
```

## Usage

Use the following commands to compile, run, and clean up the emulator:

1. **Compile the Emulator**:
   ```bash
   make
   ```

2. **Run the Emulator**:
   ```bash
   make run
   ```

3. **Clean Up Build Files**:
   ```bash
   make clean
   ```

## Project Structure

- **src/**: Contains source files (`cartridge.c`, `cpu.c`, `main.c`, `memory.c`) that implement NES components.
- **inc/**: Header files defining interfaces for each module.
- **Makefile**: Automates the build process, clean-up, and execution.

## Contributing

Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change. Please ensure code adheres to C11 standards and include documentation for new features.

