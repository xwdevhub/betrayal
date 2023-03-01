#pragma once

#include <windows.h>

#include <shellapi.h>
#include <strsafe.h>

#include <string>
#include <codecvt>
#include <map>
#include <vector>
#include "WinProc.cpp"
constexpr wchar_t kTrayWindowClassName[] = L"FLUTTER_RUNNER_WIN32_WINDOWS_TRAYU";
class TrayIcon
{
private:
  bool m_is_visible;
  bool m_icon_is_shared;
  bool tray_class_registered_ = false;;
  std::vector<UINT> m_subbed_events;
  HWND tray_window_;

public:
  NOTIFYICONDATA data;

  TrayIcon(HWND hWnd, UINT id)
  {
    // https://docs.microsoft.com/en-us/windows/win32/api/shellapi/ns-shellapi-notifyicondataw
    data.cbSize = sizeof(data);
    data.hWnd = hWnd;
    data.uID = id;
    data.uCallbackMessage = WM_USER + id;
    data.uFlags = NIF_MESSAGE;
    data.uVersion = NOTIFYICON_VERSION_4;
    //version let menu closed by blank click
    HWND window =
        CreateWindow(GetTrayWindowClass(), nullptr, WS_OVERLAPPEDWINDOW, -1, -1,
            0, 0, nullptr, nullptr, GetModuleHandle(nullptr), this);

    tray_window_ = window;
  };
  const wchar_t* GetTrayWindowClass() {
      if (!tray_class_registered_) {
          WNDCLASS window_class{};
          window_class.hCursor = nullptr;
          window_class.lpszClassName = kTrayWindowClassName;
          window_class.style = CS_HREDRAW | CS_VREDRAW;
          window_class.cbClsExtra = 0;
          window_class.cbWndExtra = 0;
          window_class.hInstance = GetModuleHandle(nullptr);
          window_class.hIcon = nullptr;
          window_class.hbrBackground = 0;
          window_class.lpszMenuName = nullptr;
          window_class.lpfnWndProc = WinProcS::TrayWndProc;
          RegisterClass(&window_class);
          tray_class_registered_ = true;
      }

      return kTrayWindowClassName;
  }
  ~TrayIcon()
  {
      if (tray_window_) {
          DestroyWindow(tray_window_);
          tray_window_ = nullptr;
      }
    if (is_visible())
      hide();
    if (!m_icon_is_shared)
      DestroyIcon(data.hIcon);
  };

  bool is_subscribed(UINT event)
  {
    return std::find(m_subbed_events.begin(), m_subbed_events.end(), event) != m_subbed_events.end();
  }

  void subscribe(UINT event)
  {
    m_subbed_events.push_back(event);
  };

  void unsubscribe(UINT event)
  {
    for (auto it = m_subbed_events.begin(); it != m_subbed_events.end(); ++it)
    {
      if (*it == event)
      {
        m_subbed_events.erase(it);
        break;
      }
    }
  };

  void set_tooltip(const std::string &tooltip)
  {
    // https://docs.microsoft.com/en-us/windows/win32/api/stringapiset/nf-stringapiset-multibytetowidechar
    MultiByteToWideChar(CP_UTF8, 0, tooltip.c_str(), -1, data.szTip, sizeof(data.szTip) / sizeof(data.szTip[0]));
    data.uFlags |= NIF_TIP | NIF_SHOWTIP;
  };

  void remove_tooltip()
  {
    data.uFlags &= ~(NIF_TIP | NIF_SHOWTIP);
  };

  void set_image(const HICON &icon, bool is_shared)
  {
    if (data.hIcon && !m_icon_is_shared)
      DestroyIcon(data.hIcon);
    m_icon_is_shared = is_shared;
    data.hIcon = icon;
    data.uFlags |= NIF_ICON;
  };

  void remove_image()
  {
    if (data.hIcon && !m_icon_is_shared)
      DestroyIcon(data.hIcon);
    m_icon_is_shared = false;
    data.hIcon = NULL;
    data.uFlags &= ~NIF_ICON;
  };

  // Wrapper around NIM_ADD
  bool show()
  {
    m_is_visible = Shell_NotifyIcon(NIM_ADD, &data) != 0;
    Shell_NotifyIcon(NIM_SETVERSION, &data);
    return is_visible();
  };
  // Wrapper around NIM_MODIFY
  bool update()
  {
    return Shell_NotifyIcon(NIM_MODIFY, &data);
  };
  // Wrapper around NIM_DELETE
  bool hide()
  {
    m_is_visible = Shell_NotifyIcon(NIM_DELETE, &data) != 0;
    return is_visible();
  };

  bool is_visible() { return m_is_visible; };
};
