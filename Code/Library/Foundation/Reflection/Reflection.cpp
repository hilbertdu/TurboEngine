// Reflection.cpp
//------------------------------------------------------------------------------

// Includes
//------------------------------------------------------------------------------
#include "Foundation/Reflection/Reflection.h"
#include "Foundation/Reflection/MetaType/TypeEnum.h"
#include "Foundation/Reflection/MetaType/TypeDatabase.h"


namespace TReflection
{
	// Initialization
	//------------------------------------------------------------------------------
	/*static*/ void Initialization()
	{
		MetaTypeDB::Initialize();
		MetaTypeDB::Instance().RegisterAll();
	}

	/*static*/ void TestGetMetaType()
	{
		const IMetaType* metaType1 = MetaTypeDB::Instance().GetMetaType<bool>();
		const IMetaType* metaType2 = MetaTypeDB::Instance().GetMetaType<Array<int, HeapAllocator>>();
		const IMetaType* metaType3 = MetaTypeDB::Instance().GetMetaType<Array<AString, HeapAllocator>>();
		const IMetaType* metaType4 = MetaTypeDB::Instance().GetMetaType<HashMap<int, float>>();

		MetaDeduce<Array<int>>::MetaKeyType a = 100;
		MetaDeduce<Array<AString>>::MetaKeyType s = AString("abc");
		MetaDeduce<HashMap<int, float>>::MetaKeyType b1 = 100;
		MetaDeduce<HashMap<int, float>>::MetaValueType b2 = 100;
	}

	class TestEnumeration : public IEnumeration
	{
	public:
		enum : int32
		{
			ValueOne,
			ValueTwo
		};
		TREFLECTION_ENUM_DECLARE(TestEnumeration)
	};

	TREFLECT_BEGIN(TestEnumeration)
		TREFLECT_FIELD(ValueOne, "Value One")
		TREFLECT_FIELD(ValueTwo, "Value Two")
	TREFLECT_END(TestEnumeration)

	/*static*/ void TestEnumeration()
	{


		// Enumeration is often no need to instantialize.
		const MetaEnum* metaInfo = TestEnumeration::GetMetaTypeS();

		// 1. Get/Set (no Set)
		int32 one, two;
		metaInfo->GetProperty("Value One", &one);
		metaInfo->GetProperty("Value Two", &two);

		// 2. Iterate fields
		const Array<Field>& fields = metaInfo->GetFields();
	}
}

//------------------------------------------------------------------------------