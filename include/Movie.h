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

/*! \file Movie.h
	\author	Rafael Eduardo Gonchor
	\brief A collection of timelines that are part of the movie
*/

#ifndef __MOVIE_H__
#define __MOVIE_H__

#include "Timeline.h"
#include "Array.h"

namespace Seed {

#define MAX_TIMELINES 36

class SEED_CORE_API Movie : public ISceneObject
{
	public:
		Movie();
		virtual ~Movie();

		void AddTimeline(Timeline *timeline);
		void Play();
		void Stop();
		void Rewind();
		void Reset();

		// IRenderable
		virtual void Update(f32 delta);
		virtual void Render();

		// IObject
		virtual const char *GetObjectName() const;
		virtual int GetObjectType() const;

	private:
		SEED_DISABLE_COPY(Movie);

	private:
		BOOL	bPlaying;
		f32		fElapsedTime;
		Array<Timeline *, MAX_TIMELINES> arTimelines;
};

} // namespace

#endif // __MOVIE_H__
