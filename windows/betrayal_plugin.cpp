#include "include/betrayal/betrayal_plugin.h"

// This must be included before many other Windows headers.
#include <windows.h>
#include <windowsx.h>

#include <shellapi.h>
#include <strsafe.h>

#include <flutter/method_channel.h>
#include <flutter/plugin_registrar_windows.h>
#include <flutter/standard_method_codec.h>

#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <locale>
#include <codecvt>

#include "icon_manager.hpp"

namespace Betrayal
{

  std::unique_ptr<flutter::MethodChannel<flutter::EncodableValue>,
                  std::default_delete<flutter::MethodChannel<flutter::EncodableValue>>>
      channel = nullptr;

  class BetrayalPlugin : public flutter::Plugin
  {
  public:
    static void RegisterWithRegistrar(flutter::PluginRegistrarWindows *registrar);
    UINT taskbar_created_message_b = RegisterWindowMessage(L"TaskbarCreated");

    BetrayalPlugin(flutter::PluginRegistrarWindows *registrar);

    virtual ~BetrayalPlugin();

  private:
    flutter::PluginRegistrarWindows *registrar;

    // The ID of the WindowProc delegate registration.
    int window_proc_id = -1;
    int icon_id = -1;
    IconManager icons;

    // Called by the Windows Event Loop
    std::optional<LRESULT> BetrayalPlugin::HandleWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
      std::optional<LRESULT> result;

      // https://wiki.winehq.org/List_Of_Windows_Messages
      if (message == WM_DESTROY)
      {
        icons.clear_all();
      }
      // https://docs.microsoft.com/en-us/windows/win32/api/shellapi/nf-shellapi-shell_notifyiconw
      // https://docs.microsoft.com/en-us/windows/win32/api/shellapi/ns-shellapi-notifyicondataw
      else if (message >= WM_USER && message < WM_APP)
      {
        auto event = LOWORD(lParam);
        auto id = HIWORD(lParam);
        auto x = GET_X_LPARAM(wParam);
        auto y = GET_Y_LPARAM(wParam);

        auto icon = icons.get(hWnd, id);
        if (icon != nullptr && icon->is_subscribed(event))
        {
          invokeInteraction(hWnd, message, event, id, x, y);
          return result;
        }
        DefWindowProc(hWnd, message, wParam, lParam);
      }
      else if (taskbar_created_message_b == message) {
          if (icon_id != -1) {
              icons.get(hWnd, icon_id)->show();
          }

      }

      return result;
    };

    HWND BetrayalPlugin::GetMainWindow()
    {
      return ::GetAncestor(registrar->GetView()->GetNativeWindow(), GA_ROOT);
    };

    void invokeInteraction(HWND hWnd, UINT message, uint16_t event, uint16_t id, int x, int y)
    {
      flutter::EncodableMap data;
      data[flutter::EncodableValue("message")] = flutter::EncodableValue(static_cast<int>(message));
      data[flutter::EncodableValue("hWnd")] = flutter::EncodableValue(PtrToInt(hWnd));
      data[flutter::EncodableValue("event")] = flutter::EncodableValue(event);
      data[flutter::EncodableValue("id")] = flutter::EncodableValue(id);
      data[flutter::EncodableValue("x")] = flutter::EncodableValue(x);
      data[flutter::EncodableValue("y")] = flutter::EncodableValue(y);
      channel->InvokeMethod("handleInteraction", std::make_unique<flutter::EncodableValue>(data));
    }

    void Print(const std::string &message)
    {
      channel->InvokeMethod("print", std::make_unique<flutter::EncodableValue>(message));
    }

#define WITH_ARGS const flutter::EncodableMap &args = std::get<flutter::EncodableMap>(*method_call.arguments());
#define WITH_HWND HWND hWnd = GetMainWindow();
#define WITH_ID const int id = std::get<int>(args.at(flutter::EncodableValue("id")));
#define WITH_ARGS_HWND_ID WITH_ARGS WITH_HWND WITH_ID

    // Called when a method is called on this plugin's channel from Dart.
    void HandleMethodCall(
        const flutter::MethodCall<flutter::EncodableValue> &method_call,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
    {
      auto method = method_call.method_name();
      if (method.compare("reset") == 0)
      {
        icons.clear_all();
        result->Success(flutter::EncodableValue(0));
      }
      else if (method.compare("getSystemMetrics") == 0)
      {
        result->Success(flutter::EncodableMap({
            {flutter::EncodableValue("preferredImageSizeX"), flutter::EncodableValue(::GetSystemMetrics(SM_CXSMICON))},
            {flutter::EncodableValue("preferredImageSizeY"), flutter::EncodableValue(::GetSystemMetrics(SM_CYSMICON))},
            {flutter::EncodableValue("preferredLargeImageSizeX"), flutter::EncodableValue(::GetSystemMetrics(SM_CXICON))},
            {flutter::EncodableValue("preferredLargeImageSizeY"), flutter::EncodableValue(::GetSystemMetrics(SM_CYICON))},
            {flutter::EncodableValue("primaryAndSecondarySwapped"), flutter::EncodableValue(::GetSystemMetrics(SM_SWAPBUTTON))},
        }));
      }
      else if (method.compare("subscribeTo") == 0)
      {
        WITH_ARGS_HWND_ID
        UINT event = std::get<int>(args.at(flutter::EncodableValue("event")));
        SubscribeTo(hWnd, id, event, std::move(result));
      }
      else if (method.compare("unsubscribeFrom") == 0)
      {
        WITH_ARGS_HWND_ID
        UINT event = std::get<int>(args.at(flutter::EncodableValue("event")));
        UnsubscribeFrom(hWnd, id, event, std::move(result));
      }
      else if (method.compare("addTray") == 0)
      {
        WITH_ARGS_HWND_ID

        AddTray(hWnd, id, std::move(result));
      }
      else if (method.compare("disposeTray") == 0)
      {
        WITH_ARGS_HWND_ID

        DisposeTray(hWnd, id, std::move(result));
      }
      else if (method.compare("hideIcon") == 0)
      {
        WITH_ARGS_HWND_ID

        HideIcon(hWnd, id, std::move(result));
      }
      else if (method.compare("showIcon") == 0)
      {
        WITH_ARGS_HWND_ID

        ShowIcon(hWnd, id, std::move(result));
        icon_id = id;
      }
      else if (method.compare("setTooltip") == 0)
      {
        WITH_ARGS_HWND_ID

        auto tooltip = std::get<std::string>(args.at(flutter::EncodableValue("tooltip")));
        SetTooltip(hWnd, id, tooltip, std::move(result));
      }
      else if (method.compare("removeTooltip") == 0)
      {
        WITH_ARGS_HWND_ID

        RemoveTooltip(hWnd, id, std::move(result));
      }
      else if (method.compare("setImageFromPath") == 0)
      {
        WITH_ARGS_HWND_ID

        auto path = std::get<std::string>(args.at(flutter::EncodableValue("path")));
        auto is_shared = std::get<bool>(args.at(flutter::EncodableValue("isShared")));
        SetImageFromPath(hWnd, id, path, is_shared, std::move(result));
      }
      else if (method.compare("setImageAsWinIcon") == 0)
      {
        WITH_ARGS_HWND_ID

        auto resource_id = std::get<int>(args.at(flutter::EncodableValue("resourceId")));
        SetImageAsWinIcon(hWnd, id, resource_id, std::move(result));
      }
      else if (method.compare("setImageAsStockIcon") == 0)
      {
        WITH_ARGS_HWND_ID

        auto stockIconId = std::get<int>(args.at(flutter::EncodableValue("stockIconId")));
        SetImageAsStockIcon(hWnd, id, stockIconId, std::move(result));
      }
      else if (method.compare("setImageFromPixels") == 0)
      {
        WITH_ARGS_HWND_ID

        auto width = std::get<int>(args.at(flutter::EncodableValue("width")));
        auto height = std::get<int>(args.at(flutter::EncodableValue("height")));
        auto pixels = std::get<std::vector<int32_t>>(args.at(flutter::EncodableValue("pixels")));

        SetImageFromPixels(hWnd, id, width, height, (uint32_t *)pixels.data(), std::move(result));
      }
      else if (method.compare("removeImage") == 0)
      {
        WITH_ARGS_HWND_ID

        RemoveImage(hWnd, id, std::move(result));
      }
      else
      {
        result->NotImplemented();
      }
    };
#undef WITH_ARGS_HWND_ID
#undef WITH_ID
#undef WITH_HWND
#undef WITH_ARGS

#define WITH_ICON                    \
  auto icon = icons.get(hWnd, id);   \
  if (icon == nullptr)               \
  {                                  \
    result->Error("Icon not found"); \
    return;                          \
  }

    void AddTray(
        const HWND hWnd, const UINT id,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>>
            result)
    {
      auto icon = new TrayIcon(hWnd, id);
      icons.manage(icon);

      result->Success(flutter::EncodableValue(static_cast<int>(id)));
    };

    void DisposeTray(
        const HWND hWnd, const UINT id,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>>
            result)
    {
      icons.dispose(hWnd, id);

      result->Success(flutter::EncodableValue(true));
    };

    void HideIcon(
        const HWND hWnd, const UINT id,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>>
            result)
    {
      if (icons.get(hWnd, id)->hide())
      {
        result->Success(flutter::EncodableValue(true));
      }
      else
      {
        result->Error("Failed to hide icon");
      }
    };

    void ShowIcon(
        const HWND hWnd, const UINT id,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>>
            result)
    {
      if (icons.get(hWnd, id)->show())
      {
        result->Success(flutter::EncodableValue(true));
      }
      else
      {
        result->Error("Failed to show icon");
      }
    };

    void SubscribeTo(
        const HWND hWnd, const UINT id, const UINT event,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>>
            result)
    {
      WITH_ICON;

      icon->subscribe(event);

      result->Success(flutter::EncodableValue(static_cast<int>(id)));
    };

    void UnsubscribeFrom(
        const HWND hWnd, const UINT id, const UINT event,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>>
            result)
    {
      WITH_ICON;

      icon->unsubscribe(event);

      result->Success(flutter::EncodableValue(static_cast<int>(id)));
    };

    void SetTooltip(
        const HWND hWnd, const UINT id, const std::string &tooltip,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>>
            result)
    {
      WITH_ICON;

      icon->set_tooltip(tooltip);
      icon->update();
      result->Success(flutter::EncodableValue(true));
    };

    void RemoveTooltip(
        const HWND hWnd, const UINT id, std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
    {
      WITH_ICON;

      icon->remove_tooltip();
      icon->update();
      result->Success(flutter::EncodableValue(true));
    }

    void SetImageFromPath(
        const HWND hWnd, const UINT id, const std::string &filepath, const bool is_shared,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>>
            result)
    {
      WITH_ICON;

	  std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	  std::wstring convertedStr = converter.from_bytes(filepath);

      auto path = std::wstring(convertedStr.begin(), convertedStr.end());

      HICON hIcon = static_cast<HICON>(
          LoadImage(
              nullptr, path.c_str(), IMAGE_ICON,
              GetSystemMetrics(SM_CXSMICON),
              GetSystemMetrics(SM_CYSMICON),
              LR_LOADFROMFILE + (is_shared ? LR_SHARED : 0x0)));

      icon->set_image(hIcon, is_shared);
      icon->update();
      result->Success(flutter::EncodableValue(true));
    };

    void SetImageAsWinIcon(
        const HWND hWnd, const UINT id, const UINT resource_id,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>>
            result)
    {
      WITH_ICON;

      icon->set_image(LoadIcon(nullptr, MAKEINTRESOURCE(resource_id)), true);
      icon->update();
      result->Success(flutter::EncodableValue(true));
    };

    void SetImageAsStockIcon(
        const HWND hWnd, const UINT id, const UINT stock_icon_id,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>>
            result)
    {
      WITH_ICON;

      SHSTOCKICONINFO info = {0};
      info.cbSize = sizeof(SHSTOCKICONINFO);

      if (0 > SHGetStockIconInfo((SHSTOCKICONID)stock_icon_id, SHGSI_SMALLICON | SHGSI_ICON, &info))
      {
        result->Error("Failed to find stock icon");
        return;
      }
      if (info.hIcon == nullptr)
      {
        result->Error("Failed to get stock icon");
        return;
      }

      icon->set_image(info.hIcon, true);
      icon->update();
      result->Success(flutter::EncodableValue(true));
    };

    void RemoveImage(const HWND hWnd, const UINT id,
                     std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>>
                         result)
    {
      WITH_ICON;

      icon->remove_image();
      icon->update();
      result->Success(flutter::EncodableValue(true));
    };

#pragma warning(disable : 4244)
    // https://cs.github.com/openjdk/jdk/blob/6013d09e82693a1c07cf0bf6daffd95114b3cbfa/src/java.desktop/windows/native/libawt/windows/awt_TrayIcon.cpp#L649
    HICON CreateIconFromBytes(HWND hWnd, int width, int height, uint32_t *bytes, TrayIcon *icon)
    {

      BITMAPV5HEADER bmhHeader = {0};
      bmhHeader.bV5Size = sizeof(BITMAPV5HEADER);
      bmhHeader.bV5Width = width;
      bmhHeader.bV5Height = -height;
      bmhHeader.bV5Planes = 1;
      bmhHeader.bV5BitCount = 32;
      bmhHeader.bV5Compression = BI_BITFIELDS;
      bmhHeader.bV5RedMask = 0x00FF0000;
      bmhHeader.bV5GreenMask = 0x0000FF00;
      bmhHeader.bV5BlueMask = 0x000000FF;
      bmhHeader.bV5AlphaMask = 0xFF000000;

      char *ptrImageData = new char[width * height * 4];

      auto dc = GetDC(hWnd);

      HBITMAP hbmpBitmap = CreateDIBSection(dc, (BITMAPINFO *)&bmhHeader, DIB_RGB_COLORS, (void **)&ptrImageData, NULL, 0);

      uint32_t *srcPtr = bytes;
      char *dstPtr = ptrImageData;
      if (!dstPtr) {
          ReleaseDC(hWnd, dc);
          return NULL;
      }
      for (int y = 0; y < height; y++)
      {
        for (int x = 0; x < width; x++)
        {
          uint32_t pixel = *srcPtr++;
          uint8_t alpha = (pixel >> 24) & 0xFF;
          uint8_t blue = (pixel >> 16) & 0xFF;
          uint8_t green = (pixel >> 8) & 0xFF;
          uint8_t red = pixel & 0xFF;
          *dstPtr++ = blue;
          *dstPtr++ = green;
          *dstPtr++ = red;
          *dstPtr++ = alpha;
        }
      }
#pragma warning(default : 4244)

      HBITMAP hBitmap = CreateDIBitmap(dc, (BITMAPINFOHEADER *)&bmhHeader, CBM_INIT, ptrImageData, (BITMAPINFO *)&bmhHeader, DIB_RGB_COLORS);

      ReleaseDC(hWnd, dc); // 1: success 0: failure
      DeleteObject(hbmpBitmap);

      HBITMAP hMask = CreateBitmap(width, height, 1, 1, NULL);

      ICONINFO iconInfo;
      memset(&iconInfo, 0, sizeof(ICONINFO));
      iconInfo.hbmMask = hMask;
      iconInfo.hbmColor = hBitmap;
      iconInfo.fIcon = TRUE;
      iconInfo.xHotspot = 0;
      iconInfo.yHotspot = 0;

      HICON hIcon = CreateIconIndirect(&iconInfo);

      DeleteObject(hBitmap);
      DeleteObject(hMask);

      return hIcon;
    }

    void SetImageFromPixels(
        const HWND hWnd, const UINT id, const UINT width, const UINT height,
        uint32_t *pixels,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>>
            result)
    {
      WITH_ICON;

      auto hIcon = CreateIconFromBytes(
          hWnd, width, height, pixels, icon);

      icon->set_image(hIcon, false);
      icon->update();

      result->Success(flutter::EncodableValue(true));
    }

#undef WITH_ICON
  };

  // static
  void BetrayalPlugin::RegisterWithRegistrar(
      flutter::PluginRegistrarWindows *registrar)
  {
    channel =
        std::make_unique<flutter::MethodChannel<flutter::EncodableValue>>(
            registrar->messenger(), "betrayal",
            &flutter::StandardMethodCodec::GetInstance());

    auto plugin = std::make_unique<BetrayalPlugin>(registrar);

    channel->SetMethodCallHandler(
        [plugin_pointer = plugin.get()](const auto &call, auto result)
        {
          plugin_pointer->HandleMethodCall(call, std::move(result));
        });

    registrar->AddPlugin(std::move(plugin));
  }

  BetrayalPlugin::BetrayalPlugin(flutter::PluginRegistrarWindows *registrar) : registrar(registrar)
  {
    window_proc_id = registrar->RegisterTopLevelWindowProcDelegate(
        [this](HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
        {
          return this->HandleWindowProc(hWnd, message, wParam, lParam);
        });
  }

  BetrayalPlugin::~BetrayalPlugin()
  {
    registrar->UnregisterTopLevelWindowProcDelegate(window_proc_id);
  }

} // namespace

void BetrayalPluginRegisterWithRegistrar(
    FlutterDesktopPluginRegistrarRef registrar)
{
  Betrayal::BetrayalPlugin::RegisterWithRegistrar(
      flutter::PluginRegistrarManager::GetInstance()
          ->GetRegistrar<flutter::PluginRegistrarWindows>(registrar));
}
