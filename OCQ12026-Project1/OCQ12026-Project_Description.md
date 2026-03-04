# Organización de Computadoras Q1 2026
# Project I: MIPS-Driven Virtual Framebuffer

## **1. Introduction**

In this project, you will bridge the gap between hardware abstraction and low-level assembly programming. You will implement a graphical driver and an interactive application using **MIPS32 Assembly**.

Instead of standard text output, you will interact with a **Virtual Framebuffer** managed by a custom C++ system library. This simulates a dedicated graphics controller that handles a window using the olcPixelGameEngine.

## **2. System Architecture**

The system follows a strict Hardware/Software split:

1. **The Backend (C++ / Hardware Layer):** A driver system managing a virtual pixel grid (VRAM). It handles window rendering via `olcPixelGameEngine` and input detection.  
2. **The Frontend (MIPS32 / Software Layer):** Your assembly code. You are responsible for the game logic and sending commands to the "Hardware" via System Calls.

## **3. System Call Specification (The HAL)**

To interact with the display and keyboard, you must use the following custom syscalls:

| Service | ID ($v0) | Arguments | Description |
| :---- | :---- | :---- | :---- |
| **Start Engine** | 100 | | Start the Pixel Game Engine |
| **Set Pixel** | 101 | $a0: x, $a1: y, $a2: color | Writes a color (0xRRGGBB) to the internal buffer at (x, y). |
| **Refresh** | 102 | *None* | Flushes the internal buffer to the physical window. |
| **Clear** | 103 | $a0: color | Fills the entire buffer with the specified color. |
| **Get Key** | 104 | **Returns** $v0: KeyID | 0: None, 1: Up, 2: Down, 3: Left, 4: Right, 5: Space. |
| **Exit Graphics** | 105 | *None* | **Mandatory.** Cleans up resources. |

## **4. Project Requirements**

### **Part A: The Primitive Library**

Implement the following subroutines in **MIPS Assembly**, strictly adhering to **Standard Calling Conventions**:

* `draw_rectangle(x, y, w, h, color)`: Uses nested loops and the syscall **Set Pixel** (101).  
* `draw_horizontal_line(x1, x2, y, color)`: Optimized line routine using the syscall **Set Pixel** (101).

### **Part B: The Interactive Application**

Create a program that:

1. **Initializes** the scene.  
2. **Runs a Game Loop:** Clears the buffer, reads input, updates positions, draws objects, and refreshes the screen.  
3. **Graceful Exit:** If the 'Space' key is pressed, the loop must break and call **Exit Graphics** before the program terminates.

## **5. Technical Implementation (C++ Backend)**

### **The Display Driver (`MipsDisplay.hpp` / `MipsDisplay.cpp`)**

This class inherits from `olc::PixelGameEngine` and serves as the simulated graphics hardware.

* **VRAM Management:** It maintains an internal buffer (`vram`) representing the screen.
* **Threading:** The engine runs in its own thread to keep the UI responsive while the MIPS code executes.
* **Input:** It captures physical key presses and maps them to the `KeyID` values defined in the syscall table.

### **Syscall Dispatcher (`Main.cpp`)**

The `handleSyscall` function is the bridge between MIPS and C++.

* **Implementation:** You must implement all syscalls defined in Section 3 within this function.
* **Persistence:** You are responsible for declaring and managing a `MipsDisplay` instance (e.g., using a `static std::unique_ptr`) to ensure the graphics state persists across syscall calls.

### **Extending the Hardware**

The provided `MipsDisplay` class is a template. To support **Set Pixel** (101) and **Clear** (103), you must:

1. **Add Methods:** Implement methods to modify the `vram` array (e.g., `void SetPixel(int x, int y, uint32_t color)` and `void Clear(uint32_t color)` in class `MipsDisplay`).
2. **Bounds Checking:** Ensure your `SetPixel` method prevents writing outside the `vram` array boundaries.
3. **Integration:** Update `Main.cpp` to call these new methods when the respective syscall IDs are requested.
