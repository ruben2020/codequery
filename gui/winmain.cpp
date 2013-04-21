
#ifdef _MSC_VER

/*

Taken from:
https://github.com/jlnr/gosu/blob/master/GosuImpl/WinMain.cpp

Copyright (C) 2004-2013 Julian Raschke, Jan Lücker and all contributors.

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
                        http://www.libgosu.org/

*/

#include <windows.h>
#include <exception>
#include <string>
#include <vector>
using namespace std;

vector<string> splitCmdLine()
{
    vector<string> result;

    const char* cmdLine = ::GetCommandLineA();

    const char* argBegin = 0;
    bool isQuotedArg = false;

    while (*cmdLine)
    {
        if (*cmdLine == '"')
        {
            if (argBegin == 0)
            {
                argBegin = cmdLine + 1;
                isQuotedArg = true;
            }
            else if (isQuotedArg)
            {
                result.push_back(std::string(argBegin, cmdLine));
                argBegin = 0;
            }
        }
        else if (!isspace((unsigned char)*cmdLine) && argBegin == 0)
        {
            argBegin = cmdLine;
            isQuotedArg = false;
        }
        else if (isspace((unsigned char)*cmdLine) && argBegin != 0 && !isQuotedArg)
        {
            result.push_back(std::string(argBegin, cmdLine + 1));
            argBegin = 0;
        }
        ++cmdLine;
    }

    if (argBegin != 0)
        result.push_back(argBegin);

    return result;
}

int main(int argc, char* argv[]);

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    try
    {
        vector<string> arguments = splitCmdLine();
        vector<char*> argv(arguments.size());
        for (unsigned i = 0; i < argv.size(); ++i)
            argv[i] = const_cast<char*>(arguments[i].c_str());
        return main(argv.size(), &argv[0]);
    }
    catch (const std::exception& e)
    {
        ::MessageBoxA(0, e.what(), "Uncaught Exception", MB_OK | MB_ICONERROR);
        return EXIT_FAILURE;
    }
}

#endif
