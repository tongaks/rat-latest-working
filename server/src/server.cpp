#include "../headers/server.h"
#include "socket.cpp"

Window::Window(const wxString &Title, const wxPoint &Point, const wxSize &Size) :
	wxFrame(NULL, wxID_ANY, Title, wxDefaultPosition, Size) {
	this->SetBackgroundColour(wxColor(73, 73, 73));
  	wxFont font(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);

	txtFieldPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(500, 400));
	clientGridPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(200, 200));
	buttonsPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(200, 50));

	updatesField = new wxTextCtrl(txtFieldPanel, wxID_ANY, "", wxDefaultPosition, wxSize(200, 100), wxTE_MULTILINE | wxTE_READONLY);
	commandField = new wxTextCtrl(txtFieldPanel, wxID_ANY, "", wxDefaultPosition, wxSize(200, 100), wxTE_MULTILINE);
	updatesField->SetFont(font);
	commandField->SetFont(font);

	startButton = new wxButton(buttonsPanel, wxID_ANY, "Start", wxDefaultPosition, wxDefaultSize);
	startButton->Bind(wxEVT_BUTTON, &Window::StartServer, this);

	closeButton = new wxButton(buttonsPanel, wxID_ANY, "Close", wxDefaultPosition, wxDefaultSize);
	closeButton->Bind(wxEVT_BUTTON, &Window::ButtonClose, this);

	sendButton = new wxButton(buttonsPanel, wxID_ANY, "Send", wxDefaultPosition, wxDefaultSize);
	sendButton->Enable(false);
	sendButton->Bind(wxEVT_BUTTON, &Window::SendCmdHandler, this);

	shutdownButton = new wxButton(buttonsPanel, wxID_ANY, "Shutdown", wxDefaultPosition, wxDefaultSize);
	shutdownButton->Bind(wxEVT_BUTTON, &Window::ShutdownButtonCommand, this);
	shutdownButton->Enable(false);

	restartButton = new wxButton(buttonsPanel, wxID_ANY, "Restart", wxDefaultPosition, wxDefaultSize);
	restartButton->Bind(wxEVT_BUTTON, &Window::ShutdownButtonCommand, this);
	restartButton->Enable(false);

	CreateClientGrid();
	clientTable->AutoSizeColumn(1);
  	int totalColSizer = clientTable->GetColSize(0) + clientTable->GetColSize(1);
  	int gridSize = clientTable->GetSize().GetWidth(); 
  	int remaining = abs(totalColSizer - gridSize);
  	clientTable->SetColSize(1, remaining + totalColSizer);

	wxBoxSizer *txtFieldPanelSizer = new wxBoxSizer(wxVERTICAL);
	txtFieldPanelSizer->Add(updatesField, 2, wxALIGN_BOTTOM | wxALIGN_CENTER_HORIZONTAL | wxEXPAND | wxALL, 10);
	txtFieldPanelSizer->Add(commandField, 1, wxALIGN_CENTER_HORIZONTAL | wxEXPAND | wxALL, 10);
	txtFieldPanel->SetSizer(txtFieldPanelSizer);

	wxBoxSizer *clientGridPanelSizer = new wxBoxSizer(wxHORIZONTAL);
	clientGridPanelSizer->Add(clientTable, 1, wxALIGN_CENTER_HORIZONTAL | wxALL, 10);
	clientGridPanel->SetSizer(clientGridPanelSizer);

	wxBoxSizer *buttonsPanelSizer = new wxBoxSizer(wxHORIZONTAL);
	buttonsPanelSizer->AddStretchSpacer();
	buttonsPanelSizer->Add(startButton, 0, wxALIGN_BOTTOM | wxALL, 10);
	buttonsPanelSizer->Add(closeButton, 0, wxALIGN_BOTTOM | wxALL, 10);
	buttonsPanelSizer->Add(sendButton, 0, wxALIGN_BOTTOM | wxALL, 10);
	buttonsPanelSizer->Add(shutdownButton, 0, wxALIGN_BOTTOM | wxALL, 10);
	buttonsPanelSizer->Add(restartButton, 0, wxALIGN_BOTTOM | wxALL, 10);
	buttonsPanel->SetSizer(buttonsPanelSizer);

	wxBoxSizer *horizontalSizer = new wxBoxSizer(wxHORIZONTAL);
	horizontalSizer->Add(txtFieldPanel, 1, wxEXPAND);
	horizontalSizer->Add(clientGridPanel, 1, wxEXPAND);

	wxBoxSizer *verticalSizer = new wxBoxSizer(wxVERTICAL);
	verticalSizer->Add(horizontalSizer, 1, wxEXPAND);
	verticalSizer->Add(buttonsPanel, 0, wxEXPAND);

	SetSizerAndFit(verticalSizer);

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