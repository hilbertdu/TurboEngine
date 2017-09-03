// SerializerBinary.h
//------------------------------------------------------------------------------
#pragma once
#ifndef FOUNDATION_REFLECTION_SERIALIZERTEXT_H
#define FOUNDATION_REFLECTION_SERIALIZERTEXT_H

// Includes
//------------------------------------------------------------------------------
#include "Foundation/FileIO/IOStream.h"
#include "Foundation/Reflection/MetaType/Type.h"
#include "Foundation/Reflection/MetaType/TypeStruct.h"
#include "Foundation/Reflection/MetaType/TypeClass.h"
#include "Foundation/Reflection/MetaType/TypeContainer.h"
#include "Foundation/Reflection/Objects/Object.h"
#include "Foundation/Reflection/Serialization/Serializer.h"


namespace TReflection
{
	class TextSerializer
	{
	public:
		void Save(IOStream* stream, const void * object, const IMetaType * objectType);
		void Load(const IOStream* stream, void * object, const IMetaType * objectType);

	private:
		void SaveItem(IOStream* stream, const void * object, const IMetaType * objectType, bool isPointer = false);
		void LoadItem(const IOStream* stream, void * object, const IMetaType * objectType, bool isPointer = false);

		void SaveStruct(IOStream* stream, const void * object, const MetaStruct * objectType);
		void SaveObject(IOStream* stream, const void * object, const MetaClass * objectType);
		void SaveContainer(IOStream* stream, const void * object, const IMetaContainer * objectType);
		void SavePointer(IOStream* stream, const void * object, const IMetaType * objectType);

		void LoadStruct(const IOStream* stream, void * object, const MetaStruct * objectType);
		void LoadObject(const IOStream* stream, void * object, const MetaClass * objectType);
		void LoadContainer(const IOStream* stream, void * object, const IMetaContainer * objectType);
		void LoadPointer(const IOStream* stream, void * object, const IMetaType * objectType);
		
		void SaveHead(IOStream* stream, const void * object, const Name& name, uint32 size, bool isPointer = false);
		void LoadHead(const IOStream* stream, void * object, const Name& name, uint32 &size, bool isPointer = false);

		void SaveIndent(IOStream* stream);
		void LoadIndent(const IOStream* stream);

		uint64 m_Indent{ 0 };
	};

	void TextSerializer::Save(IOStream* stream, const void * object, const IMetaType * objectType)
	{
		SaveItem(stream, object, objectType);
		stream->Write("\000", 1);
	}


	void TextSerializer::Load(const IOStream* stream, void * object, const IMetaType * objectType)
	{
		LoadItem(stream, object, objectType);
		stream->Read(nullptr, 1);
	}

	// SaveHead
	//------------------------------------------------------------------------------
	void TextSerializer::SaveHead(IOStream* stream, const void * object, const Name& name, uint32 size, bool isPointer)
	{
		stream->Write("NAME: ", 6);
		stream->Write(name.m_Name.Get(), name.m_Name.GetLength());
		stream->Write(" ", 1);

		AStackString<> fieldSize;
		fieldSize.Format("%u", size);
		stream->Write("FIELD SIZE: ", 12);
		stream->Write(fieldSize.Get(), fieldSize.GetLength());
		stream->Write(" ", 1);

		AStackString<> pointer;
		pointer.Format("%d", int(isPointer));
		stream->Write("Pointer: ", 9);
		stream->Write(pointer.Get(), pointer.GetLength());
	}

	// LoadHead
	//------------------------------------------------------------------------------
	void TextSerializer::LoadHead(const IOStream* stream, void * object, const Name& name, uint32 &size, bool isPointer)
	{
		char temp[1024];
		char readName[1024]{ 0 };
		int pointer;

		stream->Read(temp, 6);
		stream->Read(readName, name.m_Name.GetLength());
		stream->Read(temp, 1);
		stream->Read(temp, 12);
		STREAM_READ_SCANF(stream, %u, &size);
		stream->Read(temp, 1);
		stream->Read(temp, 9);
		STREAM_READ_SCANF(stream, %d, &pointer);

		ASSERT(name == readName);
		ASSERT(pointer == int(isPointer));
	}

	// SaveIndent
	//------------------------------------------------------------------------------
	void TextSerializer::SaveIndent(IOStream* stream)
	{
		stream->Write("\n", 1);
		for (uint8 idx = 0; idx < m_Indent; ++idx)
		{
			stream->Write("\t", 1);
		}
	}

	// LoadIndent
	//------------------------------------------------------------------------------
	void TextSerializer::LoadIndent(const IOStream* stream)
	{
		char spliter[1];
		stream->Read(spliter, 1);
		for (uint8 idx = 0; idx < m_Indent; ++idx)
		{
			stream->Read(spliter, 1);
		}
	}

	// SaveStruct
	//------------------------------------------------------------------------------
	void TextSerializer::SaveStruct(IOStream* stream, const void * object, const MetaStruct * objectType)
	{
		const Array<Field>& fields = ((MetaStruct*)objectType)->GetFields();
		SaveHead(stream, object, objectType->m_Name, fields.GetSize());
		
		for (Array<Field>::ConstIter iter = fields.Begin(); iter != fields.End(); ++iter)
		{
			SaveItem(stream, (const char *)object + (*iter).m_Offset, (*iter).m_MetaType, (*iter).m_IsPointer);
		}
	}

	// SaveObject
	//------------------------------------------------------------------------------
	void TextSerializer::SaveObject(IOStream* stream, const void * object, const MetaClass * objectType)
	{
		SaveStruct(stream, object, objectType);
	}

	// SaveContainer
	//------------------------------------------------------------------------------
	void TextSerializer::SaveContainer(IOStream* stream, const void * object, const IMetaContainer * objectType)
	{
		IReadIterator* readIter = objectType->m_ReadIterator;
		readIter->ResetContainer(object);
		SaveHead(stream, object, objectType->m_Name, readIter->GetCount());

		if (objectType->m_MetaTypeKey)
		{
			while (readIter->IsValid())
			{
				SaveItem(stream, readIter->GetKey(), objectType->m_MetaTypeKey, readIter->IsKeyPointer());
				SaveItem(stream, readIter->GetValue(), objectType->m_MetaTypeValue, readIter->IsKeyPointer());
				readIter->MoveNext();
			}
		}
		else
		{
			while (readIter->IsValid())
			{
				SaveItem(stream, readIter->GetValue(), objectType->m_MetaTypeValue, readIter->IsValuePointer());
				readIter->MoveNext();
			}
		}
	}

	// SavePointer
	//------------------------------------------------------------------------------
	void TextSerializer::SavePointer(IOStream* stream, const void * object, const IMetaType * objectType)
	{
		// TODO, pointer only support IObject class
		ASSERT(objectType->IsObject());
	}

	// LoadStruct
	//------------------------------------------------------------------------------
	void TextSerializer::LoadStruct(const IOStream* stream, void * object, const MetaStruct * objectType)
	{
		//object = (IStruct*)((MetaStruct*)objectType)->m_Creator.Invoke();
		const Array<Field>& fields = ((MetaStruct*)objectType)->GetFields();
		
		uint32 size;
		LoadHead(stream, object, objectType->m_Name, size);
		ASSERT(size == fields.GetSize());

		for (Array<Field>::ConstIter iter = fields.Begin(); iter != fields.End(); ++iter)
		{
			LoadItem(stream, (char*)object + (*iter).m_Offset, (*iter).m_MetaType, (*iter).m_IsPointer);
		}
	}

	// LoadObject
	//------------------------------------------------------------------------------
	void TextSerializer::LoadObject(const IOStream* stream, void * object, const MetaClass * objectType)
	{
		LoadStruct(stream, object, objectType);
	}

	// LoadContainer
	//------------------------------------------------------------------------------
	void TextSerializer::LoadContainer(const IOStream* stream, void * object, const IMetaContainer * objectType)
	{
		IWriteIterator* writeIter = objectType->m_WriteIterator;
		writeIter->ResetContainer(object);

		//LOUTPUT("Container %s value is pointer: %d\n", objectType->m_Name.m_Name.Get(), writeIter->IsValuePointer());
		
		uint32 count;
		LoadHead(stream, object, objectType->m_Name, count);

		if (IMetaType* keyType = objectType->m_MetaTypeKey)
		{
			void* key = keyType->Create();
			for (uint32 i = 0; i < count; i++)
			{
				LoadItem(stream, key, keyType, writeIter->IsKeyPointer());
				void* valueObject = writeIter->AddEmpty(key);
				LoadItem(stream, valueObject, objectType->m_MetaTypeValue, writeIter->IsValuePointer());
			}
			TDELETE_SAFE(key);
		}
		else
		{
			for (uint32 i = 0; i < count; i++)
			{
				void* value_object = writeIter->AddEmpty();
				LoadItem(stream, value_object, objectType->m_MetaTypeValue, writeIter->IsValuePointer());
			}
		}
	}

	// LoadPointer
	//------------------------------------------------------------------------------
	void TextSerializer::LoadPointer(const IOStream* stream, void * object, const IMetaType * objectType)
	{
		// TODO, pointer only support IObject class
		ASSERT(objectType->IsObject());
	}

	// Save
	//------------------------------------------------------------------------------
	void TextSerializer::SaveItem(IOStream* stream, const void * object, const IMetaType * objectType, bool isPointer)
	{
		SaveIndent(stream);

		++m_Indent;
		if (isPointer)
		{
			SavePointer(stream, object, objectType);
		}
		else if (objectType->IsBaseType())
		{
			SerializerSave saveFunc = objectType->GetSave(E_SERIALIZER_TEXT);
			saveFunc(stream, object);
		}
		else if (objectType->IsObject())
		{
			SaveObject(stream, object, (MetaClass*)objectType);
		}
		else if (objectType->IsStruct())
		{
			SaveStruct(stream, object, (MetaStruct*)objectType);
		}
		else if (objectType->IsContainer())
		{
			SaveContainer(stream, object, (IMetaContainer*)objectType);
		}
		else
		{
			ASSERT(false, "Not a valid meta type!");
		}
		--m_Indent;
	}

	// Load
	//------------------------------------------------------------------------------
	void TextSerializer::LoadItem(const IOStream* stream, void * object, const IMetaType * objectType, bool isPointer)
	{
		LoadIndent(stream);

		++m_Indent;
		if (isPointer)
		{
			LoadPointer(stream, object, objectType);
		}
		else if (objectType->IsBaseType())
		{
			SerializerLoad loadFunc = objectType->GetLoad(E_SERIALIZER_TEXT);
			loadFunc(stream, object);
		}
		else if (objectType->IsObject())
		{
			LoadObject(stream, object, (MetaClass*)objectType);
		}
		else if (objectType->IsStruct())
		{
			LoadStruct(stream, object, (MetaStruct*)objectType);
		}
		else if (objectType->IsContainer())
		{
			LoadContainer(stream, object, (IMetaContainer*)objectType);
		}
		else
		{
			ASSERT(false, "Not a valid meta type!");
		}
		--m_Indent;
	}
}


//------------------------------------------------------------------------------
#endif // FOUNDATION_REFLECTION_SERIALIZERTEXT_H