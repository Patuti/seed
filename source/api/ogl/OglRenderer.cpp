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

/*! \file OglRenderer.cpp
	\author	Danny Angelo Carminati Grein
	\brief Renderer OpenGL implementation
*/

#include "Renderer.h"

#if defined(_OGL_)

#include "RendererDevice.h"
#include "Log.h"
#include "MemoryManager.h"
#include "Screen.h"
#include "Vector3.h"

#if defined(__APPLE_CC__)
#include <OpenGL/gl.h>
#include <OpenGL/glext.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif

#if defined(_SDL_)
#include "platform/sdl/SdlDefines.h"
#endif

#define TAG "[OglRenderer] "

namespace Seed { namespace OGL {

OglRenderer::OglRenderer()
{
	Log(TAG "Initializing...");
	this->Initialize();
	this->Reset();
	Log(TAG "Initialization completed.");
}

OglRenderer::~OglRenderer()
{
	this->Reset();
}

INLINE BOOL OglRenderer::Initialize()
{
	return IRenderer::Initialize();
}

INLINE BOOL OglRenderer::Shutdown()
{
	return IRenderer::Shutdown();
}

INLINE BOOL OglRenderer::Reset()
{
	return IRenderer::Reset();
}

INLINE void OglRenderer::Begin() const
{
	pRendererDevice->Begin();
}

INLINE void OglRenderer::End() const
{
	pRendererDevice->End();
}

INLINE void OglRenderer::Enable2D() const
{
	pRendererDevice->Enable2D();
}

INLINE void OglRenderer::Disable2D() const
{
	pRendererDevice->Disable2D();
}

}} // namespace

#endif // _OGL_
