// AssetBrowser.cpp
//------------------------------------------------------------------------------

// Includes
//------------------------------------------------------------------------------
#include "AutoPack/UI/Panels/AssetBrowser.h"
#include "AutoPack/Engine/EngineCore.h"
#include "AutoPack/Event/EventDecl.h"
#include "AutoPack/Manager/AssetManager.h"
#include "Foundation/Container/Pair.h"


// Constructor
//------------------------------------------------------------------------------
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

// Destructor
//------------------------------------------------------------------------------
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
	ImGui::Selectable(StringCvt::ConvertToUTF8(m_FocusedFolder->m_FileInfo.m_Name.Get()).Get());
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
	auto& iter = m_CacheNames.Find(item);

	AStringView selectId, showUIName;
	if (iter != m_CacheNames.End())
	{
		selectId = (*iter).Second().First();
		showUIName = (*iter).Second().Second();
	}
	else
	{
		// item key
		const AStringView & baseName = item->GetBaseName();
		AStackString<> tmpSelectId("##");
		tmpSelectId += item->m_FileInfo.m_Name;

		// item show name
		AStackString<> tmpUIName;
		tmpUIName.AppendMultiply(" ", item->m_Level);
		tmpUIName += item->m_UnFold ? "- " : "+ ";
		tmpUIName += baseName.Get();

		m_CacheNames[item] = { tmpSelectId, StringCvt::ConvertToUTF8(tmpUIName) };
		selectId = m_CacheNames[item].First();
		showUIName = m_CacheNames[item].Second();
	}

	// be careful not to make to selectable item same label text!
	bool selected = (item == m_FocusedFolder && item->m_UnFold);
	bool clicked = ImGui::Selectable(selectId.Get(), &selected);
	if (clicked)
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

	ImGui::SameLine();
	if (clicked)
	{
		// update item show name immediately
		AStackString<> tmpUIName;
		tmpUIName.AppendMultiply(" ", item->m_Level);
		tmpUIName += item->m_UnFold ? "- " : "+ ";
		tmpUIName += item->GetBaseName().Get();		
		ImGui::Text(StringCvt::ConvertToUTF8(tmpUIName).Get());
		m_CacheNames.Clear();
	}
	else
	{
		ImGui::Text(showUIName.Get());
	}

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

			AStringView showName;
			auto& iter = m_CacheNames.Find(subAsset);
			if (iter != m_CacheNames.End())
			{
				showName = (*iter).Second().Second();
			}
			else
			{
				m_CacheNames[subAsset] = { "", StringCvt::ConvertToUTF8(baseName.Get()) };
				showName = m_CacheNames[subAsset].Second();
			}

			// is selected
			subAsset->m_UnFold = (m_FocusedFile == subAsset && subAsset->m_UnFold);
			if (ImGui::Selectable(showName.Get(), &subAsset->m_UnFold))
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