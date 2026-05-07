# Step Counting Project — Development Setup Guide

## Before You Start

You should have received a file called **`step-counting-starter.zip`**. Unzip it to a folder on your computer (e.g., your Desktop or Documents). All of the starter code and data files are included.

Then follow the steps below to install CLion and get started.

---

## 1. Install CLion

**CLion** is a professional C++ IDE from JetBrains that provides excellent debugging, CMake integration, and code navigation out of the box. It's free for students.

### Get Your Free Student License

1. Go to: https://www.jetbrains.com/community/education/#students
2. Click **"Apply now"** and sign up using your **`.edu` email address**
3. JetBrains will send you a confirmation email with activation instructions
4. Follow the link to activate your free student license (valid for 1 year, renewable)

### Download and Install CLion

Download from: https://www.jetbrains.com/clion/download/

- **Windows:** Download `.exe` installer and run it
- **macOS:** Download `.dmg`, open it, and drag CLion to Applications
- **Linux:** Download `.tar.gz`, extract, and run `bin/clion.sh`

Launch CLion when complete. Sign in with your JetBrains account to activate your student license.

---

## 2. Install C++ Toolchain

CLion requires a C++ compiler and CMake. Follow the instructions for your operating system:

### Windows

**Install MSYS2** (includes GCC compiler, GDB debugger, and build tools):

1. Download from: https://www.msys2.org/ and run the installer
2. Accept the default install location (`C:\msys64`)
3. When the installer finishes, open **"MSYS2 UCRT64"** from the Start menu
4. In the MSYS2 terminal, run:
   ```
   pacman -S mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-gdb mingw-w64-ucrt-x86_64-cmake make
   ```
5. Add to PATH: Search "Environment Variables" → Edit System Environment Variables → under **Path**, add:
   ```
   C:\msys64\ucrt64\bin
   ```
6. **Restart your computer** (PATH changes require restart on Windows)

Verify installation — open a new Command Prompt or PowerShell and run:
```cmd
gcc --version
cmake --version
```

### macOS

Install Apple's command-line tools:

```bash
xcode-select --install
```

When prompted, click **Install**. This installs `clang` (the C++ compiler) and build tools.

Install CMake using Homebrew (install Homebrew from https://brew.sh/ if needed):
```bash
brew install cmake
```

Verify it worked:
```bash
clang --version
cmake --version
```

### Linux

Install the GNU C++ compiler, GDB, CMake, and build tools:

**Ubuntu/Debian:**
```bash
sudo apt update
sudo apt install build-essential g++ gdb cmake
```

**Fedora/CentOS:**
```bash
sudo dnf install gcc-c++ gdb cmake make
```

**Arch:**
```bash
sudo pacman -S base-devel gdb cmake
```

Verify it worked:
```bash
g++ --version
cmake --version
```

---

## 3. Open the Project in CLion

1. Launch CLion
2. On the welcome screen, click **"Open"**
3. Navigate to the `step-counting` project folder (where `CMakeLists.txt` is located)
4. Click **Open**

CLion will automatically detect the CMake project and begin configuring it.

---

## 4. Configure the Toolchain (First Time Only)

When you first open the project, CLion will detect your compiler and configure CMake automatically.

### If CLion asks you to select a toolchain:

**Windows:**
- Select **"MinGW"** or **"GCC"** (points to your MSYS2 installation)
- CLion should auto-detect `C:\msys64\ucrt64\bin\gcc.exe`
- If not found, go to **File → Settings → Build, Execution, Deployment → Toolchains** and add the path manually

**macOS:**
- Select **"Clang"** or **"Apple Clang"**
- CLion auto-detects this from `xcode-select`

**Linux:**
- Select **"GCC"** or **"System GCC"**
- CLion auto-detects this from `/usr/bin/g++`

### Verify Configuration

Look at the bottom of the CLion window:
- You should see: **"CMake: Build finished"** or **"CMake loaded successfully"**
- If you see errors, go to **File → Settings → Build, Execution, Deployment → CMake** and click **"Reset Cache and Reload Project"**

---

## 5. Build the Project

CLion makes building easy:

**Option 1: Click the hammer icon** 🔨
- Located in the top-right toolbar
- Or press **Ctrl+F9** (Windows/Linux) or **Cmd+F9** (macOS)

**Option 2: Use the Build menu**
- Go to **Build → Build Project**

✅ You should see: **"Build finished"** in the Build panel at the bottom

---

## 6. Run the Project

**Option 1: Click the green play button** ▶️
- Located in the top-right toolbar next to the hammer icon
- Or press **Shift+F10** (Windows/Linux) or **Ctrl+R** (macOS)

**Option 2: Use the Run menu**
- Go to **Run → Run 'step_counting'**

You should see output in the **Run** panel at the bottom:
```
Initializing CSV reader...
Header line skipped: ...
No more samples available. Exiting.
```

---

## 7. Debugging

CLion's debugger is powerful and easy to use:

### Set a Breakpoint

1. Open `App.cpp` in the editor
2. Click in the **gutter** (left margin) next to a line number
3. A red dot appears — this is your breakpoint

### Start Debugging

**Option 1: Click the green bug icon** 🐛
- Located in the top-right toolbar
- Or press **Shift+F9** (Windows/Linux) or **Ctrl+D** (macOS)

**Option 2: Use the Run menu**
- Go to **Run → Debug 'step_counting'**

### Use the Debugger

When the program pauses at your breakpoint:

- **Variables panel** (bottom left): Shows all local variables and their values
- **Debugger toolbar** (top):
  - **Step Over (F8)**: Execute one line
  - **Step Into (F7)**: Enter a function call
  - **Step Out (Shift+F8)**: Exit current function
  - **Resume (F9)**: Continue to next breakpoint
- **Hover over variables** in the code editor to see their values
- **Watches panel**: Right-click a variable → "Add to Watches" to monitor it continuously

---

## 8. Project Structure

```
step-counting/
├── CMakeLists.txt        # Build configuration
├── main.cpp              # Entry point (calls app_init, app_process_action loop)
├── App.h                 # Declarations, AccelSample struct, CSV infrastructure
├── App.cpp               # Global state, algorithms, main loop
├── cmake-build-debug/    # Build output (generated by CLion)
└── *.csv                 # Sample acceleration data files
```

> **Note:** CLion creates `cmake-build-debug/` for debug builds and `cmake-build-release/` for release builds. You don't need to manage these folders manually.

---

## 9. Common Issues

### Issue: "CMake Error: Could not find CMAKE_CXX_COMPILER"
**Solution:**
- **Windows:** Make sure MSYS2 is installed and `C:\msys64\ucrt64\bin` is in your PATH. Restart CLion after adding to PATH.
- **macOS:** Run `xcode-select --install` in Terminal
- **Linux:** Install build tools with your package manager (see step 2)
- In CLion, go to **File → Settings → Build, Execution, Deployment → Toolchains** and verify the compiler path

### Issue: "CMake executable not found"
**Solution:**
- Make sure CMake is installed (step 2)
- **Windows:** Verify `cmake --version` works in a new Command Prompt. If not, reinstall MSYS2 and include CMake in the `pacman` command.
- In CLion, go to **File → Settings → Build, Execution, Deployment → CMake** and verify the CMake path

### Issue: Build fails with "undefined reference"
**Solution:**
- Make sure all `.cpp` files are listed in `CMakeLists.txt`
- Go to **File → Invalidate Caches / Restart** in CLion
- Try **Tools → CMake → Reset Cache and Reload Project**

### Issue: Program runs but immediately exits
**Solution:**
- Check that CSV data files exist in the project folder
- Verify `DEFAULT_CSV_PATH` in `App.h` points to a valid file
- The working directory in CLion is automatically set to the project root, so relative paths should work

### Issue: Debugger won't launch or shows "cannot find debugger"
**Solution:**
- **Windows:** Make sure you installed GDB via MSYS2: `pacman -S mingw-w64-ucrt-x86_64-gdb`
- Verify in **File → Settings → Build, Execution, Deployment → Toolchains** that the Debugger field shows the path to `gdb.exe` (Windows) or `gdb` (Linux) or `lldb` (macOS)

### Issue: CLion is slow or unresponsive
**Solution:**
- Close unused editor tabs
- Go to **File → Invalidate Caches / Restart** to clear CLion's cache
- If you have many projects open, close the ones you're not working on

---

## 10. Next Steps

Once everything is working:

1. **Explore the code** — Read through `App.h` and `App.cpp` to understand the project structure
2. **Study `get_next_sample()`** — This abstracts how you acquire acceleration data
3. **Implement the state machine** — Replace the placeholder code in `app_process_action()`
4. **Use the debugger** — Set breakpoints to trace your algorithm as it processes samples
5. **Test with different CSV files** — Modify `DEFAULT_CSV_PATH` in `App.h` to test different datasets

---

## 11. Additional Resources

- [CLion Getting Started Guide](https://www.jetbrains.com/help/clion/clion-quick-start-guide.html) — Official CLion documentation
- [CLion Debugging Tutorial](https://www.jetbrains.com/help/clion/debugging-code.html) — How to use the debugger effectively
- [CMake Documentation](https://cmake.org/cmake/help/latest/) — CMake reference
- [C++11 Reference](https://en.cppreference.com/w/cpp/11) — Language reference
- [JetBrains Student License](https://www.jetbrains.com/community/education/#students) — Renew your free license annually

---

Good luck! 🚀
