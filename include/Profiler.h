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

#ifndef __PROFILER_H__
#define __PROFILER_H__

#include "Defines.h"
#include <map>
#include <stack>
#include <time.h>

#define SEED_FUNCTION_PROFILER					ProfileContext _ctx_func(__FUNCTION__);
#define SEED_BEGIN_REGION_PROFILER(name, str)	ProfileContext _c##name(str, Profiler::regionProfilerInstance);
#define SEED_END_REGION_PROFILER(name)			_c##name.Terminate();

class ProfileContext;

struct ProfilerEntry
{
	int time;
	int maxtime;
	int calls;

	ProfilerEntry()
		: time(0)
		, maxtime(0)
		, calls(0)
	{}
};

typedef std::map<const char *, ProfilerEntry *> FuncTimeMap;
typedef std::pair<const char *, ProfilerEntry *> FuncTimePair;
typedef FuncTimeMap::iterator FuncTimeMapIt;
typedef std::stack<ProfileContext *> ContextStack;

class Profiler
{
	public:
		Profiler(const char *name);
		~Profiler();

		void AddSlice(const char *func, int time);
		void AddTotal(const char *func, int time);
		void Dump();
		void Reset();

		static Profiler *funcProfilerInstance;
		static Profiler *regionProfilerInstance;

	private:
		const char *pName;
		FuncTimeMap mapSubjectSlice;
		FuncTimeMap mapSubjectTotal;

		Profiler(const Profiler &);
		Profiler &operator=(const Profiler &);
};

class ProfileContext
{
	public:
		ProfileContext(const char *f, Profiler *prof = Profiler::funcProfilerInstance);
		~ProfileContext();

		void Terminate();
		void StopPrevious();
		void RestorePrevious();
		void StopAndCommit();
		void StartOrContinue();
		void Push();
		void Pop();

		static ContextStack stack;

	private:
		const char *func;
		int beg;
		int begTotal;
		bool bTerminated;

		Profiler *pProf;

		ProfileContext(const ProfileContext &);
		ProfileContext &operator=(const ProfileContext &);
};

#endif
