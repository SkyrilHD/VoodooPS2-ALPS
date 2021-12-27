# VoodooPS2-ALPS

This new VoodooPS2 kext, made for ALPS touchpads, adds support for Magic Trackpad 2 emulation in order to use macOS native driver instead of handling all gestures itself.

This driver is a fork of DrHurt's VoodooPS2 repo with included fixes by 1Revenger1.

The aim of this driver is to improve the usability of ALPS touchpads in macOS and merge the code with acidanthera's VoodooPS2 repo.

## Driver Features:

- Supports ALPS hardware version V7
    - not supported/tested: V1, V2, V3, V5, V6, V8
- Supports macOS 10.11 to 12.0
- Look up & data detectors
- Secondary click (with two fingers, in bottom left corner, in bottom right corner)
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
- Three finger drag (configured in 'Accessibility'/'Universal Access' prefpane, may work unreliably)
- Launchpad (may work unreliably)
- Show Desktop (may work unreliably)
- Screen zoom (configured in 'Accessibility'/'Universal Access' -> Zoom -> Advanced -> Controls -> Use trackpad gesture to zoom, may work unreliably)

## Force Touch

Force Touch has been disabled for ALPS trackpads as they do not suport width/pressure report. So, the best option is to keep Force Touch disabled.

## Old driver

If you want to use the old VoodooPS2 driver for compatibility reasons, you can use the first version of the driver which can be found [here](https://github.com/SkyrilHD/VoodooPS2-ALPS/releases/tag/1.0.0).
However, this version does not support Magic Trackpad 2 emulation but has fixes like scrolling by 1Revenger1 and smooth horizontal scrolling compared to DrHurt's latest VoodooPS2 kext.

# Credits

- SkyrilHD (for porting VoodooInput to ALPS)
- 1Revenger1 (for adding VoodooInput to the code)
- Acidanthera (for making VoodooInput)
- DrHurt (for the main code)
- Linux
- usr-sse2 (for the huge work he did for Synaptics)
