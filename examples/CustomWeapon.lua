--[[
    Custom Weapon Behaviors for BZ98R
    Features:
    - Blast cannons pierce through up to 1 target.
    - Bullets bounce off the terrain slightly.
--]]

local BOUNCE_WEAPONS = {
    ["bullet1"]  = true,
    ["bullet1a"] = true,
    ["bullet1b"] = true,
    ["bullet1g"] = true,
    ["bullet2"]  = true,
    ["bullet5"]  = true,
    ["bullet6"]  = true,
    ["bullet6a"] = true,
}

local PIERCE_WEAPONS = {
    ["blast"] = true,
}

-- Track ordnance specifically to avoid infinite recursion or over-piercing
local piercedOrdnance = {}
local piercedCount = 0

local function ReflectTerrain(odf, transform, shooter)
    local hit, normal = GetTerrainHeightAndNormal(transform)
    
    local shotDirection = SetVector(transform.front_x, transform.front_y, transform.front_z)
    local shotPosition = SetVector(transform.posit_x, transform.posit_y, transform.posit_z)

    -- Formula for reflecting a vector across a normal: R = D - 2(D.N)N
    local dot = DotProduct(shotDirection, normal)
    local reflectedDirection = shotDirection - (2 * dot * normal)
    
    -- Shift position slightly above terrain to prevent immediate re-hit
    local spawnPos = shotPosition + (normal * 0.2)
    local bounceMat = BuildDirectionalMatrix(spawnPos, reflectedDirection)

    -- Build the bounced ordnance
    exu.BuildOrdnance(odf, bounceMat, shooter)
end

local function PierceObject(odf, transform, hitObject, ordnanceHandle)
    if not hitObject then return end

    -- Check if this specific projectile instance has already pierced once
    if piercedOrdnance[ordnanceHandle] then 
        return 
    end

    -- Mark this instance as having pierced
    piercedOrdnance[ordnanceHandle] = true
    piercedCount = piercedCount + 1

    -- Importantly we set the ordnance's owner to what just got hit, 
    -- so that the new ordnance passes through them and continues onwards.
    exu.BuildOrdnance(odf, transform, hitObject)
end

function exu.BulletHit(odf, shooter, hitObject, transform, ordnanceHandle)
    -- CLEANUP: Reset the table if it grows too large (paranoia)
    if piercedCount > 200 then 
        piercedOrdnance = {} 
        piercedCount = 0
    end

    if PIERCE_WEAPONS[odf] then
        PierceObject(odf, transform, hitObject, ordnanceHandle)
    elseif BOUNCE_WEAPONS[odf] and hitObject == nil then
        ReflectTerrain(odf, transform, shooter)
    end
end
