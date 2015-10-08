#pragma once
#include <string>
#include <windows.h>

namespace DRIP
{
  class SettingsDialog
  {
    std::string hostIP_;
    int hostPort_ = 0;
    int localPort_ = 0;
    HANDLE threadHandle = nullptr;
    HWND windowHandle = nullptr;
    
  public:
    void show();
    void hide();
    
    std::string hostIP() const;
    int hostPort() const;
    int localPort() const;
    void setHostIP(const std::string& ip);
    void setHostPort(int port);
    void setLocalPort(int port);
    void setStatus(const std::string& status);
    
  private:
    std::string getItemText(int id) const;
    void setItemText(int id, const std::string& value);
    
    static DWORD WINAPI dialogThread(LPVOID lParam);
    static INT_PTR CALLBACK dialogEventHandler(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
  };
}
