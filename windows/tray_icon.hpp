#pragma once

#include <windows.h>

#include <shellapi.h>
#include <strsafe.h>

#include <string>
#include <codecvt>
#include <map>

class TrayIcon
{
private:
  bool m_is_visible;
  bool m_icon_is_shared;

public:
  NOTIFYICONDATA data;

  TrayIcon(HWND hWnd, UINT id)
  {
    data.cbSize = sizeof(data);
    data.hWnd = hWnd;
    data.uID = id;
    data.uCallbackMessage = WM_USER + id;
    data.uFlags = NIF_MESSAGE;
  };

  ~TrayIcon()
  {
    if (is_visible())
      hide();
    if (!m_icon_is_shared)
      DestroyIcon(data.hIcon);
  };

  void set_tooltip(const std::string &tooltip)
  {
    // https://docs.microsoft.com/en-us/windows/win32/api/stringapiset/nf-stringapiset-multibytetowidechar
    MultiByteToWideChar(CP_UTF8, 0, tooltip.c_str(), -1, data.szTip, sizeof(data.szTip) / sizeof(data.szTip[0]));
    data.uFlags |= NIF_TIP | NIF_SHOWTIP;
  };

  void set_icon(const HICON &icon, bool is_shared)
  {
    if (data.hIcon && !m_icon_is_shared)
      DestroyIcon(data.hIcon);
    m_icon_is_shared = is_shared;
    data.hIcon = icon;
    data.uFlags |= NIF_ICON;
  };

  // Wrapper around NIM_ADD
  bool show()
  {
    m_is_visible = Shell_NotifyIcon(NIM_ADD, &data) != 0;
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