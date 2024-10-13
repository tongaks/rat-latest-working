#ifndef WINDOW_H
#define WINDOW_H

#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#include <thread>
#include <mutex>
#include <vector>
#include <wx/wx.h>
#include <wx/grid.h>
#include <cstdlib>
#include <unordered_map>
#include <algorithm>

class Window : public wxFrame {
private:
	wxTextCtrl *updatesField;
	wxTextCtrl *commandField;
	wxTextCtrl *selectedClientField;
	wxStaticText *currentClient;

	wxPanel *txtFieldPanel;
	wxPanel *clientGridPanel;
	wxPanel *buttonsPanel;
	wxPanel *quickCommandPanel;

	wxGrid *clientTable;

	wxButton *sendButton;
	wxButton *startButton;
	wxButton *closeButton;
	
	wxButton *shutdownButton;
	wxButton *restartButton;
	wxButton *lockButton;
	wxButton *closeAppsButton;

	int clientCount = 0;
	int previousCell = 0;
	int selectedClient;
	int currentSock = 0;
	int serverSock = 0;
	std::unordered_map<int, std::string> clientsMap;
	std::vector<int> clientSocks;
	std::mutex clientMutex;
	std::mutex clientCountMutex;
	struct sockaddr_in serverAddr;

public:
	Window(const wxString &Title, const wxPoint &Point, const wxSize &Size);

	void ButtonClose(wxCommandEvent &event);
	void OnQuit(wxCloseEvent& event);
	bool SendCmd(wxString cmd);
	void SendCmdHandler(wxCommandEvent &event);
	void WarningText(std::string msg);

	// Button command
	void ShutdownButtonCommand(wxCommandEvent &event);
	void RestartButtonCommand(wxCommandEvent &event);
	void LockButtonCommand(wxCommandEvent &event);
	void CloseAppsButtonCommand(wxCommandEvent &event);

	// Grid handler
	void CreateClientGrid();
	void GridSelectHandler(wxGridEvent& event, wxGrid &grid);

	// Socket handler
	void StartServer(wxCommandEvent &event);
	void CloseSocket();
	void HandleConnection();
	void HandleClient(int sock, std::string addr, int clientID);
	void AddClient(std::string addr, int id);
	void CheckClientCount();
	std::string GetClientAddr(int sock);
};

#endif