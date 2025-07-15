--[[
    This tutorial explains how to use the BulletHit
    callback to make a custom weapon with object piercing
    and terrain reflecting or piercing features

    By VTrider
--]]

-- Can be any ordnance odf, blast gives a cool effect
local ORD = "blast"

-- Not implemented but you should be aware that if you recursively
-- build ordnance you should take measures to prevent overflows
local MAX_BOUNCES = 10

local function ReflectTerrain(transform, shooter, hitObject)
    -- This only applies on hitting terrain
    if hitObject then return end

    local _, normal = GetTerrainHeightAndNormal(transform)
    local shotDirection = SetVector(transform.front_x, transform.front_y, transform.front_z)
    local shotPosition = SetVector(transform.posit_x, transform.posit_y, transform.posit_z)

    -- Formula for reflecting a vector across a normal
    local reflectedDirection = shotDirection - (2 * DotProduct(shotDirection, normal) * normal)
    local piercingShotMat = BuildDirectionalMatrix(shotPosition, reflectedDirection)

    -- The owner could be set to another object or a dummy object if you want the shooter
    -- to be able to get hit by their reflected shots
    exu.BuildOrdnance("blast", piercingShotMat, shooter)
end

-- Meters forward that the piercing ordnance will move by
-- when building the terrain piercing shot, this is a simple implementation,
-- but you could improve it by using a more sophisticated method like using
-- the terrain height and normals for example
local TOLERANCE = 5.0

-- Note that in this function it spawns a NEW ordnance with a reset lifetime, so it could
-- inadvertently increase the range of the weapon, significantly if it already has long range (like blast),
-- so if this is a concern I would recommend making the piercing shot a special odf with reduced range, or
-- you can use exu.GetOrdnanceAttribute(ordnanceHandle, exu.ORDNANCE.INIT_TIME) to query it's starting time, and use
-- that to calculate how much of its lifetime has passed, and pick an odf accordingly, or you could just choose
-- not to use the piercing shot if the ordnance is past a certain proportion of its lifetime for example.
local function PierceTerrain(transform, shooter, hitObject)
    --- This only applies on hitting terrain
    if hitObject then return end

    local shotDirection = SetVector(transform.front_x, transform.front_y, transform.front_z)
    local shotPosition = SetVector(transform.posit_x, transform.posit_y, transform.posit_z)

    -- Building an ordnance inside the terrain will allow it to exit on the other side.
    -- Note that BulletHit is NOT called when a bullet intersects terrain from the "bottom"
    local insideTerrain = shotPosition + (shotDirection * TOLERANCE)

    local piercingTerrainMat = BuildDirectionalMatrix(insideTerrain, shotDirection)
    exu.BuildOrdnance(ORD, piercingTerrainMat, shooter)
end

local function PierceObject(transform, hitObject)
    -- This only applies on an object hit, though you could also
    -- design it to pierce terrain by building it inside the terrain
    -- or on the other side
    if not hitObject then return end

    -- Importantly we set the ordnance's owner to what
    -- just got hit, so that the new ordnance passes through
    -- them and continues onwards, otherwise it will simply hit
    -- the target again if the owner is the original shooter
    exu.BuildOrdnance(ORD, transform, hitObject)
end

function exu.BulletHit(odf, shooter, hitObject, transform, ordnanceHandle)
    PierceObject(transform, hitObject)

    -- The following two are mutually exclusive, only run one at a time
    ReflectTerrain(transform, shooter, hitObject)
    
    -- PierceTerrain(transform, shooter, hitObject)
end