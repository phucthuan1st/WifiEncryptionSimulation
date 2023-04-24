#pragma once
#include <wx/wx.h>
#include <wx/listctrl.h>
#include "SocketServer.h"

class ServerGUI : public wxFrame
{
private:
    SocketServer *m_server;
    wxButton *m_startButton;
    wxButton *m_stopButton;
    wxListView *m_connectionList;

    std::mutex m_mutex;
    wxString ipaddr;
    int port;

    void OnStart(wxCommandEvent &event)
    {
        try
        {
            m_server = new SocketServer(std::string(ipaddr.c_str()), port);
            m_server->start();
        }
        catch (ServerException ex)
        {
            wxLogError(ex.what());
            return;
        }

        if (!m_server->getState())
        {
            wxLogError("Could not open server socket.");
            return;
        }

        m_startButton->Disable();
        m_stopButton->Enable();
    }

    void OnStop(wxCommandEvent &event)
    {

        m_server->stop();

        m_startButton->Enable();
        m_stopButton->Disable();
    }

    void OnClose(wxCommandEvent &event)
    {
        Close();
    }

public:
    ServerGUI(wxString ipaddr, int port) : wxFrame(NULL, wxID_ANY, "Server", wxDefaultPosition, wxSize(800, 800))
    {
        this->ipaddr = ipaddr;
        this->port = port;

        wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
        m_startButton = new wxButton(this, wxID_ANY, "Start Server", wxDefaultPosition, wxDefaultSize);
        m_stopButton = new wxButton(this, wxID_ANY, "Stop Server", wxDefaultPosition, wxDefaultSize);
        m_stopButton->Disable();

        m_startButton->Bind(wxEVT_BUTTON, &ServerGUI::OnStart, this);
        m_stopButton->Bind(wxEVT_BUTTON, &ServerGUI::OnStop, this);

        wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
        buttonSizer->Add(m_startButton, wxSizerFlags().Expand());
        buttonSizer->Add(m_stopButton, wxSizerFlags().Expand());

        m_connectionList = new wxListView(this, wxID_ANY);
        m_connectionList->AppendColumn("IP Address");
        m_connectionList->AppendColumn("Port");

        sizer->Add(buttonSizer, wxSizerFlags().Expand());
        sizer->Add(m_connectionList, wxSizerFlags().Expand());

        SetSizer(sizer);
    };
};