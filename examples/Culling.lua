--[[
    This tutorial explains how to use the distance-based
    unit culling features in Extra Utilities.

    By VTrider
--]]

function Start()
    -- Enable the culling system (it's disabled by default)
    exu.SetCullingEnabled(true)

    -- Set the culling distance to 500 meters
    -- Units beyond this distance will have their Ogre entities hidden,
    -- skipping expensive skeletal and vertex updates.
    exu.SetCullDistance(500.0)

    print("Culling system enabled. Dist: " .. tostring(exu.GetCullDistance()))
end

-- You can also toggle this at runtime or based on performance settings
function ToggleCulling()
    local currentlyEnabled = exu.GetCullingEnabled()
    exu.SetCullingEnabled(not currentlyEnabled)
    print("Culling enabled: " .. tostring(exu.GetCullingEnabled()))
end
