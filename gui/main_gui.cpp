
/*
 * CodeQuery
 * Copyright (C) 2013-2017 ruben2020 https://github.com/ruben2020/
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
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
