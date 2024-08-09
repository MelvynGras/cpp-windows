#include "Button.hpp"

void handleCloseRender(Gdiplus::Graphics& gfx, size_t i, int x, int y)
{
    Gdiplus::Pen pen(CLOSE_BUTTON_COLOR, 1);

    gfx.DrawLine(&pen, x + 25,
        y + CLOSE_ICON_MARGIN, x + 35, y + CLOSE_ICON_MARGIN + CLOSE_ICON_SIZE);
    gfx.DrawLine(&pen, x + 35,
        y + CLOSE_ICON_MARGIN, x + 25, y + CLOSE_ICON_MARGIN + CLOSE_ICON_SIZE);
}

void handleCloseClick(HWND hDlg)
{
    PostMessage(hDlg, WM_CLOSE, 0, 0);
}

void handleMaximizeRender(Gdiplus::Graphics& gfx, size_t i, int x, int y)
{
    Gdiplus::Pen pen(CLOSE_BUTTON_COLOR, 1);

    gfx.DrawRectangle(&pen, x + 25, y + CLOSE_ICON_MARGIN, 10, 10);
}

void handleMaximizeClick(HWND hDlg)
{
    if (IsZoomed(hDlg)) {
        PostMessage(hDlg, WM_SYSCOMMAND, SC_RESTORE, 0);
    } else {
        PostMessage(hDlg, WM_SYSCOMMAND, SC_MAXIMIZE, 0);
    }
}

void handleMinimizeRender(Gdiplus::Graphics& gfx, size_t i, int x, int y)
{
    Gdiplus::Pen pen(CLOSE_BUTTON_COLOR, 1);
    gfx.DrawLine(&pen, x + 25, y + 8 + CLOSE_ICON_MARGIN, x + 35, y + 8 + CLOSE_ICON_MARGIN);
}

void handleMinimizeClick(HWND hDlg)
{
    PostMessage(hDlg, WM_SYSCOMMAND, SC_MINIMIZE, 0);
}
