#include "ScreenSizeConstants.hpp"
int WINDOW_HEIGHT = 512;
int WINDOW_WIDTH = 512;
int WINDOW_HEIGHT_HALF = WINDOW_HEIGHT / 2;
int WINDOW_WIDTH_HALF = WINDOW_WIDTH / 2;

void update(int width, int height) noexcept
{
	WINDOW_HEIGHT = height;
	WINDOW_WIDTH = width;
	WINDOW_HEIGHT_HALF = height / 2;
	WINDOW_WIDTH_HALF = width / 2;
};