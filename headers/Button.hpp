#pragma once

#include <windows.h>
#include <windowsx.h>
#include <gdiplus.h>
#include <vector>
#include <functional>

#include "Theme.hpp"

typedef struct {
    bool isHovering;
    std::function<void(Gdiplus::Graphics& gfx, size_t i, int x, int y)> handleRender;
    std::function<void(HWND)> handleClick;
} Button;

void handleCloseRender(Gdiplus::Graphics& gfx, size_t i, int x, int y);

void handleCloseClick(HWND hDlg);

void handleMaximizeRender(Gdiplus::Graphics& gfx, size_t i, int x, int y);

void handleMaximizeClick(HWND hDlg);

void handleMinimizeRender(Gdiplus::Graphics& gfx, size_t i, int x, int y);

void handleMinimizeClick(HWND hDlg);

static Button buttons[] = {
    { false, handleCloseRender, handleCloseClick },
    { false, handleMaximizeRender, handleMaximizeClick },
    { false, handleMinimizeRender, handleMinimizeClick }
};
