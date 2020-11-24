#pragma once

#include <wx/wx.h>
#include <wx/aui/auibook.h>

#include <map>

#define FileMenuNew   2001
#define FileMenuOpen  2002
#define FileMenuSave  2003
#define FileMenuQuit  2004

struct TextFileMapping
{
	wxTextCtrl* textControl;
	bool mapped;
	std::string mappedToFile;
	int uiPageIndex;
};

class cMainWindow : public wxFrame
{
public:
	cMainWindow();

private:
	std::vector<TextFileMapping> textFileMappings;
	TextFileMapping* currentTextFileMapping = nullptr;

	wxMenuBar* m_mainMenu = nullptr;
	wxAuiNotebook* m_notebook = nullptr;

	void AddNewPage();

	void HandleFileMenu(wxCommandEvent& e);
	void HandleNotebookPageChanged(wxAuiNotebookEvent& e);
	void HandleNotebookPageClosed(wxAuiNotebookEvent& e);
};
