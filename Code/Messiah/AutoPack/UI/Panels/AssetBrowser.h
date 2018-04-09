// FileBrowser.h
//------------------------------------------------------------------------------
#pragma once
#ifndef MESSIAH_AUTOPACK_FILEBROWSER_H
#define MESSIAH_AUTOPACK_FILEBROWSER_H

// Includes
//------------------------------------------------------------------------------
#include "Foundation/Platform/Types.h"
#include "Foundation/String/StringView.h"
#include "../Engine/EngineCore.h"
#include "../Manager/AssetManager.h"
#include "../Basic/UIWidget.h"
#include "../Event/EventDecl.h"

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

	TREFLECTION_DECLARE(UIAssetBrowser, UIWidget)
};

UIAssetBrowser::UIAssetBrowser()
	: m_FocusedFolder(EngineCore::Instance().GetAssetManager()->GetRootItem())
	, m_LeftColumeWidth(128)
	, m_Activate(false)
{
	m_Name = "Assets";

	m_SubNames.SetCapacity(_TOTAL_SIZE);
	m_SubNames.Append("Thumbnails");
	m_SubNames.Append(m_Name + "_Filter");
	m_SubNames.Append(m_Name + "_Splitter");
	m_SubNames.Append(m_Name + "_DirCol");
	m_SubNames.Append(m_Name + "_FileCol");

	m_FocusedFolder->m_UnFold = true;
	EngineCore::Instance().GetAssetManager()->RefreshItem(m_FocusedFolder, true);
}

UIAssetBrowser::~UIAssetBrowser()
{
}

void UIAssetBrowser::OnFrameUpdate()
{
	if (!ImGui::BeginDock(m_Name.Get()))
	{
		if (m_Activate) { ImGui::SetDockActive(); }
		ImGui::EndDock();
		return;
	}

	if (m_Activate) { ImGui::SetDockActive(); m_Activate = false; }

	float checkboxW = ImGui::GetCursorPosX();
	ImGui::Checkbox(m_SubNames[Thumbnails].Get(), &m_ShowThumbnails);
	ImGui::SameLine();
	checkboxW = ImGui::GetCursorPosX() - checkboxW;

	char mFilter[128] = {};
	if (ImGui::FilterInput(m_SubNames[Filter].Get(), mFilter, sizeof(mFilter), 128)) {}
	ImGui::SameLine(130 + checkboxW);
	ImGui::NewLine();
	ImGui::Separator();
	ImGui::Selectable(m_FocusedFolder->m_FileInfo.m_Name.Get());
	ImGui::Separator();

	float contentW = ImGui::GetContentRegionAvailWidth();
	ImVec2 mainSize(contentW - m_LeftColumeWidth, 0);
	ImVec2 leftSize(m_LeftColumeWidth, 0);
	if (leftSize.x < 10) leftSize.x = 10;
	if (leftSize.x > contentW - 10) leftSize.x = contentW - 10;
	if (mainSize.x < 10) mainSize.x = 10;
	//if (contentW - leftSize.x - mainSize.x < 60) mainSize.x = contentW - 60 - leftSize.x;

	UpdateDirPanel();

	ImGui::SameLine();
	ImGui::VSplitter(m_SubNames[Splitter].Get(), &leftSize);
	m_LeftColumeWidth = leftSize.x;
	ImGui::SameLine();

	UpdateFilePanel();

	ImGui::EndDock();
}

void UIAssetBrowser::UpdateDirPanel()
{
	ImVec2 size(m_LeftColumeWidth, 0);
	ImGui::BeginChild(m_SubNames[DirCol].Get(), size);
	ImGui::PushItemWidth(120);

	UpdateDirsView(EngineCore::Instance().GetAssetManager()->GetRootItem());

	ImGui::PopItemWidth();
	ImGui::EndChild();
}

void UIAssetBrowser::UpdateFilePanel()
{
	ImGui::BeginChild(m_SubNames[FileCol].Get());

	UpdateFilesView(m_FocusedFolder);

	ImGui::EndChild();
}

void UIAssetBrowser::UpdateDirsView(AssetItem * item)
{
	// show item
	const AStringView & baseName = item->GetBaseName();	
	AStackString<> selectId("##");
	selectId += item->m_FileInfo.m_Name;	

	// be careful not to make to selectable item same label text!
	bool selected = (item == m_FocusedFolder && item->m_UnFold);
	if (ImGui::Selectable(selectId.Get(), &selected))
	{
		LVERBOSE("AutoPack", "Select item: %s, %d, %d\n", item->m_FileInfo.m_Name.Get(), item->m_UnFold, selected);
		item->m_UnFold = selected;
		if (selected)
		{
			EngineCore::Instance().GetAssetManager()->RefreshItem(item, true);
			if (item->m_UnFold)
			{
				m_FocusedFolder = item;
			}
			else
			{
				m_FocusedFolder = item->m_Parent ? item->m_Parent : EngineCore::Instance().GetAssetManager()->GetRootItem();
			}
		}
		m_FocusedFile = nullptr;
	}

	AStackString<> subUIName;
	subUIName.AppendMultiply(" ", item->m_Level);
	subUIName += item->m_UnFold ? "- " : "+ ";
	subUIName += baseName.Get();
	ImGui::SameLine();
	ImGui::Text(subUIName.Get());

	// show sub items
	if (item->m_UnFold)
	{
		for (auto& subAsset : item->m_SubItems)
		{
			const AStringView& baseName = subAsset->GetBaseName();
			if (baseName == "." || baseName == ".." || !subAsset->IsDirectory())
			{
				continue;
			}
			UpdateDirsView(subAsset);
		}
	}
}

void UIAssetBrowser::UpdateFilesView(AssetItem * item)
{
	if (ImGui::Selectable(".."))
	{		
		item->m_UnFold = false;
		m_FocusedFolder = item->m_Parent ? item->m_Parent : EngineCore::Instance().GetAssetManager()->GetRootItem();
		m_FocusedFile = nullptr;
	}
	else
	{
		for (auto& subAsset : item->m_SubItems)
		{
			const AStringView& baseName = subAsset->GetBaseName();
			if (baseName == "." || baseName == ".." || subAsset->IsDirectory())
			{
				continue;
			}

			// is selected
			subAsset->m_UnFold = (m_FocusedFile == subAsset && subAsset->m_UnFold);
			if (ImGui::Selectable(baseName.Get(), &subAsset->m_UnFold))
			{
				m_FocusedFile = subAsset;
				EVENT_DISPATCH(UI, OnAssetFileSelected, subAsset->m_FileInfo.m_Name.Get());
			}
			else if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(1))
			{
			}
		}
	}
}

//------------------------------------------------------------------------------
#endif // MESSIAH_AUTOPACK_FILEBROWSER_H