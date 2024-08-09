#pragma once

#include <gdiplus.h>

#pragma comment(lib, "gdiplus")

#define WIN_NAME L"Test"
#define WIN_WIDTH 400
#define WIN_HEIGHT 250
#define WIN_MIN_WIDTH 950
#define WIN_MIN_HEIGHT 500
#define WIN_BACKGROUND_COLOR Gdiplus::Color{ 33, 33, 33 }
#define WIN_BORDER_WIDTH 5

#define TOPBAR_HEIGHT 30
#define TOPBAR_BACKGROUND_COLOR Gdiplus::Color{ 22, 22, 22 }
#define TOPBAR_SEPARATOR_SIZE 13
#define TOPBAR_BUTTON_WIDTH 60

#define ICON_SIZE 18
#define ICON_MARGIN ((TOPBAR_HEIGHT - ICON_SIZE) / 2) + \
    ((TOPBAR_HEIGHT - ICON_SIZE) % 2)

#define TITLE_FONT_SIZE 9
#define TITLE_HEIGHT 22
#define TITLE_MARGIN ((TOPBAR_HEIGHT - TITLE_HEIGHT) / 2) + \
    ((TOPBAR_HEIGHT - TITLE_HEIGHT) % 2)

#define CLOSE_ICON_SIZE 10
#define CLOSE_ICON_MARGIN ((TOPBAR_HEIGHT - CLOSE_ICON_SIZE) / 2) + \
    ((TOPBAR_HEIGHT - CLOSE_ICON_SIZE) % 2)
#define CLOSE_BUTTON_COLOR Gdiplus::Color{ 242, 0, 0 }
