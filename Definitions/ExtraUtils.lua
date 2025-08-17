--- @meta exu
--- This file provides the lua definitions for Extra Utilities version 1.0.0
--- These definitions also require the stock definitions found in `scriptutils.lua`
--- for basic types like Handle and Vector

error("This is a definition file, use require(\"exu\")")

--- @diagnostic disable: unused-local

--- Types

--- @alias GameObject* lightuserdata pointer the underlying object of a Handle
--- @alias Ordnance* lightuserdata pointer to the ordnance object

--- Structs

--- Note the inconsistent capitalization, this is intentional since it's the way
--- the game does it internally
--- @class CameraOrigins
--- @field Orig_x number Half of your horizontal resolution in pixels
--- @field Orig_y number Half of your vertical resolution in pixels
--- @field Const_x number Normalized coordinates
--- @field Const_y number Normalized coordinates (Const_x negated)

--- @class Color
--- @field r number
--- @field g number
--- @field b number

--- @class Fog
--- @field r number
--- @field g number
--- @field b number
--- @field start number meters
--- @field ending number meters

--- @class exu
--- @field Origins CameraOrigins
local exu = {}

--- Enums
--- DO NOT USE THE RAW VALUES YOUR CODE WILL BE UNREADABLE

--- @class CameraState
exu.CAMERA = {
    -- Camera types
    FIRST_PERSON = 0x100,
    THIRD_PERSON = 0x101,

    -- View modes
    COCKPIT = 1, -- F1
    NO_COCKPIT = 2, -- F2
    CHASE = 4, -- F3
    ORBIT = 5, -- F4
    NO_HUD = 3, -- F5
    EDITOR = 6, -- F9
    CHEAT_SATELLITE = 7, -- F10
    FREECAM = 9, -- F11

    TOGGLE_SATELLITE = 8, -- this code is used when satellite is activated with the 9 key
    TERRAIN_EDIT = 0x2A, -- CTRL+E
}

--- @class Defaults
exu.DEFAULTS = {
    CAMERA_MIN_ZOOM = 0.99,
    CAMERA_MAX_ZOOM = 1.6,
    COEFF_BALLISTIC = 4.9,
    GRAVITY_VECTOR = SetVector(0, -9.8, 0),
    RETICLE_RANGE = 200,
    SAT_MAX_ZOOM = 8,
    SAT_MIN_ZOOM = 2,
    SAT_PAN_SPEED = 1250
}

--- @class Difficulty
exu.DIFFICULTY = {
    VERY_EASY = 0,
    EASY = 1,
    MEDIUM = 2,
    HARD = 3,
    VERY_HARD = 4
}

--- @class Ogre
exu.OGRE = {
    HEADLIGHT_COLOR = {
        R = 1.0,
        G = 1.0,
        B = 1.0
    },
    HEADLIGHT_RANGE = {
        INNER_ANGLE = 0.1745,
        OUTER_ANGLE = 0.3490,
        FALLOFF = 1.0
    }
}

--- @class OrdnanceAttributes
exu.ORDNANCE = {
    ODF = 0,            -- The odf that corresponds to the ordnance
    TRANSFORM = 1,      -- The current transform of the ordnance
    INIT_TRANSFORM = 2, -- The initial transform of the ordnance when it was spawned/shot
    OWNER = 3,          -- The handle of the ordnance's owner (usually who shot it unless it's been spawned manually, then it could be anything)
    INIT_TIME = 4,      -- The time that the ordnance was spawned/shot
}

--- @class RadarState
exu.RADAR= {
    MINIMAP = 0,
    RADAR = 1
}

--- @class SatelliteState
exu.SATELLITE = {
    DISABLED = 0,
    ENABLED = 1
}

--- Camera
---
--- These functions are related to the camera in-game and its attributes

--- Gets the values of the camera origins that can be useful for computing perspective transformations. 
--- @nodiscard
--- @return CameraOrigins
function exu.GetCameraOrigins() end

--- Gets the transform matrix of the current camera.
--- @nodiscard
--- @return Matrix
function exu.GetCameraTransformMatrix() end

--- Gets the view matrix of the current camera.
--- @nodiscard
--- @return Matrix
function exu.GetCameraViewMatrix() end

--- Gets the maximum zoom factor for all cameras.
--- @nodiscard
--- @return number
function exu.GetCameraMaxZoom() end

--- Sets the maximum zoom factor for all cameras.
--- @param zoom number
function exu.SetCameraMaxZoom(zoom) end

--- Gets the minimum zoom factor for all cameras.
--- @nodiscard
--- @return number
function exu.GetCameraMinZoom() end

--- Sets the minimum zoom factor for all cameras.
--- @param zoom number
function exu.SetCameraMinZoom(zoom) end

--- Gets the current camera view mode. See the camera enum for options.
--- @nodiscard
--- @return CameraState
function exu.GetCameraView() end

--- Sets the current camera view mode.
--- @param view CameraState | any
function exu.SetCameraView(view) end

--- Gets the zoom factor of the given camera. See the camera enum for options.
--- @nodiscard
--- @param camera number
--- @return number
function exu.GetCameraZoom(camera) end

--- Sets the zoom factor of the given camera.
--- Note: This will override the min/max zoom until the player tries to zoom with -+.
--- @param camera number
--- @param zoom number
--- @return nil
function exu.SetCameraZoom(camera, zoom) end

--- Control Panel
---
--- These functions control the selection that is handled through the control panel.

--- Adds the given unit to the local player's selection.
--- @param h Handle
function exu.SelectAdd(h) end

--- Clears the local player's selection (equivalent to pressing tab with stock keybinds).
function exu.SelectNone() end

--- Sets the local player's selection to the given handle and de-selects everything else.
--- @param h Handle
function exu.SelectOne(h) end

--- Environment
---
--- These functions can query and modify attributes about the environment like gravity and lighting.

--- Returns a table with the current map fog parameters.
--- @nodiscard
--- @return Fog
function exu.GetFog() end

--- Sets the current map fog parameters. Can take either five number parameters or a fog table.
--- @param r number
--- @param g number
--- @param b number
--- @param start number
--- @param ending number
function exu.SetFog(r, g, b, start, ending) end

--- Sets the current map fog parameters. Can take either five number parameters or a fog table.
--- @param newFog Fog
function exu.SetFog(newFog) end

--- Gets the current value of the local player's gravity velocity vector.
--- @nodiscard
--- @return Vector
function exu.GetGravity() end

--- Sets the local player's current gravity velocity vector. This affects all units owned by the local player.
--- Can take either three number parameters or a bz vector userdata.
--- @param x number
--- @param y number
--- @param z number
function exu.SetGravity(x, y, z) end

--- Sets the local player's current gravity velocity vector. This affects all units owned by the local player.
--- Can take either three number parameters or a bz vector userdata.
---
--- Multiplayer remarks: this will affect ONLY local units, to keep the game looking consistent you should set this to the
--- same value across all clients if it's been changed from stock.
--- @param v Vector
function exu.SetGravity(v) end

--- Returns a table with the current sun ambient parameters.
--- @nodiscard
--- @return Color
function exu.GetSunAmbient() end

--- Sets the current map sun ambient parameters. Can take either three number parameters or a color table.
--- @param r number
--- @param g number
--- @param b number
function exu.SetSunAmbient(r, g, b) end

--- Sets the current map sun ambient parameters. Can take either three number parameters or a color table.
--- @param newColor Color
function exu.SetSunAmbient(newColor) end

--- Returns a table with the current sun diffuse parameters.
--- @nodiscard
--- @return Color
function exu.GetSunDiffuse() end

--- Sets the current map sun diffuse parameters. Can take either three number parameters or a color table.
--- @param r number
--- @param g number
--- @param b number
function exu.SetSunDiffuse(r, g, b) end

--- Sets the current map sun diffuse parameters. Can take either three number parameters or a color table.
--- @param newColor Color
function exu.SetSunDiffuse(newColor) end

--- Returns a table with the current sun specular parameters.
--- @nodiscard
--- @return Color
function exu.GetSunSpecular() end

--- Sets the current map sun specular parameters. Can take either three number parameters or a color table.
--- @param r number
--- @param g number
--- @param b number
function exu.SetSunSpecular(r, g, b) end

--- Sets the current map sun specular parameters. Can take either three number parameters or a color table.
--- @param newColor Color
function exu.SetSunSpecular(newColor) end

--- GameObject
---
--- These functions act on game objects (handles) to query and modify various attributes.

--- Sets the local player's user ship to the given handle.
--- @param h Handle
function exu.SetAsUser(h) end

--- Gets if the given comm tower is powered or not, throws an error if the handle is not a comm tower.
--- @nodiscard
--- @param h Handle
--- @return boolean
function exu.IsCommTowerPowered(h) end

--- Converts a GameObject* into a handle that can be used in lua.
--- @nodiscard
--- @param obj GameObject*
--- @return integer
function exu.GetHandle(obj) end

--- Sets the diffuse color of the headlight of the given object (if it exists).
--- @param h Handle
--- @param r number
--- @param g number
--- @param b number
function exu.SetHeadlightDiffuse(h, r, g, b) end

--- Sets the specular color of the headlight of the given object (if it exists).
--- @param h Handle
--- @param r number
--- @param g number
--- @param b number
function exu.SetHeadlightSpecular(h, r, g, b) end

--- Sets the range parameters of the headlight of the given object (if it exists).
--- From the Ogre documentation: "Sets the range of a spotlight, i.e. the angle of the inner and outer cones and the rate of falloff between them."
--- 1.0 falloff is linear, <0 is slower and >0 is faster. Angles measured in radians.
--- @param h Handle
--- @param innerAngle number
--- @param outerAngle number
--- @param falloff number
function exu.SetHeadlightRange(h, innerAngle, outerAngle, falloff) end

--- Sets whether or not a headlight is visible or hidden.
--- @param visible boolean
function exu.SetHeadlightVisible(visible) end

--- Gets the mass of the given object (most ships default to 1750 KG afaik).
--- @nodiscard
--- @param h Handle
--- @return number
function exu.GetMass(h) end

--- Sets the mass of the given object. This affects collisions and knockback.
--- @param h Handle
function exu.SetMass(h) end

--- Converts a handle into a GameObject* that can be used in patching and debugging.
--- @nodiscard
--- @param h Handle
--- @return GameObject*
function exu.GetObj(h) end

--- Gets the radar scan period for the given object (if it has a radar).
--- @nodiscard
--- @param h Handle
--- @return number
function exu.GetRadarPeriod(h) end

--- Sets the radar scan period for the given object.
--- @param h Handle
--- @param period number
function exu.SetRadarPeriod(h, period) end

--- Gets the radar range of the given object (if it has a radar).
--- @nodiscard
--- @param h Handle
--- @return number
function exu.GetRadarRange(h) end

--- Sets the radar range of the given object.
--- @param h Handle
--- @param range number
function exu.SetRadarRange(h, range) end

--- Gets the velocjam value for the given object (in m/s).
--- @nodiscard
--- @param h Handle
--- @return number
function exu.GetVelocJam(h) end

--- Sets the velocjam value for the given object.
--- @param h Handle
function exu.SetVelocJam(h) end

--- Graphics Options

--- Gets if the game is currently in fullscreen.
--- @nodiscard
--- @return boolean
function exu.GetFullscreen() end

--- Gets the actual resolution of the game, returns two values for X and Y pixel count.
--- @nodiscard
--- @return integer, integer
function exu.GetGameResolution() end

--- Gets the current UI scaling factor (1, 2, 3, etc).
--- @nodiscard
--- @return integer
function exu.GetUIScaling() end

--- IO
---
--- These functions handle input/output that the stock game doesn't provide.

--- Gets whether or not a key is held.
--- A full list of keys can be found here.
--- @nodiscard
--- @param key string
--- @return boolean
function exu.GetGameKey(key) end

--- Multiplayer
---
--- These functions work exclusively in multiplayer to help with various things.

--- Functions the same as BuildObject except the resulting object is always asynchronous regardless of hosting status.
--- @return Handle
function exu.BuildAsyncObject(...) end

--- Functions the same as BuildObject except the resulting object is always synchronized regardless of hosting status.
--- @return Handle
function exu.BuildSyncObject(...) end

--- Gets the local player's life count.
--- @nodiscard
--- @return number
function exu.GetLives() end

--- Sets the local player's life count. This must be called during or after Start(), otherwise lives are not initialized in the map yet.
--- @param lives number
function exu.SetLives(lives) end

--- Gets the net ID of the local player (the parameter that would appear in CreatePlayer()).
--- @nodiscard
--- @return number
function exu.GetMyNetID() end

--- Gets if the multiplayer scoreboard is visible.
--- @nodiscard
--- @return boolean
function exu.GetShowScoreboard() end

--- Sets the state of the multiplayer scoreboard. (Default toggle key M).
--- @param state boolean
function exu.SetShowScoreboard(state) end

--- Prevents the starting recycler from spawning in strategy and MPI missions. CALL LOOSE IN THE SCRIPT PRE-START!
function exu.DisableStartingRecycler() end

--- Ordnance
---
--- These functions modify the behavior of ordnance.

--- Builds an ordnance of the given odf (omit the .odf extension like with BuildObject) at the given transform matrix.
--- You must also provide an owner handle since the game appears to require this.
--- The owner of an ordnance will not be hit by it if it is created inside its hitbox, otherwise it can be hit.
--- Returns a handle to the ordnance object that can be used in exu.GetOrdnanceAttribute().
--- Ordnances are short lived, DO NOT access this value for longer than its lifespan.
--- If you need to hold on to a handle you should verify it still exists by checking current time minus its init time
--- (from GetOrdnanceAttribute) is less than the lifespan defined in the odf.
--- You can also use the exu.BulletHit callback to detect if the value is no longer valid,
--- however this does not account for ordnance despawning or being destroyed through other means.
---
--- Multiplayer remarks: You should not Send() another player a local ordnance handle,
--- they are likely client side and I haven't rigorously tested it in MP.
--- @param odf string
--- @param transform Matrix
--- @param owner Handle
--- @return Ordnance*
function exu.BuildOrdnance(odf, transform, owner) end

--- Queries an ordnance handle for certain values. See the valid attribute codes in the ORDNANCE enum.
--- @nodiscard
--- @param ordnanceHandle Ordnance*
--- @param attribute OrdnanceAttributes
--- @return any
function exu.GetOrdnanceAttribute(ordnanceHandle, attribute) end

--- Gets the global ballistic coefficient (how much mortars drop, default 4.9f).
--- @nodiscard
--- @return number
function exu.GetCoeffBallistic() end

--- Sets the global ballistic coefficient.
---
--- Multiplayer remarks: this is clientside, it should be set to the same value on ALL clients
--- if you changed it at any point.
--- @param coeff number
function exu.SetCoeffBallistic(coeff) end

--- OS
---
--- These functions can get various information from the operating system (windows only).

--- Gets the screen resolution for the local user.
--- First value in the multi-return is the width in pixels, second is the height.
--- @nodiscard
--- @return number, number
function exu.GetScreenResolution() end

--- Displays an application modal windows message box with the given message, useful for debugging.
--- @param message string
function exu.MessageBox(message) end

--- Patches
---
--- These functions control various patches to modify game behavior.

--- Wrapper for AddScrap that does not trigger the exu.AddScrap callback.
--- @param team any
--- @param amount any
--- @return number newScrapCount
function exu.AddScrapSilent(team, amount) end

--- Gets whether or not the global turbo mode patch is enabled (default false).
--- @nodiscard
--- @return boolean
function exu.GetGlobalTurbo() end

--- Sets the state of the global turbo mode patch.
--- @param state boolean
function exu.SetGlobalTurbo(state) end

--- Gets whether a unit has turbo enabled (off by default).
--- @nodiscard
--- @param h Handle
--- @return boolean
function exu.GetUnitTurbo(h) end

--- Sets turbo for an individual unit that overrides the global setting.
--- @param h Handle
--- @param state boolean
function exu.SetUnitTurbo(h, state) end

--- Gets the state of the ordnance velocity inheritance patch (see below).
--- @nodiscard
--- @return boolean
function exu.GetOrdnanceVelocInheritance() end

--- Makes projectiles inherit the velocity of the shooter, and patches the TLI for both the player and AI to account for this, currently does NOT work in multiplayer, and the TLI on mortars is not accurate.
--- @param state boolean
function exu.SetOrdnanceVelocInheritance(state) end

--- Returns whether or not the hovercraft shot convergence patch is enabled.
--- @nodiscard
--- @return boolean
function exu.GetShotConvergence() end

--- Sets the hovercraft shot convergence patch true or false (default false).
--- @param enabled boolean
function exu.SetShotConvergence(enabled) end

--- Play Option
---
--- These functions query and modify certain options in the stock options menu.
--- Reminder, just because you can change these settings via script doesn't mean you should,
--- these have accessibility implications and forcing TLI off for example can make the game frustrating to play.

--- Gets the local player's difficulty setting.
--- @nodiscard
--- @return number
function exu.GetDifficulty() end

--- Sets the local player's difficulty setting. See the difficulty enum for options.
--- Note that it won't change what it says in the menu, but it will in fact change in-game.
--- It also won't work in multiplayer where the difficulty is locked to very hard.
--- @param difficulty number
function exu.SetDifficulty(difficulty) end

--- Gets the local player's automatic leveling setting.
--- @nodiscard
--- @return boolean
function exu.GetAutoLevel() end

--- Sets the local player's automatic leveling setting.
--- @param newAL boolean
function exu.SetAutoLevel(newAL) end

--- Gets the local player's target lead indicator setting.
--- @nodiscard
--- @return boolean
function exu.GetTLI() end

--- Sets the local player's target lead indicator setting.
--- @param newTLI boolean
function exu.SetTLI(newTLI) end

--- Gets the local player's reverse mouse setting.
--- @nodiscard
--- @return boolean
function exu.GetReverseMouse() end

--- Sets the local player's reverse mouse setting.
--- @param newMouse boolean
function exu.SetReverseMouse(newMouse) end

--- Radar
---
--- These functions can query and modify the in game radar.

--- Gets the state of the radar. See the radar enum for options. 0 = minimap 1 = radar
--- @nodiscard
--- @return number
function exu.GetRadarState() end

--- Sets the current radar state.
--- @param state number
function exu.SetRadarState(state) end

--- Reticle
---
--- These functions can query various information about the smart reticle.

--- Gets the transform matrix of the reticle.
--- @nodiscard
--- @return Matrix
function exu.GetReticleMatrix() end

--- Gets the handle of the object under the local user's reticle, as well as the satellite cursor.
--- (The object that appears next to "SPACE No Action (object)" in the command interface).
--- The object must be within smart reticle range in order to be detected.
--- @nodiscard
--- @return Handle
function exu.GetReticleObject() end

--- Gets the location on terrain of the player's reticle.
--- Must be within smart reticle range, otherwise it will return the last known value.
--- Additionally, while looking at an object or into the sky it will return the last known value.
--- @nodiscard
--- @return Vector
function exu.GetReticlePos() end

--- Gets the current range of the smart cursor (default 200m).
--- @nodiscard
--- @return number
function exu.GetReticleRange() end

--- Sets the current range of the smart cursor.
--- @param range number
function exu.SetReticleRange(range) end

--- Satellite
---
--- These functions can query various information about the satellite view.

--- Gets the current position of the player's camera in satellite view.
--- The Y coordinate becomes static when you pan the camera, otherwise it follows the player's Y coordinate.
--- @nodiscard
--- @return Vector
function exu.GetSatCameraPos() end

--- Gets the last position the player clicked in satellite view.
--- Persists after satellite is closed so be sure to handle this case.
--- @nodiscard
--- @return Vector
function exu.GetSatClickPos() end

--- Gets the current position of the cursor in satellite view.
--- If satellite is disabled it will return the last known position of the cursor.
--- @nodiscard
--- @return Vector
function exu.GetSatCursorPos() end

--- Gets the maximum zoom level of satellite view.
--- Default = `8`
--- @nodiscard
--- @return number
function exu.GetSatMaxZoom() end

--- Sets the maximum zoom level of satellite view.
--- Default = `8`
--- @param zoom number
function exu.SetSatMaxZoom(zoom) end

--- Gets the minimum zoom level of satellite view.
--- Default = `2`
--- @nodiscard
--- @return number
function exu.GetSatMinZoom() end

--- Sets the minimum zoom level of satellite view.
--- Default = `2`
--- @param zoom number
function exu.SetSatMinZoom(zoom) end

--- Gets the current pan speed of satellite view.
--- Default = `1250`
--- @nodiscard
--- @return number
function exu.GetSatPanSpeed() end

--- Sets the current pan speed of satellite view.
--- Default = `1250`
--- @param speed number
function exu.SetSatPanSpeed(speed) end

--- Gets the satellite state of the local player. See the satellite enum for options.
--- @nodiscard
--- @return SatelliteState
function exu.GetSatState() end

--- Gets the current zoom level of satellite view.
--- Default = `4`
--- @nodiscard
--- @return number
function exu.GetSatZoom() end

--- Sets the current zoom level of satellite view.
--- Make sure to stay within the boundaries defined by min and max zoom, otherwise it'll cause issues.
--- @param zoom number
function exu.SetSatZoom(zoom) end

--- Steam
---
--- These functions can get information from steam.

--- Gets the Steam 64 ID of the local player.
--- @nodiscard
--- @return string
function exu.GetSteam64() end

--- Sound Options
---
--- These functions can query information about the stock sound bar settings.

--- Gets the value of the music slider from 0-10.
--- @nodiscard
--- @return number
function exu.GetMusicVolume() end

--- Gets the value of the effects slider from 0-10.
--- @nodiscard
--- @return number
function exu.GetEffectsVolume() end

--- Sets the value of the effects slider from 0-10.
--- @param volume number
function exu.SetEffectsVolume(volume) end

--- Gets the value of the voice slider from 0-10.
--- @nodiscard
--- @return number
function exu.GetVoiceVolume() end

--- Sets the value of the voice slider from 0-10.
--- @param volume number
function exu.SetVoiceVolume(volume) end

--- Stock Extensions
---
--- These functions are either existing in game but unbound in stock lua, or are in stock lua but were removed.

--- Interprets the given string as lua code and executes it.
--- @param code string
function exu.DoString(code) end

--- Returns the inverse of the given matrix.
--- @nodiscard
--- @param mat Matrix
--- @return Matrix
function exu.MatrixInverse(mat) end

--- Transforms the given screen coordinates in pixels to a position in the world.
--- Returns a unit vector that originates at your camera and points into the world in the direction that matches the position of the given coordinates on screen.
--- Note that if you want something to appear at a consistent place on the screen regardless of resolution, you should use a ratio of the current resolution like (currentScreenWidth * 0.5) rather than a hardcoded pixel value.
--- @nodiscard
--- @param screenX integer
--- @param screenY integer
--- @return Vector
function exu.ScreenToWorld(screenX, screenY) end

--- Unrotates the given vector by the given perspective matrix.
--- @nodiscard
--- @param v Vector
--- @param perspectiveMat Matrix
--- @return Vector
function exu.VectorUnrotate(v, perspectiveMat) end

return exu
