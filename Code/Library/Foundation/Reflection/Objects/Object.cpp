// Object.cpp
//------------------------------------------------------------------------------

// Includes
//------------------------------------------------------------------------------
#include "Foundation/Reflection/Objects/Object.h"
#include "Foundation/Reflection/Objects/ObjectPool.h"
#include "Foundation/Reflection/Serialization/SerializerText.h"


/*explicit*/ IObject::IObject()
{
}

/*virtual*/ IObject::~IObject()
{
}

/*virtual*/ void IObject::Save(IOStream* stream, ISerializer * writer, bool terminal) const
{
	writer->Save(stream, this, GetMetaTypeV(), terminal);
}

/*virtual*/ void IObject::Load(const IOStream* stream, ISerializer * reader, bool terminal)
{
	reader->Load(stream, this, GetMetaTypeV(), terminal);
}

// Reflection declare
//------------------------------------------------------------------------------
TREFLECT_BEGIN(IObject)
	TREFLECT_FIELD(m_Flag, "Flag")
TREFLECT_END(IObject)

//------------------------------------------------------------------------------