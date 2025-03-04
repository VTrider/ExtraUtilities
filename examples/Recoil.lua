--[[
    This tutorial explains how to use the BulletInit
    callback to make a weapon have recoil

    By VTrider
--]]

local function DoRecoil(odf, shooter, transform)
    if odf ~= "blast" then return end
    local front = SetVector(transform.front_x, transform.front_y, transform.front_z)
    local recoilDirection = -(Normalize(front))
    local recoilAmount = 10.0 -- m/s

    -- Do a constant recoil impulse
    local shooterCurrentVeloc = GetVelocity(shooter)
    SetVelocity(shooter, shooterCurrentVeloc + recoilDirection * recoilAmount)

    -- Shake the shooter too!
    local currentOmega = GetOmega(shooter)
    -- x = pitch shake, y = yaw shake, z = roll shake
    SetOmega(shooter, SetVector(currentOmega.x + math.random(5, 15) * 0.1, currentOmega.y + math.random(-25, 25) * 0.01, currentOmega.z))
end

function exu.BulletInit(odf, shooter, transform)
    DoRecoil(odf, shooter, transform)
end