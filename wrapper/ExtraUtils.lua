--[[
=====================================================
*   Extra Utilities
*   Version 0.6.4
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
    local version = "0.6.4"
    local crc32 = "4E05458A"
    local debug = false

    local function Start() -- put this in function Start() to print out metadata to console
        print("--------------------------------------------------------------------------------------")
        print("Extra Utilities loaded! Version: " .. version .. "                                                  ")
        print("--------------------------------------------------------------------------------------")
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
    ]]

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
    *   Return Type: Userdata
    ---------------------------------------------------------------------------
    ]]

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
    ---------------------------------------------------------------------------
    *   Name       : GetSmartCursorRange
    *   Description: Gets the current range of the smart cursor (stock is 200m)
    *   Inputs     : None
    *   Outputs    : Current range of smart cursor
    *   Return Type: Number
    ---------------------------------------------------------------------------
    ]]

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
    ]]

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
    ]]

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
    ]]

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
    ]]

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
    ]]

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
    ]]

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
    ]]

    local function SetMaxZoomFactor(factor)
        exu.SetMaxZoomFactor(factor)
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
    ]]

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
    ]]

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
    ]]

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
    ]]

    local function SetLives(lives)
        exu.SetLives(lives)
    end

    --[[
    --------------------------------------------------------------------------
    *   Name       : Ordnance Velocity Inheritance
    *   Description: Enables full velocity inheritance on the local player's
    *                ordnance. You can control the ratio of velocity inherited
    *                by providing a float argument from 0-1. The default value
    *                is 1 (full inheritance).
    *                WARNING: you MUST call Enable BEFORE calling Update,
    *                and you must call UpdateOrdnance in the update loop to
    *                apply the patch.
    *   Inputs     : Optional float ratio
    *   Outputs    : Velocity inheritance patch
    *   Return Type: Void
    --------------------------------------------------------------------------
    ]]

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

    local function UpdateOrdnance()
        if IsNetGame == true and debug == false then
            error("Extra Utilities Error: this function is incompatible with multiplayer. Turn on debug mode to override.")
            return
        end
        local dot = DotProduct(GetVelocity(GetPlayerHandle()), GetFront(GetPlayerHandle()))
        local playerVelocity = GetFront(GetPlayerHandle()) * dot
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
    ]]

    local function EnableShotConvergence()
        exu.EnableShotConvergence()
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

    local function GetWorkingDirectory()
        return exu.GetWorkingDirectory()
    end

    local function MakeDirectory(name)
        exu.MakeDirectory(name)
    end

    local function FileRead(fileName)
        return exu.FileRead(fileName)
    end

    local function FileWrite(fileName, content)
        exu.FileWrite(fileName, content)
    end

    local function CreateLog(path, level)
        return exu.CreateLog(path, level)
    end

    local function SetDiffuseColor(handle, r, g, b)
        local red = r or 1.0
        local green = g or 1.0
        local blue = b or 1.0
        
        local label = GetLabel(handle)
        exu.SetDiffuseColor(label, red, green, blue)
    end

    local function SetSpecularColor(handle, r, g, b)
        local red = r or 1.0
        local green = g or 1.0
        local blue = b or 1.0
        
        local label = GetLabel(handle)
        exu.SetSpecularColor(label, red, green, blue)
    end

    local function SetColor(handle, r, g, b)
        SetDiffuseColor(handle, r, g, b)
        SetSpecularColor(handle, r, g, b)
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
    extraUtils.EnableOrdnanceTweak   = EnableOrdnanceTweak
    extraUtils.UpdateOrdnance        = UpdateOrdnance
    extraUtils.EnableShotConvergence = EnableShotConvergence
    extraUtils.SetAsUser             = SetAsUser
    extraUtils.GetWorkingDirectory   = GetWorkingDirectory
    extraUtils.MakeDirectory         = MakeDirectory
    extraUtils.FileRead              = FileRead
    extraUtils.FileWrite             = FileWrite
    extraUtils.CreateLog             = CreateLog
    extraUtils.SetDiffuseColor       = SetDiffuseColor
    extraUtils.SetSpecularColor      = SetSpecularColor
    extraUtils.SetColor              = SetColor

end
return extraUtils