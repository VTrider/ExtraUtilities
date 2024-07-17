--[[
=====================================================
*   Extra Utilities
*   Version 0.5.0
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
]]

require("exu")

local extraUtils = {}
do
    -- Metadata
    local version = "0.5.0"
    local crc32 = "55166F8D"

    local function Start() -- put this in function Start() to print out metadata to console
        print("--------------------------------------------------------------------------------------")
        print("Extra Utilities loaded! Version: " .. version .. "                                                  ")
        print("--------------------------------------------------------------------------------------")
    end

    --[[
    ---------------------------------------------------------------------------
    *   Name       : GetObj
    *   Description: Converts a handle to it's memory address, mainly for debug
    *                purposes 
    *   Inputs     : Handle to an object
    *   Outputs    : GameObject pointer
    *   Return Type: Userdata
    ---------------------------------------------------------------------------
    ]]

    local function GetObj(handle)
        local obj = exu.GetObj(handle)
        return obj
    end

    --[[
    -------------------------------------------------------------
    *   Name       : GetGravity
    *   Description: Gets the current value of the gravity vector
    *   Inputs     : None
    *   Outputs    : VECTOR_3D velocity: X, Y, Z
    *   Return Type: Userdata
    -------------------------------------------------------------
    ]]
    
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
    ]]

    local function SetGravity(x, y, z)
        exu.SetGravity(x, y, z)
    end

    --[[
    ------------------------------------------------------
    *   Name       : GetLuminance
    *   Description: Gets the current value of Sun_Ambient
    *   Inputs     : None
    *   Outputs    : VECTOR_3D luminance: R, G, B
    *   Return Type: Userdata
    ------------------------------------------------------
    ]]

    local function GetLuminance()
        local luminance = exu.GetLuminance()
        local formattedLuminance = SetVector(luminance.r, luminance.g, luminance.b)
        return formattedLuminance
    end

    --[[
    ------------------------------------------------------
    *   Name       : SetLuminance
    *   Description: Sets the current value of Sun_Ambient
    *   Inputs     : Float R, G, B values
    *   Outputs    : New luminance value in map
    *   Return Type: Void
    ------------------------------------------------------
    ]]

    local function SetLuminance(r, g, b)
        exu.SetLuminance(r, g, b)
    end

    --[[
    --------------------------------------------------------------
    *   Name       : GetFogStart
    *   Description: Gets the current value of FogStart
    *   Inputs     : None
    *   Outputs    : Current value of FogStart
    *   Return Type: Number
    --------------------------------------------------------------
    ]]

    local function GetFogStart()
        local fogStart = exu.GetFogStart()
        return fogStart
    end

    --[[
    --------------------------------------------------------------
    *   Name       : SetFogStart
    *   Description: Sets the current value of FogStart
    *   Inputs     : Float value
    *   Outputs    : New FogStart value in map
    *   Return Type: Void
    --------------------------------------------------------------
    ]]

    local function SetFogStart(value)
        exu.SetFogStart(value)
    end

    --[[
    ---------------------------------------------------------------------------
    *   Name       : GetSmartCursorRange
    *   Description: Gets the current range of the smart cursor (stock is 200m)
    *   Inputs     : None
    *   Outputs    : Current range of smart cursor
    *   Return Type: Number
    ---------------------------------------------------------------------------
    ]]

    local function GetSmartCursorRange()
        local range = exu.GetSmartCursorRange()
        return range
    end

    --[[
    ---------------------------------------------------------------------------
    *   Name       : SetSmartCursorRange
    *   Description: Sets the current range of the smart cursor
    *   Inputs     : Float new range
    *   Outputs    : New smart cursor range
    *   Return Type: Void
    ---------------------------------------------------------------------------
    ]]

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
    ]]
    
    local function GetReticleAngle()
        local reticleAngle = exu.GetReticleAngle()
        return reticleAngle
    end

    --[[
    ------------------------------------------------------------------------------------------------------
    *   Name       : GetReticlePos
    *   Description: Gets the location on terrain of the player's reticle
    *   Inputs     : None
    *   Outputs    : VECTOR_3D coordinates: X, Y, Z
    *   Return Type: Userdata
    ------------------------------------------------------------------------------------------------------
    ]]

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
    ]]

    local function GetSatState()
        local satState = exu.GetSatState()
        return satState
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
    ]]

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
    ]]

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
    ]]

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
    ]]

    local function GetSatPanSpeed()
        local panSpeed = exu.GetSatPanSpeed()
        return panSpeed
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
    ]]

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
    ]]

    local function GetMinSatZoom()
        local minZoom = exu.GetMinSatZoom()
        return minZoom
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
    ]]

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
    ]]

    local function GetMaxSatZoom()
        local maxZoom = exu.GetMaxSatZoom()
        return maxZoom
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
    ]]

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
    ]]

    local function GetSatZoom()
        local satZoom = exu.GetSatZoom()
        return satZoom
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
    ]]

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
    ]]

    local function GetRadarState()
        local radarState = exu.GetRadarState()
        return radarState
    end

    --[[
    ---------------------------------------------------------
    *   Name       : SetRadarState
    *   Description: Sets the radar state of the local player
    *   Inputs     : Desired radar state
    *   Outputs    : New radar state
    *   Return Type: Void
    ---------------------------------------------------------
    ]]

    local function SetRadarState(state)
        exu.SetRadarState(state)
    end

    --[[
    ---------------------------------------------------------------------------
    *   Name       : GetGameKey
    *   Description: Gets whether or not a key is held, currently only supports
    *                alphanumeric characters
    *   Inputs     : String key (stock GameKey calling conventions, single 
    *                capital letter)
    *   Outputs    : True = is held, False = is not held
    *   Return Type: Bool
    ---------------------------------------------------------------------------
    ]]

    local function GetGameKey(key)
        local keyPressed = exu.GetGameKey(key)
        return keyPressed
    end

    --[[
    ---------------------------------------------------------
    *   Name       : GetSteam64
    *   Description: Gets the Steam 64 ID of the local player
    *   Inputs     : None
    *   Outputs    : Steam64 ID
    *   Return Type: String
    ---------------------------------------------------------
    ]]

    local function GetSteam64()
        local steam64 = exu.GetSteam64()
        local formattedID = string.format("%.0f", steam64)
        return formattedID
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
    ]]

    local function GetWeaponMask()
        local weaponMask = exu.GetWeaponMask()
        return weaponMask
    end

    --[[
    ----------------------------------------
    *   Name       : GetLives
    *   Description: Gets your current lives
    *   Inputs     : None
    *   Outputs    : Lives
    *   Return Type: Number
    ----------------------------------------
    ]]

    local function GetLives()
        local lives = exu.GetLives()
        return lives
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
    ]]

    local function SetLives(lives)
        exu.SetLives(lives)
    end

    --[[
    -----------------------------------------------------------
    *   Name       : `
    *   Description: `
    *   Inputs     : `
    *   Outputs    : `
    *   Return Type: `
    -----------------------------------------------------------
    ]]

    -- WARNING! Your game will certainly crash if you enable these and then enter a different map
    -- than the one with the dll, it's a one way trip and I haven't implemented safeguards

    local function EnableOrdnanceTweak()
        exu.EnableOrdnanceTweak()
    end

    local function UpdateOrdnance()
        playerVelocity = GetVelocity(GetPlayerHandle())
        exu.UpdateOrdnance(playerVelocity.x, playerVelocity.y, playerVelocity.z)
    end

    --[[
    -------------------------------------------------------------------------------
    *   Name       : SetAsUser
    *   Description: Internal function call to set the local user to a given handle
    *   Inputs     : Game object handle
    *   Outputs    : Local user assumes control of the given vehicle
    *   Return Type: Void
    -------------------------------------------------------------------------------
    ]]

    local function SetAsUser(handle)
        if not IsValid(handle) then -- needs to exist, otherwise access violation
            return
        end
        if IsBuilding(handle) then -- buildings will also AV
            return
        end
        exu.SetAsUser(handle)
    end

    -- NO SAFEGUARDS ON THESE FUNCTIONS, BE CAREFUL LOL

    local function FileRead(fileName)
        return exu.FileRead(fileName)
    end

    local function FileWrite(fileName, content)
        exu.FileWrite(fileName, content)
    end

    -- Metadata
    extraUtils.version             = version
    extraUtils.crc32               = crc32
    extraUtils.Start               = Start

    -- Exported Functions
    extraUtils.GetObj              = GetObj
    extraUtils.GetGravity          = GetGravity
    extraUtils.SetGravity          = SetGravity
    extraUtils.GetLuminance        = GetLuminance
    extraUtils.SetLuminance        = SetLuminance
    extraUtils.GetFogStart         = GetFogStart
    extraUtils.SetFogStart         = SetFogStart
    extraUtils.GetSmartCursorRange = GetSmartCursorRange
    extraUtils.SetSmartCursorRange = SetSmartCursorRange
    extraUtils.GetReticleAngle     = GetReticleAngle
    extraUtils.GetReticlePos       = GetReticlePos
    extraUtils.GetSatState         = GetSatState
    extraUtils.GetSatCursorPos     = GetSatCursorPos
    extraUtils.GetSatCamPos        = GetSatCamPos
    extraUtils.GetSatClickPos      = GetSatClickPos
    extraUtils.GetSatPanSpeed      = GetSatPanSpeed
    extraUtils.SetSatPanSpeed      = SetSatPanSpeed
    extraUtils.GetMinSatZoom       = GetMinSatZoom
    extraUtils.SetMinSatZoom       = SetMinSatZoom
    extraUtils.GetMaxSatZoom       = GetMaxSatZoom
    extraUtils.SetMaxSatZoom       = SetMaxSatZoom
    extraUtils.GetSatZoom          = GetSatZoom
    extraUtils.SetSatZoom          = SetSatZoom
    extraUtils.GetRadarState       = GetRadarState
    extraUtils.SetRadarState       = SetRadarState
    extraUtils.GetGameKey          = GetGameKey
    extraUtils.GetSteam64          = GetSteam64
    extraUtils.GetWeaponMask       = GetWeaponMask
    extraUtils.GetLives            = GetLives
    extraUtils.SetLives            = SetLives
    extraUtils.EnableOrdnanceTweak = EnableOrdnanceTweak
    extraUtils.UpdateOrdnance      = UpdateOrdnance
    extraUtils.SetAsUser           = SetAsUser
    extraUtils.FileRead            = FileRead
    extraUtils.FileWrite           = FileWrite

end
return extraUtils