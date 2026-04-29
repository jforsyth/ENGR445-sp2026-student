# Step Counting Project — Development Setup Guide

This guide will walk you through setting up Visual Studio Code and C++ development tools to build, run, and debug the step-counting project. 

**Choose your operating system below:**

---

## 1. Install Visual Studio Code

Download from: https://code.visualstudio.com/

- **Windows:** Download `.exe` installer and run it
- **macOS:** Download `.zip` for your architecture (Apple Silicon or Intel), extract, and drag VS Code to Applications
- **Linux:** Use your package manager or download `.tar.gz` from the website

Launch VS Code when complete.

---

## 2. Install C++ Toolchain

Choose your operating system:

### Windows

Download and install **one of these**:

**Option A: MinGW (easier, smaller)**
1. Download from: https://www.mingw-w64.org/
2. Run the installer
3. Select: Architecture = x86_64, Threads = posix
4. Install to: `C:\mingw64`
5. Add to PATH: Search "Environment Variables" → Edit System Environment Variables → Under "Path", add `C:\mingw64\bin`

**Option B: Microsoft Visual C++ Build Tools (larger, official)**
1. Download from: https://visualstudio.microsoft.com/visual-cpp-build-tools/
2. Run installer and select "Desktop development with C++"
3. Complete installation

Verify installation (open Command Prompt or PowerShell):
```cmd
gcc --version
```
or
```cmd
cl.exe
```

### macOS

Install Apple's command-line tools:

```bash
xcode-select --install
```

When prompted, click **Install**. This installs `clang` (the C++ compiler).

Verify it worked:
```bash
clang --version
```

### Linux

Install the GNU C++ compiler and build tools:

**Ubuntu/Debian:**
```bash
sudo apt update
sudo apt install build-essential g++ gdb
```

**Fedora/CentOS:**
```bash
sudo dnf install gcc-c++ gdb make
```

**Arch:**
```bash
sudo pacman -S base-devel gdb
```

Verify it worked:
```bash
g++ --version
```

---

## 3. Install CMake

CMake is the build system used for this project.

### Windows

Download from: https://cmake.org/download/

- Choose "Windows x64 Installer"
- Run it
- **Important:** During installation, select "Add CMake to system PATH"

### macOS

Using Homebrew (install from https://brew.sh/ if needed):
```bash
brew install cmake
```

Or download directly from: https://cmake.org/download/ (choose macOS `.dmg`)

### Linux

**Ubuntu/Debian:**
```bash
sudo apt install cmake
```

**Fedora/CentOS:**
```bash
sudo dnf install cmake
```

**Arch:**
```bash
sudo pacman -S cmake
```

Verify it worked (all platforms):
```bash
cmake --version
```

You should see: `cmake version X.XX.X`

---

## 4. Install VSCode Extensions

Open VS Code and go to the **Extensions** panel:
- **Windows/Linux:** Ctrl+Shift+X
- **macOS:** Cmd+Shift+X

1. **C/C++ Extension Pack** by Microsoft
   - Search for `ms-vscode.cpptools-extension-pack`
   - Click **Install**
   - This includes IntelliSense, debugging, and CMake tools

2. **CMake** by twxs (if not already included)
   - Search for `twxs.cmake`
   - Click **Install**

---

## 4a. Disable GitHub Copilot Inline Completion (Optional)

If you have GitHub Copilot installed and want to disable inline code suggestions (to avoid relying on AI suggestions while learning):

1. Open VS Code Settings:
   - **Windows/Linux:** File → Preferences → Settings (or Ctrl+,)
   - **macOS:** Code → Preferences → Settings (or Cmd+,)

2. Search for: `copilot.inlineChat.enabled`

3. Uncheck the box or set to `false`

4. Also search for: `github.copilot.editor.enableAutoCompletions`
   - Uncheck or set to `false`

You can re-enable these anytime by returning to Settings. VS Code will remember your preference.

## 5. Open the Project

1. Open VS Code
2. Go to **File → Open Folder**
3. Navigate to the `step-counting` project folder
4. Click **Open**

VS Code will detect the CMake project automatically.

---

## 6. Configure and Build

### First Time Build

When you open the project, VS Code should show a notification: **"Do you want to configure project 'step_counting'?"**
- Click **Yes**
- Select your compiler:
  - **Windows with MinGW:** Select `gcc` 
  - **Windows with MSVC:** Select `msvc`
  - **macOS:** Select `clang` or `Apple Clang`
  - **Linux:** Select `gcc` or `clang`
- Wait for the configuration to complete

### Manual Configuration

If you don't see the notification, open the Command Palette:
- **Windows/Linux:** Ctrl+Shift+P
- **macOS:** Cmd+Shift+P

Type: `CMake: Configure` and press Enter, then select your compiler.

### Building

Once configured, build the project:

**Option A: Via Command Palette** (all platforms)
- Open Command Palette (Ctrl+Shift+P or Cmd+Shift+P)
- Type `CMake: Build` → Press Enter

**Option B: Click the Build button**
- Look for the **Build** button in the CMake toolbar (bottom of VS Code)

**Option C: Via Terminal** (all platforms)

Open the integrated terminal (Ctrl+` or Cmd+`) and run:

```bash
cd build
cmake .. && cmake --build .
```

Or on Windows (if using MSVC):
```cmd
cd build
cmake .. && cmake --build . --config Release
```

✅ You should see: `[100%] Built target step_counting` or `Built target step_counting`

---

## 7. Run the Project

### From VS Code

**Option A: Debug with breakpoints** (all platforms)
- Press **F5** or go to **Run → Start Debugging**
- The program will start; click the play button in the debug toolbar to run through it
- You can set breakpoints by clicking on line numbers

**Option B: Run without debugging** (all platforms)
- Press **Ctrl+F5** (Windows/Linux) or **Cmd+F5** (macOS)
- Or go to **Run → Run Without Debugging**
- Output appears in the integrated terminal

### From Terminal (all platforms)

Open the integrated terminal (Ctrl+` or Cmd+`) and run:

**Windows:**
```cmd
build\step_counting.exe
```

**macOS/Linux:**
```bash
./build/step_counting
```

You should see output like:
```
Initializing CSV reader...
Header line skipped: ...
No more samples available. Exiting.
```

---

## 8. Setting Up Debugging

To debug your code with breakpoints and variable inspection:

1. Open a source file (e.g., `App.cpp`)
2. **Set a breakpoint** by clicking on a line number (a red dot appears)
3. Press **F5** to start debugging
4. The program will pause at your breakpoint
5. Use the debug toolbar to:
   - **Step over** (F10) — execute one line
   - **Step into** (F11) — enter a function
   - **Continue** (F5) — resume execution
6. Watch variables in the **Variables** pane on the left

### Viewing Variables During Debug

When paused at a breakpoint:
- The **Variables** pane shows local variables and their values
- Hover over variable names in the editor to see their values
- Right-click in the **Variables** pane to add watches

---

## 9. Project Structure

```
step-counting/
├── CMakeLists.txt        # Build configuration
├── main.cpp              # Entry point (calls app_init, app_process_action loop)
├── App.h                 # Declarations, AccelSample struct, CSV infrastructure
├── App.cpp               # Global state, algorithms, main loop
├── build/                # Build output (generated by CMake)
└── *.csv                 # Sample acceleration data files
```

---

## 10. Common Issues

### Issue: "cmake: command not found" or "cmake is not recognized"
**Solution:** Reinstall CMake (step 3) and make sure to add it to PATH during installation

### Issue: "gcc/g++/clang: command not found" or compiler not recognized
**Solution:** 
- **Windows MinGW:** Reinstall MinGW and add `C:\mingw64\bin` to System PATH
- **Windows MSVC:** Install Visual C++ Build Tools (step 2, Option B)
- **macOS:** Run `xcode-select --install`
- **Linux:** Install compiler with package manager (step 2)

### Issue: CMake configuration fails
**Solution:** 
- Delete the entire `build/` folder
- Reopen VS Code
- Try configuring again via Command Palette

### Issue: Build fails with "undefined reference"
**Solution:** Make sure all `.cpp` files are listed in `CMakeLists.txt`

### Issue: VSCode doesn't recognize C++ syntax
**Solution:**
- Install the C/C++ Extension Pack (step 4)
- Reload VS Code: Command Palette → `Reload Window`

### Issue: Debugger won't launch
**Solution:**
- Make sure the project built successfully (no compile errors)
- Try deleting `build/` and rebuilding clean
- **Windows MinGW users:** You may need to install debugging symbols

### Issue: Program runs but immediately exits
**Solution:**
- Check that CSV data files exist in the project folder
- Verify `DEFAULT_CSV_PATH` in `App.h` points to a valid file
- Run from the project's root directory, not from `build/`

---

## 11. Next Steps

Once everything is working:

1. **Explore the code** — Read through `App.h` and `App.cpp` to understand the project structure
2. **Study `get_next_sample()`** — This abstracts how you acquire acceleration data
3. **Implement the state machine** — Replace the placeholder code in `app_process_action()`
4. **Use the debugger** — Set breakpoints to trace your algorithm as it processes samples
5. **Test with different CSV files** — Modify `DEFAULT_CSV_PATH` in `App.h` to test different datasets

---

## 12. Additional Resources & Official Documentation

- [VS Code Setup Guide](https://code.visualstudio.com/docs/setup/setup-overview) — Official platform-specific setup
- [CMake Download & Installation](https://cmake.org/download/) — Official CMake download page
- [Visual C++ Build Tools](https://visualstudio.microsoft.com/visual-cpp-build-tools/) — Official MSVC tools
- [VS Code C++ Setup Guide](https://code.visualstudio.com/docs/languages/cpp) — Official C++ extension guide
- [C++11 Reference](https://en.cppreference.com/w/cpp/11) — Language reference
- [GDB Debugging Guide](https://sourceware.org/gdb/documentation/) — Debugger for Linux/Windows
- [LLDB Debugging Guide](https://lldb.llvm.org/use/tutorial.html) — Debugger for macOS

---

Good luck! 🚀
