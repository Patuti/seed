/*! \file IMutex.cpp
	\author	Danny Angelo Carminati Grein
	\brief Interface to a mutex object
*/


#include "interface/IMutex.h"


namespace Seed {


IMutex::IMutex()
	: bLocked(FALSE)
{
}

IMutex::~IMutex()
{
}

INLINE void IMutex::Lock()
{
	this->bLocked = TRUE;
}

INLINE void IMutex::Unlock()
{
	this->bLocked = FALSE;
}

INLINE BOOL IMutex::TryLock()
{
	return TRUE;
}


} // namespace