# Sonic Unleashed PS2 Debug stuff

This is a plugin that mainly restores nnPrint by porting the function over from Sonic Riders Zero Gravity.

Additionally, it adds easy access to the DebugSelect mode.

This is very unfinished and unpolished currently, use it at your own risk!

## Installation

1. Get [the latest build of PCSX2 Fork with Plugins here](https://github.com/ASI-Factory/PCSX2-Fork-With-Plugins/releases/tag/latest) (MSVC AVX2 build should be good for most people)

2. Place `UnwiishedPS2Debug.elf` in the `PLUGINS` directory of PCSX2 Fork

## Usage

This plugin should automatically bind and re-enable nnPrint.

- To access the DebugSelect mode, press Start + Select and release it. Upon release, the game should switch to the DebugSelect mode. (NOTE: be sure NOT to do this as the game is being saved, or else you risk corrupting your save data!)

- To toggle the FPS display (between off/on/constant update): press Select + L1

- To switch the FPS display type: press Select + L2

## Compatibility

Currently it is only compatible with the EU build (SLES-55380).

## Building

Currently, the PS2SDK isn't submoduled in this build. Use this exact one: [PS2SDK](https://github.com/ThirteenAG/ps2sdk/tree/e2e4c4fc75b75f147a8598bd30108e0d71760c7d) and place it in external/ps2sdk.

## Additional stuff

I highly recommend you also get the cheats from this repo for some QoL improvements (such as 60FPS and Progressive Scan and more): [UnwiishedPS2Hacks](https://github.com/xan1242/UnwiishedPS2Hacks)


