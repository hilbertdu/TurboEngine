// FileBrowser.h
//------------------------------------------------------------------------------
#pragma once
#ifndef MESSIAH_AUTOPACK_FILEBROWSER_H
#define MESSIAH_AUTOPACK_FILEBROWSER_H

// Includes
//------------------------------------------------------------------------------
#include "Foundation/Platform/Types.h"
#include "Foundation/String/StringView.h"
#include "Foundation/String/StringConvert.h"
#include "AutoPack/UI/Basic/UIWidget.h"


// Forward decl
//------------------------------------------------------------------------------
class AssetItem;


// class FileBrowser
//------------------------------------------------------------------------------
class UIAssetBrowser : public UIWidget
{
public:
	enum SubWidget {
		Thumbnails,
		Filter,
		Splitter,
		DirCol,
		FileCol,
		_TOTAL_SIZE
	};

	UIAssetBrowser();
	~UIAssetBrowser();

	virtual void OnFrameUpdate();

private:
	void UpdateDirPanel();
	void UpdateFilePanel();	
	void UpdateDirsView(AssetItem * item);
	void UpdateFilesView(AssetItem * item);
	
	AssetItem *		m_FocusedFolder;
	AssetItem *		m_FocusedFile;
	float			m_LeftColumeWidth;
	bool			m_Activate;
	bool			m_ShowThumbnails;
	Array<AString>	m_SubNames;

	HashMap<AssetItem*, Pair<AString, AString>> m_CacheNames;

	TREFLECTION_DECLARE(UIAssetBrowser, UIWidget)
};


//------------------------------------------------------------------------------
#endif // MESSIAH_AUTOPACK_FILEBROWSER_H