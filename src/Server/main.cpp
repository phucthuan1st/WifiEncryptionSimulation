#include "ServerGUI.h"
#include <wx/textdlg.h>
#include <wx/numdlg.h>

class ServerApp : public wxApp
{
public:
    virtual bool OnInit()
    {
        wxString ipaddr = wxGetTextFromUser("Enter IP address: ", "IP address", "127.0.0.1");
        int port = wxGetNumberFromUser("Enter port number: ", "", "Port number", 12345, 80, 32655);

        ServerGUI *serverFrame = new ServerGUI(ipaddr.c_str(), port);
        serverFrame->Show(true);
        SetTopWindow(serverFrame);

        return true;
    }
};

wxIMPLEMENT_APP(ServerApp);
