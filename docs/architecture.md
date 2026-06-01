# Architecture

## Project Structure

```
src/
├── app/
│   └── main_window.h/cpp
│
├── layout/
│
├── chrome/
│ 
├── windows/
│   └── editor/
│
├── widgets/
│   └── code_editor/
│
└── config/
```

## Components

| Component | Purpose |
|-----------|---------|
| **app/** | Main application entry point - MainWindow is the root window |
| **chrome/** | Window chrome - FramelessWindow (frameless resize/move), ControlBar (title bar UI) |
| **layout/** | Layout system - LayoutWindow (base window), VirtualWindow/Group/Splitter (tiling model) |
| **windows/** | Concrete VirtualWindow implementations (EditorWindow for code editing) |
| **widgets/** | Reusable UI components (CodeEditor with syntax highlighting, LineNumberArea) |
| **config/** | Application configuration loading and saving |

## Key Classes

- `ControlBar` - Custom title bar with minimize/maximize/close buttons
- `FramelessWindow` - Base window with custom resize/move and content_bar
- `LayoutWindow` - Window with content group splitter

- `VirtualWindow` - Window model/state
- `VirtualGroup` - Group of virtual windows
- `VirtualSplitter` - Tiling splitter for groups
