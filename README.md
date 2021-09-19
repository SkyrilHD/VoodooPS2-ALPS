# VoodooPS2-ALPS

This driver has been ported from 1Revenger1's VoodooPS2 fork by DrHurt to support Magic Trackpad 2 emulation.

The aim of this driver is to improve the usability of ALPS touchpads in macOS and merge the code with acidanthera's VoodooPS2 repo.

## Driver Features:

- Supports ALPS hardware version V7
    - not supported: V1, V2, V3, V5, V6, V8
- Supports Mac OS 10.12 to 11.0
- Look up & data detectors
- Secondary click (with two fingers, in bottom left corner*, in bottom right corner*)
- Tap to click
- Scrolling
- Zoom in or out
- Smart zoom
- Rotate
- Swipe between pages
- Swipe between full-screen apps (with three or four fingers)
- Notification Centre
- Mission Control (with three or four fingers)
- App Exposé (with three or four fingers)
- Dragging with or without drag lock (configured in 'Accessibility'/'Universal Access' prefpane)
- Three finger drag (configured in 'Accessibility'/'Universal Access' prefpane, may work unreliably**)
- Launchpad (may work unreliably)
- Show Desktop (may work unreliably)
- Screen zoom (configured in 'Accessibility'/'Universal Access' -> Zoom -> Advanced -> Controls -> Use trackpad gesture to zoom)

## Force Touch

Force Touch has been disabled for ALPS trackpads as they do not suport width/pressure report. So, the best option is to keep Force Touch disabled.

# Credits

- SkyrilHD (for porting VoodooInput to ALPS)
- 1Revenger1 (for adding VoodooInput to the code)
- Acidanthera (for making VoodooInput)
- DrHurt (for the main code)
- Linux
- usr-sse2 (for the huge work he did for Synaptics)
