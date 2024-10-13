#include "../headers/server.h"
#include <iostream>
#include <fstream>
#include <string>
#include "socket.cpp"
#include "commands.cpp"
#include "send-file.cpp"

/*
panels:
- text field
- client table
- quick commands
- controls

sizers
- text field (vertical)
- client table (vertical)
- quick commands (horizontal)
- controls (horizontal)
- main horizontal
- main vertical (main sizer)
*/

Window::Window(const wxString &Title, const wxPoint &Point, const wxSize &Size) :
	wxFrame(NULL, wxID_ANY, Title, wxDefaultPosition, Size) {
	this->SetBackgroundColour(wxColor(73, 73, 73));
  	wxFont font(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);

	txtFieldPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(500, 400));
	clientGridPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(300, 200));
	buttonsPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(200, 50));
	quickCommandPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(420, 50));

	updatesField = new wxTextCtrl(txtFieldPanel, wxID_ANY, "", wxDefaultPosition, wxSize(200, 100), wxTE_MULTILINE | wxTE_READONLY);
	commandField = new wxTextCtrl(txtFieldPanel, wxID_ANY, "", wxDefaultPosition, wxSize(200, 100), wxTE_MULTILINE);
	updatesField->SetFont(font);
	commandField->SetFont(font);

	wxStaticText *updateLabel = new wxStaticText(txtFieldPanel, wxID_ANY, "Updates field", wxDefaultPosition);
	wxStaticText *commandLabel = new wxStaticText(txtFieldPanel, wxID_ANY, "Command field", wxDefaultPosition);
	wxStaticText *tableLabel = new wxStaticText(clientGridPanel, wxID_ANY, "Client table", wxDefaultPosition);

	startButton = new wxButton(buttonsPanel, wxID_ANY, "Start", wxDefaultPosition, wxDefaultSize);
	startButton->Bind(wxEVT_BUTTON, &Window::StartServer, this);

	closeButton = new wxButton(buttonsPanel, wxID_ANY, "Close", wxDefaultPosition, wxDefaultSize);
	closeButton->Bind(wxEVT_BUTTON, &Window::ButtonClose, this);

	sendButton = new wxButton(buttonsPanel, wxID_ANY, "Send", wxDefaultPosition, wxDefaultSize);
	sendButton->Enable(false);
	sendButton->Bind(wxEVT_BUTTON, &Window::SendCmdHandler, this);

	shutdownButton = new wxButton(quickCommandPanel, wxID_ANY, "Shutdown", wxDefaultPosition, wxDefaultSize);
	shutdownButton->Bind(wxEVT_BUTTON, &Window::ShutdownButtonCommand, this);
	shutdownButton->Enable(false);

	restartButton = new wxButton(quickCommandPanel, wxID_ANY, "Restart", wxDefaultPosition, wxDefaultSize);
	restartButton->Bind(wxEVT_BUTTON, &Window::RestartButtonCommand, this);
	restartButton->Enable(false);

	sendFileButton = new wxButton(buttonsPanel, wxID_ANY, "Send file", wxDefaultPosition, wxDefaultSize);
	sendFileButton->Bind(wxEVT_BUTTON, &Window::SendFileDialog, this);
	sendFileButton->Enable(false);

	lockButton = new wxButton(quickCommandPanel, wxID_ANY, "Lock", wxDefaultPosition, wxDefaultSize);
	lockButton->Bind(wxEVT_BUTTON, &Window::LockButtonCommand, this);
	lockButton->Enable(false);

	closeAppsButton = new wxButton(quickCommandPanel, wxID_ANY, "Close apps", wxDefaultPosition, wxDefaultSize);
	closeAppsButton->Bind(wxEVT_BUTTON, &Window::CloseAppsButtonCommand, this);
	closeAppsButton->Enable(false);

	CreateClientGrid();
	clientTable->AutoSizeColumn(1);
  	int totalColSizer = clientTable->GetColSize(0) + clientTable->GetColSize(1);
  	int gridSize = clientTable->GetSize().GetWidth(); 
  	int remaining = abs(totalColSizer - gridSize);
  	clientTable->SetColSize(1, remaining + totalColSizer);

	wxBoxSizer *txtFieldPanelSizer = new wxBoxSizer(wxVERTICAL);
	txtFieldPanelSizer->Add(updateLabel, 0, wxALIGN_LEFT | wxLEFT | wxTOP, 10);
	txtFieldPanelSizer->Add(updatesField, 2, wxALIGN_CENTER_HORIZONTAL | wxEXPAND | wxALL, 10);
	txtFieldPanelSizer->Add(commandLabel, 0, wxALIGN_LEFT | wxLEFT | wxTOP, 10);
	txtFieldPanelSizer->Add(commandField, 1, wxALIGN_CENTER_HORIZONTAL | wxEXPAND | wxALL, 10);
	txtFieldPanel->SetSizer(txtFieldPanelSizer);

	wxBoxSizer *clientGridPanelSizer = new wxBoxSizer(wxVERTICAL);
	clientGridPanelSizer->Add(tableLabel, 0, wxTOP | wxLEFT, 5);
	clientGridPanelSizer->Add(clientTable, 1, wxEXPAND | wxALL, 10);
	clientGridPanel->SetSizer(clientGridPanelSizer);

	wxBoxSizer *buttonsPanelSizer = new wxBoxSizer(wxHORIZONTAL);
	buttonsPanelSizer->AddStretchSpacer();
	buttonsPanelSizer->Add(sendFileButton, 0, wxALL, 10);
	buttonsPanelSizer->Add(sendButton, 0, wxALL, 10);
	buttonsPanelSizer->Add(startButton, 0, wxALL, 10);
	buttonsPanelSizer->Add(closeButton, 0, wxALL, 10);
	buttonsPanel->SetSizer(buttonsPanelSizer);

	wxBoxSizer *quickCommandSizer = new wxBoxSizer(wxHORIZONTAL);
	quickCommandSizer->Add(shutdownButton, 0, wxALIGN_RIGHT | wxALL, 10);
	quickCommandSizer->Add(restartButton, 0, wxALIGN_RIGHT | wxALL, 10);
	quickCommandSizer->Add(lockButton, 0, wxALIGN_RIGHT | wxALL, 10);
	quickCommandSizer->Add(closeAppsButton, 0, wxALIGN_RIGHT | wxALL, 10);
	quickCommandPanel->SetSizer(quickCommandSizer);

//  main sizers
	
	wxBoxSizer *verticalSizer = new wxBoxSizer(wxVERTICAL);
	verticalSizer->Add(clientGridPanel, 1, wxEXPAND | wxALL, 10);
	verticalSizer->Add(quickCommandPanel, 0, wxEXPAND | wxALL, 10);
	verticalSizer->Add(buttonsPanel, 0, wxEXPAND | wxALL, 10);

	wxBoxSizer *horizontalSizer = new wxBoxSizer(wxHORIZONTAL);
	horizontalSizer->Add(txtFieldPanel, 1, wxEXPAND);
	horizontalSizer->Add(verticalSizer, 0, wxEXPAND);

	wxBoxSizer *mainVerticalSizer = new wxBoxSizer(wxVERTICAL);
	mainVerticalSizer->Add(horizontalSizer, 1, wxEXPAND);

	SetSizerAndFit(mainVerticalSizer);

	Bind(wxEVT_CLOSE_WINDOW, &Window::OnQuit, this);
}


void Window::CreateClientGrid() {
	clientTable = new wxGrid(clientGridPanel, wxID_ANY, wxDefaultPosition, wxSize(200, 200));
	clientTable->CreateGrid(0, 2);
	clientTable->EnableEditing(false);
	clientTable->SetColLabelValue(0, "ID");
	clientTable->SetColLabelValue(1, "Client Address");
	clientTable->HideRowLabels();
	clientTable->Bind(wxEVT_GRID_SELECT_CELL, [this](wxGridEvent &event){
		GridSelectHandler(event, *clientTable);
	});
}