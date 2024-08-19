--[[
=============================================================
*   Extra Utilities                                         *
*   Version 0.8.0                                           *
=============================================================
*   This module is a script extender for Battlezone         *
*   98 Redux 2.2.301. It includes over 60 custom lua        *
*   functions, restored file i/o, dynamic sound effects     *
*   and more!                                               *
=============================================================
*   Github repo: https://github.com/VTrider/ExtraUtilities  *
=============================================================
*   Be sure to join the Battlezone Community Discord:       *
*   https://discord.gg/battlezonestrategy                   *
=============================================================
*   Special thanks to:                                      *
*   - Janne, for letting me use her dll code                *
*   - DivisionByZero, for the game directory function       *
*   - GrizzlyOne95, for the workshop directory function     *
=============================================================
*   - Script and DLL extension made by VTrider              *
=============================================================
--]]

--- To use function and type annotations install the
--- Lua langauge server plugin for vscode by sumneko

--- Typedefs for annotations
--- @alias handle lightuserdata
--- @alias pointer lightuserdata
--- @alias float number
--- @alias vector userdata
--- @alias matrix userdata

-- This is the dll file, never require this in your own scripts,
-- always require this lua script
local exu = require("exu")

local ExtraUtils = {}
do
    --------------
    -- Metadata --
    --------------

    -- Use the version and/or the crc32 to check for mod version
    -- mismatches that aren't caught by the game
    ExtraUtils.version = exu.GetVersion()
    ExtraUtils.crc32 = "EB227D80"
    ExtraUtils.debug = false

    -------------
    -- Helpers --
    -------------

    --- Lua C API doesn't have a check boolean function so this casts
    --- a bool to an integer
    ---@param value boolean
    ---@return integer | nil
    local function CastBool(value)
        if type(value) ~= "boolean" then
            error("Extra Utilities Error: input must be a bool")
            return
        end
        return value and 1 or 0
    end

    --- @param x number
    --- @param min number
    --- @param max number
    --- @return number
    local function Clamp(x, min, max)
        return x < min and min or x > max and max or x
    end

    -------------
    -- Exports --
    -------------

    --- Sets the method for accessing the game's memory, valid
    --- options:
    --- 
    --- 0: (direct access - blazing fast but potentially unstable)
    --- 
    --- 1: (windows api - much slower but potentially more stable)
    --- 
    --- Default is 0 (direct access)
    --- @param mode integer 0 or 1
    --- @return nil void
    function ExtraUtils.SetAccessMode(mode)
        if mode ~= 0 and mode ~= 1 then
            error("Extra Utilities Error: input must be either 0 or 1")
            return
        end
        exu.SetAccessMode(mode)
    end

    --- Gets the GameObject* for the given handle, useful for debugging
    --- reverse engineered stuff
    --- @param handle handle
    --- @return pointer GameObject*
    function ExtraUtils.GetObj(handle)
        return exu.GetObj(handle)
    end

    --- Gets the current value of the gravity velocity vector
    --- @return userdata vector
    function ExtraUtils.GetGravity()
        local gravityVector = exu.GetGravity()
        local formattedVector = SetVector(gravityVector.x, gravityVector.y, gravityVector.z)
        return formattedVector
    end

    --- Sets the current gravity velocity vector
    --- 
    --- This affects all units owned by the local player
    --- @param x float
    --- @param y float
    --- @param z float
    --- @return nil void
    function ExtraUtils.SetGravity(x, y, z)
        exu.SetGravity(x, y, z)
    end

    --- Gets the current range of the smart cursor (default 200m)
    --- @return float range
    function ExtraUtils.GetSmartCursorRange()
        return exu.GetSmartCursorRange()
    end

    --- Sets the current range of the smart cursor
    --- @param range float
    --- @return nil void
    function ExtraUtils.SetSmartCursorRange(range)
        exu.SetSmartCursorRange(range)
    end

    --- Gets the reticle angle of the local player ranging from -1.0 to 1.0
    --- 
    --- Aligned with the world not local rotation
    --- @return float angle
    function ExtraUtils.GetReticleAngle()
        return exu.GetReticleAngle()
    end

    --- Gets the location on terrain of the player's reticle
    --- 
    --- Must be within smart reticle range, otherwise it will return the last known value
    --- 
    --- Additionally, while looking at an object or into the sky it will return the last known value
    --- @return userdata vector 
    function ExtraUtils.GetReticlePos()
        local reticlePos = exu.GetReticlePos() -- The vector components arrive in a table from the DLL
        local formattedPos = SetVector(reticlePos.x, reticlePos.y, reticlePos.z) -- Make the table into a BZ-usable vector
        return formattedPos
    end

    --- Gets the handle of the object under the local user's reticle, as well as the satellite cursor
    --- 
    --- (The object that appears next to "SPACE No Action (object)" in the command interface)
    --- 
    --- The object must be within smart reticle range in order to be detected
    --- @return handle | nil handle
    function ExtraUtils.GetReticleObject()
        local handle = exu.GetReticleObject()
        if handle == 0 then
            return nil
        else
            return handle
        end
    end

    --- Gets the satellite state of the local player
    --- 
    --- `1` = enabled
    --- 
    --- `0` = disabled
    --- @return boolean state
    function ExtraUtils.GetSatState()
        return exu.GetSatState()
    end

    --- Gets the current position of the cursor in satellite view
    --- 
    --- If satellite is disabled it will return
    --- the last known position of the cursor
    --- @return userdata vector
    function ExtraUtils.GetSatCursorPos()
        local cursorPos = exu.GetSatCursorPos()
        local formattedPos = SetVector(cursorPos.x, cursorPos.y, cursorPos.z)
        return formattedPos
    end

    --- Gets the current position of the player's camera in satellite view
    --- 
    --- The Y coordinate becomes static when you pan the camera, otherwise it follows
    --- the player's Y coordinate
    --- @return userdata vector
    function ExtraUtils.GetSatCamPos()
        local camPos = exu.GetSatCamPos()
        local formattedPos = SetVector(camPos.x, camPos.y, camPos.z)
        return formattedPos
    end

    --- Gets the last position the player clicked in satellite view
    --- 
    --- Persists after satellite is closed so be sure to handle this case
    --- @return userdata vector
    function ExtraUtils.GetSatClickPos()
        local clickPos = exu.GetSatClickPos()
        local formattedPos = SetVector(clickPos.x, clickPos.y, clickPos.z)
        return formattedPos
    end

    --- Gets the current pan speed of satellite view
    --- 
    --- Default = `1250`
    --- @return float speed
    function ExtraUtils.GetSatPanSpeed()
        return exu.GetSatPanSpeed()
    end

    --- Sets the current pan speed of satellite view
    --- 
    --- Default = `1250`
    --- @param speed float
    --- @return nil void
    function ExtraUtils.SetSatPanSpeed(speed)
        exu.SetSatPanSpeed(speed)
    end

    --- Gets the minimum zoom level of satellite view.
    --- 
    --- Default = `2`
    --- @return float
    function ExtraUtils.GetMinSatZoom()
        return exu.GetMinSatZoom()
    end

    --- Sets the minimum zoom level of satellite view.
    --- 
    --- Default = `2`
    --- @param zoom float
    --- @return nil void
    function ExtraUtils.SetMinSatZoom(zoom)
        exu.SetMinSatZoom(zoom)
    end

    --- Gets the maximum zoom level of satellite view.
    --- 
    --- Default = `8`
    --- @return float
    function ExtraUtils.GetMaxSatZoom()
        return exu.GetMaxSatZoom()
    end

    --- Sets the maximum zoom level of satellite view.
    --- 
    --- Default = `8`
    --- @param zoom float
    --- @return nil void
    function ExtraUtils.SetMaxSatZoom(zoom)
        exu.SetMaxSatZoom(zoom)
    end

    --- Gets the current zoom level of satellite view.
    --- 
    --- Default = `4`
    --- @return float
    function ExtraUtils.GetSatZoom()
        return exu.GetSatZoom()
    end

    --- Sets the current zoom level of satellite view.
    --- 
    --- Make sure to stay within the boundaries defined by min and max zoom, otherwise it'll cause issues.
    --- @param zoom float
    --- @return nil void
    function ExtraUtils.SetSatZoom(zoom)
        exu.SetSatZoom(zoom)
    end

    --- Gets the radar state of the local player.
    --- 
    --- 1 = radar, 0 = minimap
    --- @return integer state
    function ExtraUtils.GetRadarState()
        return exu.GetRadarState()
    end

    --- Sets the radar state of the local player.
    --- @param state integer
    --- @return nil void
    function ExtraUtils.SetRadarState(state)
        exu.SetRadarState(state)
    end

    --- Gets the zoom factor of the given camera.
    --- 
    --- Both cameras are tied together except for when you are in third person or freecam, then only the global cam will be active.
    --- @param camera string `'F'` (first person) or `'G'` (global)
    --- @return float | nil
    function ExtraUtils.GetZoomFactor(camera)
        if string.upper(camera) ~= 'F' and string.upper(camera) ~= 'G' then
            error("Extra Utilities Error: Invalid camera")
            return
        end
        return exu.GetZoomFactor(string.upper(camera))
    end

    --- Sets the zoom factor of the given camera.
    --- 
    --- Note this will override the min/max until the player tries to zoom with -+.
    --- @param factor float
    --- @param camera string `'F'` (first person) or `'G'` (global)
    --- @return nil void
    function ExtraUtils.SetZoomFactor(factor, camera)
        if string.upper(camera) ~= 'F' and string.upper(camera) ~= 'G' then
            error("Extra Utilities Error: Invalid camera")
            return
        end
        exu.SetZoomFactor(factor, string.upper(camera))
    end

    --- Gets the minimum zoom factor for all cameras.
    --- @return float factor
    function ExtraUtils.GetMinZoomFactor()
        return exu.GetMinZoomFactor()
    end

    --- Sets the minimum zoom factor for all cameras.
    --- @param factor float
    --- @return nil void
    function ExtraUtils.SetMinZoomFactor(factor)
        exu.SetMinZoomFactor(factor)
    end

    --- Gets the maximum zoom factor for all cameras.
    --- @return float factor
    function ExtraUtils.GetMaxZoomFactor()
        return exu.GetMaxZoomFactor()
    end

    --- Sets the maximum zoom factor for all cameras.
    --- @param factor float
    --- @return nil void
    function ExtraUtils.SetMaxZoomFactor(factor)
        exu.SetMaxZoomFactor(factor)
    end

    --- Gets whether or not a key is held.
    --- 
    --- A full list of keys can be found [here](https://github.com/VTrider/ExtraUtilities/wiki/GetGameKey()-Keycodes).
    --- @param key string Stock `GameKey` calling conventions, single capital letter.
    --- @return boolean
    function ExtraUtils.GetGameKey(key)
        return exu.GetGameKey(string.upper(key)) -- jk I'll convert it to uppercase anyways lol
    end

    --- Gets the Steam 64 ID of the local player.
    --- 
    --- @return string
    function ExtraUtils.GetSteam64()
        return exu.GetSteam64()
    end

    --- Gets the weapon mask for the local user.
    --- 
    --- Uses a 0-based index (first weapon slot returns 0).
    --- 
    --- Note this isn't actually the weapon mask, just
    --- the index of the selected weapon
    --- 
    --- This might be updated in the future with a different 
    --- name or maybe the "real" weapon mask
    --- @return integer weaponMask
    function ExtraUtils.GetWeaponMask()
        return exu.GetWeaponMask()
    end

    --- Gets your current lives.
    --- 
    --- @return integer lives
    function ExtraUtils.GetLives()
        return exu.GetLives()
    end

    --- Sets your current lives.
    --- 
    --- Note: the display value doesn't update until you lose a life.
    --- @param lives integer
    --- @return nil void
    function ExtraUtils.SetLives(lives)
        exu.SetLives(lives)
    end

    --- Gets the local player's difficulty setting.
    --- @return string difficulty
    function ExtraUtils.GetDifficulty()
        return exu.GetDifficulty()
    end

    --- Sets the local player's difficulty setting.
    --- 
    --- Note that it won't change what it says in the menu, but it will in fact change in-game. It also won't work in multiplayer where the difficulty is locked to very hard.
    --- @param newDifficulty string Difficulty as it appears in-game, e.g., `"Very Easy"`, `"Medium"`, `"Very Hard"`, etc.
    --- @return nil void
    function ExtraUtils.SetDifficulty(newDifficulty)
        exu.SetDifficulty(newDifficulty)
    end


    --- Gets the local player's automatic leveling setting
    --- @return boolean setting
    function ExtraUtils.GetAutoLevel()
        return exu.GetAutoLevel()
    end

    --- Sets the local player's automatic leveling setting
    --- @param newAL boolean
    --- @return nil void
    function ExtraUtils.SetAutoLevel(newAL)
        exu.SetAutoLevel(CastBool(newAL))
    end

    --- Gets the local player's target lead indicator setting
    --- @return boolean setting
    function ExtraUtils.GetTLI()
        return exu.GetTLI()
    end

    --- Sets the local player's target lead indicator setting
    --- @param newTLI boolean
    --- @return nil void
    function ExtraUtils.SetTLI(newTLI)
        exu.SetTLI(CastBool(newTLI))
    end

    --- Gets the local player's reverse mouse setting
    --- @return boolean setting
    function ExtraUtils.GetReverseMouse()
        return exu.GetReverseMouse()
    end

    --- Sets the local player's reverse mouse setting
    --- @param newSetting boolean
    --- @return nil void
    function ExtraUtils.SetReverseMouse(newSetting)
        exu.SetReverseMouse(CastBool(newSetting))
    end

    --- Enables velocity inheritance on the local player's ordnance
    --- You can control the ratio of velocity inherited by providing a float argument from 0-1.
    --- 
    --- The default value is 1 (full inheritance). 
    --- 
    --- WARNING: you MUST call Enable BEFORE calling Update,
    --- and you must call UpdateOrdnance in the update loop to apply the patch.
    --- @param ratio? float
    --- @return nil void
    function ExtraUtils.EnableOrdnanceTweak(ratio)
        if IsNetGame == true and debug == false then
            error("Extra Utilities Error: this function is incompatible with multiplayer. Turn on debug mode to override.")
            return
        end
        local velocityScalingFactor
        if not ratio then
            velocityScalingFactor = 1
        else
            velocityScalingFactor = ratio
        end
        if velocityScalingFactor < 0 or velocityScalingFactor > 1 then
            error("Extra Utilities Error: the velocity scaling for EnableOrdnanceTweak must be between 0 and 1!")
            return
        end
        exu.EnableOrdnanceTweak(velocityScalingFactor)
    end

    --- Updates the local player's ordnance velocity inheritance
    --- @param what? string "FULL" for full inheritance, "FRONT" for front only inheritance
    --- @return nil void
    function ExtraUtils.UpdateOrdnance(what)
        if IsNetGame == true and debug == false then
            error("Extra Utilities Error: this function is incompatible with multiplayer. Turn on debug mode to override.")
            return
        end

        local inheritanceType = what or "FULL"
        assert(type(inheritanceType) == "string", "Extra Utilities Error: to specify velocity inheritance, use one of the string parameters")

        local playerVelocity

        if inheritanceType == "FULL" then
            playerVelocity = GetVelocity(GetPlayerHandle())
        elseif inheritanceType == "FRONT" then
            local dot = DotProduct(GetVelocity(GetPlayerHandle()), GetFront(GetPlayerHandle()))
            local playerFront = GetFront(GetPlayerHandle()) * dot
            playerVelocity = SetVector(playerFront.x, 0, playerFront.z) -- we only want the horizontal component
        end

        local playerPosition = GetPosition(GetPlayerHandle())
        exu.UpdateOrdnance(playerVelocity.x, playerVelocity.y, playerVelocity.z, playerPosition.x, playerPosition.y, playerPosition.z)
    end

    --- Activates shot convergence for hovercraft
    --- @return nil void
    function ExtraUtils.EnableShotConvergence()
        exu.EnableShotConvergence()
    end

    --- Sets the behavior of selections when performing certain actions.
    --- For example, if set to false, giving a move order will not deselect units, and the tab key will not work (you'll be locked into your selection); true makes it behave normally.
    --- @param setting boolean
    --- @return nil void
    function ExtraUtils.SetSelectNone(setting)
        exu.SetSelectNone(CastBool(setting))
    end

    --- Sets the local user to a given handle.
    --- 
    --- (Assume control of the ship)
    --- 
    --- Limitations: Will kill the ai inside the ship you control, producers will also break
    --- if you control them
    --- @param handle handle
    --- @return nil void
    function ExtraUtils.SetAsUser(handle)
        if not IsValid(handle) then -- needs to exist, otherwise access violation
            return
        end
        if IsBuilding(handle) then -- buildings will also AV
            return
        end
        exu.SetAsUser(handle)
    end

    --- Selects one unit, overriding previous selections.
    --- @param handle handle
    --- @return nil void
    function ExtraUtils.SelectOne(handle)
        if not IsValid(handle) then
            return
        end
        if IsBuilding(handle) then
            return
        end
        -- commands don't work on selected enemies and it makes the game unstable
        if GetTeamNum(handle) ~= GetTeamNum(GetPlayerHandle()) then
            return
        end
        exu.SelectOne(handle)
    end

    --- Deselects all units, equivalent to pressing tab.
    --- @return nil void
    function ExtraUtils.SelectNone()
        exu.SelectNone()
    end

    --- Adds a unit to the current selection. 
    --- 
    --- Selecting across categories is possible but may break certain behaviors such as hunt, get repair, defend.
    --- @param handle handle
    --- @return nil void
    function ExtraUtils.SelectAdd(handle)
        if not IsValid(handle) then
            return
        end
        if IsBuilding(handle) then
            return
        end
        if GetTeamNum(handle) ~= GetTeamNum(GetPlayerHandle()) then
            return
        end
        exu.SelectAdd(handle)
    end

    --- Returns the absolute directory of the Battlezone executable (should be the default game folder).
    --- @return string directory
    function ExtraUtils.GetWorkingDirectory()
        return exu.GetWorkingDirectory()
    end

    --- Creates an empty folder at the given location. 
    --- 
    --- NOTE: Use backslashes for path names.
    --- @param name string
    --- @return nil void
    function ExtraUtils.MakeDirectory(name)
        exu.MakeDirectory(name)
    end

    --- Reads the contents of the given file into a single formatted string.
    --- @param fileName string
    --- @return string contents
    function ExtraUtils.FileRead(fileName)
        return exu.FileRead(fileName)
    end

    --- Writes the input string into the given file. 
    --- 
    --- NOTE: Overwrites ALL contents of the file, so save it before if you need to.
    --- @param fileName string
    --- @param content string
    --- @return nil void
    function ExtraUtils.FileWrite(fileName, content)
        exu.FileWrite(fileName, content)
    end

    --[[
    -------------------------------------------------------------------------------
    *   Name       : User Logging Class
    *              :
    *   Description: Allows users to create a custom log object to easily write
    *              : debug, warning, or error information to a file that is
    *              : timestamped, but without the bloat of stock bz print.
    *              : The log will automatically clear itself if the size exceeds
    *              : 100kb, otherwise it will continue to write to the same file.
    *              : IO operations are expensive so don't make like 100 logs that
    *              : are writing many times per frame.
    *              : The predefined levels are: 0: OFF, 1: WARNING, 2: ERROR,
    *              : 3: INFO, but you can use them as you please.
    -------------------------------------------------------------------------------
    --]]

    -- OOP Bindings are done in here in lua because doing it in C is aids
    --- @class UserLog
    --- @field pointer lightuserdata DO NOT MODIFY THIS VALUE
    local UserLog = {}
    UserLog.__index = UserLog

    local function RegisterLogObject(logPointer)
        local instance = setmetatable({}, UserLog)
        instance.pointer = logPointer
        return instance
    end

    --- This method outputs the `content` to the log if the `level` of the message is greater than or equal
    --- to the log level.
    --- 
    --- It will always be casted to a string so no need to call tostring() beforehand
    --- 
    --- The default `level` if none is given is `3`, or `INFO`
    --- @param content any
    --- @param level? integer
    --- @return nil void
    function UserLog:Out(content, level)
       exu.LogOut(self.pointer, tostring(content), level)
    end

    --- This method returns the current `level` of the log
    --- @return integer
    function UserLog:GetLevel()
        return exu.GetLogLevel(self.pointer)
    end

    --- This method sets the `level` of the log to the given value
    --- @param level integer
    --- @return nil void
    function UserLog:SetLevel(level)
        exu.SetLogLevel(self.pointer, level)
    end

    --- This method returns the current `path` of the log
    --- @return string
    function UserLog:GetPath()
        return exu.GetLogPath(self.pointer)
    end

    --- This method sets the current `path` of the log
    --- @param path string
    --- @return nil void
    function UserLog:SetPath(path)
        exu.SetLogPath(self.pointer, path)
    end

    --- Creates a unique log object
    --- 
    --- `path` is the relative path from the root directory (Battlezone 98 Redux), but it can also be an absolute path (not recommended)
    --- 
    --- `level` is the user defined logging level to describe how verbose the log will be, default if none given is 3
    --- @param path string
    --- @param level? integer
    --- @return UserLog
    function ExtraUtils.CreateLog(path, level)
        return RegisterLogObject(exu.CreateLog(path, level))
    end

    --- Changes the diffuse color of the headlight on the given ship.
    --- 
    --- LIMITATIONS: Only tested on ships, unknown if other game objects with lights will work. The script must observe the object being created to register the handle with the light;
    --- pre-placed objects on the map will not work.
    --- @param handle handle
    --- @param red float default 1.0
    --- @param green float default 1.0
    --- @param blue float default 1.0
    --- @return boolean
    function ExtraUtils.SetDiffuseColor(handle, red, green, blue)
        local _red = red or 1.0
        local _green = green or 1.0
        local _blue = blue or 1.0
        local label = GetLabel(handle)
        return exu.SetDiffuseColor(label, _red, _green, _blue)
    end

    --- Changes the specular color of the headlight on the given ship.
    --- 
    --- LIMITATIONS: Only tested on ships, unknown if other game objects with lights will work. The script must observe the object being created to register the handle with the light;
    --- pre-placed objects on the map will not work.
    --- @param handle handle
    --- @param red float default 1.0
    --- @param green float default 1.0
    --- @param blue float default 1.0
    --- @return boolean
    function ExtraUtils.SetSpecularColor(handle, red, green, blue)
        local _red = red or 1.0
        local _green = green or 1.0
        local _blue = blue or 1.0
        local label = GetLabel(handle)
        return exu.SetSpecularColor(label, _red, _green, _blue)
    end

    --- Macro to use both light color functions at the same time.
    --- @param handle handle
    --- @param red float
    --- @param green float
    --- @param blue float
    --- @return nil void
    function ExtraUtils.SetColor(handle, red, green, blue)
        SetDiffuseColor(handle, red, green, blue)
        SetSpecularColor(handle, red, green, blue)
    end

    --- Sets the range of a spotlight, i.e., the angle of the inner and outer cones and the rate of falloff between them.
    --- 
    --- Angles in `radians`
    --- @param handle handle
    --- @param innerAngle float default = 0.1745
    --- @param outerAngle float default = 0.35
    --- @param falloff float default 1.0 is linear, <1 or >1 is exponential falloff
    --- @return boolean
    function ExtraUtils.SetSpotlightRange(handle, innerAngle, outerAngle, falloff)
        local _falloff = falloff or 1.0
        local _outerAngle = outerAngle or 0.35
        local _innerAngle = innerAngle or 0.1745
        local label = GetLabel(handle)
        return exu.SetSpotlightRange(label, _innerAngle, _outerAngle, _falloff)
    end

    --[[
    -------------------------------------------------------------------------------
    *   Name       : Audio System
    *              :
    *   Description: Play dynamic sound effects that let you change their volume, 
    *              : paused, and looping status. There are no restrictions on
    *              : file names (goodbye 8 character limit), and there are 256
    *              : sound channels available. Note that sources are recycled after
    *              : they stop playing so do not hold on to the handle of an old
    *              : source or you may accidently change the parameters of a
    *              : different one. Supports .wav, .mp3, .ogg (vorbis & opus), 
    *              : .aiff, .flac, and much more, for a full list see: 
    *              : <https://libsndfile.github.io/libsndfile/formats.html>
    -------------------------------------------------------------------------------
    --]]

    --- @class Sound
    --- @field handle integer DO NOT MODIFY
    --- @field ActiveSounds table DO NOT MODIFY
    local Sound = {}
    Sound.__index = Sound

    Sound.ActiveSounds = {}

    --- Plays the sound file
    --- 
    --- Note 3D sounds are required to be mono sound files
    --- 
    --- If you attempt to play a stereo file as a 3D sound it
    --- will play as 2D (globally)
    --- @param filePath string
    --- @param position any
    --- @param volume float
    --- @param loop boolean
    --- @return Sound SoundObject
    function ExtraUtils.PlaySound(filePath, position, volume, loop)
        --- @class Sound
        local instance = setmetatable({}, Sound)

        instance.handle = exu.PlaySound(filePath)
        instance.position = position
        Sound.ActiveSounds[instance.handle] = position

        if volume ~= nil then
            instance:SetVolume(volume)
        end
        if loop ~= nil then
            instance:SetLooping(loop)
        end

        return instance
    end

    --- Returns the global volume level
    --- @return float volume 0.0 to 1.0
    function ExtraUtils.GetMainVolume()
        return exu.GetMainVolume()
    end

    --- Sets the global volume level
    --- @param volume float 0.0 to 1.0
    --- @return nil void
    function ExtraUtils.SetMainVolume(volume)
        exu.SetMainVolume(Clamp(volume, 0.0, 1.0))
    end

    --- Gets the volume level of the given sound
    --- @return float volume
    function Sound:GetVolume()
        return exu.GetVolume(self.handle)
    end

    --- Sets the volume level of the given sound
    --- @param volume float  0.0 to 1.0
    --- @return nil void
    function Sound:SetVolume(volume)
        exu.SetVolume(self.handle, Clamp(volume, 0.0, 1.0))
    end

    --- Gets the pause status of the sound
    --- 
    --- Note that paused sounds are not recycled and
    --- must be stopped manually in order for the sound
    --- channel to be freed
    --- @return boolean pause
    function Sound:GetPaused()
        return exu.GetPaused(self.handle)
    end

    --- Sets the paused status of the sound
    --- @param paused boolean
    --- @return nil void
    function Sound:SetPaused(paused)
        exu.SetPaused(self.handle, CastBool(paused))
    end

    --- Stops the sound and frees its handle
    --- 
    --- You should set the sound object to nil after
    --- calling this as good practice
    --- @return nil void
    function Sound:Stop()
        exu.Stop(self.handle)
    end

    --- Gets the looping status of the sound
    --- @return boolean
    function Sound:GetLooping()
        return exu.GetLooping(self.handle)
    end

    --- Sets the looping status of the sound
    --- @param looping boolean
    --- @return nil void
    function Sound:SetLooping(looping)
        exu.SetLooping(self.handle, CastBool(looping))
    end

    local function SetListenerTransform(transform, velocity)
        exu.GetListenerTransform(
            transform.posit_x, transform.posit_y, transform.posit_z,
            velocity.x, velocity.y, velocity.z,
            transform.front_x, transform.front_y, transform.front_z,
            transform.up_x, transform.up_y, transform.up_z)
    end

    local function SetSourceTransform(source, position, velocity)
        exu.GetSourceTransform(source,
            position.x, position.y, position.z,
            velocity.x, velocity.y, velocity.z)
    end

    --- In order to use 3D sound it is REQUIRED to call
    --- this in the update loop of your main script
    --- @param dt float delta time
    --- @return nil void
    function ExtraUtils.Update(dt)
        local playerHandle = GetPlayerHandle()
        local playerTransform = GetTransform(playerHandle)
        local playerVelocity = GetVelocity(playerHandle)

        SetListenerTransform(playerTransform, playerVelocity)

        for handle, position in pairs(Sound.ActiveSounds) do
            SetSourceTransform(handle, position, SetVector(0, 0, 0))
        end
    end

end
return ExtraUtils