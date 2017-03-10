// Reflection.cpp
//------------------------------------------------------------------------------

// Includes
//------------------------------------------------------------------------------
#include "Foundation/Reflection/Reflection.h"


// Initialization
//------------------------------------------------------------------------------
/*static*/ void Reflection::Initialization()
{
	s_MetaTypeDB.RegisterAll();
}

// GetMetaTypeDB
//------------------------------------------------------------------------------
/*static*/ const MetaTypeDB & Reflection::GetMetaTypeDB()
{
	return s_MetaTypeDB;
}

//------------------------------------------------------------------------------