/*
	Copyright 2010-2014 David "Alemarius Nexus" Lerch

	This file is part of gtatools-gui.

	gtatools-gui is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	gtatools-gui is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with gtatools-gui.  If not, see <http://www.gnu.org/licenses/>.

	Additional permissions are granted, which are listed in the file
	GPLADDITIONS.
 */

#ifndef MAPITEMDEFINITION_H_
#define MAPITEMDEFINITION_H_

#include "ItemDefinition.h"
#include "resource/mesh/MeshPointer.h"
#include "resource/texture/TextureSource.h"
#include "resource/collision/CollisionShapePointer.h"
#include "resource/mesh/Submesh.h"
#include "resource/smesh/ShadowMeshPointer.h"
#include "resource/physics/PhysicsPointer.h"
#include "resource/animation/AnimationPackagePointer.h"
#include <gtaformats/ide/IDEStaticObject.h>
#include <gtaformats/ide/IDEAnimation.h>


class MapItemDefinition : public ItemDefinition {
public:
	enum Flags
	{
		AlphaTransparency = (1 << 0)
	};

public:
	MapItemDefinition(MeshPointer* mptr, TextureSource* tsrc, CollisionShapePointer* cptr,
			ShadowMeshPointer* smptr, PhysicsPointer* pptr, float dd, unsigned int flags)
			: meshPtr(mptr), texSrc(tsrc), colPtr(cptr), smeshPtr(smptr), physicsPtr(pptr), drawDist(dd), flags(flags) {}
	MapItemDefinition(const IDEStaticObject* sobj);
	MapItemDefinition(const IDEAnimation* aobj);
	MapItemDefinition() : meshPtr(NULL), texSrc(NULL), colPtr(NULL), smeshPtr(NULL), physicsPtr(NULL), animPtr(NULL),
			drawDist(0.0f), flags(0), timeOn(0), timeOff(0) {}

	MeshPointer* getMeshPointer() { return meshPtr; }
	TextureSource* getTextureSource() { return texSrc; }
	CollisionShapePointer* getCollisionShapePointer() { return colPtr; }
	ShadowMeshPointer* getShadowMeshPointer() { return smeshPtr; }
	PhysicsPointer* getPhysicsPointer() { return physicsPtr; }
	AnimationPackagePointer* getAnimationPackagePointer() { return animPtr; }

	void setMeshPointer(MeshPointer* p) { meshPtr = p; }
	void setTextureSource(TextureSource* s) { texSrc = s; }
	void setCollisionShapePointer(CollisionShapePointer* p) { colPtr = p; }
	void setShadowMeshPointer(ShadowMeshPointer* p) { smeshPtr = p; }
	void setPhysicsPointer(PhysicsPointer* p) { physicsPtr = p; }
	void setAnimationPackagePointer(AnimationPackagePointer* p) { animPtr = p; }

	float getDrawDistance() const { return drawDist; }
	unsigned int getFlags() const { return flags; }
	bool hasAlphaTransparency() const { return (flags & AlphaTransparency) != 0; }

	CString getDefaultAnimation() const { return defaultAnim; }
	void setDefaultAnimation(const CString& anim) { defaultAnim = anim; }

protected:
	MeshPointer* meshPtr;
	TextureSource* texSrc;
	CollisionShapePointer* colPtr;
	ShadowMeshPointer* smeshPtr;
	PhysicsPointer* physicsPtr;
	AnimationPackagePointer* animPtr;
	float drawDist;
	unsigned int flags;
	CString defaultAnim;
	uint8_t timeOn, timeOff;
};

#endif /* MAPITEMDEFINITION_H_ */
