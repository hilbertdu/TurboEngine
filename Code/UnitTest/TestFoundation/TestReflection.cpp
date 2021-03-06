// TestReflection.cpp
//------------------------------------------------------------------------------

// Includes
//------------------------------------------------------------------------------
#include "TestFramework/UnitTest.h"
#include "Foundation/Pattern/Delegate.h"
#include "Foundation/FileIO/MemWStream.h"
#include "Foundation/FileIO/MemRStream.h"
#include "Foundation/Logging/Logger.h"
#include "Foundation/Container/HashMap.h"
#include "Foundation/Objects/ObjectPool.h"
#include "Foundation/Objects/Object.h"
#include "Foundation/Reflection/Reflection.h"
#include "Foundation/Reflection/ReflectionMacros.h"
#include "Foundation/Reflected/ReflectionDecl.h"
#include "Foundation/Reflection/Serialization/SerializerText.h"


// TestReflection
//------------------------------------------------------------------------------
class TestReflection : public UnitTest
{
private:
	DECLARE_TESTS

	void TestGetMetaType() const;
	void TestGetSet() const;
	void TestInheritence() const;
	void TestSerialization() const;
	void TestClearDB() const;
};

// Register Tests
//------------------------------------------------------------------------------
REGISTER_TESTS_BEGIN(TestReflection)
	REGISTER_TEST(TestGetMetaType)
	REGISTER_TEST(TestGetSet)
	REGISTER_TEST(TestSerialization)
	REGISTER_TEST(TestInheritence)
	REGISTER_TEST(TestClearDB)
REGISTER_TESTS_END

// TestStruct
//------------------------------------------------------------------------------
struct TestStruct : public RefObject<TestStruct>
{
public:
	TestStruct() : m_MyInt(851) {}
	uint32 m_MyInt;

	TREFLECTION_DECLARE(TestStruct, TReflection::IStruct)
};

TREFLECT_BEGIN(TestStruct)
	TREFLECT_FIELD(m_MyInt,	"MyInt")
TREFLECT_END(TestStruct)

// TestObject
//------------------------------------------------------------------------------
class TestObject : public IObject
{
public:
	TestObject()
		: m_Float(0.0f)
		, m_UInt8(0)
		, m_UInt16(0)
		, m_UInt32(0)
		, m_UInt64(0)
		, m_Int8(0)
		, m_Int16(0)
		, m_Int32(0)
		, m_Int64(0)
		, m_Bool(false)
		, m_AString("")
		, m_FloatArray({ 1.0, 2.0, 3.0 })
		, m_StructPtrArray({ nullptr })
	{
	}

	~TestObject()
	{
	}

	void PopulateWithTestData(bool addChildRef = true)
	{
		m_Float = 99.0f;
		m_UInt8 = 42;
		m_UInt16 = 9901;
		m_UInt32 = 1000000;
		m_UInt64 = 100000000000000;
		m_Int8 = 43;
		m_Int16 = 9202;
		m_Int32 = 1000001;
		m_Int64 = 100000000000001;
		m_Bool = true;
		m_AString =  "Test string.";

		if (addChildRef)
		{
			m_TestObjectPtr = (TestObject *)ObjectPool::Instance().CreateObject("TestObject");
			m_TestObjectPtr->PopulateWithTestData(false);
		}

		m_FloatArray.Append(111.0f);
		m_FloatArray.Append(222.0f);
		m_FloatArray.Append(333.0f);
		m_StructArray.SetSize(3);

		m_IntFloatMap[100] = 1000.0;
		m_IntFloatMap[200] = 2000.0;

		if (addChildRef)
		{
			m_StrTestMap["Test"] = (TestObject *)ObjectPool::Instance().CreateObject("TestObject");
			//m_StrTestMap["Test"]->PopulateWithTestData(false);
		}
	}

private: // ensure reflection can set private members

	friend class TestReflection;

	float		m_Float;
	uint8		m_UInt8;
	uint16		m_UInt16;
	uint32		m_UInt32;
	uint64		m_UInt64;
	int8		m_Int8;
	int16		m_Int16;
	int32		m_Int32;
	int64		m_Int64;
	bool		m_Bool;
	AString		m_AString;
	TestStruct	m_TestStruct;	

	Array<float>						m_FloatArray;
	Array<TestStruct>					m_StructArray;
	Array<Ref<TestStruct>>				m_StructPtrArray;
	Ref<TestObject>						m_TestObjectPtr;
	Array<Array<Ref<TestObject>>>		m_TestObjectPtrArray;
	HashMap<int32, float>				m_IntFloatMap;
	HashMap<AString, Ref<TestObject>>	m_StrTestMap;

	TREFLECTION_DECLARE(TestObject, IObject)
};

TREFLECT_BEGIN(TestObject)
	TREFLECT_FIELD(m_Float,					"Float",				ATTRIBUTE(AttrSerializer(true)))
	TREFLECT_FIELD(m_UInt8,					"UInt8",				ATTRIBUTE(AttrSerializer(true)))
	TREFLECT_FIELD(m_UInt16,				"UInt16",				ATTRIBUTE(AttrSerializer(true)))
	TREFLECT_FIELD(m_UInt32,				"UInt32",				ATTRIBUTE(AttrSerializer(true)))
	TREFLECT_FIELD(m_UInt64,				"UInt64",				ATTRIBUTE(AttrSerializer(true)))
	TREFLECT_FIELD(m_Int8,					"Int8",					ATTRIBUTE(AttrSerializer(true)))
	TREFLECT_FIELD(m_Int16,					"Int16",				ATTRIBUTE(AttrSerializer(true)))
	TREFLECT_FIELD(m_Int32,					"Int32",				ATTRIBUTE(AttrSerializer(true)))
	TREFLECT_FIELD(m_Int64,					"Int64",				ATTRIBUTE(AttrSerializer(true)))
	TREFLECT_FIELD(m_Bool,					"Bool",					ATTRIBUTE(AttrSerializer(true)))
	TREFLECT_FIELD(m_AString,				"AString",				ATTRIBUTE(AttrSerializer(true)))
	TREFLECT_FIELD(m_TestStruct,			"TestStruct",			ATTRIBUTE(AttrSerializer(true)))
	TREFLECT_FIELD(m_FloatArray,			"FloatArray",			ATTRIBUTE(AttrSerializer(true)))
	TREFLECT_FIELD(m_StructArray,			"StructArray",			ATTRIBUTE(AttrSerializer(true)))
	TREFLECT_FIELD(m_StructPtrArray,		"StructPtrArray",		ATTRIBUTE(AttrSerializer(false)))
	TREFLECT_FIELD(m_TestObjectPtrArray,	"TestObjectPtrArray",	ATTRIBUTE(AttrSerializer(true)))
	TREFLECT_FIELD(m_TestObjectPtr,			"TestObjectPtr",		ATTRIBUTE(AttrSerializer(true)))
	TREFLECT_FIELD(m_IntFloatMap,			"IntFloatMap",			ATTRIBUTE(AttrSerializer(true)))
	TREFLECT_FIELD(m_StrTestMap,			"StrTestMap",			ATTRIBUTE(AttrSerializer(true)))
TREFLECT_END(TestObject)


// TestGetMetaType
//------------------------------------------------------------------------------
void TestReflection::TestGetMetaType() const
{
	using namespace TReflection;
	Initialization();
	ObjectPool::Initialize();

	const IMetaType* metaType1 = MetaTypeDB::Instance().GetMetaType<bool>();
	const IMetaType* metaType2 = MetaTypeDB::Instance().GetMetaType<Array<int, HeapAllocator>>();
	const IMetaType* metaType3 = MetaTypeDB::Instance().GetMetaType<Array<AString, HeapAllocator>>();
	const IMetaType* metaType4 = MetaTypeDB::Instance().GetMetaType<HashMap<int, float>>();
	const IMetaType* metaType5 = MetaTypeDB::Instance().GetMetaType<TestStruct>();

	MetaType<Array<int>>::MetaValueType a = 100;
	MetaType<Array<AString>>::MetaValueType s = AString("abc");
	MetaType<HashMap<int, float>>::MetaKeyType b1 = 100;
	MetaType<HashMap<int, float>>::MetaValueType b2 = 100;
	MetaType<Array<Array<AString>>>::MetaValueType v1 = Array<AString>();
	MetaType<HashMap<int, Array<AString>>>::MetaValueType v2 = Array<AString>();

	LOUTPUT("MetaType IStruct: %s\n", MetaType<IStruct>::StaticName().m_Name.Get());
	LOUTPUT("MetaType IClass: %s\n", MetaType<IClass>::StaticName().m_Name.Get());
	LOUTPUT("MetaType IObject: %s\n", MetaType<IObject>::StaticName().m_Name.Get());
	LOUTPUT("MetaType TestStruct: %s\n", MetaType<TestStruct>::StaticName().m_Name.Get());
}

// TestGetSet
//------------------------------------------------------------------------------
void TestReflection::TestGetSet() const
{
	TestStruct::BindReflectionInfo();
	TestObject::BindReflectionInfo();

	TestObject o;
	const TReflection::MetaClass* info = TestObject::GetMetaTypeS();

	#define CHECK(name, member, type, value) \
	{															\
		TEST_ASSERT(info->SetProperty(&o, name, (type)value));	\
		TEST_ASSERT(o.member == value);							\
		type v;													\
		TEST_ASSERT(info->GetProperty(&o, name, v));			\
		TEST_ASSERT(v == value);								\
	}

	CHECK("Float", m_Float, float, 3.0f)
	CHECK("UInt8", m_UInt8, uint8, 7)
	CHECK("UInt16", m_UInt16, uint16, 10101)
	CHECK("UInt32", m_UInt32, uint32, 0x12345678)
	CHECK("UInt64", m_UInt64, uint64, 0x8765432112345678)
	CHECK("Int8", m_Int8, int8, 7)
	CHECK("Int16", m_Int16, int16, 10101)
	CHECK("Int32", m_Int32, int32, 0x12345678)
	CHECK("Int64", m_Int64, int64, 0x7765432112345678)
	CHECK("Bool", m_Bool, bool, true)
	CHECK("AString", m_AString, AString, AString("hello"))

	{
		Array<float> arr;
		info->GetProperty(&o, "FloatArray", arr);
		TEST_ASSERT(arr.GetSize() == 3);
		TEST_ASSERT(arr[0] == 1.0f);
	}

	{
		TestObject * obj = (TestObject *)ObjectPool::Instance().CreateObject("TestObject");
		TEST_ASSERT(info->SetProperty(&o, "TestObjectPtr", Ref<TestObject>(obj)));		// obj ref: 2, o->m_TestObjectPtr: 2
		TEST_ASSERT(o.m_TestObjectPtr == obj);
	}

	#undef CHECK
}

// TestSerialization
//------------------------------------------------------------------------------
void TestReflection::TestSerialization() const
{
	TestObject o;
	o.PopulateWithTestData();

	MemWStream wStream;
	TReflection::TextSerializer writer1;
	writer1.Save(&wStream, &o, TestObject::GetMetaTypeS(), true);
	LOUTPUT("Stream: \n%s\n", wStream.GetData());

	MemRStream rStream(wStream.GetData(), wStream.GetSize());
	TestObject obj;
	TReflection::TextSerializer reader;
	reader.Load(&rStream, &obj, TestObject::GetMetaTypeS(), true);

	MemWStream wStream1;
	TReflection::TextSerializer writer2;
	writer2.Save(&wStream1, &obj, TestObject::GetMetaTypeS(), true);

	LOUTPUT("Stream1: \n%s\n", wStream1.GetData());

	bool streamsMatch = (wStream.GetSize() == wStream1.GetSize());
	streamsMatch = streamsMatch && (memcmp(wStream.GetData(), wStream1.GetData(), wStream.GetSize()) == 0);
	TEST_ASSERT(streamsMatch); // Streams don't match
}

// TestInheritence
//------------------------------------------------------------------------------
class BaseClass : public IObject
{
public:
	BaseClass() : m_A(-1) {}
	int m_A;
	int GetA() { return m_A; }
	TREFLECTION_DECLARE(BaseClass, IObject)
};

class DerivedClass : public BaseClass
{
public:
	DerivedClass() : m_B(-1) {}
	int m_B;
	TREFLECTION_DECLARE(DerivedClass, BaseClass)
};

TREFLECT_BEGIN(BaseClass)
	TREFLECT_FIELD(m_A, "a")
	TREFLECT_METHOD(GetA, "GetA")
TREFLECT_END(BaseClass)

TREFLECT_BEGIN(DerivedClass)
	TREFLECT_FIELD(m_B, "b")
TREFLECT_END(DerivedClass)

void TestReflection::TestInheritence() const
{
	BaseClass::BindReflectionInfo();
	DerivedClass::BindReflectionInfo();

	// Create an object with inheritence
	DerivedClass obj;

	// Set property on class
	const TReflection::MetaClass * bi = BaseClass::GetMetaTypeS();
	const TReflection::MetaClass * ri = DerivedClass::GetMetaTypeS();

	TEST_ASSERT(ri->m_Super == bi);

	int bValue = 200;
	TEST_ASSERT(ri->SetProperty(&obj, "b", bValue));
	int bValueGet = 0;
	TEST_ASSERT(ri->GetProperty( &obj, "b", bValueGet));
	TEST_ASSERT(bValue == bValueGet);

	// Set property on base class
	int aValue = 100;
	TEST_ASSERT(ri->SetProperty(&obj, "a", aValue));
	int aValueGet = 0;
	TEST_ASSERT(ri->GetProperty(&obj, "a", aValueGet));
	TEST_ASSERT(aValue == aValueGet);

	// 4. Method
	Delegate<int()> d;
	ri->GetMethod<decltype(d)>("GetA", d);
	int ret = d.Invoke(&obj);
}

// TestClearDB
//------------------------------------------------------------------------------
void TestReflection::TestClearDB() const
{
	TReflection::Finalization();
	ObjectPool::Finalize();
}

//------------------------------------------------------------------------------
