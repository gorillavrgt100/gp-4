# GP 4 — Windows Prototype

This repository is being used to build a keyboard-and-mouse Windows recreation/prototype inspired by the supplied GP V4 APK.

## Current status

- Native Windows desktop window (Win32 API)
- Basic first-person camera
- WASD movement and Shift sprint
- Click the game window to capture the mouse; press Esc to release it
- Procedural placeholder forest environment rendered with the Windows OpenGL API
- This is an early prototype, **not yet a complete GP 4 port**. The APK is an Android Unity IL2CPP build and does not include the original editable Windows project or a ready-made EXE.

## Build

GitHub Actions builds the Windows executable on a Windows runner. Open the repository's **Actions** tab, select the Windows build workflow, and download the artifact after a successful run.

The project uses the Windows SDK and system OpenGL library; it does not use Unity or another game engine.
