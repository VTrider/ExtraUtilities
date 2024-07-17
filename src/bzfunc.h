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

extern DWORD moduleBase;

DWORD GetPointerAddress(DWORD ptr, std::vector<DWORD> offsets);

void ResetValues();

unsigned char* Hook(void* hookAddress, void* function, int length);

/*-----------------------
* Environment Functions *
----------------------- */

extern VECTOR_3D* gravityVector;

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

extern float* reticleAngle;

float GetReticleAngle();

extern VECTOR_3D* reticlePos;

VECTOR_3D GetReticlePos();

extern float* smartCursorRange;

float GetSmartCursorRange();

void SetSmartCursorRange(float range);

/*-----------------
* Radar Functions *
----------------- */

extern char* radarState;

char GetRadarState();

void SetRadarState(int state);

/*---------------------
* Satellite Functions *
--------------------- */

extern char* satState;

char GetSatState();

extern VECTOR_3D* satCursorPos;

VECTOR_3D* GetSatCursorPos();

extern VECTOR_3D* satCamPos;

VECTOR_3D* GetSatCamPos();

extern VECTOR_3D* satClickPos;

VECTOR_3D* GetSatClickPos();

extern float* satPanSpeed;

float GetSatPanSpeed();

void SetSatPanSpeed(float speed);

extern float* minSatZoom;

float GetMinSatZoom();

void SetMinSatZoom(float zoom);

extern float* maxSatZoom;

float GetMaxSatZoom();

void SetMaxSatZoom(float zoom);

extern float* satZoom;

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

extern long long* steam64;

const char* GetSteam64();

extern int weaponMask;

int GetWeaponMask();

extern int* lives;

int GetLives();

void SetLives(int newLives);

extern char* difficulty;

const char* GetDifficulty();

int SetDifficulty(std::string newDifficulty);

extern unsigned char* ordnanceTweakBytes;

void EnableOrdnanceTweak();

void UpdateOrdnance(float vx, float vy, float vz, float px, float py, float pz);

/*-------------------------
* Internal Function Hooks *
--------------------------*/

typedef void(__thiscall* _SetAsUser)(GameObject* obj);

extern _SetAsUser SetAsUser;