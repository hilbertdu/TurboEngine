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

	/*static*/ void Test()
	{
		const IMetaType* metaType1 = s_MetaTypeDB.GetMetaType<bool>();
		const IMetaType* metaType2 = s_MetaTypeDB.GetMetaType<Array<int, HeapAllocator>>();
		const IMetaType* metaType3 = s_MetaTypeDB.GetMetaType<Array<AString, HeapAllocator>>();
		const IMetaType* metaType4 = s_MetaTypeDB.GetMetaType<HashMap<int, float>>();

		MetaDeduce<Array<int>>::MetaKeyType a = 100;
		MetaDeduce<Array<AString>>::MetaKeyType s = AString("abc");
		MetaDeduce<HashMap<int, float>>::MetaKeyType b1 = 100;
		MetaDeduce<HashMap<int, float>>::MetaValueType b2 = 100;
	}
}

//------------------------------------------------------------------------------