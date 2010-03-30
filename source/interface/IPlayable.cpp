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

/*! \file IPlayable.cpp
	\author	Danny Angelo Carminati Grein
	\brief Defines the Playable interface
*/

#include "interface/IPlayable.h"

namespace Seed {

IPlayable::IPlayable()
	: fFrameRate(30.0f)
	, iTotalFrames(0)
	, nState(PlayableStopped)
	, bFinished(FALSE)
	, bLoop(FALSE)
{
}

IPlayable::~IPlayable()
{
}

INLINE void IPlayable::Play()
{
	nState = PlayablePlaying;
}

INLINE BOOL IPlayable::IsPlaying() const
{
	return (nState == PlayablePlaying);
}

INLINE void IPlayable::Stop()
{
	nState = PlayableStopped;
}

INLINE BOOL IPlayable::IsStopped() const
{
	return (nState == PlayableStopped);
}

INLINE void IPlayable::Pause()
{
	nState = PlayablePaused;
}

INLINE BOOL IPlayable::IsPaused() const
{
	return (nState == PlayablePaused);
}

INLINE BOOL IPlayable::IsSeekable() const
{
	return FALSE;
}

INLINE void IPlayable::Seek(f32 pos)
{
	UNUSED(pos);
}

} // namespace
