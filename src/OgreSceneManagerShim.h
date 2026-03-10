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

namespace Ogre
{
	using Real = float;

	class SceneNode;
	class Camera;
	class Viewport;

	// Minimal imported OGRE declarations based on OGRECave/ogre v1.10.0
	// (tag v1.10.0, commit f1f1937fd6cbad05a4b9170b9882da91f42f53a5).
	class SceneManager
	{
	public:
		struct SkyDomeGenParameters
		{
			Real skyDomeCurvature;
			Real skyDomeTiling;
			Real skyDomeDistance;
			int skyDomeXSegments;
			int skyDomeYSegments;
			int skyDomeYSegments_keep;
		};

		struct SkyPlaneGenParameters
		{
			Real skyPlaneScale;
			Real skyPlaneTiling;
			Real skyPlaneBow;
			int skyPlaneXSegments;
			int skyPlaneYSegments;
		};

		struct SkyBoxGenParameters
		{
			Real skyBoxDistance;
		};

		__declspec(dllimport) virtual SceneNode* getSkyPlaneNode(void) const;
		__declspec(dllimport) virtual const SkyPlaneGenParameters& getSkyPlaneGenParameters(void) const;
		__declspec(dllimport) virtual SceneNode* getSkyBoxNode(void) const;
		__declspec(dllimport) virtual const SkyBoxGenParameters& getSkyBoxGenParameters(void) const;
		__declspec(dllimport) virtual SceneNode* getSkyDomeNode(void) const;
		__declspec(dllimport) virtual const SkyDomeGenParameters& getSkyDomeGenParameters(void) const;
		__declspec(dllimport) Viewport* getCurrentViewport(void) const;
	};

	class Viewport
	{
	public:
		__declspec(dllimport) Camera* getCamera(void) const;
	};
}
