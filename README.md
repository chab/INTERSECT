# INTERSECT

A nondestructive, time-stretching, and intersecting sample slicer plugin with independent per-slice parameter control.

![INTERSECT screenshot](.github/assets/screenshot.png)

## Features

- **Intersecting sample slicing** - Place slices freely on the waveform, independent of other slices.
- **Drag-and-drop sample loading** - WAV, OGG, AIFF, FLAC, MP3.
- **Asynchronous sample loading** - Decode/load runs off the audio thread for safer live use.
- **Slice-based playback** - Create regions with start/end points, each mapped to a MIDI note.
- **Parameter inheritance** - Slices inherit sample-level defaults (BPM, pitch, ADSR, mute group, etc.) unless individually overridden.
- **Fine tuning (cents detune)** - Sample-level and per-slice detune exposed as **TUNE** (`Xct`).
- **Three stretch algorithms:**
  - **Repitch** - Classic sample-rate manipulation where pitch and speed are linked.
  - **Stretch** - Independent pitch and time control via [Signalsmith Stretch](https://github.com/Signalsmith-Audio/signalsmith-stretch), with tonality, formant shift, and formant compensation controls.
  - **Bungee** - Grain-based time-stretch via [Bungee](https://github.com/bungee-audio-stretch/bungee), with adjustable grain mode (Fast / Normal / Smooth).
- **Lazy chop** - Play the sample continuously and place slice boundaries in real time by pressing MIDI keys.
- **Auto chop** - Split slices equally (2-128 divisions) or detect transients with adjustable sensitivity and live preview.
- **Snap-to-zero-crossing** - Click-free slice boundaries (ZX button).
- **Undo/redo** - Snapshot-based undo/redo for all slice and parameter changes.
- **SET BPM** - Calculate BPM from slice length and a musical time unit (16 bars down to 1/16 note).
- **Mute groups** - Voices in the same group cut each other off.
- **One-shot mode** - Voice plays to the end of the slice regardless of note-off; available per-slice or as a sample-level default.
- **Follow MIDI** - Optionally auto-select a slice in the UI when its MIDI note is played (FM button).
- **Duplicate slice** - Clone a slice with all locked parameters (button or `Ctrl+drag` in waveform).
- **Shift-click waveform preview** - Hold `Shift` and click the waveform to audition from cursor position.
- **Panic** - Dedicated PANIC button to immediately kill active voices.
- **Hi-DPI scaling** - Adjustable UI scale factor (0.5x to 3x).
- **Session recall** - All parameters/slices are saved; sample file paths are restored on project load.
- **Custom theming** - Dark, light, and custom themes.
- **DAW transport stop handling** - Responds to All Notes Off (CC 123) and All Sound Off (CC 120), including one-shot-safe behavior.

## What's New in v0.8.8 (Since v0.8.7)

- Added async sample decode/loading and safer sample-swap handling.
- Added **TUNE** (cents detune) at sample and slice levels.
- In **Repitch + Stretch** mode, BPM now drives both **PITCH** and **TUNE** displays (read-only).
- Added non-destructive slice move/resize previews, plus `Ctrl+drag` duplicate ghost overlays.
- Added `Shift+click` waveform preview and a dedicated **PANIC** button.
- Drag stepping updates: BPM/ATK/DEC/SUS/REL now step by `1`, or `5` while holding `Shift`.

### Potentially Breaking Changes in v0.8.8

- Project state is now **v16-only**. Older saved states are not loaded.
- Max playable voices is now **31** (one voice is permanently reserved for preview playback).

## Install

Download the latest release zip for your platform from the [Releases](https://github.com/tucktuckg00se/INTERSECT/releases) page and extract it.

Copy `INTERSECT.vst3` to your VST3 folder or run INTERSECT as a standalone app:

### Windows

```
C:\Program Files\Common Files\VST3\
```

Or run `INTERSECT.exe` as a standalone app.

### macOS

```
~/Library/Audio/Plug-Ins/VST3/
```

For AU, copy `INTERSECT.component` to:

```
~/Library/Audio/Plug-Ins/Components/
```

> **"INTERSECT is damaged and can't be opened"** - macOS blocks unsigned apps downloaded from the internet. Run this in Terminal after copying the files, replacing the path with wherever you put them:
>
> ```bash
> xattr -cr ~/Library/Audio/Plug-Ins/VST3/INTERSECT.vst3
> xattr -cr ~/Library/Audio/Plug-Ins/Components/INTERSECT.component
> xattr -cr /Applications/INTERSECT.app
> ```
>
> Then relaunch/rescan. This strips the quarantine flag macOS adds to downloaded files.

### Linux

```
~/.vst3/
```

After installing, rescan plugins in your DAW to pick up INTERSECT.

## Usage

### Getting Started

1. **Load a sample** - Drag and drop an audio file onto the waveform area, or click **LOAD**.
2. **Create slices** - Use **ADD** to draw regions on the waveform, or **LAZY** to chop in real time.
3. **Play slices** - Each slice is mapped to a MIDI note starting at C2 (note 36).

### Creating Slices

- **ADD** - Click the button, then click and drag on the waveform to draw a slice region.
- **Alt + drag** - Hold `Alt` and drag on the waveform for quick slice drawing (without toggling ADD).
- **LAZY** - Starts continuous playback; press any MIDI key to place a slice boundary at the current playhead. Click **STOP** when done. The final slice closes at the end of the sample.
- **AUTO** - Opens the Auto Chop bar at the bottom of the waveform:
  - **SENS** slider (0-100%) - Adjusts transient detection sensitivity with live preview lines on the waveform.
  - **SPLIT TRANSIENTS** - Splits the selected slice at detected transient positions.
  - **DIV** field - Number of equal parts for time-based splitting (2-128).
  - **SPLIT EQUAL** - Splits the selected slice into equal divisions.
- **COPY** - Duplicates the selected slice with all its parameters.
- **DEL** - Deletes the selected slice.

### Editing Slices

- Drag the **S** handle (left edge) or **E** handle (right edge) to resize a slice.
- Drag the middle of a slice to move it.
- Hold **Ctrl** while dragging a selected slice to duplicate it at a new position.
- Click a slice in the waveform or slice lane to select it.
- Toggle **ZX** to snap slice edges to zero-crossings (eliminates clicks).

### Keyboard / Mouse Shortcuts

- **Scroll wheel** - Zoom in/out (anchored to cursor position).
- **Shift + scroll wheel** - Scroll horizontally.
- **Middle-click drag** - Simultaneous scroll (horizontal) and zoom (vertical).
- **Shift + click waveform** - Preview playback from cursor position.
- **Ctrl + drag selected slice** - Duplicate slice to dragged location.
- **Alt + drag waveform** - Draw slice directly.

### Sample Controls (top bar)

These are defaults inherited by all slices:

- **BPM** - Sample tempo (drag vertically or double-click to type). Drag step is `1`, or `5` with `Shift`.
- **PITCH** - Pitch shift in semitones (`Xst`, -24 to +24).
- **TUNE** - Fine pitch in cents (`Xct`, -100 to +100).
- **ALGO** - Click to cycle: Repitch (speed=pitch), Stretch (independent pitch/time), or Bungee (granular).
- **STRETCH** - Enable time-stretching (syncs playback to DAW tempo), shown next to ALGO.
- **Repitch + Stretch note** - When both are active, **PITCH** and **TUNE** become BPM-driven read-only displays.
- **1SHOT** - One-shot mode: voice plays to the end of the slice regardless of note-off.
- **ATK / DEC / SUS / REL** - ADSR amplitude envelope. Drag step is `1`, or `5` with `Shift`.
- **LOOP** - Loop mode: OFF, LOOP (repeat), or PP (ping-pong bounce). Works in all algorithm modes.
- **MUTE** - Mute group (voices in the same group cut each other off).
- **GAIN** - Master gain in dB (-100 to +24 dB).
- **TAIL** - Release tail. When enabled, audio continues reading past the slice boundary during release.
- **PANIC** - Immediately kills active voices.

### Slice Controls (second bar)

Per-slice overrides. Each parameter has an override button to lock a per-slice value instead of inheriting the sample default.

Includes per-slice **TUNE** and the same BPM/ADSR drag stepping behavior (`1`, or `5` with `Shift`).

### SET BPM

Calculates BPM from a slice's length. Select a slice, click **SET BPM**, and choose a time division (16 bars, 1 bar, 1/4 note, etc.). INTERSECT sets BPM so that the slice equals the chosen duration at your DAW tempo.

### Follow MIDI

Click the **FM** button to toggle. When active, playing a MIDI note automatically selects that slice in the UI.

### Custom Themes

INTERSECT supports custom color themes via `.intersectstyle` files. On first launch, the plugin creates default `dark.intersectstyle` and `light.intersectstyle` in the themes folder.

| OS | Path |
|----|------|
| Windows | `%APPDATA%\Roaming\INTERSECT\themes\` |
| macOS | `~/Library/Application Support/INTERSECT/themes/` |
| Linux | `~/.config/INTERSECT/themes/` |

To create a custom theme:

1. Copy one of the starter files from [`themes/`](themes/) (or from the local themes folder) and rename it, e.g. `mytheme.intersectstyle`.
2. Change the `name:` field to something unique (this is what appears in the theme picker).
3. Edit the 6-digit hex color values (`RRGGBB`).
4. Place the file in the themes folder listed above.
5. Restart the plugin. Your theme appears in the theme selector (right-click the header bar).

## Build

Requires CMake 3.22+ and a C++20 compiler.

### Windows

**Prerequisites:** [Visual Studio 2022](https://visualstudio.microsoft.com/) (Desktop development with C++ workload), [CMake 3.22+](https://cmake.org/download/)

```bash
git clone --recursive git@github.com:tucktuckg00se/INTERSECT.git
cd INTERSECT
cmake -B build
cmake --build build --config Release
```

**Output:** `build/Intersect_artefacts/Release/VST3/INTERSECT.vst3`

### macOS

**Prerequisites:** Xcode 14+ (install Command Line Tools via `xcode-select --install`), [CMake 3.22+](https://cmake.org/download/) (`brew install cmake`)

```bash
git clone --recursive git@github.com:tucktuckg00se/INTERSECT.git
cd INTERSECT
cmake -B build
cmake --build build --config Release
```

**Output:**
- VST3: `build/Intersect_artefacts/Release/VST3/INTERSECT.vst3`
- AU: `build/Intersect_artefacts/Release/AU/INTERSECT.component`

### Linux

**Prerequisites:** GCC 12+ or Clang 15+, CMake 3.22+, and JUCE system dependencies.

**Debian / Ubuntu:**

```bash
sudo apt install build-essential cmake libasound2-dev libfreetype6-dev \
  libx11-dev libxcomposite-dev libxcursor-dev libxinerama-dev \
  libxrandr-dev libxrender-dev libwebkit2gtk-4.0-dev libglu1-mesa-dev
```

**Fedora:**

```bash
sudo dnf install gcc-c++ cmake alsa-lib-devel freetype-devel \
  libX11-devel libXcomposite-devel libXcursor-devel libXinerama-devel \
  libXrandr-devel libXrender-devel webkit2gtk4.0-devel mesa-libGLU-devel
```

**Arch:**

```bash
sudo pacman -S base-devel cmake alsa-lib freetype2 \
  libx11 libxcomposite libxcursor libxinerama libxrandr libxrender \
  webkit2gtk-4.1 glu
```

Then build:

```bash
git clone --recursive git@github.com:tucktuckg00se/INTERSECT.git
cd INTERSECT
cmake -B build
cmake --build build --config Release
```

**Output:** `build/Intersect_artefacts/Release/VST3/INTERSECT.vst3`

Builds VST3, AU (macOS), and Standalone targets.

## Dependencies

- [JUCE](https://github.com/juce-framework/JUCE) (git submodule)
- [Signalsmith Stretch](https://github.com/Signalsmith-Audio/signalsmith-stretch) (git submodule, MIT license)
- [Signalsmith Linear](https://github.com/Signalsmith-Audio/linear) (git submodule, dependency of Signalsmith Stretch)
- [Bungee](https://github.com/bungee-audio-stretch/bungee) (git submodule, MPL-2.0 license)

## USE AT YOUR OWN RISK

This plugin is 100% vibe-coded.

## License

This project is licensed under the [GNU General Public License v3.0](LICENSE).
