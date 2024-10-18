--[[
=============================================================
*   Extra Utilities                                         *
*   Version 0.8.4                                           *
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
exu.Init()

local ExtraUtils = {}
do
    --------------
    -- Metadata --
    --------------

    -- Use the version and/or the crc32 to check for mod version
    -- mismatches that aren't caught by the game
    ExtraUtils.version = exu_api.GetVersion()
    ExtraUtils.crc32 = "E866527C"
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

    --- DBZ Directory Functions:

    -- Function to print field names of an object
    local function SplitAtSemicolon(str)
        local results = {}
        for substr in string.gmatch(str, '([^;]+)') do
            table.insert(results, substr)
        end
        return results
    end

    local function GetGameDirectory()
        local path = SplitAtSemicolon(package.cpath)[2]
        return string.match(path, "(.*)\\%?")
    end

    -------------
    -- Exports --
    -------------

    --- Note to modders: never call undocumented exu_api.xxx or local functions
    --- since those are internal and usually provide an interface with the
    --- game to the dll

    -- Important Misc

    function ExtraUtils.SetAccessMode(mode)
        if mode ~= 0 and mode ~= 1 then
            error("Extra Utilities Error: input must be either 0 or 1")
            return
        end
        exu_api.SetAccessMode(mode)
    end

    ExtraUtils.GetObj = exu_api.GetObj

    -- Environment

    ExtraUtils.GetGravity = exu_api.GetGravity
    ExtraUtils.SetGravity = exu_api.SetGravity

    -- Reticle

    ExtraUtils.GetSmartCursorRange = exu_api.GetSmartCursorRange
    ExtraUtils.SetSmartCursorRange = exu_api.SetSmartCursorRange
    ExtraUtils.GetReticleAngle = exu_api.GetReticleAngle
    ExtraUtils.GetReticlePos = exu_api.GetReticlePos
    ExtraUtils.GetReticleObject = exu_api.GetReticleObject

    -- Satellite

    ExtraUtils.GetSatState = exu_api.GetSatState
    ExtraUtils.GetSatCursorPos = exu_api.GetSatCursorPos
    ExtraUtils.GetSatCamPos = exu_api.GetSatCamPos
    ExtraUtils.GetSatClickPos = exu_api.GetSatClickPos
    ExtraUtils.GetSatPanSpeed = exu_api.GetSatPanSpeed
    ExtraUtils.SetSatPanSpeed = exu_api.SetSatPanSpeed
    ExtraUtils.GetMinSatZoom = exu_api.GetMinSatZoom
    ExtraUtils.SetMinSatZoom = exu_api.SetMinSatZoom
    ExtraUtils.GetMaxSatZoom = exu_api.GetMaxSatZoom
    ExtraUtils.SetMaxSatZoom = exu_api.SetMaxSatZoom
    ExtraUtils.GetSatZoom = exu_api.GetSatZoom
    ExtraUtils.SetSatZoom = exu_api.SetSatZoom

    -- Radar

    ExtraUtils.GetRadarState = exu_api.GetRadarState
    ExtraUtils.SetRadarState = exu_api.SetRadarState

    -- Camera

    ExtraUtils.GetZoomFactor = exu_api.GetZoomFactor
    ExtraUtils.SetZoomFactor = exu_api.SetZoomFactor
    ExtraUtils.GetMinZoomFactor = exu_api.GetMinZoomFactor
    ExtraUtils.SetMinZoomFactor = exu_api.SetMinZoomFactor
    ExtraUtils.GetMaxZoomFactor = exu_api.GetMaxZoomFactor
    ExtraUtils.SetMaxZoomFactor = exu_api.SetMaxZoomFactor

    -- IO

    ExtraUtils.GetGameKey = exu_api.GetGameKey

    -- Misc

    ExtraUtils.GetSteam64 = exu_api.GetSteam64
    ExtraUtils.GetWeaponMask = exu_api.GetWeaponMask
    ExtraUtils.GetLives = exu_api.GetLives
    ExtraUtils.SetLives = exu_api.SetLives
    ExtraUtils.GetDifficulty = exu_api.GetDifficulty
    ExtraUtils.SetDifficulty = exu_api.SetDifficulty
    ExtraUtils.GetAutoLevel = exu_api.GetAutoLevel
    ExtraUtils.SetAutoLevel = exu_api.SetAutoLevel
    ExtraUtils.GetTLI = exu_api.GetTLI
    ExtraUtils.SetTLI = exu_api.SetTLI
    ExtraUtils.GetReverseMouse = exu_api.GetReverseMouse
    ExtraUtils.SetReverseMouse = exu_api.SetReverseMouse

    -- Patches

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

    -- Filesystem

    ExtraUtils.GetWorkingDirectory = exu_api.GetWorkingDirectory

    --- Gets the steam workshop directory, use this to
    --- relative path to your workshop item folder in order to access
    --- files of interest
    --- @return string workshopDir
    function ExtraUtils.GetSteamWorkshopDirectory()
        local gameDirectory = GetGameDirectory()
        local workshopRelativePath = "steamapps/workshop/content/301650"

        -- Remove the common directory from the game directory
        local commonDirectory = "steamapps\\common\\Battlezone 98 Redux"
        local commonDirectoryIndex = string.find(gameDirectory, commonDirectory)

        if commonDirectoryIndex then
            gameDirectory = gameDirectory:sub(1, commonDirectoryIndex - 2)
        end

        -- Combine paths using path.join
        local steamWorkshopDirectory = gameDirectory .. "\\" .. workshopRelativePath:gsub("/", "\\")

        return steamWorkshopDirectory
    end

    ExtraUtils.MakeDirectory = exu_api.MakeDirectory
    ExtraUtils.FileRead = exu_api.FileRead
    ExtraUtils.FileWrite = exu_api.FileWrite

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

    --- Plays the sound file at the given path. (Relative to BZR root by default)  
    --- (Note that you need to use an absolute path for a sound file in a workshop folder)
    --- 
    --- The position can be either a vector position for a positional emitter,  
    --- or it can be a handle in which case the emitter will follow the given object  
    --- throughout its lifetime.
    --- 
    --- Note 3D sounds are required to be mono sound files.  
    --- If you attempt to play a stereo file as a 3D sound it
    --- will play as 2D (globally).
    --- 
    --- `WARNING:` In order to use 3D sounds you MUST call exu.Update()
    --- in your map script's update loop
    --- @param filePath string
    --- @param position? any
    --- @param volume? float
    --- @param loop? boolean
    --- @return Sound SoundObject
    function ExtraUtils.PlaySound(filePath, position, volume, loop)
        --- @class Sound
        local instance = setmetatable({}, Sound)

        instance.handle = exu.PlaySound(filePath)
        instance.path = filePath
        instance.global = true -- marks a global (2D) sound
        instance.velocity = SetVector(0, 0, 0)

        -- One time warnings
        instance.monoWarningShown = false
        instance.stereoWarningShown = false

        -- This will overwrite sounds that have been cleaned up so the 
        -- table size doesn't get out of control
        Sound.ActiveSounds[instance.handle] = instance

        if position ~= nil then
            if type(position) ~= "userdata" then
                error("Extra Utilities error: position must be either a handle or vector")
            end

            instance.global = false

            -- Hack to determine if position is a handle or vector
            if GetPosition(position) == SetVector(0, 0, 0) then
                instance.position = position -- position is vector
                exu_api.SysLogOut(tostring(position))
            else
                instance.obj = position -- position is handle of game object
            end
        end

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

    --- Stops all currently playing sounds
    --- @return nil void
    function ExtraUtils.StopAllSounds()
        for _, sound in pairs(Sound.ActiveSounds) do
            sound:Stop()
        end
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

    --- (3D only) Gets the reference distance of the source.
    --- The distance under which gain is clamped to 1.0 (max)
    --- 
    --- The default is 1.0 and results in a small point emitter
    --- 
    --- This controls the "size" of the sound emitter
    --- @return float distance
    function Sound:GetReferenceDist()
        return exu.GetSourceRefDist(self.handle)
    end

    --- (3D only) Gets the reference distance of the source.
    --- The distance under which gain is clamped to 1.0 (max)
    --- 
    --- The default is 1.0 and results in a small point emitter
    --- 
    --- This controls the "size" of the sound emitter
    --- @param distance float
    --- @return nil void
    function Sound:SetReferenceDist(distance)
        exu.SetSourceRefDist(self.handle, distance)
    end

    --- (3D only) Gets the rolloff factor of the source.
    --- Controls the attenuation of the source over distance
    --- 
    --- The default is `1.0`, lower will result in slower attenuation,
    --- higher will result in faster attenuation
    --- 
    --- Use this combined with reference distance to control the 
    --- size and scope of a 3D sound emitter
    --- @return float rolloff
    function Sound:GetRolloff()
        return exu.GetSourceRolloff(self.handle)
    end

    --- (3D only) Sets the rolloff factor of the source.
    --- Controls the attenuation of the source over distance
    --- 
    --- The default is `1.0`, lower will result in slower attenuation,
    --- higher will result in faster attenuation
    --- 
    --- Use this combined with reference distance to control the 
    --- size and scope of a 3D sound emitter
    --- @param rolloff float
    --- @return nil void
    function Sound:SetRolloff(rolloff)
        return exu.SetSourceRolloff(self.handle, rolloff)
    end

    --- (3D only) Gets the max distance of the source.
    --- Clamps the attenuation of the source to the given distance.
    --- 
    --- For example if you want a sound to never fully attenuate,
    --- set the max distance to a reasonable value
    --- 
    --- Default = `MAX_FLOAT` (effectively infinite)
    --- @return float distance
    function Sound:GetMaxDist()
        return exu.GetSourceMaxDist(self.handle)
    end

    --- (3D only) Gets the max distance of the source.
    --- Clamps the attenuation of the source to the given distance.
    --- 
    --- For example if you want a sound to never fully attenuate,
    --- set the max distance to a reasonable value
    --- 
    --- Default = `MAX_FLOAT` (effectively infinite)
    --- @param maxDist float
    --- @return nil void
    function Sound:SetMaxDist(maxDist)
        return exu.SetSourceMaxDist(self.handle, maxDist)
    end

    --- Internal do not use
    local function SetListenerTransform(transform, velocity)
        exu.GetListenerTransform(
            transform.posit_x, transform.posit_y, transform.posit_z,
            velocity.x, velocity.y, velocity.z,
            transform.front_x, transform.front_y, transform.front_z,
            transform.up_x, transform.up_y, transform.up_z)
    end

    --- Internal do not use
    local function SetSourceTransform(source, position, velocity)
        exu.GetSourceTransform(source,
            position.x, position.y, position.z,
            velocity.x, velocity.y, velocity.z)
    end

    --- (3D only) Sets the position and velocity of the source
    --- @param position any
    --- @param velocity any
    --- @return nil void
    function Sound:SetTransform(position, velocity)
        if velocity == nil then
            velocity = SetVector(0, 0, 0)
        end
        if self.position == nil then
            self.position = SetVector(0, 0, 0)
        end
        self.position.x = velocity.x
        self.position.y = velocity.y
        self.position.z = velocity.z
        exu.GetSourceTransform(self.handle,
        position.x, position.y, position.z,
        velocity.x, velocity.y, velocity.z)
    end

    function Sound:IsMono()
        return exu.SourceIsMono(self.handle)
    end

    --- Internal do not use
    --- @param handle integer
    --- @param sound Sound
    local function UpdateSource(handle, sound, playerPos)
        if sound.global == true and sound:IsMono() == false then return end

        if sound:IsMono() == false then
            if not sound.stereoWarningShown then -- stops log spam
                exu_api.SysLogOut(string.format("[WARNING] Unable to play stereo sound %s as 3D, use a mono sound file", sound.path), 2)
                sound.stereoWarningShown = true
            end
        elseif sound.global == true then
            if not sound.monoWarningShown then
                exu_api.SysLogOut(string.format("[WARNING] mono file %s is playing globally, recommend using a stereo file for global sounds", sound.path), 2)
                sound.monoWarningShown = true
            end
            -- need to "fake" global mono sounds
            -- in OpenAL
            sound.position = playerPos
            sound.velocity = SetVector(0, 0, 0)
        end

        if sound.obj ~= nil then
            if not IsValid(sound.obj) then
                sound:Stop()
            end
            sound.position = GetPosition(sound.obj)
            sound.velocity = GetVelocity(sound.obj)
        end

        SetSourceTransform(handle, sound.position, sound.velocity)
    end

    --- In order to use 3D sound it is REQUIRED to call
    --- this in the update loop of your main script
    --- @param dt float delta time
    --- @return nil void
    function ExtraUtils.Update(dt)
        -- If you're an optimization freak you can plug in your own
        -- globals if you store them, but this way it's portable and
        -- should have relatively low overhead since tables are passed
        -- by reference
        local playerHandle = GetPlayerHandle()
        local playerTransform = GetTransform(playerHandle)
        local playerVelocity = GetVelocity(playerHandle)

        SetListenerTransform(playerTransform, playerVelocity)

        --- @param handle integer
        --- @param sound Sound
        for handle, sound in pairs(Sound.ActiveSounds) do
            UpdateSource(handle, sound, GetPosition(playerHandle))
        end
    end

end
return ExtraUtils