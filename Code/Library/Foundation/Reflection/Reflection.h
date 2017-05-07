// Reflection.h
//------------------------------------------------------------------------------
#pragma once
#ifndef FOUNDATION_REFLECTION_H
#define FOUNDATION_REFLECTION_H

// Includes
//------------------------------------------------------------------------------
#include "Foundation/Reflection/MetaType/TypeDecl.h"
#include "Foundation/Container/SmartPtr.h"
#include "Foundation/Pattern/Singleton.h"


namespace TReflection
{
	static void Initialization();
	static const MetaTypeDB & GetMetaTypeDB();
	static MetaTypeDB s_MetaTypeDB;
};


#endif // FOUNDATION_REFLECTION_H