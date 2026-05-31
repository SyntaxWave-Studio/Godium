# Architecture

## Project Structure

```
src/
├── windowing/     # Core windowing system - tiling, layout management
├── windows/       # Virtual windows (VirtualWindow, VirtualGroup, VirtualSplitter)
├── widgets/       # Reusable UI widgets (ControlBar, CodeEditor)
├── config/        # Configuration and settings
└── app/           # Main application window (MainWindow)
```

## Components

| Component | Purpose |
|-----------|---------|
| **windowing/** | Core system - tiling, frameless windows, control bar, layout |
| **windows/** | Virtual window model - represents window state and groups |
| **widgets/** | Common UI components - CodeEditor, etc. |
| **config/** | Application configuration and windowing settings |
| **app/** | Application MainWindow |

## Key Classes

- `FramelessWindow` - Base window with custom resize/move and content_bar
- `LayoutWindow` - Window with content group splitter

- `VirtualWindow` - Window model/state
- `VirtualGroup` - Group of virtual windows
- `VirtualSplitter` - Tiling splitter for groups
