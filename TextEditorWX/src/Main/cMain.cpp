#include "cMain.h"

#include <streambuf>
#include <fstream>
#include <sstream>
#include <string>

std::string ORiellyGetFileName(const std::string& s)
{

	char sep = '/';

#ifdef _WIN32
	sep = '\\';
#endif

	size_t i = s.rfind(sep, s.length());
	if (i != std::string::npos) {
		return(s.substr(i + 1, s.length() - i));
	}

	return("");
}



cMainWindow::cMainWindow() : wxFrame(nullptr, wxID_ANY, "Tabbed notepad")
{
	m_mainMenu = new wxMenuBar();
	this->SetMenuBar(m_mainMenu);

	wxMenu* fileMenu = new wxMenu();
	fileMenu->Append(FileMenuNew, "New");
	fileMenu->AppendSeparator();
	fileMenu->Append(FileMenuOpen, "Open");
	fileMenu->Append(FileMenuSave, "Save");
	fileMenu->AppendSeparator();
	fileMenu->Append(FileMenuQuit, "Quit");
	m_mainMenu->Append(fileMenu, "File");

	fileMenu->Bind(wxEVT_MENU, &cMainWindow::HandleFileMenu, this);
	wxPanel* panel = new wxPanel(this, wxID_ANY);

	m_notebook = new wxAuiNotebook(panel, wxID_ANY);
	m_notebook->Bind(wxEVT_AUINOTEBOOK_PAGE_CHANGED, &cMainWindow::HandleNotebookPageChanged, this);
	m_notebook->Bind(wxEVT_AUINOTEBOOK_PAGE_CLOSED, &cMainWindow::HandleNotebookPageClosed, this);

	wxBoxSizer* panelSizer = new wxBoxSizer(wxHORIZONTAL);
	panelSizer->Add(m_notebook, 1, wxEXPAND);
	panel->SetSizer(panelSizer);

	wxBoxSizer* topSizer = new wxBoxSizer(wxHORIZONTAL);
	topSizer->SetMinSize(250, 200);
	topSizer->Add(panel, 1, wxEXPAND);
	SetSizerAndFit(topSizer);

	AddNewPage();
}

void cMainWindow::AddNewPage()
{
	wxTextCtrl* txtCtrlNew = new wxTextCtrl(m_notebook, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE, wxDefaultValidator, wxTextCtrlNameStr);

	TextFileMapping newMapping;
	newMapping.textControl = txtCtrlNew;
	newMapping.mapped = false;
	newMapping.mappedToFile = "";

	textFileMappings.push_back(newMapping);

	m_notebook->AddPage(txtCtrlNew, "Untitled " + std::to_string(textFileMappings.size()));
}

void cMainWindow::HandleFileMenu(wxCommandEvent& e)
{
	int menuItemClicked = e.GetId();
	switch (menuItemClicked)
	{
		case FileMenuNew:
		{
			AddNewPage();
		} break;
		case FileMenuOpen:
		{
			wxFileDialog openFileDialog(this, _("Open file"), "", "", "", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
			if (openFileDialog.ShowModal() == wxID_CANCEL) return;
			std::string filePath = (std::string)(openFileDialog.GetPath().mb_str());
			std::fstream openedFile(filePath);
			if (!openedFile.good())
			{
				wxLogError("Could not open file");
				return;
			}

			wxTextCtrl* txtCtrlNew = new wxTextCtrl(m_notebook, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE, wxDefaultValidator, wxTextCtrlNameStr);

			TextFileMapping newMapping;
			newMapping.textControl = txtCtrlNew;
			newMapping.mapped = true;
			newMapping.mappedToFile = filePath;

			textFileMappings.push_back(newMapping);

			m_notebook->AddPage(txtCtrlNew, ORiellyGetFileName(filePath));
			std::stringstream fileContents;
			fileContents << openedFile.rdbuf();
			*txtCtrlNew << fileContents.str();
		} break;
		case FileMenuSave:
		{
			if (!currentTextFileMapping->mapped)
			{
				wxFileDialog saveFileDialog(this, _("Save file"), "", "", "", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
				if (saveFileDialog.ShowModal() == wxID_CANCEL) return;

				std::string filePath = (std::string)(saveFileDialog.GetPath().mb_str());
				std::fstream saveingFile(filePath, std::fstream::out);
				if (!saveingFile.good())
				{
					wxLogError("Could not save file");
					return;
				}
				if (currentTextFileMapping == nullptr)
				{
					wxLogError("select a tab to save");
					return;
				}

				saveingFile << currentTextFileMapping->textControl->GetValue();
				currentTextFileMapping->mapped = true;
				currentTextFileMapping->mappedToFile = filePath;
				m_notebook->SetPageText(currentTextFileMapping->uiPageIndex, ORiellyGetFileName(filePath));
			}
			else
			{
				std::fstream savingFile(currentTextFileMapping->mappedToFile);
				if (!savingFile.good())
				{
					wxLogError("Could not save file. try saving it again using the file selection dialog box");
					currentTextFileMapping->mapped = false;
					return;
				}
				savingFile << currentTextFileMapping->textControl->GetValue();
			}
			

		} break;
		case FileMenuQuit:
		{
			Close();
		} break;
		default: 
		{
		} break;
	}
	
}

void cMainWindow::HandleNotebookPageChanged(wxAuiNotebookEvent& e)
{
	currentTextFileMapping = &textFileMappings[e.GetSelection()];
	currentTextFileMapping->uiPageIndex = e.GetSelection();
}

void cMainWindow::HandleNotebookPageClosed(wxAuiNotebookEvent& e)
{
	textFileMappings.erase(textFileMappings.begin() + e.GetSelection());

	for (int i = 0; i < textFileMappings.size(); i++)
	{
		if (!textFileMappings[i].mapped)
		{
			m_notebook->SetPageText(i, "Untitled " + std::to_string(i + 1));
		}
	}
}
