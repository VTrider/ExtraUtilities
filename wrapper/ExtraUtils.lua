--[[
=======================================================
*   Extra Utilities
*   Version 0.7.0
=======================================================
*   This module extends scripting functionality
*   through a custom DLL and adds some useful 
*   helper functions
=======================================================
*   Special thanks to:
*   - Janne, for letting me use her dll code
*   - DivisionByZero, for the game directory function
*   - GrizzlyOne95, for the workshop directory function
=======================================================
*   - Script and DLL extension made by VTrider
=======================================================
--]]

--- To use function/type annotations install the Lua langauge server plugin
--- for vscode by sumneko

--- Typedefs for annotations
--- @alias handle lightuserdata
--- @alias pointer lightuserdata
--- @alias float number
--- @alias vector userdata
--- @alias matrix userdata
--- @alias void nil

local exu = require("exu")

local extraUtils = {}
do
    -- Metadata
    local version = exu.GetVersion()
    local crc32 = "EB227D80"
    local debug = false

    local function Start() -- put this in function Start() to print out metadata to console
        print("--------------------------------------------------------------------------------------")
        print("Extra Utilities loaded! Version: " .. version .. "                                                  ")
        print("--------------------------------------------------------------------------------------")
    end

    -- Helpers

    --- Lua C API doesn't have a check boolean function so this casts
    --- a bool to an integer
    ---@param value boolean
    ---@return integer | nil
    local function CastBool(value) -- TODO FIX ALWAYS RETURNING 0
        if type(value) ~= "boolean" then 
            error("Extra Utilities Error: input must be a bool")
            return
        end
        return value and 1 or 0
    end

    --[[
    ------------------------------------------------------------------------------------
    *   Name       : SetAccessMode
    *   Description: Sets the method for accessing the game's memory, valid
    *                options: 0 (direct access - blazing fast but potentially unstable),
    *                1 (windows api - much slower but potentially more stable), default
    *                is 0 (direct access)
    *   Inputs     : Int mode 0 or 1
    *   Outputs    : New access mode
    *   Return Type: Void
    -----------------------------------------------------------------------------------
    --]]

    --- @param mode integer
    --- @return void
    local function SetAccessMode(mode)
        if mode ~= 0 and mode ~= 1 then
            error("Extra Utilities Error: input must be either 0 or 1")
            return
        end
        exu.SetAccessMode(mode)
    end

    --[[
    ---------------------------------------------------------------------------
    *   Name       : GetObj
    *   Description: Converts a handle to it's memory address, mainly for debug
    *                purposes 
    *   Inputs     : Handle to an object
    *   Outputs    : GameObject pointer
    *   Return Type: Lightuserdata
    ---------------------------------------------------------------------------
    --]]

    --- @param handle handle
    --- @return pointer
    local function GetObj(handle)
        return exu.GetObj(handle)
    end

    --[[
    -------------------------------------------------------------
    *   Name       : GetGravity
    *   Description: Gets the current value of the gravity vector
    *   Inputs     : None
    *   Outputs    : VECTOR_3D velocity: X, Y, Z
    *   Return Type: Userdata
    -------------------------------------------------------------
    --]]

    --- @return vector
    local function GetGravity()
        local gravityVector = exu.GetGravity()
        local formattedVector = SetVector(gravityVector.x, gravityVector.y, gravityVector.z)
        return formattedVector
    end

    --[[
    --------------------------------------------------------------
    *   Name       : SetGravity
    *   Description: Sets the current value of the gravity vector 
    *   Inputs     : Float X, Y, Z values
    *   Outputs    : New gravity value
    *   Return Type: Void
    --------------------------------------------------------------
    --]]

    --- @param x float
    --- @param y float
    --- @param z float
    --- @return void
    local function SetGravity(x, y, z)
        exu.SetGravity(x, y, z)
    end

    --[[
    ---------------------------------------------------------------------------
    *   Name       : GetSmartCursorRange
    *   Description: Gets the current range of the smart cursor (stock is 200m)
    *   Inputs     : None
    *   Outputs    : Current range of smart cursor
    *   Return Type: Number
    ---------------------------------------------------------------------------
    --]]

    --- @return float
    local function GetSmartCursorRange()
        return exu.GetSmartCursorRange()
    end

    --[[
    ---------------------------------------------------------------------------
    *   Name       : SetSmartCursorRange
    *   Description: Sets the current range of the smart cursor
    *   Inputs     : Float new range
    *   Outputs    : New smart cursor range
    *   Return Type: Void
    ---------------------------------------------------------------------------
    --]]

    --- @param range float
    --- @return void
    local function SetSmartCursorRange(range)
        exu.SetSmartCursorRange(range)
    end

    --[[
    ------------------------------------------------------------------------
    *   Name       : GetReticleAngle
    *   Description: Gets the reticle angle of the local player
    *   Inputs     : None
    *   Outputs    : -1.0 to 1.0 with 0 as level with horizon, note: aligned
    *                with world not local rotation
    *   Return Type: Number
    ------------------------------------------------------------------------
    --]]

    --- @return float
    local function GetReticleAngle()
        return exu.GetReticleAngle()
    end

    --[[
    ------------------------------------------------------------------------------------------------------
    *   Name       : GetReticlePos
    *   Description: Gets the location on terrain of the player's reticle
    *   Inputs     : None
    *   Outputs    : VECTOR_3D coordinates: X, Y, Z
    *   Return Type: Userdata
    ------------------------------------------------------------------------------------------------------
    --]]

    ---@return vector
    local function GetReticlePos()
        local reticlePos = exu.GetReticlePos() -- The vector components arrive in a table from the DLL
        local formattedPos = SetVector(reticlePos.x, reticlePos.y, reticlePos.z) -- Make the table into a BZ-usable vector
        return formattedPos
    end

    --[[
    -------------------------------------------------------------
    *   Name       : GetSatState
    *   Description: Gets the satellite state of the local player
    *   Inputs     : None
    *   Outputs    : 1 = enabled, 0 = disabled
    *   Return Type: Number
    -------------------------------------------------------------
    --]]

    --- @return boolean
    local function GetSatState()
        return exu.GetSatState()
    end

    --[[
    ---------------------------------------------------------------------------
    *   Name       : GetSatCursorPos
    *   Description: Gets the current position of the cursor in satellite view,
    *                if satellite is disabled then it will return the LAST
    *                known position of the cursor, or possibly an undefined
    *                value under some circumstances, be careful
    *   Inputs     : None
    *   Outputs    : VECTOR_3D coordinates: X, Y, Z
    *   Return Type: Userdata
    ---------------------------------------------------------------------------
    --]]

    --- @return vector
    local function GetSatCursorPos()
        local cursorPos = exu.GetSatCursorPos()
        local formattedPos = SetVector(cursorPos.x, cursorPos.y, cursorPos.z)
        return formattedPos
    end

    --[[
    ----------------------------------------------------------------------
    *   Name       : GetSatCamPos
    *   Description: Gets the current position of the player's camera in
    *                satellite view. Note: the Y coordinate becomes static
    *                when you pan the camera, otherwise it follows the
    *                player's Y coordinate
    *   Inputs     : None
    *   Outputs    : VECTOR_3D coordinates: X, Y, Z
    *   Return Type: Userdata
    ----------------------------------------------------------------------
    --]]

    --- @return vector
    local function GetSatCamPos()
        local camPos = exu.GetSatCamPos()
        local formattedPos = SetVector(camPos.x, camPos.y, camPos.z)
        return formattedPos
    end

    --[[
    -------------------------------------------------------------------------
    *   Name       : GetSatClickPos
    *   Description: Gets the last position the player clicked in satellite
    *                view. NOTE: value will persist even after satellite view
    *                is closed, make sure to handle this to avoid unintended
    *                behavior
    *   Inputs     : None
    *   Outputs    : VECTOR_3D coordinates: X, Y, Z
    *   Return Type: Userdata
    -------------------------------------------------------------------------
    --]]

    --- @return vector
    local function GetSatClickPos()
        local clickPos = exu.GetSatClickPos()
        local formattedPos = SetVector(clickPos.x, clickPos.y, clickPos.z)
        return formattedPos
    end

    --[[
    --------------------------------------------------------------
    *   Name       : GetSatPanSpeed
    *   Description: Gets the current pan speed of satellite view.
    *                Default = 1250
    *   Inputs     : None
    *   Outputs    : Current pan speed
    *   Return Type: Number
    --------------------------------------------------------------
    --]]


    --- @return float
    local function GetSatPanSpeed()
        return exu.GetSatPanSpeed()
    end

    --[[
    --------------------------------------------------------------
    *   Name       : SetSatPanSpeed
    *   Description: Sets the current pan speed of satellite view.
    *                Default = 1250
    *   Inputs     : Float pan speed
    *   Outputs    : New pan speed
    *   Return Type: Void
    --------------------------------------------------------------
    --]]

    --- @param speed float
    --- @return void
    local function SetSatPanSpeed(speed)
        exu.SetSatPanSpeed(speed)
    end

    --[[
    ---------------------------------------------------------------
    *   Name       : GetMinSatZoom
    *   Description: Gets the minimum zoom level of satellite view.
    *                Default = 2
    *   Inputs     : None
    *   Outputs    : Current minimum zoom
    *   Return Type: Number
    ---------------------------------------------------------------
    --]]

    --- @return float
    local function GetMinSatZoom()
        return exu.GetMinSatZoom()
    end

    --[[
    ---------------------------------------------------------------
    *   Name       : SetMinSatZoom
    *   Description: Sets the minimum zoom level of satellite view.
    *                Default = 2
    *   Inputs     : Float new minimum zoom
    *   Outputs    : New minimum zoom
    *   Return Type: Void
    ---------------------------------------------------------------
    --]]

    --- @param zoom float
    --- @return void
    local function SetMinSatZoom(zoom)
        exu.SetMinSatZoom(zoom)
    end

    --[[
    ---------------------------------------------------------------
    *   Name       : GetMaxSatZoom
    *   Description: Gets the Maximum zoom level of satellite view.
    *                Default = 8
    *   Inputs     : None
    *   Outputs    : Current Maximum zoom
    *   Return Type: Number
    ---------------------------------------------------------------
    --]]

    --- @return float
    local function GetMaxSatZoom()
        return exu.GetMaxSatZoom()
    end

    --[[
    ---------------------------------------------------------------
    *   Name       : SetMaxSatZoom
    *   Description: Sets the Maximum zoom level of satellite view.
    *                Default = 8
    *   Inputs     : Float new Maximum zoom
    *   Outputs    : New Maximum zoom
    *   Return Type: Void
    ---------------------------------------------------------------
    --]]

    --- @param zoom float
    --- @return void
    local function SetMaxSatZoom(zoom)
        exu.SetMaxSatZoom(zoom)
    end

    --[[
    ---------------------------------------------------------------
    *   Name       : GetSatZoom
    *   Description: Gets the current zoom level of satellite view.
    *                Default = 4
    *   Inputs     : None
    *   Outputs    : Current satellite zoom level
    *   Return Type: Number
    ---------------------------------------------------------------
    --]]

    --- @return float
    local function GetSatZoom()
        return exu.GetSatZoom()
    end

    --[[
    ------------------------------------------------------------------
    *   Name       : SetSatZoom
    *   Description: Sets the current zoom level of satellite view.
    *                Make sure to stay within the boundaries defined
    *                by min and max zoom, otherwise it'll cause issues
    *   Inputs     : Float zoom level
    *   Outputs    : New satellite zoom level
    *   Return Type: Void
    ------------------------------------------------------------------
    --]]

    --- @param zoom float
    --- @return void
    local function SetSatZoom(zoom)
        exu.SetSatZoom(zoom)
    end

    --[[
    ---------------------------------------------------------
    *   Name       : GetRadarState
    *   Description: Gets the radar state of the local player
    *   Inputs     : None
    *   Outputs    : 1 = radar, 0 = minimap
    *   Return Type: Number
    ---------------------------------------------------------
    --]]

    --- @return integer
    local function GetRadarState()
        return exu.GetRadarState()
    end

    --[[
    ---------------------------------------------------------
    *   Name       : SetRadarState
    *   Description: Sets the radar state of the local player
    *   Inputs     : Desired radar state
    *   Outputs    : New radar state
    *   Return Type: Void
    ---------------------------------------------------------
    --]]

    --- @param state integer
    --- @return void
    local function SetRadarState(state)
        exu.SetRadarState(state)
    end

    --[[
    ----------------------------------------------------------------------------------
    *   Name       : GetZoomFactor
    *   Description: Gets the zoom factor of the given camera, both cameras are tied
    *              : together except for when you are in third person or freecam, then
    *              : only the global cam will be active.
    *   Inputs     : String 'F' (first person) or 'G' (global)
    *   Outputs    : Current zoom factor
    *   Return Type: Float
    ----------------------------------------------------------------------------------
    --]]

    --- @param camera string
    --- @return float | nil
    local function GetZoomFactor(camera)
        if string.upper(camera) ~= 'F' and string.upper(camera) ~= 'G' then
            error("Extra Utilities Error: Invalid camera")
            return
        end
        return exu.GetZoomFactor(string.upper(camera))
    end

    --[[
    -----------------------------------------------------------------------------
    *   Name       : SetZoomFactor
    *   Description: Sets the zoom factor of the given camera. Note this will
    *              : override the min/max until the player tries to zoom with -+
    *   Inputs     : Float zoom factor, String 'F' (first person) or 'G' (global)
    *   Outputs    : New zoom factor
    *   Return Type: Void
    -----------------------------------------------------------------------------
    --]]

    --- @param factor float
    --- @param camera string
    --- @return void
    local function SetZoomFactor(factor, camera)
        if string.upper(camera) ~= 'F' and string.upper(camera) ~= 'G' then
            error("Extra Utilities Error: Invalid camera")
            return
        end
        exu.SetZoomFactor(factor, string.upper(camera))
    end

    --[[
    -----------------------------------------------------------------------------
    *   Name       : GetMinZoomFactor
    *   Description: Gets the minimum zoom factor for all cameras
    *   Inputs     : None
    *   Outputs    : Current minimum zoom factor
    *   Return Type: Float
    -----------------------------------------------------------------------------
    --]]

    --- @return float
    local function GetMinZoomFactor()
        return exu.GetMinZoomFactor()
    end

    --[[
    -----------------------------------------------------------------------------
    *   Name       : SetMinZoomFactor
    *   Description: Sets the minimum zoom factor for all cameras
    *   Inputs     : Float desired minimum zoom factor
    *   Outputs    : New minimum zoom factor
    *   Return Type: Void
    -----------------------------------------------------------------------------
    --]]

    --- @param factor float
    --- @return void
    local function SetMinZoomFactor(factor)
        exu.SetMinZoomFactor(factor)
    end

        --[[
    -----------------------------------------------------------------------------
    *   Name       : GetMaxZoomFactor
    *   Description: Gets the maximum zoom factor for all cameras
    *   Inputs     : None
    *   Outputs    : Current maximum zoom factor
    *   Return Type: Float
    -----------------------------------------------------------------------------
    --]]

    --- @return float
    local function GetMaxZoomFactor()
        return exu.GetMaxZoomFactor()
    end

    --[[
    -----------------------------------------------------------------------------
    *   Name       : SetMaxZoomFactor
    *   Description: Sets the maximum zoom factor for all cameras
    *   Inputs     : Float desired maximum zoom factor
    *   Outputs    : New maximum zoom factor
    *   Return Type: Void
    -----------------------------------------------------------------------------
    --]]

    --- @param factor float
    --- @return void
    local function SetMaxZoomFactor(factor)
        exu.SetMaxZoomFactor(factor)
    end

    --[[
    ------------------------------------------------------------------------------------------
    *   Name       : GetGameKey
    *   Description: Gets whether or not a key is held, a full list of keys can be found here:
    *              : https://github.com/VTrider/ExtraUtilities/wiki/GetGameKey()-Keycodes
    *   Inputs     : String key (stock GameKey calling conventions, single 
    *                capital letter)
    *   Outputs    : True = is held, False = is not held
    *   Return Type: Bool
    ------------------------------------------------------------------------------------------
    --]]

    --- @param key string
    --- @return boolean
    local function GetGameKey(key)
        return exu.GetGameKey(key)
    end

    --[[
    ---------------------------------------------------------
    *   Name       : GetSteam64
    *   Description: Gets the Steam 64 ID of the local player
    *   Inputs     : None
    *   Outputs    : Steam64 ID
    *   Return Type: String
    ---------------------------------------------------------
    --]]

    --- @return string
    local function GetSteam64()
        return exu.GetSteam64()
    end

    --[[
    -----------------------------------------------------------------
    *   Name       : GetWeaponMask
    *   Description: This was a huge pain in the ass to implement ._.
    *                Gets the weapon mask for the local user, uses
    *                a 0 based index (first weapon slot returns 0)
    *   Inputs     : None
    *   Outputs    : Current weapon mask
    *   Return Type: Number
    -----------------------------------------------------------------
    --]]

    --- @return integer
    local function GetWeaponMask()
        return exu.GetWeaponMask()
    end

    --[[
    ----------------------------------------
    *   Name       : GetLives
    *   Description: Gets your current lives
    *   Inputs     : None
    *   Outputs    : Lives
    *   Return Type: Number
    ----------------------------------------
    --]]

    --- @return integer
    local function GetLives()
        return exu.GetLives()
    end

    --[[
    -----------------------------------------------------------
    *   Name       : SetLives
    *   Description: Sets your current lives
    *   Inputs     : Number new lives - Note: the display value
    *                doesn't update until you lose a life
    *   Outputs    : New lives
    *   Return Type: Void
    -----------------------------------------------------------
    --]]

    --- @param lives integer
    --- @return void
    local function SetLives(lives)
        exu.SetLives(lives)
    end

   --[[
    =======================================================
    *   IMPORTANT NOTICE REGARDING THE NEXT FUNCTIONS!
    *   These functions have accessibility implications
    *   due to modifying things like difficulty and mouse
    *   settings, please use them responsibly
    =======================================================
    --]]

    --[[
    -----------------------------------------------------------
    *   Name       : GetDifficulty
    *   Description: Gets the local player's difficulty setting
    *   Inputs     : None
    *   Outputs    : Current difficulty setting
    *   Return Type: String
    -----------------------------------------------------------
    --]]

    --- @return string
    local function GetDifficulty()
        return exu.GetDifficulty()
    end

    --[[
    ---------------------------------------------------------------------------
    *   Name       : SetDifficulty
    *   Description: Sets the local player's difficulty setting note that it
    *              : won't change what it says in the menu, but it will in fact
    *              : change in-game, note it also won't work in multiplayer
    *              : where the difficulty is locked to very hard
    *   Inputs     : String difficulty as it appears in game, ie. "Very Easy",
    *              : "Medium", "Very Hard", etc.
    *   Outputs    : New difficulty setting
    *   Return Type: Void
    ---------------------------------------------------------------------------
    --]]

    --- @param newDifficulty string
    --- @return void
    local function SetDifficulty(newDifficulty)
        exu.SetDifficulty(newDifficulty)
    end

    --[[
    -------------------------------------------------------------------
    *   Name       : GetAutoLevel
    *   Description: Gets the local player's automatic leveling setting
    *   Inputs     : None
    *   Outputs    : Current automatic leveling setting on or off
    *   Return Type: Bool
    -------------------------------------------------------------------
    --]]

    --- @return boolean
    local function GetAutoLevel()
        return exu.GetAutoLevel()
    end

    --[[
    -------------------------------------------------------------------
    *   Name       : SetAutoLevel
    *   Description: Sets the local player's automatic leveling setting
    *   Inputs     : Bool on or off
    *   Outputs    : New automatic leveling setting
    *   Return Type: Void
    -------------------------------------------------------------------
    --]]

    --- @param newAL boolean
    --- @return void
    local function SetAutoLevel(newAL)
        exu.SetAutoLevel(CastBool(newAL))
    end

    --[[
    ----------------------------------------------------------------------
    *   Name       : GetTLI
    *   Description: Gets the local player's target lead indicator setting
    *   Inputs     : None
    *   Outputs    : Current target lead indicator setting on or off
    *   Return Type: Bool
    ----------------------------------------------------------------------
    --]]

    --- @return boolean
    local function GetTLI()
        return exu.GetTLI()
    end

    --[[
    ----------------------------------------------------------------------
    *   Name       : SetTLI
    *   Description: Sets the local player's target lead indicator setting
    *   Inputs     : Bool on or off
    *   Outputs    : New target lead indicator setting on or off
    *   Return Type: Void
    ----------------------------------------------------------------------
    --]]

    --- @param newTLI boolean
    --- @return boolean
    local function SetTLI(newTLI)
        exu.SetTLI(CastBool(newTLI))
    end

    --[[
    ----------------------------------------------------------------------
    *   Name       : GetReverseMouse
    *   Description: Gets the local player's reverse mouse setting
    *   Inputs     : None
    *   Outputs    : Current reverse mouse setting setting on or off
    *   Return Type: Bool
    ----------------------------------------------------------------------
    --]]

    --- @return boolean
    local function GetReverseMouse()
        return exu.GetReverseMouse()
    end

    --[[
    ----------------------------------------------------------------------
    *   Name       : SetReverseMouse
    *   Description: Sets the local player's reverse mouse setting
    *   Inputs     : Bool on or off
    *   Outputs    : New reverse mouse setting on or off
    *   Return Type: Void
    ----------------------------------------------------------------------
    --]]

    --- @param newSetting boolean
    --- @return void
    local function SetReverseMouse(newSetting)
        exu.SetReverseMouse(CastBool(newSetting))
    end

    --[[
    --------------------------------------------------------------------------
    *   Name       : Ordnance Velocity Inheritance
    *   Description: Enables velocity inheritance on the local player's
    *                ordnance. You can control the ratio of velocity inherited
    *                by providing a float argument from 0-1. The default value
    *                is 1 (full inheritance).
    *                WARNING: you MUST call Enable BEFORE calling Update,
    *                and you must call UpdateOrdnance in the update loop to
    *                apply the patch.
    *   Inputs     : Optional float ratio, optional inheritance type "FULL" or
    *              : "FRONT"
    *   Outputs    : Velocity inheritance patch
    *   Return Type: Void
    --------------------------------------------------------------------------
    --]]

    --- @param ratio? float
    --- @return void
    local function EnableOrdnanceTweak(ratio)
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

    --- @param what? string
    --- @return void
    local function UpdateOrdnance(what)
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

    --[[
    -------------------------------------------------------------------------------
    *   Name       : EnableShotConvergence
    *   Description: Activates shot convergence for hovercraft
    *   Inputs     : None
    *   Outputs    : Shot convergence 
    *   Return Type: Void
    -------------------------------------------------------------------------------
    --]]

    --- @return void
    local function EnableShotConvergence()
        exu.EnableShotConvergence()
    end

    --[[
    ---------------------------------------------------------------------------------
    *   Name       : SetSelectNone
    *   Description: Sets the behavior of selections when performing certain actions,
    *              : for example if it is set to false, giving a move order will not
    *              : deselect units, and the tab key will not work (you'll be locked
    *              : into your selection), true makes it behave normally
    *   Inputs     : Bool true or false
    *   Outputs    : New selection behavior
    *   Return Type: Void
    ---------------------------------------------------------------------------------
    --]]

    --- @param setting boolean
    --- @return  void
    local function SetSelectNone(setting)
        exu.SetSelectNone(CastBool(setting))
    end

    --[[
    -------------------------------------------------------------------------------
    *   Name       : SetAsUser
    *   Description: Internal function call to set the local user to a given handle
    *   Inputs     : Game object handle
    *   Outputs    : Local user assumes control of the given vehicle
    *   Return Type: Void
    -------------------------------------------------------------------------------
    --]]

    --- @param handle handle
    --- @return void
    local function SetAsUser(handle)
        if not IsValid(handle) then -- needs to exist, otherwise access violation
            return
        end
        if IsBuilding(handle) then -- buildings will also AV
            return
        end
        exu.SetAsUser(handle)
    end

    --[[
    ----------------------------------------------------------------
    *   Name       : SelectOne
    *   Description: Selects one unit, overrides previous selections
    *   Inputs     : Game object handle
    *   Outputs    : Unit is selected
    *   Return Type: Void
    ----------------------------------------------------------------
    --]]

    ---@param handle handle
    ---@return void
    local function SelectOne(handle)
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

    --[[
    ----------------------------------------------------------------
    *   Name       : SelectOne
    *   Description: Deselects all units, equivalent to pressing tab
    *   Inputs     : None
    *   Outputs    : Deselects all units
    *   Return Type: Void
    ----------------------------------------------------------------
    --]]

    --- @return void
    local function SelectNone()
        exu.SelectNone()
    end

    --[[
    ----------------------------------------------------------------
    *   Name       : SelectAdd
    *   Description: Adds a unit to the current selection, selecting
    *              : across categories is possible but may break
    *              : certain behavior eg. hunt, get repair, defend
    *   Inputs     : Game object handle
    *   Outputs    : Adds the given unit to the selection
    *   Return Type: Void
    ----------------------------------------------------------------
    --]]

    --- @param handle handle
    --- @return void
    local function SelectAdd(handle)
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

    --[[
    -------------------------------------------------------------------------------
    *   Name       : GetWorkingDirectory
    *   Description: Returns the current directory of the battlezone executable
    *              : (should be the default game folder)
    *   Inputs     : None
    *   Outputs    : Game directory
    *   Return Type: String
    -------------------------------------------------------------------------------
    --]]

    --- @return string 
    local function GetWorkingDirectory()
        return exu.GetWorkingDirectory()
    end

    --[[
    -------------------------------------------------------------------------------
    *   Name       : MakeDirectory
    *   Description: Creates an empty folder at the given location - NOTE: use
    *              : backslashes for path names
    *   Inputs     : String path
    *   Outputs    : New folder
    *   Return Type: Void
    -------------------------------------------------------------------------------
    --]]

    --- @param name string
    --- @return void
    local function MakeDirectory(name)
        exu.MakeDirectory(name)
    end

    --[[
    -------------------------------------------------------------------------------
    *   Name       : FileRead
    *   Description: Reads the contents of the given file into a single formatted
    *              : string
    *   Inputs     : String path to file
    *   Outputs    : Contents of given file
    *   Return Type: String
    -------------------------------------------------------------------------------
    --]]

    --- @param fileName string
    --- @return string
    local function FileRead(fileName)
        return exu.FileRead(fileName)
    end

    --[[
    -------------------------------------------------------------------------------
    *   Name       : FileWrite
    *   Description: Writes the input string into the given file - NOTE: overwrites
    *              : ALL contents of the file, so save it before if you need to
    *   Inputs     : String path to file, string content
    *   Outputs    : Writes the given string to the file
    *   Return Type: Void
    -------------------------------------------------------------------------------
    --]]

    --- @param fileName string
    --- @param content string
    --- @return void
    local function FileWrite(fileName, content)
        exu.FileWrite(fileName, content)
    end

    -- OOP Bindings are done in here in lua because doing it in C is aids
    local UserLog = {}
    UserLog.__index = UserLog

    local function RegisterLogObject(logPointer)
        local instance = setmetatable({}, UserLog)
        instance.pointer = logPointer
        return instance
    end

    function UserLog:Out(content, level)
       exu.LogOut(self.pointer, content, level)
    end

    function UserLog:GetLogLevel()
        return exu.GetLogLevel(self.pointer)
    end

    function UserLog:SetLogLevel()
        
    end

    function UserLog:GetLogPath()
        
    end

    function UserLog:SetLogPath()
        
    end

    local function CreateLog(path, level)
        return RegisterLogObject(exu.CreateLog(path, level))
    end


    --[[
    -------------------------------------------------------------------------------
    *   Name       : SetDiffuseColor
    *   Description: Changes the diffuse color of the headlight on the given ship
    *              : LIMITATIONS: only tested on ships, unknown if other game
    *              : objects with lights will work, and the script must observe
    *              : the object being created to register the handle with the light,
    *              : pre-placed objects on the map will not work
    *              : DEFAULT value = { 1.0, 1.0, 1.0 }
    *   Inputs     : Userdata handle, float color r, g, b
    *   Outputs    : Changes the diffuse color of the light
    *   Return Type: Bool - true if successful, false otherwise
    -------------------------------------------------------------------------------
    --]]

    --- @param _handle handle
    --- @param _red float
    --- @param _green float
    --- @param _blue float
    --- @return boolean
    local function SetDiffuseColor(_handle, _red, _green, _blue)
        local red = _red or 1.0
        local green = _green or 1.0
        local blue = _blue or 1.0
        local label = GetLabel(_handle)
        return exu.SetDiffuseColor(label, red, green, blue)
    end

    --[[
    -------------------------------------------------------------------------------
    *   Name       : SetSpecularColor
    *   Description: Changes the diffuse color of the headlight on the given ship
    *              : LIMITATIONS: only tested on ships, unknown if other game
    *              : objects with lights will work, and the script must observe
    *              : the object being created to register the handle with the light,
    *              : pre-placed objects on the map will not work
    *              : DEFAULT value = { 1.0, 1.0, 1.0 }
    *   Inputs     : Userdata handle, float color r, g, b
    *   Outputs    : Changes the specular color of the light
    *   Return Type: Bool - true if successful, false otherwise
    -------------------------------------------------------------------------------
    --]]

    --- @param _handle handle
    --- @param _red float
    --- @param _green float
    --- @param _blue float
    --- @return boolean
    local function SetSpecularColor(_handle, _red, _green, _blue)
        local red = _red or 1.0
        local green = _green or 1.0
        local blue = _blue or 1.0
        local label = GetLabel(_handle)
        return exu.SetSpecularColor(label, red, green, blue)
    end

    --[[
    -------------------------------------------------------------------------------
    *   Name       : SetColor
    *   Description: Macro to use both light color functions at the same time
    *   Inputs     : Userdata handle, float color r, g, b
    *   Outputs    : Changes the color of the light
    *   Return Type: Void
    -------------------------------------------------------------------------------
    --]]

    --- @param _handle handle
    --- @param _red float
    --- @param _green float
    --- @param _blue float
    --- @return boolean
    local function SetColor(_handle, _red, _green, _blue)
        SetDiffuseColor(_handle, _red, _green, _blue)
        SetSpecularColor(_handle, _red, _green, _blue)
    end

    --[[
    -------------------------------------------------------------------------------
    *   Name       : SetSpotlightRange
    *   Description: Copied from ogre docs: "Sets the range of a spotlight, i.e.
    *              : the angle of the inner and outer cones and the rate of falloff 
    *              : between them."
    *   Inputs     : Userdata handle, float inner angle (bright inner cone) in
    *              : radians, float outer angle (outer cone) in radians, float
    *              : falloff (rate of falloff between cones), 1.0 is linear, 
    *              : <1.0 is slower falloff, >1.0 is faster falloff 
    *   Outputs    : Changes the range of the light
    *   Return Type: Bool - true if successful, false otherwise
    -------------------------------------------------------------------------------
    --]]

    --- @param _handle handle
    --- @param _innerAngle float
    --- @param _outerAngle float
    --- @param _falloff float
    --- @return boolean
    local function SetSpotlightRange(_handle, _innerAngle, _outerAngle, _falloff)
        local falloff = _falloff or 1.0 -- default value
        local outerAngle = _outerAngle or 0.35 -- default value
        local innerAngle = _innerAngle or 0.1745 -- default value
        local label = GetLabel(_handle)
        return exu.SetSpotlightRange(label, innerAngle, outerAngle, falloff)
    end

    -- Metadata
    extraUtils.version             = version
    extraUtils.crc32               = crc32
    extraUtils.Start               = Start

    -- Exported Functions
    extraUtils.SetAccessMode         = SetAccessMode
    extraUtils.GetObj                = GetObj
    extraUtils.GetGravity            = GetGravity
    extraUtils.SetGravity            = SetGravity
    extraUtils.GetSmartCursorRange   = GetSmartCursorRange
    extraUtils.SetSmartCursorRange   = SetSmartCursorRange
    extraUtils.GetReticleAngle       = GetReticleAngle
    extraUtils.GetReticlePos         = GetReticlePos
    extraUtils.GetSatState           = GetSatState
    extraUtils.GetSatCursorPos       = GetSatCursorPos
    extraUtils.GetSatCamPos          = GetSatCamPos
    extraUtils.GetSatClickPos        = GetSatClickPos
    extraUtils.GetSatPanSpeed        = GetSatPanSpeed
    extraUtils.SetSatPanSpeed        = SetSatPanSpeed
    extraUtils.GetMinSatZoom         = GetMinSatZoom
    extraUtils.SetMinSatZoom         = SetMinSatZoom
    extraUtils.GetMaxSatZoom         = GetMaxSatZoom
    extraUtils.SetMaxSatZoom         = SetMaxSatZoom
    extraUtils.GetSatZoom            = GetSatZoom
    extraUtils.SetSatZoom            = SetSatZoom
    extraUtils.GetRadarState         = GetRadarState
    extraUtils.SetRadarState         = SetRadarState
    extraUtils.GetZoomFactor         = GetZoomFactor
    extraUtils.SetZoomFactor         = SetZoomFactor
    extraUtils.GetMinZoomFactor      = GetMinZoomFactor
    extraUtils.SetMinZoomFactor      = SetMinZoomFactor
    extraUtils.GetMaxZoomFactor      = GetMaxZoomFactor
    extraUtils.SetMaxZoomFactor      = SetMaxZoomFactor
    extraUtils.GetGameKey            = GetGameKey
    extraUtils.GetSteam64            = GetSteam64
    extraUtils.GetWeaponMask         = GetWeaponMask
    extraUtils.GetLives              = GetLives
    extraUtils.SetLives              = SetLives
    extraUtils.GetDifficulty         = GetDifficulty
    extraUtils.SetDifficulty         = SetDifficulty
    extraUtils.GetAutoLevel          = GetAutoLevel
    extraUtils.SetAutoLevel          = SetAutoLevel
    extraUtils.GetTLI                = GetTLI
    extraUtils.SetTLI                = SetTLI
    extraUtils.GetReverseMouse       = GetReverseMouse
    extraUtils.SetReverseMouse       = SetReverseMouse
    extraUtils.EnableOrdnanceTweak   = EnableOrdnanceTweak
    extraUtils.UpdateOrdnance        = UpdateOrdnance
    extraUtils.EnableShotConvergence = EnableShotConvergence
    extraUtils.SetSelectNone         = SetSelectNone
    extraUtils.SetAsUser             = SetAsUser
    extraUtils.SelectOne             = SelectOne
    extraUtils.SelectNone            = SelectNone
    extraUtils.SelectAdd             = SelectAdd
    extraUtils.GetWorkingDirectory   = GetWorkingDirectory
    extraUtils.MakeDirectory         = MakeDirectory
    extraUtils.FileRead              = FileRead
    extraUtils.FileWrite             = FileWrite
    extraUtils.CreateLog             = CreateLog
    extraUtils.SetDiffuseColor       = SetDiffuseColor
    extraUtils.SetSpecularColor      = SetSpecularColor
    extraUtils.SetColor              = SetColor
    extraUtils.SetSpotlightRange     = SetSpotlightRange

end
return extraUtils