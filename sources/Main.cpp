#include "Main.hpp"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR lpCmdLine,
    _In_ int nCmdShow)
{
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdipToken = 0;
    Gdiplus::GdiplusStartup(&gdipToken, &gdiplusStartupInput, nullptr);

    struct Dialog : DLGTEMPLATE {
        WORD dummy[3] = { 0 };
    } dlg;

    dlg.style = WS_POPUP | WS_CAPTION | DS_CENTER | WS_THICKFRAME;
    dlg.dwExtendedStyle = WS_EX_LAYERED;
    dlg.cdit = 0;
    dlg.x = 0;
    dlg.y = 0;
    dlg.cx = WIN_WIDTH;
    dlg.cy = WIN_HEIGHT;

    HWND hDlg = CreateDialogIndirectW(hInstance, &dlg, nullptr, Program::WinProc);

    if (hDlg) {
        SetLayeredWindowAttributes(hDlg, 0, 255, LWA_ALPHA);
        
        ShowWindow(hDlg, nCmdShow);
        UpdateWindow(hDlg);

        MSG msg;
        while (GetMessage(&msg, nullptr, 0, 0)) {
            if (!IsDialogMessage(hDlg, &msg)) {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
    }

    Gdiplus::GdiplusShutdown(gdipToken);
    return 0;
}
