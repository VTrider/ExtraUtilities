--[[
    PhysicsImpact.lua (v1.4.0)
    Adds data-driven recoil, knockback, piercing, and bouncing to weapons.
    
    This module centralizes all physical weapon behaviors.
    It maps effects to Ordnance names via a tuning table.
--]]

local PhysicsImpact = {
    -- Global tuning multipliers
    GlobalRecoil = 0.5,
    GlobalKnockback = 1.0,
    GlobalShake = 0.2,

    -- The master table for fine-tuning
    -- recoil, knockback, shake: multipliers for forces
    -- pierce: boolean or number (max pierces)
    -- bounce: boolean or table { ratio, strength, jitter }
    Weapons = {
        -- CANNONS
        ["blast"]    = { recoil = 8.5, knockback = 10.0, shake = 1.2, pierce = 1 }, -- gblast
        ["blastb"]   = { recoil = 9.0, knockback = 11.0, shake = 1.3, pierce = 1 }, -- gblastb
        ["blastg"]   = { recoil = 8.0, knockback = 9.0,  shake = 1.1, pierce = 1 }, -- gblastg
        ["bolt"]     = { recoil = 2.0, knockback = 4.0,  shake = 0.5 },
        ["snipe"]    = { recoil = 12.0, knockback = 15.0, shake = 1.8 },
        
        -- MACHINE GUNS
        ["bullet1"]  = { recoil = 0.2, knockback = 0.4, shake = 0.1, bounce = { ratio = 0.3, strength = 0.6, jitter = 0.1 } }, -- 30% bounce
        ["bullet1a"] = { recoil = 0.3, knockback = 0.5, shake = 0.2, bounce = { ratio = 0.4, strength = 0.7, jitter = 0.15 } },
        ["bullet1b"] = { recoil = 0.2, knockback = 0.4, shake = 0.1, bounce = { ratio = 0.3, strength = 0.6, jitter = 0.1 } },
        ["bullet1g"] = { recoil = 0.2, knockback = 0.3, shake = 0.1, bounce = { ratio = 0.2, strength = 0.5, jitter = 0.05 } },
        ["bullet2"]  = { recoil = 0.4, knockback = 0.6, shake = 0.2, bounce = { ratio = 0.5, strength = 0.8, jitter = 0.1 } },
        ["bullet5"]  = { recoil = 0.4, knockback = 0.6, shake = 0.2, bounce = { ratio = 0.5, strength = 0.8, jitter = 0.1 } },
        ["bullet6"]  = { recoil = 12.0, knockback = 15.0, shake = 1.8 },
        ["bullet6a"] = { recoil = 8.0,  knockback = 10.0, shake = 1.2 },
        
        -- ROCKETS / MISSILES
        ["rocket"]   = { recoil = 5.0, knockback = 12.0, shake = 0.8 }, 
        ["rocket2"]  = { recoil = 6.0, knockback = 15.0, shake = 1.0 }, 
        ["rocket3"]  = { recoil = 4.5, knockback = 10.0, shake = 0.7 },
        ["heatmsl"]  = { recoil = 3.0, knockback = 8.0,  shake = 0.5 }, 
        ["imagemsl"] = { recoil = 2.0, knockback = 5.0,  shake = 0.4 }, 
        ["swarmer"]  = { recoil = 1.0, knockback = 2.0,  shake = 0.2 }, 
        
        -- MORTARS / GRENADES
        ["grenade"]  = { recoil = 12.0, knockback = 20.0, shake = 2.0 }, 
        ["splintbm"] = { recoil = 10.0, knockback = 5.0,  shake = 1.5 }, 
        ["splinter"] = { recoil = 0.0,  knockback = 5.0,  shake = 0.0 }, 
        ["bouncebm"] = { recoil = 15.0, knockback = 25.0, shake = 2.5 }, 
        
        -- CHARGE WEAPONS
        ["charge1"]  = { recoil = 1.0, knockback = 2.0,  shake = 0.2 },
        ["charge2"]  = { recoil = 2.0, knockback = 4.0,  shake = 0.4 },
        ["charge3"]  = { recoil = 4.0, knockback = 8.0,  shake = 0.8 },
        ["charge4"]  = { recoil = 7.0, knockback = 12.0, shake = 1.2 },
        ["charge5"]  = { recoil = 10.0, knockback = 18.0, shake = 1.8 },
        ["charge6"]  = { recoil = 15.0, knockback = 25.0, shake = 2.5 }, 
        
        -- SPECIAL
        ["bolt"]     = { recoil = 2.0, knockback = 4.0,  shake = 0.5 }, 
        ["flashch"]  = { recoil = 0.0, knockback = 0.1,  shake = 0.0 }, 
        ["seismic"]  = { recoil = 0.0, knockback = 50.0, shake = 5.0 }, 
    },

    Cache = {},
    Pierced = {}, -- key: handle, val: count
    PiercedCount = 0,
    StockPath = nil
}

local function GetStats(odfName)
    if PhysicsImpact.Weapons[odfName] then return PhysicsImpact.Weapons[odfName] end
    if PhysicsImpact.Cache[odfName] then return PhysicsImpact.Cache[odfName] end

    -- Fallback to ODF parsing if path is provided
    if not PhysicsImpact.StockPath then 
        return { recoil = 1.0, knockback = 1.0, shake = 0.2 } -- Generic fallback
    end

    local totalDamage = 0
    local speed = 100
    
    local calculated = { recoil = 1, knockback = 1, shake = 0.2 }
    local f = io.open(PhysicsImpact.StockPath .. odfName .. ".odf", "r")
    if f then
        for line in f:lines() do
            local k, v = line:match("^%s*([%w%d_]+)%s*=%s*([^%s;]+)")
            if k then
                if k:find("^damage") then
                    totalDamage = totalDamage + (tonumber(v) or 0)
                elseif k == "shotSpeed" then
                    speed = tonumber(v) or speed
                end
            end
        end
        f:close()
    else
        -- Absolute fallback if file missing
        totalDamage = 50
    end

    local speedFactor = math.log10(speed + 1)
    local calculated = {
        recoil = (totalDamage * speedFactor) * 0.02,
        knockback = (totalDamage * speedFactor) * 0.05,
        shake = (totalDamage * speedFactor) * 0.005
    }

    PhysicsImpact.Cache[odfName] = calculated
    return calculated
end

function exu.BulletInit(odf, shooter, transform)
    if not IsHandle(shooter) then return end
    
    local stats = GetStats(odf)
    if stats.recoil <= 0 and stats.shake <= 0 then return end

    local front = SetVector(transform.front_x, transform.front_y, transform.front_z)
    
    -- Linear Recoil
    local vel = GetVelocity(shooter)
    SetVelocity(shooter, vel - front * stats.recoil * PhysicsImpact.GlobalRecoil * 0.1)

    -- Rotational Shake
    local omega = GetOmega(shooter)
    local shakeVal = stats.shake * PhysicsImpact.GlobalShake
    local pitch = (math.random() * 2 - 1) * shakeVal
    local yaw = (math.random() * 2 - 1) * shakeVal * 0.5
    SetOmega(shooter, SetVector(omega.x + pitch, omega.y + yaw, omega.z))
end

function exu.BulletHit(odf, shooter, hitObject, transform, ordnanceHandle)
    local stats = GetStats(odf)
    
    -- 1. PIERCING (Blast Cannons)
    if stats.pierce and hitObject then
        local pCount = PhysicsImpact.Pierced[ordnanceHandle] or 0
        local maxPierce = type(stats.pierce) == "number" and stats.pierce or 1

        if pCount < maxPierce then
            PhysicsImpact.Pierced[ordnanceHandle] = pCount + 1
            if pCount == 0 then PhysicsImpact.PiercedCount = PhysicsImpact.PiercedCount + 1 end
            
            -- Owner-swap allows projectile to pass through hitObject
            exu.BuildOrdnance(odf, transform, hitObject)
        end
    end

    -- 2. BOUNCING (Bullets)
    if stats.bounce and not hitObject then
        local bConfig = type(stats.bounce) == "table" and stats.bounce or { ratio = 1.0, strength = 1.0, jitter = 0.0 }
        
        if math.random() <= bConfig.ratio then
            local hit, normal = GetTerrainHeightAndNormal(transform)
            local shotDir = SetVector(transform.front_x, transform.front_y, transform.front_z)
            local shotPos = SetVector(transform.posit_x, transform.posit_y, transform.posit_z)

            -- Reflect across normal: R = D - 2(D.N)N
            local dot = DotProduct(shotDir, normal)
            local reflected = shotDir - (2 * dot * normal)
            
            -- Apply Jitter
            if bConfig.jitter > 0 then
                local jitterVec = SetVector((math.random()*2-1)*bConfig.jitter, (math.random()*2-1)*bConfig.jitter, (math.random()*2-1)*bConfig.jitter)
                reflected = Normalize(reflected + jitterVec)
            end

            -- Shift position slightly above terrain
            local spawnPos = shotPos + (normal * 0.2)
            local bounceMat = BuildDirectionalMatrix(spawnPos, reflected)
            
            local newOrd = exu.BuildOrdnance(odf, bounceMat, shooter)
            
            -- Apply Strength (Velocity multiplier)
            if bConfig.strength ~= 1.0 and newOrd then
                local currentVel = exu.GetOrdnanceAttribute(newOrd, exu.ORDNANCE.VELOCITY)
                exu.SetOrdnanceVelocity(newOrd, currentVel * bConfig.strength)
            end
        end
    end

    -- 3. KNOCKBACK (Physics)
    if IsHandle(hitObject) and stats.knockback > 0 then
        local front = SetVector(transform.front_x, transform.front_y, transform.front_z)
        local vel = GetVelocity(hitObject)
        SetVelocity(hitObject, vel + front * stats.knockback * PhysicsImpact.GlobalKnockback * 0.1)
    end

    -- Cleanup tracking table
    if PhysicsImpact.PiercedCount > 200 then
        PhysicsImpact.Pierced = {}
        PhysicsImpact.PiercedCount = 0
    end
end

local count = 0
for _ in pairs(PhysicsImpact.Weapons) do count = count + 1 end
print("PhysicsImpact v1.4 loaded with " .. tostring(count) .. " tuned ordnances.")
return PhysicsImpact
