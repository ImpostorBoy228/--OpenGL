# 🎮 Fucking C Game

A minimalistic game engine in pure C using OpenGL.
Made with love and swear words to learn graphics programming.

## 📋 About

This project is the first step towards creating my own game engine.
Currently implements basic triangle rendering with matrix transformations (model-view-projection).

**Tech stack:**
- C (C99)
- OpenGL 3.3 Core Profile
- GLFW for window creation and input handling
- GLAD for OpenGL function loading
- cglm for math (matrices, vectors)

## 🏗 Project Structure

```
.
├── Makefile              # build system
├── src/
│   ├── main.c            # entry point, game loop
│   └── shaders/          # GLSL shaders
│       ├── vertex.glsl
│       └── fragment.glsl
├── include/              # header files (empty)
├── libs/                  # external libraries
└── fuck                   # compiled binary
```

## 🔧 Building

### Dependencies
- GLFW3
- GLAD (files should already be in the project)
- cglm

### Compile
```bash
make
```

### Run
```bash
./fuck
```

## 🎯 Current Features

- [x] OpenGL context creation (GLFW)
- [x] Shader loading from files
- [x] Basic matrix transformations (MVP)
- [x] Mesh abstraction with custom attribute support
- [x] Triangle rendering with camera

