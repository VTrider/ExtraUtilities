--[[
    This tutorial explains how to use ScreenToWorld

    By VTrider
--]]

local function DoSpin()
    local myTarget = GetTarget(GetPlayerHandle())
    if myTarget then
        -- These are the pixel counts of the X and Y resolution of your game
        local screenX, screenY = exu.GetGameResolution()

        -- These are just ratios between 0-1 that will smoothly change with time
        local ratio1 = (math.sin(GetTime()) + 1) / 2
        local ratio2 = (math.cos(GetTime()) + 1) / 2

        -- Extract the position from your camera's transform matrix
        local cameraTransform = exu.GetCameraTransformMatrix()
        local cameraPosition = SetVector(cameraTransform.posit_x, cameraTransform.posit_y, cameraTransform.posit_z)

        -- exu.ScreenToWorld returns a unit vector that originates at your camera and points
        -- into the world. The two ratios make it smoothly rotate around the edges of the screen.
        local worldDirection = exu.ScreenToWorld(screenX * ratio1, screenY * ratio2)

        -- Finally multiplying by a number (50 in this case) and adding it to your original position will place the object
        -- 50 meters in front of you in that direction.
        SetPosition(myTarget, cameraPosition + (worldDirection * 50))
    end
end

function Update(dt)
    DoSpin()
end