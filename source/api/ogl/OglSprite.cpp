/******************************************************************************
 ** Copyright (c) 2010 Seed Developers
 ** All rights reserved
 ** Contact: licensing@seedframework.org
 ** Website: http://www.seedframework.org

 ** This file is part of the Seed Framework.

 ** Commercial Usage
 ** Seed Framework is available under proprietary license for those who cannot,
 ** or choose not to, use LGPL and GPL code in their projects (eg. iPhone,
 ** Nintendo Wii and others).

 ** GNU Lesser General Public License Usage
 ** Alternatively, this file may be used under the terms of the GNU Lesser
 ** General Public License version 2.1 as published by the Free Software
 ** Foundation and appearing in the file LICENSE.LGPL included in the
 ** packaging of this file.  Please review the following information to
 ** ensure the GNU Lesser General Public License version 2.1 requirements
 ** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
 **
 ** In addition, as a special exception, Seed developers gives you certain
 ** additional rights. These rights are described in the Seed Framework LGPL
 ** Exception version 1.1, included in the file LGPL_EXCEPTION.txt in this
 ** package.
 **
 ** If you have questions regarding the use of this file, please contact
 ** Seed developers at licensing@seedframework.org.
 **
 *****************************************************************************/

/*! \file OglSprite.cpp
	\author	Danny Angelo Carminati Grein
	\brief Sprite OpenGL Implementation
*/

#include "Sprite.h"

#if defined(_OGL_)

#include "FileSystem.h"
#include "MemoryManager.h"
#include "Screen.h"
#include "SeedInit.h"
#include "Formats.h"
#include "Log.h"
#include "Image.h"
#include "SpriteObject.h"
#include "Renderer2D.h"
#include "Enum.h"
#include "ViewManager.h"
#include "Viewport.h"

#include <math.h>
#if defined(__APPLE_CC__)
#include <OpenGL/glext.h>
#else
#include <GL/glext.h>
#endif

#define TAG		"[Sprite] "

#if DEBUG_ENABLE_RECT_SPRITE == 1
#define DEBUG_SPRITE_RECT DEBUG_RECT(this->GetX(), this->GetY(), this->GetWidth(), this->GetHeight(), DEBUG_RECT_COLOR_SPRITE);
#else
#define DEBUG_SPRITE_RECT
#endif

namespace Seed { namespace OGL {

Sprite::Sprite()
{
	arCurrentVertexData = &vert[0];
	this->Reset();
}

Sprite::~Sprite()
{
	this->Reset();
}

u32 Sprite::GetWidthInPixel() const
{
	return pFrameImage->GetWidthInPixel();
}

u32 Sprite::GetHeightInPixel() const
{
	return pFrameImage->GetHeightInPixel();
}

void Sprite::Update(f32 delta)
{
	ISprite::Update(delta);

	if (!bChanged && !this->IsChanged())
		return;

	if (!arCustomCoordsData)
	{
		coords[0] = fTexS0;
		coords[1] = fTexT0;
		coords[2] = fTexS1;
		coords[3] = fTexT0;
		coords[4] = fTexS0;
		coords[5] = fTexT1;
		coords[6] = fTexS1;
		coords[7] = fTexT1;
	}

	if (!arCustomVertexData)
	{
		#if defined(SEED_USE_REAL_COORDINATE_SYSTEM)
			arCurrentVertexData = &vert[0];
			vert[0] = Vector3f(-(f32)iHalfWidth, -(f32)iHalfHeight, fPriority);
			vert[1] = Vector3f(+(f32)iHalfWidth, -(f32)iHalfHeight, fPriority);
			vert[2] = Vector3f(-(f32)iHalfWidth, +(f32)iHalfHeight, fPriority);
			vert[3] = Vector3f(+(f32)iHalfWidth, +(f32)iHalfHeight, fPriority);
		#else
			arCurrentVertexData = &vert[0];
			/*vert[0] = Vector3f(-fAspectHalfWidth, -fAspectHalfHeight, fPriority);
			vert[1] = Vector3f(+fAspectHalfWidth, -fAspectHalfHeight, fPriority);
			vert[2] = Vector3f(-fAspectHalfWidth, +fAspectHalfHeight, fPriority);
			vert[3] = Vector3f(+fAspectHalfWidth, +fAspectHalfHeight, fPriority);*/
			vert[0] = Vector3f(-fAspectHalfWidth, -fAspectHalfHeight, fPriority);
			vert[1] = Vector3f(+fAspectHalfWidth, -fAspectHalfHeight, fPriority);
			vert[2] = Vector3f(-fAspectHalfWidth, +fAspectHalfHeight, fPriority);
			vert[3] = Vector3f(+fAspectHalfWidth, +fAspectHalfHeight, fPriority);
		#endif
	}
	else
	{
		arCurrentVertexData = arCustomVertexData;
	}

	f32 x, y;
	#if defined(SEED_USE_REAL_COORDINATE_SYSTEM)
		x = this->iHalfWidth + ISprite::GetX();
		y = this->iHalfHeight + ISprite::GetY();
	#else
		x = this->fAspectHalfWidth + ISprite::GetX();
		y = this->fAspectHalfHeight + ISprite::GetY() * pScreen->GetAspectRatio();
	#endif

	f32 lx = ISprite::GetLocalX();
	f32 ly = ISprite::GetLocalY();

	Matrix4x4f t1, t2, r, s;
	t1 = TranslationMatrix(lx + x, ly + y, 0.0f);
	r = RotationMatrix(AxisZ, DegToRad(ISprite::GetRotation()));
	s = ScaleMatrix(ISprite::GetScaleX(), ISprite::GetScaleY(), 1.0f);
	t2 = TranslationMatrix(-lx, -ly, 0.0f);
	Matrix4x4f transform = ((t1 * r) * s) * t2;

	if (bTransformationChanged || !arCustomVertexData)
	{
		for (u32 i = 0; i < iNumVertices; i++)
		{
			transform.Transform(arCurrentVertexData[i]);
		}
	}

	bChanged = FALSE;
	bTransformationChanged = FALSE;
}

void Sprite::Render()
{
	if (!this->bInitialized)
		return;

	ASSERT(pFrameImage);

	Renderer *rend = static_cast<Renderer *>(pViewManager->GetCurrentRenderer());
	ASSERT(rend);

	rend->SetBlendingOperation(this->eBlendOperation, this->iColor);

	RendererPacket packet;
	packet.iSize = iNumVertices;
	packet.nMeshType = nMeshType;
	packet.pTexCoordData = arCustomCoordsData ? arCustomCoordsData : coords;
	packet.pVertexData = arCurrentVertexData;
	packet.nPacketType = Seed::RendererImmediate;
	//packet.nPacketType = Seed::RendererDrawArray;
	//packet.nPacketType = Seed::RendererDisplayList;
	//packet.nPacketType = Seed::RendererVertexBuffer;
	packet.pTexture = pFrameImage;
	rend->UploadData(&packet);

#if SEED_ENABLE_PRELOAD_TEXTURE == 0
	image->UnloadTexture();
#endif // SEED_ENABLE_PRELOAD_TEXTURE

//	DEBUG_SPRITE_RECT;
}

}} // namespace

#endif // _OGL_
