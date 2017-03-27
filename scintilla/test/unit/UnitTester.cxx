// UnitTester.cpp : Defines the entry point for the console application.
//

#include <cstdio>
#include <cstdarg>

#include "Platform.h"

#define CATCH_CONFIG_RUNNER
#include "catch.hpp"

// Needed for PLATFORM_ASSERT in code being tested

void Platform::Assert(const char *c, const char *file, int line) {
	fprintf(stderr, "Assertion [%s] failed at %s %d\n", c, file, line);
	abort();
}

void Platform::DebugPrintf(const char *format, ...) {
	char buffer[2000];
	va_list pArguments;
	va_start(pArguments, format);
	vsprintf(buffer, format, pArguments);
	va_end(pArguments);
	fprintf(stderr, "%s", buffer);
}

int main(int argc, char* const argv[]) {
	const int result = Catch::Session().run(argc, argv);

	_CrtDumpMemoryLeaks();

	return result;
}
