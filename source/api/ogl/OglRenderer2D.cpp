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

/*! \file OglRenderer2D.h
	\author	Danny Angelo Carminati Grein
	\brief 2D Renderer OpenGL implementation
*/

#include "Renderer2D.h"

#if defined(_OGL_)

#include "Log.h"
#include "Screen.h"
#include "Renderer.h"
#include "Sprite.h"
#include "Image.h"

#include <math.h>

#if defined(_SDL_)
#include <SDL/SDL.h>
#endif // _SDL_

#if defined(__APPLE_CC__)
#include <OpenGL/gl.h>
#include <OpenGL/glext.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif

#define TAG "[Renderer2D] "

namespace Seed { namespace OGL {

Renderer2D::Renderer2D()
	: fScreenW(0)
	, fScreenH(0)
	, bInsideDisplayList(FALSE)
	, pCurrentTexture(NULL)
	, iDataCount(0)
	, iPacketCount(0)
	, nLastType(Seed::RendererImmediate)
	, iVertexVboId(0)
	, iTexCoordVboId(0)
	, iDisplayList(0)
	, nVboMeshType(GL_TRIANGLE_STRIP)
{
}

Renderer2D::~Renderer2D()
{
	this->Disable2D();
}

INLINE BOOL Renderer2D::Initialize()
{
	Renderer::Initialize();

	MEMSET(arVertexData, '\0', sizeof(arVertexData));
	MEMSET(arTexCoordData, '\0', sizeof(arTexCoordData));

	if (bHasVBO)
	{
		glGenBuffersARB(1, &iVertexVboId);
		glGenBuffersARB(1, &iTexCoordVboId);
/*
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, iTexCoordVboId);
		glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeof(arTexCoordData), arTexCoordData, GL_STREAM_DRAW_ARB);

		glBindBufferARB(GL_ARRAY_BUFFER_ARB, iVertexVboId);
		glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeof(arVertexData), arVertexData, GL_STREAM_DRAW_ARB);
*/
	}

	iDisplayList = glGenLists(1);
#if !defined(_QT_)
	glFrontFace(GL_CW); // defino os vertices em sentido horario
	glDisable(GL_CULL_FACE); // desabilita back face culling

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glClearStencil(0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	this->Enable2D();
#endif
	return TRUE;
}

INLINE BOOL Renderer2D::Shutdown()
{
	glDeleteLists(iDisplayList, 1);

	if (bHasVBO)
	{
		glDeleteBuffersARB(1, &iVertexVboId);
		glDeleteBuffersARB(1, &iTexCoordVboId);
	}

	this->Disable2D();

	return TRUE;
}

INLINE void Renderer2D::Begin() const
{
	this->Enable2D();

	glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	Renderer::Begin();
	this->Begin(nLastType);
}

INLINE void Renderer2D::End() const
{
	this->CommitData();
	this->End(nLastType);
	Renderer::End();

	this->Disable2D();
}

INLINE void Renderer2D::Enable2D() const
{
#if !defined(_QT_)
	fScreenW = static_cast<f32>(pScreen->GetWidth());
	fScreenH = static_cast<f32>(pScreen->GetHeight());

//	cProjectionMatrix = OrthoMatrix(0.0f, 1.0f, aspectH, 0.0f, -10000.0f, 0.0f);
//	cModelViewMatrix = ScaleMatrix(Vector3f(1.0f, 1.0f, -1.0f));

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	#if defined(SEED_USE_REAL_COORDINATE_SYSTEM)
		glOrtho(0.0f, pScreen->GetWidth(), pScreen->GetHeight(), 0.0f, -1000000, 0);
	#else
		f32 aspectH = fScreenH / fScreenW;
		glOrtho(0.0f, 1.0f, aspectH, 0.0f, -1000000, 0);
	#endif

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glScalef(1.0f, 1.0f, -1.0f);
//	glTranslatef(-0.375f, -0.375f, 0.0f);

	// Save previous Renderer state
	glPushAttrib(GL_DEPTH_BUFFER_BIT);

#if SEED_ENABLE_DEPTH_TEST
	glClearDepth(1.0f);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	//glDepthMask(GL_TRUE);
#else
	glDisable(GL_DEPTH_TEST);
#endif

	glAlphaFunc(GL_GREATER, 0.1f);
	glEnable(GL_ALPHA_TEST);
#endif
}

INLINE void Renderer2D::Disable2D() const
{
#if !defined(_QT_)
	// Restore previous Renderer state
	glPopAttrib();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
#endif
}

INLINE void Renderer2D::DrawRect(f32 x, f32 y, f32 w, f32 h, PIXEL color, BOOL fill) const
{
	GLfloat vertices[8];

	f32 ratio = (fScreenH / fScreenW);
	f32 fAspectHalfWidth = w / 2.0f;
	f32 fAspectHalfHeight = (h * ratio) / 2.0f;

	// A
	vertices[0] = - fAspectHalfWidth;
	vertices[1] = - fAspectHalfHeight;

	// B
	vertices[2] = + fAspectHalfWidth;
	vertices[3] = - fAspectHalfHeight;

	if (!fill)
	{
		// C
		vertices[4] = + fAspectHalfWidth;
		vertices[5] = + fAspectHalfHeight;

		// D
		vertices[6] = - fAspectHalfWidth;
		vertices[7] = + fAspectHalfHeight;
	}
	else
	{
		// C
		vertices[4] = - fAspectHalfWidth;
		vertices[5] = + fAspectHalfHeight;

		// D
		vertices[6] = + fAspectHalfWidth;
		vertices[7] = + fAspectHalfHeight;
	}
	uPixel p;
	p.pixel = color;
	glColor4ub(p.component.r, p.component.g, p.component.b, p.component.a);

	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(2, GL_FLOAT, 0, vertices);
	glEnableClientState(GL_VERTEX_ARRAY);

	f32 nx = fAspectHalfWidth + x;
	f32 ny = fAspectHalfHeight + y * ratio;

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glPushMatrix();
		glLoadIdentity();
		glTranslatef(nx, ny, 0.0f);
		if (fill)
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		else
			glDrawArrays(GL_LINE_LOOP, 0, 4);
	glPopMatrix();
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
}

INLINE void Renderer2D::SetPacketType(eRendererPacketType type)
{
	if (type != Seed::RendererLastType && nLastType != type)
	{
		this->CommitData();

		this->End(nLastType);
		this->Begin(type);

		nLastType = type;
	}
}

INLINE void Renderer2D::SelectTexture(IImage *texture)
{
	if (texture && texture != pCurrentTexture)
	{
		if (nLastType == Seed::RendererVertexBuffer)
			this->CommitData();

		Image *tex = static_cast<Image *>((void *)texture);
		glBindTexture(GL_TEXTURE_2D, tex->LoadTexture());
		pCurrentTexture = texture;
	}
}

INLINE void Renderer2D::End(eRendererPacketType type) const
{
	switch (type)
	{
		case Seed::RendererVertexBuffer:
		{
			if (bHasVBO && bUseVBO)
			{
				glDisableClientState(GL_VERTEX_ARRAY);
				glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			}
			else
			{
				this->End(Seed::RendererDisplayList);
			}
		}
		break;

		case Seed::RendererDisplayList:
		{
			if (bInsideDisplayList)
			{
				bInsideDisplayList = FALSE;
				glPopMatrix();
				glEndList();
				glCallList(iDisplayList);
			}
		}
		break;

		case Seed::RendererDrawArray:
		{
			glDisableClientState(GL_VERTEX_ARRAY);
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		}
		break;

		case Seed::RendererImmediate:
		{
			glPopMatrix();
		}
		break;

		default:
		{
		}
		break;
	}
}

INLINE void Renderer2D::Begin(eRendererPacketType type) const
{
	switch (type)
	{
		case Seed::RendererVertexBuffer:
		{
			if (bHasVBO && bUseVBO)
			{
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);
				glEnableClientState(GL_VERTEX_ARRAY);
			}
			else
			{
				this->Begin(Seed::RendererDisplayList);
			}
		}
		break;

		case Seed::RendererDisplayList:
		{
			if (!bInsideDisplayList)
			{
				bInsideDisplayList = TRUE;
				glNewList(iDisplayList, GL_COMPILE);
				glPushMatrix();
				glLoadIdentity();
			}
		}
		break;

		case Seed::RendererDrawArray:
		{
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glEnableClientState(GL_VERTEX_ARRAY);
		}
		break;

		case Seed::RendererImmediate:
		{
			glPushMatrix();
			glLoadIdentity();
		}
		break;

		default:
		{
		}
		break;
	}
}

INLINE void Renderer2D::UploadData(void *userData)
{
	RendererPacket *packet = static_cast<RendererPacket *>(userData);
	this->SetPacketType(packet->nPacketType);
	this->SelectTexture(packet->pTexture);

	switch (nLastType)
	{
		case Seed::RendererVertexBuffer:
		{
			this->UseVertexBuffer(packet);
		}
		break;

		case Seed::RendererDisplayList:
		{
			this->UseDisplayList(packet);
		}
		break;

		case Seed::RendererDrawArray:
		{
			this->UseDrawArray(packet);
		}
		break;

		case Seed::RendererImmediate:
		{
			this->UseImmediate(packet);
		}
		break;

		default:
		{
			ASSERT_MSG(false, "Invalid packet type.");
		}
		break;
	}
}

INLINE void Renderer2D::UseVertexBuffer(RendererPacket *packet)
{
	if (iPacketCount + 1 >= SEED_RENDERER_PACKET_MAX || iDataCount + packet->iSize >= SEED_RENDERER_DATA_MAX)
	{
		this->CommitData();
	}

	// Position of the first vertex of this mesh
	arPacketIndex[iPacketCount] = iDataCount;
	// Vetex count in this mesh
	arPacketCount[iPacketCount] = packet->iSize;

	void *ptr = &arVertexData[iDataCount];
	MEMCOPY(ptr, packet->pVertexData, packet->iSize * sizeof(Vector3f));

	ptr = &arTexCoordData[iDataCount*2];
	MEMCOPY(ptr, packet->pTexCoordData, packet->iSize * sizeof(f32) * 2);

	nVboMeshType = this->GetOpenGLMeshType(packet->nMeshType);
	iDataCount += packet->iSize;
	iPacketCount++;
}

INLINE void Renderer2D::UseDrawArray(RendererPacket *packet)
{
	glTexCoordPointer(2, GL_FLOAT, 0, packet->pTexCoordData);
	glVertexPointer(3, GL_FLOAT, 0, packet->pVertexData);
	glPushMatrix();
		glLoadIdentity();
		glDrawArrays(this->GetOpenGLMeshType(packet->nMeshType), 0, packet->iSize);
	glPopMatrix();
}

INLINE void Renderer2D::UseDisplayList(RendererPacket *packet)
{
	Vector3f *vertex = static_cast<Vector3f *>(packet->pVertexData);

	struct tc { GLfloat t[2]; };
	tc *texcoord = static_cast<tc *>(packet->pTexCoordData);

	glPushMatrix();
	glLoadIdentity();

	glBegin(this->GetOpenGLMeshType(packet->nMeshType));
	for (u32 i = 0; i < packet->iSize; i++)
	{
		glTexCoord2f(texcoord[i].t[0], texcoord[i].t[1]);
		glVertex3f(vertex[i].x, vertex[i].y, vertex[i].z);
	}
	glEnd();

	glPopMatrix();
}

INLINE void Renderer2D::UseImmediate(RendererPacket *packet)
{
	Vector3f *vertex = static_cast<Vector3f *>(packet->pVertexData);

	struct tc { GLfloat t[2]; };
	tc *texcoord = static_cast<tc *>(packet->pTexCoordData);

	glPushMatrix();
	glLoadIdentity();

	//glPolygonMode(GL_FRONT, GL_LINE);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBegin(this->GetOpenGLMeshType(packet->nMeshType));
	for (u32 i = 0; i < packet->iSize; i++)
	{
		glTexCoord2f(texcoord[i].t[0], texcoord[i].t[1]);
		glVertex3f(vertex[i].x, vertex[i].y, vertex[i].z);
	}
	glEnd();
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();
}

INLINE void Renderer2D::CommitData() const
{
	switch (nLastType)
	{
		case Seed::RendererVertexBuffer:
		{
			if (iDataCount)
			{
/*
				f32 *ptr = NULL;
				glBindBufferARB(GL_ARRAY_BUFFER, iTexCoordVboId);
				ptr = (f32 *)glMapBufferARB(GL_ARRAY_BUFFER_ARB, GL_READ_WRITE_ARB);
				if (ptr)
				{
					MEMCOPY(ptr, arTexCoordData, iDataCount * 2 * sizeof(f32));
					glUnmapBufferARB(GL_ARRAY_BUFFER_ARB);
				}
				glTexCoordPointer(2, GL_FLOAT, 0, 0);

				glBindBufferARB(GL_ARRAY_BUFFER, iVertexVboId);
				ptr = (f32 *)glMapBufferARB(GL_ARRAY_BUFFER_ARB, GL_READ_WRITE_ARB);
				if (ptr)
				{
					MEMCOPY(ptr, arVertexData, iDataCount * sizeof(Vector3f));
					glUnmapBufferARB(GL_ARRAY_BUFFER_ARB);
				}
				glVertexPointer(3, GL_FLOAT, 0, 0);
*/

				glBindBufferARB(GL_ARRAY_BUFFER_ARB, iTexCoordVboId);
				glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeof(arTexCoordData), arTexCoordData, GL_STREAM_DRAW_ARB);
				glTexCoordPointer(2, GL_FLOAT, 0, 0);

				glBindBufferARB(GL_ARRAY_BUFFER_ARB, iVertexVboId);
				glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeof(arVertexData), arVertexData, GL_STREAM_DRAW_ARB);
				glVertexPointer(3, GL_FLOAT, 0, 0);

				glPushMatrix();
					glLoadIdentity();
					//glDrawArrays(GL_TRIANGLE_STRIP, 0, iDataCount);

					for (u32 i = 0; i < iPacketCount; i++)
					{
						glDrawArrays(GL_TRIANGLE_STRIP, arPacketIndex[i], arPacketCount[i]);
					}

					//glMultiDrawArraysEXT(nVboMeshType, arPacketIndex, arPacketCount, iPacketCount);
				glPopMatrix();

				glBindBufferARB(GL_ARRAY_BUFFER, 0);
			}

			MEMSET(arTexCoordData, '\0', iDataCount * sizeof(f32) * 2);
			MEMSET(arVertexData, '\0', iDataCount * sizeof(Vector3f));
			MEMSET(arPacketIndex, '\0', iPacketCount * sizeof(GLint));
			MEMSET(arPacketCount, '\0', iPacketCount * sizeof(GLsizei));

			iDataCount = 0;
			iPacketCount = 0;
		}
		break;

		case Seed::RendererDisplayList:
		{
			glPopMatrix();
			glEndList();
			glCallList(iDisplayList);
			bInsideDisplayList = FALSE;
		}
		break;

		default:
		break;
	}
}

}} // namespace


#endif // _OGL_
