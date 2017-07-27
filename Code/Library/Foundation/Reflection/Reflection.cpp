// Reflection.cpp
//------------------------------------------------------------------------------

// Includes
//------------------------------------------------------------------------------
#include "Foundation/Reflection/Reflection.h"
#include "Foundation/Reflection/MetaType/TypeStruct.h"
#include "Foundation/Reflection/MetaType/TypeClass.h"
#include "Foundation/Reflection/MetaType/TypeDatabase.h"


namespace TReflection
{
	// Initialization
	//------------------------------------------------------------------------------
	void Initialization()
	{
		MetaTypeDB::Initialize();
		MetaTypeDB::Instance().RegisterAll();
	}

	void TestGetMetaType()
	{
		const IMetaType* metaType1 = MetaTypeDB::Instance().GetMetaType<bool>();
		const IMetaType* metaType2 = MetaTypeDB::Instance().GetMetaType<Array<int, HeapAllocator>>();
		const IMetaType* metaType3 = MetaTypeDB::Instance().GetMetaType<Array<AString, HeapAllocator>>();
		const IMetaType* metaType4 = MetaTypeDB::Instance().GetMetaType<HashMap<int, float>>();

		MetaDeduce<Array<int>>::MetaKeyType a = 100;
		MetaDeduce<Array<AString>>::MetaKeyType s = AString("abc");
		MetaDeduce<HashMap<int, float>>::MetaKeyType b1 = 100;
		MetaDeduce<HashMap<int, float>>::MetaValueType b2 = 100;
		MetaDeduce<Array<Array<AString>>>::MetaKeyType v1 = Array<AString>();
		MetaDeduce<HashMap<int, Array<AString>>>::MetaValueType v2 = Array<AString>();
	}

	class TestStruct
	{	
		int32	m_Int32;
		AString m_AString;
		TREFLECTION_STRUCT_DECLARE(TestStruct)
	};

	TREFLECT_STRUCT_BEGIN(TestStruct, IStruct)
		TREFLECT_FIELD(m_Int32,		m_Int32)
		TREFLECT_FIELD(m_AString,	m_AString)
	TREFLECT_STRUCT_END(TestStruct)

	class TestClass
	{
	public:
		TestClass() : m_UInt64(0), m_AStackString("Test") {}
	private:
		uint64				m_UInt64;
		AString				m_AString;
		AStackString<1024>	m_AStackString;
		TREFLECTION_CLASS_DECLARE(TestClass)
	};

	TREFLECT_CLASS_BEGIN(TestClass, IClass)
		TREFLECT_FIELD(m_UInt64, m_UInt64)
		TREFLECT_FIELD(m_AString, m_AString)
	TREFLECT_CLASS_END(TestClass)

	class TestChildClass : public TestClass
	{
	public:
		float GetFloat(int a) { return m_Float; }
	private:
		float		m_Float;
		Array<int>	m_ArrayInt;
		TREFLECTION_CLASS_DECLARE(TestChildClass)
	};

	TREFLECT_CLASS_BEGIN(TestChildClass, TestClass)
		TREFLECT_FIELD(m_Float, m_Float)
		TREFLECT_FIELD(m_ArrayInt, m_ArrayInt)
		TREFLECT_METHOD(GetFloat, GetFloat)
	TREFLECT_CLASS_END(TestChildClass)

	void TestMetaStruct()
	{
		TestStruct::BindReflectionInfo();

		TestStruct testStruct;
		const MetaStruct* metaInfo = TestStruct::GetMetaTypeS();

		// 1. Get/Set (no Set)
		int32 memberInt32;
		AString memberAString;
		Array<int> memberArrayInt;
		metaInfo->GetProperty(&testStruct, "m_Int32", memberInt32);
		metaInfo->GetProperty(&testStruct, "m_AString", memberAString);

		metaInfo->SetProperty(&testStruct, "m_Int32", (int32)100);
		metaInfo->SetProperty(&testStruct, "m_AString", AString("abcdef"));

		metaInfo->GetProperty(&testStruct, "m_ArrayInt", memberArrayInt);

		// 2. Iterate fields
		const Array<Field>& fields = metaInfo->GetFields();
		for (const auto & f : fields)
		{
			LOUTPUT("TestStruct iterate fields: %s", f.m_Name.m_Name.Get());
		}

		// 3. Inherits
		TestChildClass childClass;
		const MetaClass* metaInfo2 = TestChildClass::GetMetaTypeS();

		float membertFloat;
		uint64 membertUInt64;
		metaInfo2->GetProperty(&childClass, "m_Float", membertFloat);
		metaInfo2->GetProperty(&childClass, "m_UInt64", membertUInt64);

		// 4. Method
		float ret = metaInfo2->GetMethod("GetFloat").Invoke<TestChildClass, float>(&childClass, 100);
	}
}

//------------------------------------------------------------------------------