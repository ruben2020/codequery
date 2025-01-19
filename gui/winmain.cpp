
/*

Taken from:
https://github.com/gosu/gosu/blob/master/src/WinMain.cpp

This license applies only to this file:

Copyright (C) 2001-2023 Julian Raschke, Jan Lücker, cyberarm, and all
other contributors: https://github.com/gosu/gosu/graphs/contributors

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.

            Julian Raschke <julian@raschke.de> & contributors
                        https://www.libgosu.org/

*/

//#ifdef _MSC_VER
#ifdef _WIN32

#include <exception>
#include <string>
#include <vector>
#include <windows.h>
using namespace std;

vector<string> split_cmd_line()
{
    vector<string> result;

    const char* cmd_line = ::GetCommandLineA();

    const char* arg_begin = nullptr;
    bool is_quoted_arg = false;

    while (*cmd_line) {
        if (*cmd_line == '"') {
            if (arg_begin == nullptr) {
                arg_begin = cmd_line + 1;
                is_quoted_arg = true;
            }
            else if (is_quoted_arg) {
                result.push_back(string(arg_begin, cmd_line));
                arg_begin = nullptr;
            }
        }
        else if (!isspace((unsigned char)*cmd_line) && arg_begin == nullptr) {
            arg_begin = cmd_line;
            is_quoted_arg = false;
        }
        else if (isspace((unsigned char)*cmd_line) && arg_begin != nullptr && !is_quoted_arg) {
            result.push_back(string(arg_begin, cmd_line + 1));
            arg_begin = nullptr;
        }
        ++cmd_line;
    }

    if (arg_begin != 0) result.push_back(arg_begin);

    return result;
}

int main(int argc, char* argv[]);

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    try {
        vector<string> arguments = split_cmd_line();
        vector<char*> argv(arguments.size());
        for (unsigned i = 0; i < argv.size(); ++i) {
            argv[i] = const_cast<char*>(arguments[i].c_str());
        }
        return main(argv.size(), &argv[0]);
    }
    catch (const exception& e) {
        ::MessageBoxA(0, e.what(), "Uncaught Exception", MB_OK | MB_ICONERROR);
        return EXIT_FAILURE;
    }
}

#endif
