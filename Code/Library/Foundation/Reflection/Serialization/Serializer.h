// Serializer.h
//------------------------------------------------------------------------------
#pragma once
#ifndef FOUNDATION_REFLECTION_SERIALIZER_H
#define FOUNDATION_REFLECTION_SERIALIZER_H

// Includes
//------------------------------------------------------------------------------
#include "Foundation/Reflection/MetaType/Type.h"
#include "Foundation/Reflection/MetaType/TypeStruct.h"
#include "Foundation/Reflection/MetaType/TypeClass.h"
#include "Foundation/Reflection/MetaType/TypeContainer.h"
#include "Foundation/Reflection/Objects/Object.h"
#include "Foundation/Reflection/Serialization/Serializers.h"

namespace TReflection
{
	void SaveStruct(IOStream & stream, const void * object, const MetaStruct * objectType, SerializeType sType)
	{
		if (objectType->IsPointer())
		{
			// TODO
		}

		const Array<Field>& fields = ((MetaStruct*)objectType)->GetFields();
		stream.Write((uint16)fields.GetSize());
		for (Array<Field>::ConstIter iter = fields.Begin(); iter != fields.End(); ++iter)
		{
			Save(stream, (const char *)object + (*iter).m_Offset, (*iter).m_MetaType, sType);
		}
	}

	void SaveObject(IOStream & stream, const void * object, const MetaClass * objectType, SerializeType sType)
	{
		SaveStruct(stream, object, objectType, sType);
	}

	void SaveContainer(IOStream & stream, const void * object, const IMetaContainer * objectType, SerializeType sType)
	{
	}

	void LoadStruct(const IOStream & stream, void * object, const MetaStruct * objectType, SerializeType sType)
	{
		if (objectType->IsPointer())
		{
			// TODO
		}

		object = (IObject*)((MetaClass*)objectType)->m_Creator.Invoke();

		const Array<Field>& fields = ((MetaStruct*)objectType)->GetFields();

		uint16 fieldSize;
		stream.Read(fieldSize);
		ASSERT(fieldSize == fields.GetSize());

		for (Array<Field>::ConstIter iter = fields.Begin(); iter != fields.End(); ++iter)
		{
			Load(stream, (char*)object + (*iter).m_Offset, (*iter).m_MetaType, sType);
		}
	}

	void LoadObject(const IOStream & stream, void * object, const MetaClass * objectType, SerializeType sType)
	{
		LoadStruct(stream, object, objectType, sType);
	}

	void LoadContainer(const IOStream & stream, void * object, const IMetaContainer * objectType, SerializeType sType)
	{
	}

	// Save
	//------------------------------------------------------------------------------
	void Save(IOStream & stream, const void * object, const IMetaType * objectType, SerializeType sType)
	{
		if (objectType->IsBaseType())
		{
			SerializerSave saveFunc = objectType->GetSave(sType);
			saveFunc(stream, object);
		}
		else if (objectType->IsObject())
		{
			SaveObject(stream, object, (MetaClass*)objectType, sType);
		}
		else if (objectType->IsStruct())
		{
			SaveStruct(stream, object, (MetaStruct*)objectType, sType);
		}
		else if (objectType->IsContainer())
		{
			SaveContainer(stream, object, (IMetaContainer*)objectType, sType);
		}
		else
		{
			ASSERT(false, "Not a valid meta type!");
		}
	}

	// Load
	//------------------------------------------------------------------------------
	void Load(const IOStream & stream, void * object, const IMetaType * objectType, SerializeType sType)
	{
		if (objectType->IsBaseType())
		{
			SerializerLoad loadFunc = objectType->GetLoad(sType);
			loadFunc(stream, object);
		}
		else if (objectType->IsObject())
		{
			LoadObject(stream, object, (MetaClass*)objectType, sType);
		}
		else if (objectType->IsStruct())
		{
			LoadStruct(stream, object, (MetaStruct*)objectType, sType);
		}
		else if (objectType->IsContainer())
		{
			LoadContainer(stream, object, (IMetaContainer*)objectType, sType);
		}
		else
		{
			ASSERT(false, "Not a valid meta type!");
		}
	}
}

//------------------------------------------------------------------------------
#endif // FOUNDATION_REFLECTION_SERIALIZER_H

