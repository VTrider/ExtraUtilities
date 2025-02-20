--[[
    This tutorial explains how to use the BulletHit
    callback to make a certain projectile do knockback

    By VTrider
--]]

local function DoKnockback(odf, hitObject, transform)
    if odf ~= "rocket2" then return end
    local front = SetVector(transform.front_x, transform.front_y, transform.front_z)
    local kbVector = Normalize(front)
    local amount = 100.0 -- m/s

    local objCurrentVeloc = GetVelocity(hitObject)
    SetVelocity(hitObject, objCurrentVeloc + kbVector * amount) -- wheeeeeeee
end

function exu.BulletHit(odf, shooter, hitObject, transform)
    exu.MessageBox(odf)
    DoKnockback(odf, hitObject, transform)
end