#include "ScreenSizeConstants.hpp"
int WINDOW_HEIGHT = 512;
int WINDOW_LENGTH = 512;
int WINDOW_HEIGHT_HALF = WINDOW_HEIGHT / 2;
int WINDOW_LENGTH_HALF = WINDOW_LENGTH / 2;

void update(int width, int height) noexcept
{
	WINDOW_HEIGHT = height;
	WINDOW_LENGTH = width;
	WINDOW_HEIGHT_HALF = height / 2;
	WINDOW_LENGTH_HALF = width / 2;
};