--[[
    This tutorial explains how to use GetGameKey()
    to detect when the player is holding down a key

    By VTrider
--]]

-- Different game key experiments
local fly = true
local boom = false
local teleport = false

-- Since this function will be called on each frame,
-- it will be inside of Update()
function Update(dt)
    -- Use one of the designated key codes: https://github.com/VTrider/ExtraUtilities/wiki/GetGameKey()-Keycodes
    --
    -- This block will execute every frame as long as
    -- the player is holding the left shift key.
    if exu.GetGameKey("LSHIFT") then
        -- Now you can do all kinds of fun stuff...
        
        -- Fly forwards when you hold shift
        if fly then
            SetVelocity(GetPlayerHandle(), Normalize(GetFront(GetPlayerHandle())) * 50)
        end

        -- Make wrecker explosions where you point the cursor (far away hopefully lol)
        -- Note that the position must also be within your smart reticle range,
        -- try exu.SetReticleRange(bigNumber)!
        if boom then
            MakeExplosion("xbmbxpl", exu.GetReticlePos())
        end

        -- Teleport to where you point the cursor
        if teleport then
            SetPosition(GetPlayerHandle(), exu.GetReticlePos())
        end
    end
end