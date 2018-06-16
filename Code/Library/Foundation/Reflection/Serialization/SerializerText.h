// SerializerBinary.h
//------------------------------------------------------------------------------
#pragma once
#ifndef FOUNDATION_REFLECTION_SERIALIZERTEXT_H
#define FOUNDATION_REFLECTION_SERIALIZERTEXT_H

// Includes
//------------------------------------------------------------------------------
#include "Foundation/Container/HashMap.h"
#include "Foundation/Reflection/Serialization/Serializer.h"

class IOStream;
class IObject;

namespace TReflection
{
	class IMetaType;
	class MetaStruct;
	class MetaClass;
	class IMetaContainer;

	class TextSerializer : public ISerializer
	{
	public:
		virtual void Save(IOStream* stream, const void * object, const IMetaType * objectType, bool terminal = false);
		virtual void Load(const IOStream* stream, void * object, const IMetaType * objectType, bool terminal = false);

	private:
		void SaveItem(IOStream* stream, const void * object, const IMetaType * objectType, bool isPointer = false);
		void LoadItem(const IOStream* stream, void * object, const IMetaType * objectType, bool isPointer = false);

		void SaveField(IOStream* stream, const void * object, const IMetaType * objectType, bool isPointer = false, const char * name = nullptr);
		void LoadField(const IOStream* stream, void * object, const IMetaType * objectType, bool isPointer = false, const char * name = nullptr);

		void SaveStruct(IOStream* stream, const void * object, const MetaStruct * objectType);
		void SaveObject(IOStream* stream, const void * object, const MetaClass * objectType);
		void SaveContainer(IOStream* stream, const void * object, const IMetaContainer * objectType);
		void SavePointer(IOStream* stream, const void * object, const IMetaType * objectType);

		void LoadStruct(const IOStream* stream, void * object, const MetaStruct * objectType);
		void LoadObject(const IOStream* stream, void * object, const MetaClass * objectType);
		void LoadContainer(const IOStream* stream, void * object, const IMetaContainer * objectType);
		void LoadPointer(const IOStream* stream, void * object, const IMetaType * objectType);

		void SaveIndent(IOStream* stream);
		void LoadIndent(const IOStream* stream);

		void IndentFront();
		void IndentBack();

		void SaveBrace(IOStream* stream, const char* brace);
		void LoadBrace(const IOStream* stream, const char* brace);

		void SaveCR(IOStream* stream);
		void LoadCR(const IOStream* stream);

		uint64						m_Indent = 0;
		uint64						m_Id = 1;
		HashMap<uint64, IObject*>	m_Objects;	// save { addr: obj }, load { id: obj }
	};	
}


//------------------------------------------------------------------------------
#endif // FOUNDATION_REFLECTION_SERIALIZERTEXT_H