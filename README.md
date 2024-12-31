# Operating Systems Homework Repository  

This repository contains solutions to assignments focusing on key operating systems concepts, including file system simulation and memory management.  

## Homework 1: Simulating a File System  
This assignment implements a simple file system simulation with the following features:  
- **Disk Size**: 128 KB.  
- **Root Directory**: The top-level directory is `/`.  
- **File/Directory Limits**: Up to 16 files or directories.  
- **File Blocks**: Each file can have a maximum of 8 blocks (1 KB per block, no indirect pointers).  
- **Naming Rules**: File and directory names can have up to 8 characters (including the NULL character). Each directory allows only one file with a given name.  

## Homework 3: Interactive Shell for Memory Management  
This assignment develops an interactive shell-like environment for executing commands and managing memory. The features include:  
- **Command Execution**: An interactive interface to accept and execute commands (input is assumed to be correctly formatted).  
- **Memory Management**:  
  - A 500-byte in-memory data structure is initialized to represent stack and heap memory.  
  - The program updates the heap and stack states dynamically based on executed commands.  
  - Data formatting uses spaces for improved readability.  

