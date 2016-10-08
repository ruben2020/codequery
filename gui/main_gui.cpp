
/*
 * CodeQuery
 * Copyright (C) 2013-2016 ruben2020 https://github.com/ruben2020/
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */

#ifdef _WIN32
#include <windows.h>
#endif

#include "mainwindow.h"


int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	QMainWindow *wndw = new QMainWindow;
	mainwindow mw(wndw, &app);

	mw.show();

#ifdef _WIN32
	ShowWindow( (HWND) mw.winId(), SW_HIDE);
	SetWindowLongPtr( (HWND) mw.winId(), GWL_EXSTYLE, 
			GetWindowLongPtr( (HWND) mw.winId(), GWL_EXSTYLE) | WS_EX_APPWINDOW);
	ShowWindow( (HWND) mw.winId(), SW_SHOW);
#endif

	return app.exec();
}
