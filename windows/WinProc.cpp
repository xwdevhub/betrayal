#include <windows.h>
class WinProcS {
public:
    static LRESULT CALLBACK TrayWndProc(HWND const window,
        UINT const message,
        WPARAM const wparam,
        LPARAM const lparam) noexcept {
        if (message == WM_NCCREATE) {
            auto window_struct = reinterpret_cast<CREATESTRUCT*>(lparam);
            SetWindowLongPtr(window, GWLP_USERDATA,
                reinterpret_cast<LONG_PTR>(window_struct->lpCreateParams));
        }


        return DefWindowProc(window, message, wparam, lparam);
    }
};