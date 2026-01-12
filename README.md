# Sonic Unleashed PS2 Debug stuff

This is a plugin that mainly restores nnPrint by porting the function over from Sonic Riders Zero Gravity.

Additionally, it adds easy access to the DebugSelect mode, an FPS display and a heap info display.

## Installation

1. Get [the latest build of PCSX2 Fork with Plugins here](https://github.com/ASI-Factory/PCSX2-Fork-With-Plugins/releases/tag/latest) (MSVC AVX2 build should be good for most people)

2. Place `UnwiishedPS2Debug.elf` in the `PLUGINS` directory of PCSX2 Fork

## Usage

This plugin should automatically bind and re-enable nnPrint.

- To access the DebugSelect mode, press Start + Select and release it. Upon release, the game should switch to the DebugSelect mode. (NOTE: be sure NOT to do this as the game is being saved, or else you risk corrupting your save data!)

- Toggle the FPS display (between off/on/constant update): Select + L1

- Switch the FPS display type: Select + L2

- Toggle heap display: Select + R3

- While in heap display, to control the cursor, hold R1. To dump the heap stats of the selected item to the TTY, press R3 while holding R1. (NOTE: dumping is very rudimentary)

## FPS display

Currently, the plugin only measures the frametime on the main loop and not rendering specifically.

It works the same as the Black Box NFS games otherwise.

You can toggle between 3 types: FPS+Frametime, frametime only and FPS only

By default, the FPS display only updates every 8th frame (1/8th framerate). If you want to disable this, press the toggle button and it will go into the "constant" mode. Please be aware that this may affect performance.

NOTE: nnPrint currently has an issue where it doesn't draw on top of everything, so you may see the printouts be affected by effects and rendered behind the pause menu.

## Compatibility

Currently it is only compatible with the EU build (SLES-55380).

## Building

Currently, the PS2SDK isn't submoduled in this build. Use this exact one: [PS2SDK](https://github.com/ThirteenAG/ps2sdk/tree/e2e4c4fc75b75f147a8598bd30108e0d71760c7d) and place it in external/ps2sdk.

## Additional stuff

I highly recommend you also get the cheats from this repo for some QoL improvements (such as 60FPS and Progressive Scan and more): [UnwiishedPS2Hacks](https://github.com/xan1242/UnwiishedPS2Hacks)


