# Extra Utilities

Finally figured out how to work git, if something's wrong let me know on discord please.

Dependencies: (use vcpkg to install, must be x86 static binaries)
- Windows SDK
- DirectX 9 SDK
- OpenAL-Soft:x86-windows-static
- Libsndfile:x86-windows-static

You should be able to download the above dependencies in the visual studio installer. Everything else is included in the project, importantly you need to use the the provided lua.5.1 static library in order for it to work with battlezone's particular version of lua. Minhook and imgui are how the WIP UI works. When dealing with the lua C api, don't access it concurrently on separate threads or it will cause instability. This build is still unstable on some systems, but I have yet to reproduce the cause of the crashing. Calling print in lua before an exu function seems to fix most of the instability, use this as a temporary fix if you experience crashing. Also note that it's required to compile for x86 since BZR is a 32 bit game, and any libraries you use should be for windows x86, and statically linked or directly in the source like imgui and minhook require. 

Usage:
Build the DLL or download the latest release from the releases tab. The two important files are exu.dll and ExtraUtils.lua, those two files should always be together since the latter is the wrapper for the dll functions that you can use in-game. Further info is provided on the steam workshop page. If you want the bleeding edge build it will always be on this repo, the workshop version will always be a bit behind to make sure it's relatively stable.

Compatibility:
- Windows: Yes
- Mac: No
- Linux: No
