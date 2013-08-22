#include "Window.h"


HWND Window::handle;


void Window::setHandle(HWND handle)
{
	Window::handle = handle;
}

Point Window::getCenter()
{
	RECT rect;

	GetWindowRect(handle, &rect);
	
	return Point((rect.left + rect.right) / 2, (rect.top + rect.bottom) / 2);
}