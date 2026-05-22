# 🚀 Godium

**Godium** is my first serious C++ project. A high-performance, responsive, and minimalist code editor built on **Qt6 (C++17)**.

The project is being created as a fast and native alternative to heavyweight Electron applications like VS Code. The main goal is to prove that a modern and user-friendly development tool can be extremely lightweight.

---

## 🎯 Philosophy and Architecture

*   **📦 Monolithic Core:** Out of the box, only basic modules are provided — a built-in file explorer and a high-performance editor window.
*   **🔌 Modularity (Plugin-first):** Any additional functionality (syntax highlighting, LSP autocomplete, terminal, Git) is extracted into isolated plugins that load only when needed.
*   **📐 Smart Interface Splitting:** A custom dynamic window system allows flexible splitting of the screen into independent workspaces without memory leaks or FPS drops.
*   **⚙️ Deep Optimization:** The project is built via CMake with aggressive machine code compression flags, and the linker removes all unused Qt library code.

---

## 🤝 Contacts and Development

This is my first major step into the world of C++ systems programming and working with the Qt6 framework. If you have ideas for optimizing interface rendering algorithms, managing memory allocations, or designing a stable Plugin API — pull requests and issues are always welcome! Let's build the lightest editor together.

Since this is my first large-scale project, the code architecture and memory management systems may contain bugs, hacks, or suboptimal solutions. If you notice leaks, excessive allocations, or places where Qt6 could perform better — feel free to open an issue or send pull requests with fixes. I am actively learning and welcome constructive criticism!

You can write here or message me directly on Discord — vladik88 is my username.