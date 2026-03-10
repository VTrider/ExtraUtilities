/* Copyright (C) 2023-2026 VTrider
 *
 * This file is part of Extra Utilities.
 *
 * Extra Utilities is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include "BZR.h"
#include "OgreSceneManagerShim.h"
#include "Scanner.h"

#include <cstdint>
#include <memory>
#include <string>

namespace ExtraUtilities::Ogre
{
	struct Fog
	{
		float r;
		float g;
		float b;
		uint8_t padding[4] = { 0 };
		float start;
		float ending;

		Fog() = default;
		Fog(float r, float g, float b, float start, float ending)
			: r(r), g(g), b(b), start(start), ending(ending) {}
	};

	struct Color
	{
		float r = 0.0f;
		float g = 0.0f;
		float b = 0.0f;
		float a = 1.0f;
	};

	struct SkyBoxGenParameters
	{
		float distance;
	};

	struct SkyDomeGenParameters
	{
		float curvature;
		float tiling;
		float distance;
		int32_t xsegments;
		int32_t ysegments;
		int32_t ysegments_keep;
	};

	struct SkyPlaneGenParameters
	{
		float scale;
		float tiling;
		float bow;
		int32_t xsegments;
		int32_t ysegments;
	};

	inline Scanner terrain_masterlight(BZR::Ogre::terrain_masterlight, BasicScanner::Restore::DISABLED);
	inline Scanner sceneManager(BZR::Ogre::sceneManagerStructure, { BZR::Ogre::sceneManagerOffset }, BasicScanner::Restore::DISABLED);

	// Gets the fog structure from the scene manager
	inline Fog* GetFog()
	{
		void* sm = sceneManager.Read();
		Fog* fog;
		__asm
		{
			mov eax, [sm]
			add eax, 0x128
			mov [fog], eax
		}
		return fog;
	}

	// Function Pointers

	// This is bordering on voodoo

	using enum BasicScanner::BaseAddress;

	inline uintptr_t getAmbientLightAddr = BasicScanner::CalculateAddress(BZR::Ogre::getAmbientLightOffset, OGRE);
	using _GetAmbientLight = Color*(__thiscall*)(void*);
	inline _GetAmbientLight GetAmbientLight = (_GetAmbientLight)getAmbientLightAddr;

	inline uintptr_t setAmbientLightAddr = BasicScanner::CalculateAddress(BZR::Ogre::setAmbientLightOffset, OGRE);
	using _SetAmbientLight = void(__thiscall*)(void*, Color*);
	inline _SetAmbientLight SetAmbientLight = (_SetAmbientLight)setAmbientLightAddr;

	inline uintptr_t getDiffuseColorAddr = BasicScanner::CalculateAddress(BZR::Ogre::getDiffuseColorOffset, OGRE);
	using _GetDiffuseColor = Color*(__thiscall*)(void*);
	inline _GetDiffuseColor GetDiffuseColor = (_GetDiffuseColor)getDiffuseColorAddr;

	inline uintptr_t setDiffuseColorAddr = BasicScanner::CalculateAddress(BZR::Ogre::setDiffuseColorOffset, OGRE);
	using _SetDiffuseColor = void(__thiscall*)(void*, float, float, float);
	inline _SetDiffuseColor SetDiffuseColor = (_SetDiffuseColor)setDiffuseColorAddr;

	inline uintptr_t getSpecularColorAddr = BasicScanner::CalculateAddress(BZR::Ogre::getSpecularColorOffset, OGRE);
	using _GetSpecularColor = Color*(__thiscall*)(void*);
	inline _GetSpecularColor GetSpecularColor = (_GetDiffuseColor)getSpecularColorAddr;

	inline uintptr_t setSpecularColorAddr = BasicScanner::CalculateAddress(BZR::Ogre::setSpecularColorOffset, OGRE);
	using _SetSpecularColor = void(__thiscall*)(void*, float, float, float);
	inline _SetSpecularColor SetSpecularColor = (_SetSpecularColor)setSpecularColorAddr;

	inline uintptr_t getDirectionAddr = BasicScanner::CalculateAddress(0x14042, OGRE);
	using _GetDirection = BZR::VECTOR_3D*(__thiscall*)(void*);
	inline _GetDirection GetDirection = (_GetDirection)getDirectionAddr;

	inline uintptr_t setDirectionAddr = BasicScanner::CalculateAddress(0x0FB69, OGRE);
	using _SetDirection = void(__thiscall*)(void*, float, float, float);
	inline _SetDirection SetDirection = (_SetDirection)setDirectionAddr;

	inline uintptr_t getPowerScaleAddr = BasicScanner::CalculateAddress(0x3A5CB, OGRE);
	using _GetPowerScale = float(__thiscall*)(void*);
	inline _GetPowerScale GetPowerScale = (_GetPowerScale)getPowerScaleAddr;

	inline uintptr_t setPowerScaleAddr = BasicScanner::CalculateAddress(0x10FBE, OGRE);
	using _SetPowerScale = void(__thiscall*)(void*, float);
	inline _SetPowerScale SetPowerScale = (_SetPowerScale)setPowerScaleAddr;

	inline uintptr_t getShadowFarDistanceAddr = BasicScanner::CalculateAddress(0x1A9B0, OGRE);
	using _GetShadowFarDistance = float(__thiscall*)(void*);
	inline _GetShadowFarDistance GetShadowFarDistance = (_GetShadowFarDistance)getShadowFarDistanceAddr;

	inline uintptr_t setShadowFarDistanceAddr = BasicScanner::CalculateAddress(0x0F5C4, OGRE);
	using _SetShadowFarDistance = void(__thiscall*)(void*, float);
	inline _SetShadowFarDistance SetShadowFarDistance = (_SetShadowFarDistance)setShadowFarDistanceAddr;

	inline uintptr_t setSpotlightRangeAddr = BasicScanner::CalculateAddress(BZR::Ogre::setSpotlightRangeOffset, OGRE);
	using _SetSpotlightRange = void(__thiscall*)(void*, float*, float*, float);
	inline _SetSpotlightRange SetSpotlightRange = (_SetSpotlightRange)setSpotlightRangeAddr;

	inline uintptr_t setVisibleAddr = BasicScanner::CalculateAddress(BZR::Ogre::setVisibleOffset, OGRE);
	using _SetVisible = void(__thiscall*)(void*, bool);
	inline _SetVisible SetVisible = (_SetVisible)setVisibleAddr;

	inline uintptr_t getVisibleAddr = BasicScanner::CalculateAddress(0x05E70, OGRE);
	using _GetVisible = bool(__thiscall*)(void*);
	inline _GetVisible GetVisible = (_GetVisible)getVisibleAddr;

	inline uintptr_t getCastShadowsAddr = BasicScanner::CalculateAddress(0x37C40, OGRE);
	using _GetCastShadows = bool(__thiscall*)(void*);
	inline _GetCastShadows GetCastShadows = (_GetCastShadows)getCastShadowsAddr;

	inline uintptr_t setCastShadowsAddr = BasicScanner::CalculateAddress(0x325C4, OGRE);
	using _SetCastShadows = void(__thiscall*)(void*, bool);
	inline _SetCastShadows SetCastShadows = (_SetCastShadows)setCastShadowsAddr;

	inline uintptr_t getRenderingDistanceAddr = BasicScanner::CalculateAddress(0x3C48E, OGRE);
	using _GetRenderingDistance = float(__thiscall*)(void*);
	inline _GetRenderingDistance GetRenderingDistance = (_GetRenderingDistance)getRenderingDistanceAddr;

	inline uintptr_t setRenderingDistanceAddr = BasicScanner::CalculateAddress(0x11E82, OGRE);
	using _SetRenderingDistance = void(__thiscall*)(void*, float);
	inline _SetRenderingDistance SetRenderingDistance = (_SetRenderingDistance)setRenderingDistanceAddr;

	inline uintptr_t getVisibilityFlagsAddr = BasicScanner::CalculateAddress(0x2B819, OGRE);
	using _GetVisibilityFlags = uint32_t(__thiscall*)(void*);
	inline _GetVisibilityFlags GetVisibilityFlags = (_GetVisibilityFlags)getVisibilityFlagsAddr;

	inline uintptr_t setVisibilityFlagsAddr = BasicScanner::CalculateAddress(0x33906, OGRE);
	using _SetVisibilityFlags = void(__thiscall*)(void*, uint32_t);
	inline _SetVisibilityFlags SetVisibilityFlags = (_SetVisibilityFlags)setVisibilityFlagsAddr;

	inline uintptr_t getQueryFlagsAddr = BasicScanner::CalculateAddress(0x2526B, OGRE);
	using _GetQueryFlags = uint32_t(__thiscall*)(void*);
	inline _GetQueryFlags GetQueryFlags = (_GetQueryFlags)getQueryFlagsAddr;

	inline uintptr_t setQueryFlagsAddr = BasicScanner::CalculateAddress(0x361BF, OGRE);
	using _SetQueryFlags = void(__thiscall*)(void*, uint32_t);
	inline _SetQueryFlags SetQueryFlags = (_SetQueryFlags)setQueryFlagsAddr;

	inline uintptr_t getRenderQueueGroupAddr = BasicScanner::CalculateAddress(0x1584D, OGRE);
	using _GetRenderQueueGroup = uint8_t(__thiscall*)(void*);
	inline _GetRenderQueueGroup GetRenderQueueGroup = (_GetRenderQueueGroup)getRenderQueueGroupAddr;

	inline uintptr_t setRenderQueueGroupMovableAddr = BasicScanner::CalculateAddress(0x11CBB, OGRE);
	using _SetRenderQueueGroupMovable = void(__thiscall*)(void*, uint8_t);
	inline _SetRenderQueueGroupMovable SetRenderQueueGroupMovable = (_SetRenderQueueGroupMovable)setRenderQueueGroupMovableAddr;

	inline uintptr_t setRenderQueueGroupSubEntityAddr = BasicScanner::CalculateAddress(0x2454B, OGRE);
	using _SetRenderQueueGroupSubEntity = void(__thiscall*)(void*, uint8_t);
	inline _SetRenderQueueGroupSubEntity SetRenderQueueGroupSubEntity = (_SetRenderQueueGroupSubEntity)setRenderQueueGroupSubEntityAddr;

	inline uintptr_t getNumSubEntitiesAddr = BasicScanner::CalculateAddress(0x2635F, OGRE);
	using _GetNumSubEntities = uint32_t(__thiscall*)(void*);
	inline _GetNumSubEntities GetNumSubEntities = (_GetNumSubEntities)getNumSubEntitiesAddr;

	inline uintptr_t getSubEntityByIndexAddr = BasicScanner::CalculateAddress(0x1A163, OGRE);
	using _GetSubEntityByIndex = void*(__thiscall*)(void*, uint32_t);
	inline _GetSubEntityByIndex GetSubEntityByIndex = (_GetSubEntityByIndex)getSubEntityByIndexAddr;

	inline uintptr_t getMaterialNameSubEntityAddr = BasicScanner::CalculateAddress(0x07B08, OGRE);
	using _GetMaterialNameSubEntity = const std::string&(__thiscall*)(void*);
	inline _GetMaterialNameSubEntity GetMaterialNameSubEntity = (_GetMaterialNameSubEntity)getMaterialNameSubEntityAddr;

	inline uintptr_t setMaterialNameEntityAddr = BasicScanner::CalculateAddress(0x1BF9F, OGRE);
	using _SetMaterialNameEntity = void(__thiscall*)(void*, const std::string&, const std::string&);
	inline _SetMaterialNameEntity SetMaterialNameEntity = (_SetMaterialNameEntity)setMaterialNameEntityAddr;

	inline uintptr_t setMaterialNameSubEntityAddr = BasicScanner::CalculateAddress(0x06E3D, OGRE);
	using _SetMaterialNameSubEntity = void(__thiscall*)(void*, const std::string&, const std::string&);
	inline _SetMaterialNameSubEntity SetMaterialNameSubEntity = (_SetMaterialNameSubEntity)setMaterialNameSubEntityAddr;

	inline uintptr_t hasSkeletonAddr = BasicScanner::CalculateAddress(0x1D5E8, OGRE);
	using _HasSkeleton = bool(__thiscall*)(void*);
	inline _HasSkeleton HasSkeleton = (_HasSkeleton)hasSkeletonAddr;

	inline uintptr_t getAllAnimationStatesAddr = BasicScanner::CalculateAddress(0x3F684, OGRE);
	using _GetAllAnimationStates = void*(__thiscall*)(void*);
	inline _GetAllAnimationStates GetAllAnimationStates = (_GetAllAnimationStates)getAllAnimationStatesAddr;

	inline uintptr_t hasAnimationStateAddr = BasicScanner::CalculateAddress(0x1D624, OGRE);
	using _HasAnimationState = bool(__thiscall*)(void*, const std::string&);
	inline _HasAnimationState HasAnimationState = (_HasAnimationState)hasAnimationStateAddr;

	inline uintptr_t getAnimationStateAddr = BasicScanner::CalculateAddress(0x32B1E, OGRE);
	using _GetAnimationState = void*(__thiscall*)(void*, const std::string&);
	inline _GetAnimationState GetAnimationState = (_GetAnimationState)getAnimationStateAddr;

	inline uintptr_t getAnimationLengthAddr = BasicScanner::CalculateAddress(0x0F439, OGRE);
	using _GetAnimationLength = float(__thiscall*)(void*);
	inline _GetAnimationLength GetAnimationLength = (_GetAnimationLength)getAnimationLengthAddr;

	inline uintptr_t getAnimationTimePositionAddr = BasicScanner::CalculateAddress(0x3FE09, OGRE);
	using _GetAnimationTimePosition = float(__thiscall*)(void*);
	inline _GetAnimationTimePosition GetAnimationTimePosition = (_GetAnimationTimePosition)getAnimationTimePositionAddr;

	inline uintptr_t getAnimationWeightAddr = BasicScanner::CalculateAddress(0x1CD05, OGRE);
	using _GetAnimationWeight = float(__thiscall*)(void*);
	inline _GetAnimationWeight GetAnimationWeight = (_GetAnimationWeight)getAnimationWeightAddr;

	inline uintptr_t getAnimationLoopAddr = BasicScanner::CalculateAddress(0x0C3B5, OGRE);
	using _GetAnimationLoop = bool(__thiscall*)(void*);
	inline _GetAnimationLoop GetAnimationLoop = (_GetAnimationLoop)getAnimationLoopAddr;

	inline uintptr_t getAnimationEnabledAddr = BasicScanner::CalculateAddress(0x21256, OGRE);
	using _GetAnimationEnabled = bool(__thiscall*)(void*);
	inline _GetAnimationEnabled GetAnimationEnabled = (_GetAnimationEnabled)getAnimationEnabledAddr;

	inline uintptr_t setAnimationTimePositionAddr = BasicScanner::CalculateAddress(0x3BE62, OGRE);
	using _SetAnimationTimePosition = void(__thiscall*)(void*, float);
	inline _SetAnimationTimePosition SetAnimationTimePosition = (_SetAnimationTimePosition)setAnimationTimePositionAddr;

	inline uintptr_t setAnimationWeightAddr = BasicScanner::CalculateAddress(0x2EABE, OGRE);
	using _SetAnimationWeight = void(__thiscall*)(void*, float);
	inline _SetAnimationWeight SetAnimationWeight = (_SetAnimationWeight)setAnimationWeightAddr;

	inline uintptr_t setAnimationEnabledAddr = BasicScanner::CalculateAddress(0x0B6DB, OGRE);
	using _SetAnimationEnabled = void(__thiscall*)(void*, bool);
	inline _SetAnimationEnabled SetAnimationEnabled = (_SetAnimationEnabled)setAnimationEnabledAddr;

	inline uintptr_t setAnimationLoopAddr = BasicScanner::CalculateAddress(0x16027, OGRE);
	using _SetAnimationLoop = void(__thiscall*)(void*, bool);
	inline _SetAnimationLoop SetAnimationLoop = (_SetAnimationLoop)setAnimationLoopAddr;

	inline uintptr_t getLightPositionAddr = BasicScanner::CalculateAddress(0x2B68E, OGRE);
	using _GetLightPosition = BZR::VECTOR_3D * (__thiscall*)(void*);
	inline _GetLightPosition GetLightPosition = (_GetLightPosition)getLightPositionAddr;

	inline uintptr_t setLightPositionAddr = BasicScanner::CalculateAddress(0x3D695, OGRE);
	using _SetLightPosition = void(__thiscall*)(void*, float, float, float);
	inline _SetLightPosition SetLightPosition = (_SetLightPosition)setLightPositionAddr;

	inline uintptr_t setAttenuationAddr = BasicScanner::CalculateAddress(0x2BC42, OGRE);
	using _SetAttenuation = void(__thiscall*)(void*, float, float, float, float);
	inline _SetAttenuation SetAttenuation = (_SetAttenuation)setAttenuationAddr;

	inline uintptr_t getCameraNearClipDistanceAddr = BasicScanner::CalculateAddress(0x34040, OGRE);
	using _GetCameraNearClipDistance = float(__thiscall*)(void*);
	inline _GetCameraNearClipDistance GetCameraNearClipDistance = (_GetCameraNearClipDistance)getCameraNearClipDistanceAddr;

	inline uintptr_t getCameraFarClipDistanceAddr = BasicScanner::CalculateAddress(0x3C8EE, OGRE);
	using _GetCameraFarClipDistance = float(__thiscall*)(void*);
	inline _GetCameraFarClipDistance GetCameraFarClipDistance = (_GetCameraFarClipDistance)getCameraFarClipDistanceAddr;

	inline uintptr_t getFrustumAspectRatioAddr = BasicScanner::CalculateAddress(0x3F012, OGRE);
	using _GetFrustumAspectRatio = float(__thiscall*)(void*);
	inline _GetFrustumAspectRatio GetFrustumAspectRatio = (_GetFrustumAspectRatio)getFrustumAspectRatioAddr;

	inline uintptr_t setFrustumAspectRatioAddr = BasicScanner::CalculateAddress(0x02077, OGRE);
	using _SetFrustumAspectRatio = void(__thiscall*)(void*, float);
	inline _SetFrustumAspectRatio SetFrustumAspectRatio = (_SetFrustumAspectRatio)setFrustumAspectRatioAddr;

	inline uintptr_t setFrustumNearClipDistanceAddr = BasicScanner::CalculateAddress(0x321E1, OGRE);
	using _SetFrustumNearClipDistance = void(__thiscall*)(void*, float);
	inline _SetFrustumNearClipDistance SetFrustumNearClipDistance = (_SetFrustumNearClipDistance)setFrustumNearClipDistanceAddr;

	inline uintptr_t setFrustumFarClipDistanceAddr = BasicScanner::CalculateAddress(0x1BC2A, OGRE);
	using _SetFrustumFarClipDistance = void(__thiscall*)(void*, float);
	inline _SetFrustumFarClipDistance SetFrustumFarClipDistance = (_SetFrustumFarClipDistance)setFrustumFarClipDistanceAddr;

	inline uintptr_t getFrustumProjectionTypeAddr = BasicScanner::CalculateAddress(0x03373, OGRE);
	using _GetFrustumProjectionType = int(__thiscall*)(void*);
	inline _GetFrustumProjectionType GetFrustumProjectionType = (_GetFrustumProjectionType)getFrustumProjectionTypeAddr;

	inline uintptr_t setFrustumProjectionTypeAddr = BasicScanner::CalculateAddress(0x2C165, OGRE);
	using _SetFrustumProjectionType = void(__thiscall*)(void*, int);
	inline _SetFrustumProjectionType SetFrustumProjectionType = (_SetFrustumProjectionType)setFrustumProjectionTypeAddr;

	inline uintptr_t getCameraPolygonModeAddr = BasicScanner::CalculateAddress(0x2A2D4, OGRE);
	using _GetCameraPolygonMode = int(__thiscall*)(void*);
	inline _GetCameraPolygonMode GetCameraPolygonMode = (_GetCameraPolygonMode)getCameraPolygonModeAddr;

	inline uintptr_t setCameraPolygonModeAddr = BasicScanner::CalculateAddress(0x1F352, OGRE);
	using _SetCameraPolygonMode = void(__thiscall*)(void*, int);
	inline _SetCameraPolygonMode SetCameraPolygonMode = (_SetCameraPolygonMode)setCameraPolygonModeAddr;

	inline uintptr_t getShowBoundingBoxesAddr = BasicScanner::CalculateAddress(0x09C64, OGRE);
	using _GetShowBoundingBoxes = bool(__thiscall*)(void*);
	inline _GetShowBoundingBoxes GetShowBoundingBoxes = (_GetShowBoundingBoxes)getShowBoundingBoxesAddr;

	inline uintptr_t showBoundingBoxesAddr = BasicScanner::CalculateAddress(0x0D8D2, OGRE);
	using _ShowBoundingBoxes = void(__thiscall*)(void*, bool);
	inline _ShowBoundingBoxes ShowBoundingBoxes = (_ShowBoundingBoxes)showBoundingBoxesAddr;

	inline uintptr_t getShowDebugShadowsAddr = BasicScanner::CalculateAddress(0x23CB8, OGRE);
	using _GetShowDebugShadows = bool(__thiscall*)(void*);
	inline _GetShowDebugShadows GetShowDebugShadows = (_GetShowDebugShadows)getShowDebugShadowsAddr;

	inline uintptr_t setShowDebugShadowsAddr = BasicScanner::CalculateAddress(0x0D396, OGRE);
	using _SetShowDebugShadows = void(__thiscall*)(void*, bool);
	inline _SetShowDebugShadows SetShowDebugShadows = (_SetShowDebugShadows)setShowDebugShadowsAddr;

	inline uintptr_t getSceneVisibilityMaskAddr = BasicScanner::CalculateAddress(0x03A7B, OGRE);
	using _GetSceneVisibilityMask = uint32_t(__thiscall*)(void*);
	inline _GetSceneVisibilityMask GetSceneVisibilityMask = (_GetSceneVisibilityMask)getSceneVisibilityMaskAddr;

	inline uintptr_t setSceneVisibilityMaskAddr = BasicScanner::CalculateAddress(0x0DF71, OGRE);
	using _SetSceneVisibilityMask = void(__thiscall*)(void*, uint32_t);
	inline _SetSceneVisibilityMask SetSceneVisibilityMask = (_SetSceneVisibilityMask)setSceneVisibilityMaskAddr;

	inline uintptr_t getViewportShadowsEnabledAddr = BasicScanner::CalculateAddress(0x2617A, OGRE);
	using _GetViewportShadowsEnabled = bool(__thiscall*)(void*);
	inline _GetViewportShadowsEnabled GetViewportShadowsEnabled = (_GetViewportShadowsEnabled)getViewportShadowsEnabledAddr;

	inline uintptr_t setViewportShadowsEnabledAddr = BasicScanner::CalculateAddress(0x06735, OGRE);
	using _SetViewportShadowsEnabled = void(__thiscall*)(void*, bool);
	inline _SetViewportShadowsEnabled SetViewportShadowsEnabled = (_SetViewportShadowsEnabled)setViewportShadowsEnabledAddr;

	inline ::Ogre::SceneManager* AsSceneManager(void* sceneManagerPtr)
	{
		return reinterpret_cast<::Ogre::SceneManager*>(sceneManagerPtr);
	}

	inline ::Ogre::Viewport* AsViewport(void* viewportPtr)
	{
		return reinterpret_cast<::Ogre::Viewport*>(viewportPtr);
	}

	inline void* GetSkyBoxNode(void* sceneManagerPtr)
	{
		return AsSceneManager(sceneManagerPtr)->::Ogre::SceneManager::getSkyBoxNode();
	}

	inline void* GetCurrentViewport(void* sceneManagerPtr)
	{
		return AsSceneManager(sceneManagerPtr)->::Ogre::SceneManager::getCurrentViewport();
	}

	inline void* GetViewportCamera(void* viewportPtr)
	{
		return AsViewport(viewportPtr)->::Ogre::Viewport::getCamera();
	}

	inline bool GetSkyBoxGenParameters(void* sceneManagerPtr, SkyBoxGenParameters& outParams)
	{
		const auto& params = AsSceneManager(sceneManagerPtr)->::Ogre::SceneManager::getSkyBoxGenParameters();
		outParams.distance = params.skyBoxDistance;
		return true;
	}

	inline void* GetSkyDomeNode(void* sceneManagerPtr)
	{
		return AsSceneManager(sceneManagerPtr)->::Ogre::SceneManager::getSkyDomeNode();
	}

	inline bool GetSkyDomeGenParameters(void* sceneManagerPtr, SkyDomeGenParameters& outParams)
	{
		const auto& params = AsSceneManager(sceneManagerPtr)->::Ogre::SceneManager::getSkyDomeGenParameters();
		outParams.curvature = params.skyDomeCurvature;
		outParams.tiling = params.skyDomeTiling;
		outParams.distance = params.skyDomeDistance;
		outParams.xsegments = params.skyDomeXSegments;
		outParams.ysegments = params.skyDomeYSegments;
		outParams.ysegments_keep = params.skyDomeYSegments_keep;
		return true;
	}

	inline void* GetSkyPlaneNode(void* sceneManagerPtr)
	{
		return AsSceneManager(sceneManagerPtr)->::Ogre::SceneManager::getSkyPlaneNode();
	}

	inline bool GetSkyPlaneGenParameters(void* sceneManagerPtr, SkyPlaneGenParameters& outParams)
	{
		const auto& params = AsSceneManager(sceneManagerPtr)->::Ogre::SceneManager::getSkyPlaneGenParameters();
		outParams.scale = params.skyPlaneScale;
		outParams.tiling = params.skyPlaneTiling;
		outParams.bow = params.skyPlaneBow;
		outParams.xsegments = params.skyPlaneXSegments;
		outParams.ysegments = params.skyPlaneYSegments;
		return true;
	}
}
