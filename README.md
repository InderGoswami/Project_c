# File Manager in C

## Project Overview

This project implements a **File Manager** in C that allows multiple users to manage their files and directories independently. The key feature is ensuring user privacy by restricting access so that one user cannot view or manage another user’s files or directories. Each user gets a private root directory, and they can perform various operations like creating, reading, writing, and deleting files, as well as managing directories. User authentication is built in to ensure secure access.

## Key Features

- **Multi-User Support**: Each user has their own isolated file system, ensuring privacy.
- **File Management**: Users can create, read, write, and delete files.
- **Directory Management**: Create, open, and delete directories, with support for hierarchical structures.
- **User Authentication**: Users must register and log in to manage their files and directories.
- **Linux Integration**: Uses Linux commands like `mkdir` and `rmdir` to handle directories on disk.
- **File Handling**: Core file operations are done using standard C file I/O methods.

## Concepts Used

- **File Handling in C**: Basic file operations using `fopen`, `fread`, `fwrite`, and `fclose`.
- **Data Structures**: Dynamic data structures like linked lists are used to represent directories and files.
- **System Calls**: Integration of Linux commands such as `mkdir` and `rmdir` for disk-level directory management.
- **Authentication**: A simple system where user credentials are stored in a text file for login and registration.
- **Memory Management**: Efficient use of dynamic memory allocation for managing directory and file structures.

## Directory Structure

Each user is given a root directory that comes with default subdirectories:
- `Images`
- `Docs`
- `More`
- `Bluetooth`
- `Lib`
- `Src`

Users can create additional files and directories within this structure.
## How to run this project
```bash
  git clone https://github.com/InderGoswami/Project_c.git
```

Go to the project directory

```bash
  cd Project_c
```

