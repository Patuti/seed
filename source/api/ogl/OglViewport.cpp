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

/*! \file OglViewport.cpp
	\author	Everton Fernando Patitucci da Silva
	\brief Viewport OpenGL Implementation
*/


#include "Viewport.h"
#include "Screen.h"

#ifdef _OGL_

#if defined(__APPLE_CC__)
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#define TAG		"[Viewport] "

//#if DEBUG_ENABLE_RECT_VIEWPORT == 1
#define DEBUG_VIEWPORT_RECT DEBUG_RECT(this->GetX(), this->GetY(), this->GetWidth(), this->GetHeight(), DEBUG_RECT_COLOR_SPRITE);
//#else
//#define DEBUG_VIEWPORT_RECT
//#endif


namespace Seed { namespace OGL {


Viewport::Viewport()
{
}


Viewport::~Viewport()
{
}


void Viewport::PrepareViewport()
{
	GLint x, y;
	GLsizei width, height;
	x = static_cast<GLint>(cArea.x * pScreen->GetWidth());
	y = static_cast<GLint>(cArea.y * pScreen->GetHeight());
	width = static_cast<GLsizei>(cArea.width * pScreen->GetWidth());
	height = static_cast<GLsizei>(cArea.height * pScreen->GetHeight());

	//glViewport(x, height - y, width, height);
	glViewport(x, pScreen->GetHeight() - y - height, width, height);
}


}} // namespace


#endif // _OGL_
