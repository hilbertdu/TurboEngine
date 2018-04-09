// AsetManager.cpp
//------------------------------------------------------------------------------

// Includes
//------------------------------------------------------------------------------
#include "AssetManager.h"
#include "Foundation/FileIO/FileIO.h"


// Constructor
//------------------------------------------------------------------------------
AssetManager::AssetManager()
	: m_RootItem(nullptr)
{
}

AssetManager::AssetManager(const AStringView & root)
	: m_Root(root.Get())
	, m_RootItem(nullptr)
{
}

AssetManager::~AssetManager()
{
}

void AssetManager::RefreshRoot()
{
	m_RootItem = TNEW(AssetItem);
	FileIO::GetFileInfo(m_Root, m_RootItem->m_FileInfo);
	ASSERT(m_RootItem->IsDirectory());
}

void AssetManager::RefreshItem(AssetItem * item, bool sub, bool recursive)
{
	item->m_Valid = FileIO::GetFileInfo(item->m_FileInfo.m_Name, item->m_FileInfo);
	if (sub)
	{
		RefreshSubItems(item, recursive);
	}
}

void AssetManager::RefreshSubItems(AssetItem * item, bool recursive)
{
	Array<FileIO::FileInfo> fileInfos;
	FileIO::GetFilesEx(item->m_FileInfo.m_Name, "*", false, &fileInfos, true);

	item->m_SubItems.SetSize(fileInfos.GetSize());
	for (uint32 idx = 0; idx < fileInfos.GetSize(); ++idx)
	{
		AssetItem* subItem = GetItem(fileInfos[idx].m_Name);
		subItem->m_FileInfo = fileInfos[idx];
		subItem->m_Level = item->m_Level + 1;
		subItem->m_Parent = item;
		subItem->Reset();

		item->m_SubItems[idx] = subItem;

		if (recursive)
		{
			RefreshSubItems(subItem, recursive);
		}
	}
}

AssetItem * AssetManager::GetItem(const AStringView & path)
{
	AssetMap::Iter iter = m_AssetMap.Find((AStackString<>)path.Get());
	if (iter == m_AssetMap.End())
	{
		AssetItem * item = TNEW(AssetItem);
		m_AssetMap[AStackString<>(path.Get())] = item;
		return item;
	}
	else
	{
		return (*iter).Second();
	}
}

//------------------------------------------------------------------------------