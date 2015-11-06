//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//
// copyright            : (C) 2008 by Eran Ifrah
// file name            : fileexplorer.cpp
//
// -------------------------------------------------------------------------
// A
//              _____           _      _     _ _
//             /  __ \         | |    | |   (_) |
//             | /  \/ ___   __| | ___| |    _| |_ ___
//             | |    / _ \ / _  |/ _ \ |   | | __/ _ )
//             | \__/\ (_) | (_| |  __/ |___| | ||  __/
//              \____/\___/ \__,_|\___\_____/_|\__\___|
//
//                                                  F i l e
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
#include "wx/xrc/xmlres.h"
#include "fileexplorer.h"
#include "wx/sizer.h"
#include "wx/tokenzr.h"
#include "event_notifier.h"

#include "macros.h"
#include "globals.h"
#include "plugin.h"
#include "editor_config.h"
#include "manager.h"
#include "workspace_pane.h"
#include "frame.h"
#include "FileExplorerTab.h"
#include "file_logger.h"
#include "cl_config.h"
#include "OpenFolderDlg.h"
#include "globals.h"
#include <wx/arrstr.h>
#include "clTreeCtrlPanel.h"
#include "clFileOrFolderDropTarget.h"
#include "codelite_events.h"
#include "clWorkspaceView.h"

FileExplorer::FileExplorer(wxWindow* parent, const wxString& caption)
    : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(250, 300))
    , m_caption(caption)
{
    CreateGUIControls();
    m_keyboardHelper.reset(new clTreeKeyboardInput(m_view->GetTreeCtrl()));
    
    m_themeHelper = new ThemeHandlerHelper(this);
    SetDropTarget(new clFileOrFolderDropTarget(this));
    Bind(wxEVT_DND_FOLDER_DROPPED, &FileExplorer::OnFolderDropped, this);
}

FileExplorer::~FileExplorer()
{
    m_keyboardHelper.reset(NULL);
    wxDELETE(m_themeHelper);
    Unbind(wxEVT_DND_FOLDER_DROPPED, &FileExplorer::OnFolderDropped, this);
}

void FileExplorer::CreateGUIControls()
{
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    SetSizer(mainSizer);
    m_view = new clTreeCtrlPanel(this);
    
    // For the file explorer we use the standard configuration tool
    m_view->SetConfig(&clConfig::Get());
    m_view->SetViewName(_("File Explorer"));
    mainSizer->Add(m_view, 1, wxEXPAND | wxALL, 0);
    Layout();
}

void FileExplorer::OnFolderDropped(clCommandEvent& event)
{
    const wxArrayString& folders = event.GetStrings();
    for(size_t i = 0; i < folders.size(); ++i) {
        m_view->AddFolder(folders.Item(i));
    }
    size_t index = clGetManager()->GetWorkspacePaneNotebook()->GetPageIndex(_("Explorer"));
    if(index != wxString::npos) {
        clGetManager()->GetWorkspacePaneNotebook()->ChangeSelection(index);
    }
}

void FileExplorer::OpenFolder(const wxString& path)
{
    m_view->AddFolder(path);
}
