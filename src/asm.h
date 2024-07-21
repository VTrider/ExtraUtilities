#pragma once

extern int weaponMask;

// asm functions

void WeaponMaskHook();

void OrdnanceVelocityHook();

void OrdnancePositionHook();

void ShotConvergenceHook();

// callable functions

void EnableOrdnanceTweak(float scalingFactor);

void UpdateOrdnance(float vx, float vy, float vz, float px, float py, float pz);

void EnableShotConvergence();

