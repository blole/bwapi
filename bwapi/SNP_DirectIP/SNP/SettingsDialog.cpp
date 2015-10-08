#include "SettingsDialog.h"
#include <windows.h>
#include "resource.h"

extern HINSTANCE hInstance;

namespace DRIP
{
  void SettingsDialog::show()
  {
    if (!threadHandle)
      threadHandle = CreateThread(NULL, NULL, dialogThread, this, NULL, NULL);
  }
  void SettingsDialog::hide()
  {
    //possible race condition since the handles are reset from another thread, but unlikely
    if (windowHandle)
      SendMessage(windowHandle, WM_CLOSE, 0, 0);
    if (threadHandle)
      WaitForSingleObject(threadHandle, 10000);
  }


  std::string SettingsDialog::hostIP() const
  {
    if (windowHandle)
      return getItemText(IDC_EDITIP);
    else
      return hostIP_;
  }

  int SettingsDialog::hostPort() const
  {
    if (windowHandle)
      return stoi(getItemText(IDC_EDITPORT));
    else
      return hostPort_;
  }

  int SettingsDialog::localPort() const
  {
    if (windowHandle)
      return stoi(getItemText(IDC_EDITLPORT));
    else
      return localPort_;
  }

  void SettingsDialog::setHostIP(const std::string& ip)
  {
    hostIP_ = ip;
    setItemText(IDC_EDITIP, ip);
  }

  void SettingsDialog::setHostPort(int port)
  {
    hostPort_ = port;
    setItemText(IDC_EDITPORT, std::to_string(port));
  }

  void SettingsDialog::setLocalPort(int port)
  {
    localPort_ = port;
    setItemText(IDC_EDITLPORT, std::to_string(port));
  }

  void SettingsDialog::setStatus(const std::string& status)
  {
    setItemText(IDC_STATUS, status);
  }



  std::string SettingsDialog::getItemText(int id) const
  {
    char buffer[32] = { 0 };
    if (windowHandle)
      GetDlgItemTextA(windowHandle, id, buffer, 32);
    return std::string(buffer);
  }

  void SettingsDialog::setItemText(int id, const std::string& value)
  {
    if (windowHandle)
      SetDlgItemTextA(windowHandle, id, value.c_str());
  }



  DWORD WINAPI SettingsDialog::dialogThread(LPVOID lParam)
  {
    SettingsDialog* settingsDialog = (SettingsDialog*)lParam;
    DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_SETTINGS), NULL, dialogEventHandler, (LONG)settingsDialog);
    settingsDialog->threadHandle = nullptr;
    return 0;
  }

  INT_PTR CALLBACK SettingsDialog::dialogEventHandler(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam)
  {
    static SettingsDialog* settingsDialog = nullptr;

    switch (message)
    {
    case WM_INITDIALOG:
      settingsDialog = (SettingsDialog*)lParam;
      settingsDialog->windowHandle = windowHandle;
      settingsDialog->setHostIP(settingsDialog->hostIP_);
      settingsDialog->setHostPort(settingsDialog->hostPort_);
      settingsDialog->setLocalPort(settingsDialog->localPort_);
      return TRUE;

    case WM_CLOSE:
      settingsDialog->hostIP_    = settingsDialog->hostIP();
      settingsDialog->hostPort_  = settingsDialog->hostPort();
      settingsDialog->localPort_ = settingsDialog->localPort();
      settingsDialog->windowHandle = nullptr;
      settingsDialog = nullptr;
      EndDialog(windowHandle, 0);
      break;
    }

    return FALSE;
  }
}
