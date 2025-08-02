# upkg

this readme.md is under construction and this program is under construction:

a curious tester can:

```
cd gemini_upkg
nano upkgconfig

make
make install
make termux-install
make uninstall
make clean

```
current funtionality: very little rather not say...

### disregard anything after this in readme.

![Build Status](https://img.shields.io/badge/build-passing-green)
![License](https://img.shields.io/badge/license-GPLv3-blue)

### A lightweight and simple package manager for `.deb` packages.

`upkg` is a small-scale, command-line utility designed to handle the installation and management of `.deb` packages. It is written in C and focuses on simplicity and minimal dependencies, making it ideal for constrained environments like Termux or for educational purposes.

---

## 🚀 Features

-   **Install packages** from `.deb` files.
-   **Remove packages** by name, including a simple cleanup of files.
-   **List all installed packages** managed by `upkg`.
-   **Tilde (~) expansion** support in configuration files.
-   **Configurable** with a single file (`~/.upkgconfig`).
-   **Termux compatible** installation target.

---

## 🛠️ Installation

### Prerequisites

You need a C compiler (`gcc`) and `make` to build `upkg`.

```bash
# For a standard Linux system
sudo apt-get install build-essential

# For Termux
pkg install build-essential
```

# end of file
