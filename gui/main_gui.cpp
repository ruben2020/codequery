
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

int gui_main(int argc, char *argv[]);

#ifndef _WIN32
int main(int argc, char *argv[])
{
  return gui_main(argc, argv);
}
#endif

int gui_main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	QMainWindow *wndw = new QMainWindow;

/*
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#if defined(__unix__) || defined(__linux__)
	app.setAttribute(Qt::AA_DontUseNativeDialogs, true);
#endif
#endif
*/

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
