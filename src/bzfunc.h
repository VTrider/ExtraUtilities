#pragma once

#include "bzr.h"
#include <Windows.h>
#include <string>
#include <vector>
#include <atomic>

// Global function and variable declarations (Rhade would kill me)
// Explanations are provided where they are defined

/*---------------------
* Important Functions *
----------------------*/

GameObject* GetObj(unsigned int handle);

void ResetValues();

unsigned char* Hook(void* hookAddress, void* function, int length);

/*-----------------------
* Environment Functions *
----------------------- */

VECTOR_3D GetGravity();

void SetGravity(float x, float y, float z);

extern Luminance* sunlight;

Luminance* GetLuminance();

void SetLuminance(float r, float g, float b);

extern float* fogStart;

float GetFogStart();

void SetFogStart(float value);

/*-------------------
* Reticle Functions *
------------------- */

float GetReticleAngle();

VECTOR_3D GetReticlePos();

float GetSmartCursorRange();

void SetSmartCursorRange(float range);

/*-----------------
* Radar Functions *
----------------- */

char GetRadarState();

void SetRadarState(int state);

/*---------------------
* Satellite Functions *
--------------------- */

char GetSatState();

VECTOR_3D GetSatCursorPos();

VECTOR_3D GetSatCamPos();

VECTOR_3D GetSatClickPos();

float GetSatPanSpeed();

void SetSatPanSpeed(float speed);

float GetMinSatZoom();

void SetMinSatZoom(float zoom);

float GetMaxSatZoom();

void SetMaxSatZoom(float zoom);

float GetSatZoom();

void SetSatZoom(float zoom);

/*--------------
* Input/Output *
---------------*/

int GetKeyCode(std::string keycode);

extern bool keyPressed;

bool GetGameKey(int key);

/*----------------
* Misc Functions *
-----------------*/
const char* GetSteam64();

int GetWeaponMask();

int GetLives();

void SetLives(int newLives);

const char* GetDifficulty();

int SetDifficulty(std::string newDifficulty);

/*-------------------------
* Internal Function Hooks *
--------------------------*/

typedef void(__thiscall* _SetAsUser)(GameObject* obj);

extern _SetAsUser SetAsUser;