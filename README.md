# GTA2 Texture Loader
This plugin loads textures for any .sty file that has been loaded and performs a hot swap at runtime, making the game possible to handle high quality textures.

## Compatibility:
Compatible only with GTA2 9.6.0.0

## Compiling:
Requirements:
 - Visual Studio 2022
 - [Plugin SDK](https://github.com/DK22Pac/plugin-sdk)
 
 ## Download:
Download the latest archive from the [releases](https://github.com/gennariarmando/gta2-texture-loader/releases) page.

# Installation:
#### Installing an ASI Loader:
An ASI Loader is required in order to inject the plugin into the game, if you already have one skip to the next step.\
Recommended: [Ultimate ASI Loader](https://github.com/ThirteenAG/Ultimate-ASI-Loader)

Place the DLL file (renamed into "dinput.dll") into your GTA2 directory.

#### Installing Texture Loader:
Archive content: 
- TextureLoaderII.asi


#### Usage:
Create a folder called "scripts" inside your GTA2 directory and paste TextureLoaderII.asi in it.
Launching a level for the first time will create a few folder inside "data\".

To replace textures paste one with the matching id and a prefix: GTA2_TILE_X or GTA2_SPRITE_X

#### Example:
<p align="center">
<img src="https://i.imgur.com/W5wBSpe.png" width="320" height="180">
<img src="https://i.imgur.com/zGhHmMj.png" width="320" height="180">
<img src="https://i.imgur.com/9OcCYpr.png" width="320" height="180">
<img src="https://i.imgur.com/DeJ9tu3.png" width="320" height="180">
</p>

## Links:
- [plugin-sdk](https://github.com/DK22Pac/plugin-sdk)
