// Reflection.cpp
//------------------------------------------------------------------------------

// Includes
//------------------------------------------------------------------------------
#include "Foundation/Reflection/Reflection.h"


namespace TReflection
{
	// Initialization
	//------------------------------------------------------------------------------
	/*static*/ void Initialization()
	{
		s_MetaTypeDB.RegisterAll();
	}

	// GetMetaTypeDB
	//------------------------------------------------------------------------------
	/*static*/ const MetaTypeDB & GetMetaTypeDB()
	{
		return s_MetaTypeDB;
	}
}

//------------------------------------------------------------------------------