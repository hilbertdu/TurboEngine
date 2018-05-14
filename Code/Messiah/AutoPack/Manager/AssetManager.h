// AssetManager.h
//------------------------------------------------------------------------------
#pragma once
#ifndef MESSIAH_AUTOPACK_ASSETMANAGER_H
#define MESSIAH_AUTOPACK_ASSETMANAGER_H

// Includes
//------------------------------------------------------------------------------
#include "Foundation/Platform/Types.h"
#include "Foundation/String/String.h"
#include "Foundation/String/StringView.h"
#include "Foundation/FileIO/FileIO.h"
#include "Foundation/FileIO/PathUtils.h"
#include "Foundation/Container/HashMap.h"


// class FileInfo
//------------------------------------------------------------------------------
class AssetItem
{
public:
	AssetItem(): m_UnFold(false), m_Valid(true), m_Level(0), m_Parent(nullptr) {}

	inline void Reset() { m_UnFold = false; m_Valid = true; }	
	inline bool IsDirectory() const { return m_FileInfo.IsDirectory(); }
	inline const AStringView GetBaseName() const { return PathUtils::GetFileBaseName(m_FileInfo.m_Name); }

	FileIO::FileInfo	m_FileInfo;
	Array<AssetItem*>	m_SubItems;
	AssetItem *			m_Parent;
	uint8				m_Level;
	bool				m_UnFold;
	bool				m_Valid;
};


// class AssetManager
//------------------------------------------------------------------------------
class AssetManager
{
public:
	using AssetMap = HashMap<AStackString<>, AssetItem*>;

	AssetManager();
	AssetManager(const AStringView & root);
	~AssetManager();

	FORCE_INLINE void SetRoot(const AStringView & root) { m_Root = root.Get(); }
	FORCE_INLINE const AString & GetRoot() const { return m_Root; }
	FORCE_INLINE AssetItem * GetRootItem() const { return m_RootItem; }
	FORCE_INLINE AssetItem * GetAssetItem(const AStringView & path) { 
		AssetMap::Iter iter = m_AssetMap.Find((AStackString<>)path.Get()); return iter == m_AssetMap.End() ? nullptr : iter->Second(); 
	}

	void RefreshRoot();
	void RefreshItem(AssetItem * item, bool sub = false, bool recursive = false);
	void RefreshSubItems(AssetItem * item, bool recursive);

private:	
	AssetItem * GetItem(const AStringView & path);

	AString		m_Root;
	AssetItem * m_RootItem;
	AssetMap	m_AssetMap;
};


//------------------------------------------------------------------------------
# endif // #define MESSIAH_AUTOPACK_ASSETMANAGER_H