// Arribute.h
//------------------------------------------------------------------------------
#pragma once
#ifndef FOUNDATION_REFLECTION_ATTRIBUTE_H
#define FOUNDATION_REFLECTION_ATTRIBUTE_H

// Includes
//------------------------------------------------------------------------------

namespace TReflection
{
	enum MetaAttrFlag : uint16
	{
		E_ATTR_SERIALIZER,
		E_ATTR_RANGER,

		E_ATTR_COUNT
	};

	class IAttribute
	{
	public:
		static constexpr uint16 AttrType = -1;

		explicit IAttribute() {}
		virtual ~IAttribute() {}
	};
}

//------------------------------------------------------------------------------
#endif // FOUNDATION_REFLECTION_ATTRIBUTE_H