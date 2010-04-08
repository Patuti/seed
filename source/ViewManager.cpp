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

/*! \file ViewManager.cpp
	\author	Danny Angelo Carminati Grein
	\brief Renderer Manager
*/

#include "ViewManager.h"
#include "Defines.h"
#include "Log.h"
#include "Enum.h"
#include "interface/IViewport.h"
#include "interface/IRenderer.h"

#define TAG		"[ViewManager] "

namespace Seed {

ViewManager ViewManager::instance;

ViewManager::ViewManager()
	: arViewport()
	, pCurrentViewport(NULL)
	, bEnabled(TRUE)
{
	arViewport.Truncate();
}

ViewManager::~ViewManager()
{
	this->Reset();
}

INLINE BOOL ViewManager::Initialize()
{
	IModule::Initialize();
	
	for (u32 i = 0; i < arViewport.Size(); i++)
	{
		arViewport[i]->GetRenderer()->Initialize();
	}

	return TRUE;
}

INLINE BOOL ViewManager::Reset()
{
	arViewport.Truncate();
	return IModule::Reset();
}

INLINE BOOL ViewManager::Shutdown()
{
	for (u32 i = 0; i < arViewport.Size(); i++)
	{
		arViewport[i]->GetRenderer()->Shutdown();
	}

	this->Reset();

	return IModule::Shutdown();
}

void ViewManager::Add(IViewport *view)
{
	ASSERT_NULL(view);

	BOOL found = FALSE;
	for (u32 i = 0; i < arViewport.Size(); i++)
	{
		if (arViewport[i] == view)
		{
			found = TRUE;
			break;
		}
	}

	if (!found)
	{
		arViewport.Add();
		arViewport[arViewport.Size() - 1] = view;
	}
}

void ViewManager::Remove(IViewport *view)
{
	ASSERT_NULL(view);
	arViewport.Remove(view);
}

INLINE void ViewManager::Disable()
{
	bEnabled = FALSE;
}

INLINE void ViewManager::Enable()
{
	bEnabled = TRUE;
}

INLINE void ViewManager::Render()
{
	if (bEnabled)
	{
		u32 len = arViewport.Size();

		for (u32 i = 0; i < len; i++)
		{
			pCurrentViewport = arViewport[i];
			pCurrentViewport->Render();
		}
	}

	pCurrentViewport = NULL;
}

INLINE IRenderer *ViewManager::GetCurrentRenderer() const
{
	ASSERT_MSG(pCurrentViewport, TAG "GetCurrentRenderer must be called within Render call.");

	return pCurrentViewport->GetRenderer();
}

INLINE const char *ViewManager::GetObjectName() const
{
	return "ViewManager";
}

INLINE int ViewManager::GetObjectType() const
{
	return Seed::ObjectViewManager;
}

} // namespace
