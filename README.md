# Extra Utilities

Extra Utilities is the only public script extender for Battlezone 98 Redux 2.2.301. Support open modding!

Usage:
Build the DLL or download the latest release from the releases tab. Then simply require() exu.dll from lua and refer to the function documentation on the wiki page.

Compatibility:
- Windows: Yes
- Mac: No
- Linux: No
- Steam: Yes
- GOG: No
- Battlezone 1.5: of course not lol

Contact me on discord: vtrider

Join the battlezone community discord: https://discord.gg/battlezonecommunity

### For Developers:

Written in C++20, contributions welcome! One important note is that it is critical that you use the custom lua library, it is patched to be compatible with BZR. Using different binaries will cause a heap corruption.

Build Dependencies:
- Windows SDK
