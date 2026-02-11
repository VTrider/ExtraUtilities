--[[
    This tutorial explains how to use the built-in 
    3D primitive rendering features.

    By VTrider
--]]

function Update(dt)
    local player = GetPlayerHandle()
    if player then
        local playerPos = GetPosition(player)
        
        -- Draw a red line from player to 10m above
        exu.DrawLine(playerPos, playerPos + SetVector(0, 10, 0), { r = 1, g = 0, b = 0 })

        -- Draw a green 5x5x5 box around the player
        exu.DrawBox(playerPos, SetVector(5, 5, 5), { r = 0, g = 1, b = 0 })
    end
end

function PostSave()
    -- In some versions, you may want to clear visuals after a save or map reload
    exu.ClearVisuals()
end
