// Object.cpp
//------------------------------------------------------------------------------

// Includes
//------------------------------------------------------------------------------
#include "Foundation/Reflected/ReflectionDecl.h"
#include "Foundation/Reflection/Serialization/Serializer.h"
#include "Foundation/Objects/Object.h"


IObject::IObject()
{
}

IObject::~IObject()
{
}

void IObject::Save(IOStream* stream, TReflection::ISerializer * writer, bool terminal) const
{
	writer->Save(stream, this, GetMetaTypeV(), terminal);
}

/*virtual*/ void IObject::Load(const IOStream* stream, TReflection::ISerializer * reader, bool terminal)
{
	reader->Load(stream, this, GetMetaTypeV(), terminal);
}

//------------------------------------------------------------------------------