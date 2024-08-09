#include "Program.hpp"
#include <iostream>

namespace Program
{
    HICON hIcon;

    INT_PTR CALLBACK WinProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
    {
        static bool isMaximized = false;
        static HCURSOR hCursorArrow = LoadCursor(nullptr, IDC_ARROW);
        static HCURSOR hCursorHand = LoadCursor(nullptr, IDC_HAND);
        static TRACKMOUSEEVENT tme = { sizeof(TRACKMOUSEEVENT), TME_LEAVE, NULL, 0 };

        switch (message)
        {
            case WM_INITDIALOG:
            {
                SetWindowTextW(hDlg, WIN_NAME);
                LONG_PTR style = GetWindowLongPtr(hDlg, GWL_STYLE);
                style |= WS_OVERLAPPEDWINDOW;
                SetWindowLongPtr(hDlg, GWL_STYLE, style);
                MARGINS m{ 0, 0, 0, 1 };
                DwmExtendFrameIntoClientArea(hDlg, &m);
                SetWindowPos(hDlg, nullptr, 0, 0, 0, 0,
                    SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED);
                tme.hwndTrack = hDlg;
                hIcon = LoadIcon(GetModuleHandle(nullptr), MAKEINTRESOURCE(IDI_ICON));
                SendMessage(hDlg, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
                SendMessage(hDlg, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
                return TRUE;
            }

            case WM_NCCALCSIZE:
            {
                if (wParam == TRUE) {
                    NCCALCSIZE_PARAMS* pncsp = reinterpret_cast<NCCALCSIZE_PARAMS*>(lParam);
                    
                    if (isMaximized) {
                        pncsp->rgrc[0].left += 8;
                        pncsp->rgrc[0].top += 8;
                        pncsp->rgrc[0].right -= 8;
                        pncsp->rgrc[0].bottom -= 8;
                    }
                    
                    SetWindowLong(hDlg, DWLP_MSGRESULT, 0);
                    return TRUE;
                }
                return FALSE;
            }

            case WM_ERASEBKGND:
            {
                return TRUE;
            }

            case WM_ENTERSIZEMOVE:
            case WM_EXITSIZEMOVE:
            case WM_MOVING:
            {
                InvalidateRect(hDlg, nullptr, FALSE);
                return 0;
            }

            case WM_PAINT:
            {
                PAINTSTRUCT ps;
                HDC hdc = BeginPaint(hDlg, &ps);
                RECT clientRect;
                GetClientRect(hDlg, &clientRect);
                HDC hdcMem = CreateCompatibleDC(hdc);
                HBITMAP hbmMem = CreateCompatibleBitmap(hdc, clientRect.right - clientRect.left, clientRect.bottom - clientRect.top);
                HGDIOBJ hOld = SelectObject(hdcMem, hbmMem);

                Gdiplus::Graphics gfx(hdcMem);
                Gdiplus::SolidBrush topbarBrush(TOPBAR_BACKGROUND_COLOR);
                gfx.FillRectangle(&topbarBrush, 0, 0, clientRect.right, TOPBAR_HEIGHT);
                Gdiplus::SolidBrush winBrush(WIN_BACKGROUND_COLOR);
                gfx.FillRectangle(&winBrush, 0, TOPBAR_HEIGHT, clientRect.right, clientRect.bottom - TOPBAR_HEIGHT);

                size_t i = 0;

                for (const Button& btn : buttons) {
                    int closeButtonX = clientRect.right - TOPBAR_BUTTON_WIDTH - (TOPBAR_BUTTON_WIDTH * i);
                    int closeButtonY = 0;

                    if (btn.isHovering) {
                        Gdiplus::SolidBrush closeHoverBrush(Gdiplus::Color(90, 90, 90));
                        gfx.FillRectangle(&closeHoverBrush, closeButtonX, closeButtonY, TOPBAR_BUTTON_WIDTH, TOPBAR_HEIGHT);
                    }

                    btn.handleRender(gfx, i, closeButtonX, closeButtonY);
                    i++;
                }

                DrawIconEx(hdcMem, ICON_MARGIN, ICON_MARGIN, hIcon, ICON_SIZE, ICON_SIZE, 0, nullptr, DI_NORMAL);
                Gdiplus::Font font(L"Segoe UI", TITLE_FONT_SIZE);
                Gdiplus::SolidBrush textBrush(Gdiplus::Color(255, 255, 255, 255));
                gfx.DrawString(WIN_NAME, -1, &font, Gdiplus::PointF(5 + ICON_SIZE + TOPBAR_SEPARATOR_SIZE, TITLE_MARGIN), &textBrush);
                BitBlt(hdc, 0, 0, clientRect.right, clientRect.bottom, hdcMem, 0, 0, SRCCOPY);
                SelectObject(hdcMem, hOld);
                DeleteObject(hbmMem);
                DeleteDC(hdcMem);
                EndPaint(hDlg, &ps);
                return TRUE;
            }

            case WM_LBUTTONDOWN:
            {
                POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
                RECT clientRect;
                GetClientRect(hDlg, &clientRect);
                size_t i = 0;

                for (const Button& btn : buttons) {
                    int closeButtonX = clientRect.right - TOPBAR_BUTTON_WIDTH - (TOPBAR_BUTTON_WIDTH * i);
                    int closeButtonY = 0;

                    if (pt.x >= closeButtonX && pt.x <= closeButtonX + TOPBAR_BUTTON_WIDTH && pt.y >= closeButtonY && pt.y <= closeButtonY + TOPBAR_HEIGHT) {
                        btn.handleClick(hDlg);
                    }
                    i++;
                }
                return 0;
            }

            case WM_MOUSEMOVE:
            {
                POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
                RECT clientRect;
                GetClientRect(hDlg, &clientRect);
                size_t i = 0;
                bool hasTmePending = false;

                for (Button& btn : buttons) {
                    int closeButtonX = clientRect.right - TOPBAR_BUTTON_WIDTH - (TOPBAR_BUTTON_WIDTH * i);
                    int closeButtonY = 0;

                    bool hovering = pt.x >= closeButtonX &&
                        pt.x <= closeButtonX + TOPBAR_BUTTON_WIDTH &&
                        pt.y >= closeButtonY &&
                        pt.y <= closeButtonY + TOPBAR_HEIGHT;

                    if (hovering != btn.isHovering) {
                        btn.isHovering = hovering;
                        InvalidateRect(hDlg, nullptr, TRUE);

                        if (btn.isHovering) {
                            SetCursor(hCursorHand);
                        } else {
                            SetCursor(hCursorArrow);
                        }
                    }

                    if (!btn.isHovering) {
                        hasTmePending = true;
                    }
                    i++;
                }

                if (hasTmePending) {
                    TrackMouseEvent(&tme);
                }
                return 0;
            }

            case WM_MOUSELEAVE:
            {
                size_t i = 0;
                bool isUnhoverState = false;

                for (Button& btn : buttons) {
                    if (btn.isHovering) {
                        btn.isHovering = false;
                        isUnhoverState = true;
                    }
                }

                if (isUnhoverState == true) {
                    InvalidateRect(hDlg, nullptr, TRUE);
                    SetCursor(hCursorArrow);
                }

                return 0;
            }

            case WM_SETCURSOR:
            {
                size_t i = 0;

                for (const Button& btn : buttons) {
                    if (btn.isHovering) {
                        SetCursor(hCursorHand);
                        return TRUE;
                    }
                }
                return FALSE;
            }

            case WM_NCHITTEST:
            {
                POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
                ScreenToClient(hDlg, &pt);
                RECT rc;
                GetClientRect(hDlg, &rc);

                const int borderWidth = WIN_BORDER_WIDTH;

                if (pt.y < borderWidth) {
                    if (pt.x < borderWidth) {
                        SetWindowLong(hDlg, DWLP_MSGRESULT, HTTOPLEFT);
                        return TRUE;
                    } else if (pt.x > rc.right - borderWidth) {
                        SetWindowLong(hDlg, DWLP_MSGRESULT, HTTOPRIGHT);
                        return TRUE;
                    } else {
                        SetWindowLong(hDlg, DWLP_MSGRESULT, HTTOP);
                        return TRUE;
                    }
                }

                if (pt.y <= TOPBAR_HEIGHT) {
                    bool isButtonArea = false;
                    size_t i = 0;

                    for (const Button& btn : buttons) {
                        int buttonX = rc.right - TOPBAR_BUTTON_WIDTH - (TOPBAR_BUTTON_WIDTH * i);
                        int buttonY = 0;

                        if (pt.x >= buttonX && pt.x <= buttonX + TOPBAR_BUTTON_WIDTH) {
                            isButtonArea = true;
                            break;
                        }
                        i++;
                    }

                    if (!isButtonArea) {
                        SetWindowLong(hDlg, DWLP_MSGRESULT, HTCAPTION);
                        return TRUE;
                    } else {
                        SetWindowLong(hDlg, DWLP_MSGRESULT, HTCLIENT);
                        return TRUE;
                    }
                }

                if (pt.y > rc.bottom - borderWidth) {
                    if (pt.x < borderWidth) {
                        SetWindowLong(hDlg, DWLP_MSGRESULT, HTBOTTOMLEFT);
                        return TRUE;
                    } else if (pt.x > rc.right - borderWidth) {
                        SetWindowLong(hDlg, DWLP_MSGRESULT, HTBOTTOMRIGHT);
                        return TRUE;
                    } else {
                        SetWindowLong(hDlg, DWLP_MSGRESULT, HTBOTTOM);
                        return TRUE;
                    }
                }

                if (pt.x < borderWidth) {
                    SetWindowLong(hDlg, DWLP_MSGRESULT, HTLEFT);
                    return TRUE;
                } else if (pt.x > rc.right - borderWidth) {
                    SetWindowLong(hDlg, DWLP_MSGRESULT, HTRIGHT);
                    return TRUE;
                }

                SetWindowLong(hDlg, DWLP_MSGRESULT, HTCLIENT);
                return TRUE;
            }

            case WM_COMMAND:
            {
                WORD id = LOWORD(wParam);

                if (id == IDOK || id == IDCANCEL) {
                    EndDialog(hDlg, id);
                    return TRUE;
                }

                return FALSE;
            }

            case WM_NCLBUTTONDBLCLK:
            {
                if (wParam == HTCAPTION) {
                    if (isMaximized) {
                        ShowWindow(hDlg, SW_RESTORE);
                    } else {
                        ShowWindow(hDlg, SW_MAXIMIZE);
                    }

                    isMaximized = !isMaximized;
                    return TRUE;
                }

                break;
            }

            case WM_GETMINMAXINFO:
            {
                LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;

                lpMMI->ptMaxSize.x = GetSystemMetrics(SM_CXMAXIMIZED);
                lpMMI->ptMaxSize.y = GetSystemMetrics(SM_CYMAXIMIZED);
                lpMMI->ptMaxPosition.x = 0;
                lpMMI->ptMaxPosition.y = 0;
                lpMMI->ptMinTrackSize.x = WIN_MIN_WIDTH;
                lpMMI->ptMinTrackSize.y = WIN_MIN_HEIGHT;
                return 0;
            }

            case WM_SIZE:
            {
                if (wParam == SIZE_MAXIMIZED) {
                    isMaximized = true;
                } else if (wParam == SIZE_RESTORED) {
                    isMaximized = false;
                }

                InvalidateRect(hDlg, nullptr, TRUE);
                return 0;
            }

            case WM_SYSCOMMAND:
            {
                if ((wParam & 0xFFF0) == SC_MOVE)
                {
                    LRESULT result = DefWindowProc(hDlg, message, wParam, lParam);

                    if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
                    {
                        RECT rect;
                        GetWindowRect(hDlg, &rect);
                        if (rect.top <= 0)
                        {
                            ShowWindow(hDlg, SW_MAXIMIZE);
                            isMaximized = true;
                        }
                    }

                    return result;
                }

                break;
            }
        }

        return FALSE;
    }
}
