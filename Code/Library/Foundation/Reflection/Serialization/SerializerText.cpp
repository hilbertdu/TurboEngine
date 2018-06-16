// SerializerText.cpp
//------------------------------------------------------------------------------

// Includes
//------------------------------------------------------------------------------
#include "Foundation/FileIO/IOStream.h"
#include "Foundation/Reflection/Reflection.h"
#include "Foundation/Objects/Object.h"
#include "Foundation/Reflection/Serialization/SerializerText.h"


namespace TReflection
{
	// Save
	//------------------------------------------------------------------------------
	/*virtual*/ void TextSerializer::Save(IOStream* stream, const void * object, const IMetaType * objectType, bool terminal)
	{
		SaveItem(stream, object, objectType);
		if (terminal) stream->Write("\000", 1);
	}

	// Load
	//------------------------------------------------------------------------------
	/*virtual*/ void TextSerializer::Load(const IOStream* stream, void * object, const IMetaType * objectType, bool terminal)
	{
		LoadItem(stream, object, objectType);
		if (terminal) stream->Read(nullptr, 1);
	}

	// SaveIndent
	//------------------------------------------------------------------------------
	void TextSerializer::SaveIndent(IOStream* stream)
	{
		for (uint8 idx = 0; idx < m_Indent; ++idx)
		{
			stream->Write("\t", 1);
		}
	}

	// LoadIndent
	//------------------------------------------------------------------------------
	void TextSerializer::LoadIndent(const IOStream* stream)
	{
		for (uint8 idx = 0; idx < m_Indent; ++idx)
		{
			stream->Read(nullptr, 1);
		}
	}

	void TextSerializer::IndentFront()
	{
		++m_Indent;
	}

	void TextSerializer::IndentBack()
	{
		--m_Indent;
	}

	void TextSerializer::SaveBrace(IOStream* stream, const char* brace)
	{
		stream->Write(brace, StringHelper::StrLen(brace));
	}

	void TextSerializer::LoadBrace(const IOStream* stream, const char* brace)
	{
		AStackString<8> str;
		str.SetLength(StringHelper::StrLen(brace));
		stream->Read(str.Get(), StringHelper::StrLen(brace));
		ASSERT(str == brace);
	}

	void TextSerializer::SaveCR(IOStream* stream)
	{
		stream->Write("\n", 1);
	}

	void TextSerializer::LoadCR(const IOStream* stream)
	{
		char cr[1];
		stream->Read(cr, 1);
		ASSERT(cr[0] == '\n');
	}

	// SaveField
	//------------------------------------------------------------------------------
	void TextSerializer::SaveField(IOStream* stream, const void * object, const IMetaType * objectType, bool isPointer, const char * name)
	{
		SaveIndent(stream);
		if (name) stream->Write(name, StringHelper::StrLen(name));
		SaveItem(stream, object, objectType, isPointer);
	}

	// SaveStruct
	//------------------------------------------------------------------------------
	void TextSerializer::SaveStruct(IOStream* stream, const void * object, const MetaStruct * objectType)
	{
		SaveBrace(stream, "[");
		IndentFront();

		bool hasFields = false;
		const MetaStruct* meta = objectType;
		while (meta)
		{
			const Array<Field>& fields = meta->GetFields();
			for (Array<Field>::ConstIter iter = fields.Begin(); iter != fields.End(); ++iter)
			{
				auto attr = (*iter).GetAttribute<AttrSerializer>();
				if (attr && attr->IsEnable())
				{
					if (!hasFields)
					{
						SaveCR(stream);
					}
					AStackString<> name(iter->m_Name.m_Name); name += " = ";
					SaveField(stream, (const char *)object + iter->m_Offset, iter->m_MetaType, iter->m_IsPointer, name.Get());
					hasFields = true;
				}
			}
			meta = meta->m_Super;
		}

		if (hasFields)
		{
			IndentBack();
			SaveIndent(stream);
			SaveBrace(stream, "]");
		}
		else
		{
			IndentBack();
			SaveBrace(stream, "]");
		}
		SaveCR(stream);
	}

	// SaveObject
	//------------------------------------------------------------------------------
	void TextSerializer::SaveObject(IOStream* stream, const void * object, const MetaClass * objectType)
	{
		((IObject*)object)->PreSerialize();
		SaveStruct(stream, object, objectType);
		((IObject*)object)->PostSerialize();
	}

	// SaveContainer
	//------------------------------------------------------------------------------
	void TextSerializer::SaveContainer(IOStream* stream, const void * object, const IMetaContainer * objectType)
	{
		IReadIterator* readIter = objectType->m_ReadIterator->Clone();
		readIter->ResetContainer(object);

		uint64 count = readIter->GetCount();
		AStackString<> countInfo;
		countInfo.Format("<count : %lld> ", count);
		stream->Write(countInfo.Get(), countInfo.GetLength());

		SaveBrace(stream, "{");
		IndentFront();

		if (count > 0)
		{
			SaveCR(stream);
			if (objectType->m_MetaTypeKey)
			{
				while (readIter->IsValid())
				{
					SaveField(stream, readIter->GetKey(), objectType->m_MetaTypeKey, readIter->IsKeyPointer(), "key = ");
					SaveField(stream, readIter->GetValue(), objectType->m_MetaTypeValue, readIter->IsValuePointer(), "value = ");
					readIter->MoveNext();
				}
			}
			else
			{
				while (readIter->IsValid())
				{
					SaveField(stream, readIter->GetValue(), objectType->m_MetaTypeValue, readIter->IsValuePointer());
					readIter->MoveNext();
				}
			}

			IndentBack();
			SaveIndent(stream);
			SaveBrace(stream, "}");
		}
		else
		{
			IndentBack();
			SaveBrace(stream, "}");
		}
		SaveCR(stream);

		TDELETE_SAFE(readIter);
	}

	// SavePointer
	//------------------------------------------------------------------------------
	void TextSerializer::SavePointer(IOStream* stream, const void * object, const IMetaType * objectType)
	{
		// pointer only support IObject class
		ASSERT(objectType->IsObject());

		// TODO: find pointer in object map
		// if found, save object id; if not found, save this object and put it into object map
		SaveBrace(stream, "(");
		IndentFront();

		IObject * objPtr = *(IObject**)object;
		AStackString<> objectId;
		AStackString<> objectName;
		uint64 saveId;

		if (objPtr)
		{
			auto iter = m_Objects.Find((uint64)objPtr);
			saveId = iter == m_Objects.End() ? m_Id++ : (uint64)objPtr;
		}
		else
		{
			saveId = 0;
		}

		objectId.Format(" <object id : %lld, name : %s> ", saveId, objPtr ? objPtr->GetMetaTypeV()->m_Name.m_Name.Get() : "");
		stream->Write(objectId.Get(), objectId.GetLength());

		if (objPtr)
		{
			SaveCR(stream);

			auto iter = m_Objects.Find((UINTPTR)objPtr);
			if (iter == m_Objects.End())
			{
				SaveIndent(stream);
				objPtr->Save(stream, this, false);
				m_Objects[(UINTPTR)objPtr] = objPtr;

				IndentBack();
				SaveIndent(stream);
				SaveBrace(stream, ")");
			}
			else
			{
				IndentBack();
				SaveBrace(stream, ")");
			}
		}
		else
		{
			IndentBack();
			SaveBrace(stream, ")");
		}
		SaveCR(stream);
	}

	// LoadField
	//------------------------------------------------------------------------------
	void TextSerializer::LoadField(const IOStream* stream, void * object, const IMetaType * objectType, bool isPointer, const char * name)
	{
		LoadIndent(stream);
		if (name)
		{
			SIZET len = StringHelper::StrLen(name);
			AStackString<32> loadedName;
			loadedName.SetLength(len);
			stream->Read(loadedName.Get(), len);
			ASSERT(loadedName == name);
		}
		LoadItem(stream, object, objectType, isPointer);
	}

	// LoadStruct
	//------------------------------------------------------------------------------
	void TextSerializer::LoadStruct(const IOStream* stream, void * object, const MetaStruct * objectType)
	{
		LoadBrace(stream, "[");
		IndentFront();

		bool hasFields = false;
		const MetaStruct* meta = objectType;
		while (meta)
		{
			const Array<Field>& fields = meta->GetFields();
			for (Array<Field>::ConstIter iter = fields.Begin(); iter != fields.End(); ++iter)
			{
				auto attr = (*iter).GetAttribute<AttrSerializer>();
				if (attr && attr->IsEnable())
				{
					if (!hasFields)
					{
						LoadCR(stream);
					}
					AStackString<> name(iter->m_Name.m_Name); name += " = ";
					LoadField(stream, (char *)object + iter->m_Offset, iter->m_MetaType, iter->m_IsPointer, name.Get());
					hasFields = true;
				}
			}
			meta = meta->m_Super;
		}

		if (hasFields)
		{
			IndentBack();
			LoadIndent(stream);
			LoadBrace(stream, "]");
		}
		else
		{
			IndentBack();
			LoadBrace(stream, "]");
		}
		LoadCR(stream);
	}

	// LoadObject
	//------------------------------------------------------------------------------
	void TextSerializer::LoadObject(const IOStream* stream, void * object, const MetaClass * objectType)
	{
		((IObject*)object)->PreDeserialize();
		LoadStruct(stream, object, objectType);
		((IObject*)object)->PostDeserialize();
	}

	// LoadContainer
	//------------------------------------------------------------------------------
	void TextSerializer::LoadContainer(const IOStream* stream, void * object, const IMetaContainer * objectType)
	{
		IWriteIterator* writeIter = objectType->m_WriteIterator->Clone();
		writeIter->ResetContainer(object);

		uint64 count;
		AString content;
		stream->Read(nullptr, 9);
		stream->ReadLine(content, '>');
#if defined(__LINUX__)
		sscanf_s(content.Get(), "%lu", &count);
#else
		sscanf_s(content.Get(), "%llu", &count);
#endif
		stream->Read(nullptr, 1);

		LoadBrace(stream, "{");
		IndentFront();

		if (count > 0)
		{
			LoadCR(stream);
			if (IMetaType* keyType = objectType->m_MetaTypeKey)
			{
				ASSERT(!writeIter->IsKeyPointer());

				void* key = keyType->Create();
				for (uint32 i = 0; i < count; i++)
				{
					LoadField(stream, key, keyType, false, "key = ");
					void* valueObject = writeIter->AddEmpty(key);
					LoadField(stream, valueObject, objectType->m_MetaTypeValue, writeIter->IsValuePointer(), "value = ");
				}
				//TDELETE_SAFE(key);		// FIXME: memory leak here. (Done)
				keyType->Destory(key);		// TODO: use stack memory
			}
			else
			{
				for (uint32 i = 0; i < count; i++)
				{
					if (writeIter->IsValuePointer())
					{
						// find in object pool
						void* pointer = nullptr;
						LoadField(stream, &pointer, objectType->m_MetaTypeValue, true);
						writeIter->Add(&pointer);
					}
					else
					{
						// default constructor
						void* value_object = writeIter->AddEmpty();
						LoadField(stream, value_object, objectType->m_MetaTypeValue, false);
					}
				}
			}

			IndentBack();
			LoadIndent(stream);
			LoadBrace(stream, "}");
		}
		else
		{
			IndentBack();
			LoadBrace(stream, "}");
		}
		LoadCR(stream);
		TDELETE_SAFE(writeIter);
	}

	// LoadPointer
	//------------------------------------------------------------------------------
	void TextSerializer::LoadPointer(const IOStream* stream, void * object, const IMetaType * objectType)
	{
		// TODO, pointer only support IObject class
		ASSERT(objectType->IsObject());

		LoadBrace(stream, "(");
		IndentFront();

		uint64 id;
		AString content, name;
		stream->Read(nullptr, 14);
		stream->ReadLine(content, ',');
		stream->Read(nullptr, 8);
		stream->ReadLine(name, '>');
		stream->Read(nullptr, 1);
#if defined(__LINUX__)
		sscanf_s(content.Get(), "%lu", &id);
#else
		sscanf_s(content.Get(), "%llu", &id);
#endif	

		if (id)
		{
			LoadCR(stream);

			auto iter = m_Objects.Find(id);
			if (iter == m_Objects.End())
			{
				IMetaType * metaType = MetaTypeDB::Instance().GetMetaType(name);
				IObject * obj = (IObject*)metaType->Create();
				LoadIndent(stream);
				obj->Load(stream, this);
				*(IObject**)object = obj;
				m_Objects[id] = obj;

				IndentBack();
				LoadIndent(stream);
				LoadBrace(stream, ")");
			}
			else
			{
				*(IObject**)object = iter->Second();
				IndentBack();
				LoadBrace(stream, ")");
			}
		}
		else
		{
			*(IObject**)object = nullptr;
			IndentBack();
			LoadBrace(stream, ")");
		}
		LoadCR(stream);
	}

	// Save
	//------------------------------------------------------------------------------
	void TextSerializer::SaveItem(IOStream* stream, const void * object, const IMetaType * objectType, bool isPointer)
	{
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
	}

	// Load
	//------------------------------------------------------------------------------
	void TextSerializer::LoadItem(const IOStream* stream, void * object, const IMetaType * objectType, bool isPointer)
	{
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
	}
}

//------------------------------------------------------------------------------