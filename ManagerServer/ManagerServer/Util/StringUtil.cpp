#include "StringUtil.h"
#include <cstdarg>
#include <memory>

string StringUtil::Format(const char* format, ...)
{
	va_list args;
	va_start(args, format);
	size_t size = vsnprintf(nullptr, 0, format, args) + 1; // Extra space for '\0'
	unique_ptr<char[]> buf(new char[size]);
	vsnprintf(buf.get(), size, format, args);
	va_end(args);

	return string(buf.get(), buf.get() + size - 1); // we don't want the '\0' inside
		
}
