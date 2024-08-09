#pragma once

#include <windows.h>
#include <windowsx.h>
#include <dwmapi.h>
#include <unknwn.h>
#include <gdiplus.h>
#include "resources.hpp"
#include "Theme.hpp"
#include "Button.hpp"

#pragma comment(lib, "dwmapi")
#pragma comment(lib, "gdiplus")

namespace Program
{
    INT_PTR CALLBACK WinProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
}
