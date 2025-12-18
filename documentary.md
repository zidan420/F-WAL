# Live Bootable OS — Technical Documentation

This document explains the internal design and execution flow of our **Live Bootable Operating System**, from bootloader to kernel execution, memory layout, and current limitations. It is intended for readers with basic knowledge of x86 architecture and low-level systems programming.

---

## 1. Project Overview

This project implements a **live bootable x86 OS** that boots from a USB (pendrive). The system consists of:

* A **512-byte bootloader**
* A **FAT32-partitioned disk** (USB)
* A **C-based kernel** loaded into memory
* A **VGA text-mode terminal interface**
* A **RAM-only pseudo-filesystem**

The OS does **not modify disk data** and runs entirely in memory after boot.

---

## 2. Boot Process Overview

The complete boot flow is:

1. BIOS loads the 512-byte bootloader into memory
2. Bootloader reads kernel sectors from USB using **CHS addressing**
3. Kernel is loaded into memory at a fixed address (`0x1000`)
4. Bootloader sets up the **Global Descriptor Table (GDT)**
5. CPU switches from **Real Mode → Protected Mode**
6. Control jumps to the kernel entry point
7. Kernel initializes terminal, filesystem, and input handling

---

## 3. Bootloader Design

### 3.1 Size Constraint

The bootloader strictly fits within **512 bytes**, as required by BIOS boot rules.

### 3.2 Disk Access (CHS)

* Disk access is performed using **BIOS interrupts (INT 13h)**
* Kernel sectors are read using **CHS (Cylinder, Head, Sector)** addressing
* The "disk" refers to a **USB pendrive**, not a traditional hard disk

Once loaded, the kernel resides fully in memory.

---

## 4. Kernel Loading

### 4.1 Kernel Load Address

The kernel is loaded at the physical memory address:

```text
0x1000
```

This address is chosen to avoid overwriting the bootloader and BIOS data areas.

### 4.2 Why a Kernel Entry is Required

The kernel is written in C. Due to compiler behavior:

* The compiler may place runtime setup code **before `main()`**
* Jumping directly to `main()` is unsafe

To solve this, we define a **kernel entry point** at `0x1000`:

```text
Bootloader → kernel_entry → main()
```

The kernel entry:

* Sets up the stack
* Ensures correct calling conventions
* Transfers control safely to `main()`

---

## 5. Protected Mode Transition

### 5.1 Why Protected Mode?

Protected mode is required to:

* Access more than 1MB of memory
* Use 32-bit registers
* Enable modern OS features

### 5.2 Global Descriptor Table (GDT)

The **Global Descriptor Table (GDT)** defines memory segmentation rules for protected mode.

Although segmentation is often minimized, the GDT is **mandatory**:

* Without a valid GDT, protected mode **will not work**
* It defines code and data segments used by the kernel

### 5.3 Switching to Protected Mode

The bootloader performs the following steps:

1. Disable interrupts
2. Load the GDT
3. Set the PE bit in `CR0`
4. Perform a far jump to flush the instruction pipeline
5. Execute kernel code in 32-bit protected mode

---

## 6. Kernel Execution

Once control is transferred, the kernel is fully active in protected mode.

### 6.1 Graphics and Terminal

* Uses **VGA Text Mode (80x25)**
* Draws a startup logo
* Initializes a terminal interface
* Supports text output, scrolling, and clearing

### 6.2 Keyboard Input

* Keyboard input is handled using **polling**
* No interrupts are used
* Input is read synchronously from the keyboard controller

---

## 7. Filesystem Design

### 7.1 RAM-Based Pseudo Filesystem

This OS does **not implement a real filesystem**.

Instead:

* Files are **hardcoded in memory**
* No disk read/write operations occur after boot
* The filesystem exists only to simulate behavior

### 7.2 Persistence

* The filesystem is **non-persistent**
* All data is lost when the system shuts down or reboots
* Disk contents remain unchanged

---

## 8. Supported Shell Commands

The kernel provides a minimal shell with the following commands:

| Command | Description                        |
| ------- | ---------------------------------- |
| `ls`    | List available (hardcoded) files   |
| `echo`  | Write content to a file (RAM only) |
| `cat`   | Display file contents              |
| `clear` | Clear the terminal screen          |

These commands operate entirely on the in-memory pseudo filesystem.

---

## 9. Limitations

Current limitations of the system:

* No real filesystem implementation
* No disk persistence
* No multitasking
* No interrupts for keyboard input (polling only)
* No memory protection or paging

These limitations are intentional for simplicity and educational clarity.

---

## 10. Future Improvements

Planned enhancements include:

* Real FAT32 filesystem support
* Disk write capability
* Interrupt-driven keyboard input
* Paging and memory management
* Basic process/task scheduling

---

## 11. Summary

This project demonstrates the complete low-level process of:

* Booting from a USB device
* Loading a kernel using BIOS services
* Switching to protected mode
* Running a C-based kernel
* Implementing terminal I/O and a simulated filesystem

It serves as a strong foundation for further OS development and experimentation.
