# VoodooPS2-ALPS

### VoodooPS2-ALPS is now part of [acidanthera's VoodooPS2](https://github.com/acidanthera/VoodooPS2), please [use it](https://github.com/acidanthera/VoodooPS2/releases) instead! This repo is archived for history tracking.

This new VoodooPS2 kext, made for ALPS touchpads, adds support for Magic Trackpad 2 emulation in order to use macOS native driver instead of handling all gestures itself.

This driver is a fork of DrHurt's VoodooPS2 repo with included fixes by 1Revenger1.

The aim of this driver is to improve the usability of ALPS touchpads in macOS and merge the code with acidanthera's VoodooPS2 repo.

V1, V2 and V6 touchpads only support as a normal mouse due to hardware limitation.

## Driver Features:

- Supports ALPS hardware version V1, V2, V6, V7 and V8
    - not tested: V3, V4, V5
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

By default, Force Touch is disabled. However, if you have a clickpad (button inside the touchpad), you can use ForceTouchMode 1, which uses the button as a hard press. In addition, V3, V4, V5 and V8 touchpads support pressure reports. To enable pressure report, one must set ForceTouchMode to 2. The touchpad will now trigger a hard press after exceeding a threshold (defined in ForceTouchPressureThreshold [100 by default]).

| ALPS version | supported <br> ForceTouchMode |
| -- | --- |
| V1 | -/- |
| V2 | -/- |
| V3 | 0, 2, 3, 4 |
| V4 | 0, 2, 3, 4 |
| V5 | 0, 2, 3, 4 |
| V6 | -/- |
| V7 | 0, 1* |
| V8 | 0, 1*, 2, 3, 4 |

*: only if touchpad is a clickpad


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
