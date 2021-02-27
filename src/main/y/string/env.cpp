#include "y/string/env.hpp"
#include "y/memory/allocator/pooled.hpp"
#include "y/exceptions.hpp"

#include <cerrno>
#include <cstring>

#if defined(Y_BSD)
#include <cstdlib>
extern char **environ;
#endif

#if defined(Y_WIN)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "y/memory/zblock.hpp"
#endif


#include <iostream>

namespace upsylon {

	bool environment::verbose = false;

	bool environment::get(string &value, const string &name)
	{
		Y_GIANT_LOCK();
		value.clear();

#if defined(Y_BSD)
		const char *res = getenv(*name);
		if (res)
		{
			value = res;
			if (verbose) std::cerr << "getenv('" << name << "')='" << value << "'" << std::endl;
			return true;
		}
		else
		{
			if (verbose) std::cerr << "getenv('" << name << "') : UNDEFINED" << std::endl;
			return false;
		}
#endif


#if defined(Y_WIN)
		const DWORD res = ::GetEnvironmentVariableA(*name, NULL, 0);
		const DWORD err = ::GetLastError();
		if (err == ERROR_ENVVAR_NOT_FOUND)
		{
			if (verbose) std::cerr << "getenv('" << name << "') : UNDEFINED" << std::endl;
			return false;
		}
		else
		{
			assert(res > 0);
			zblock<char, memory::pooled> blk(res); assert(blk.bytes >= res);
			char                        *buf = *blk;
			//	char                         buf[1024] = { 0 };
			const DWORD                  ret = ::GetEnvironmentVariableA(*name,buf, res);
			const DWORD                  len = res - 1;
			if (ret != len)
			{
				throw win32::exception(::GetLastError(), "::GetEnvironmentVariable(%s)", *name);
			}
			//std::cerr << "buf='" << buf << "'" << std::endl;
			value.assign(buf, len);
			if (verbose) std::cerr << "getenv('" << name << "')='" << value << "'" << std::endl;
			return true;
		}
#endif

	}

	bool environment::get(string &value, const char *name)
	{
		const string _(name);
		return environment::get(value, _);
	}


	void environment::set(const string &name, const string &value)
	{
		Y_GIANT_LOCK();
		if (verbose)  std::cerr << "setenv('" << name << "')='" << value << "'" << std::endl;
#if defined(Y_BSD)
		if (setenv(*name, *value, 1) < 0)
		{
			throw libc::exception(errno, "setenv");
		}
#endif

#if defined(Y_WIN)
		if (!::SetEnvironmentVariable(*name, *value))
		{
			throw win32::exception(::GetLastError(), "::SetEnvironmentVariable");
		}
#endif

	}

	void environment::set(const char *name, const string &value)
	{
		const string _(name);
		set(_, value);
	}

	void environment::set(const char *name, const char *value)
	{
		const string _(name);
		const string __(value);
		set(_, __);
	}


	static void _parse_env(const char *str, const size_t len, void(*proc)(const string &, const string &, void *), void *args)
	{
		assert(str != NULL);
		assert(len > 0);
		size_t idx = 1;
		bool   has_eq = false;
		while (idx < len) {
			if (str[idx] == '=') {
				has_eq = true;
				break;
			}
			++idx;
		}
		if (!has_eq)
			throw libc::exception(EINVAL, "invalid system environment string");

		string name(str, idx);
		++idx;
		string value(str + idx, len - idx);
		proc(name, value, args);
	}


	void environment::get(void(*proc)(const string &, const string &, void *), void *args)
	{
		Y_GIANT_LOCK();
		assert(proc);

#if defined(Y_BSD)
		char **env_reg = environ;
		while (*env_reg) {
			const char *str = *env_reg;
			_parse_env(str, strlen(str), proc, args);
			++env_reg;
		}
#endif


#if defined(Y_WIN)
		LPTCH es = 0;
		{
			es = ::GetEnvironmentStrings();
			if (!es)
				throw win32::exception(::GetLastError(), "::GetEnvironmentStrings()");
		}

		try {

			const char *str = (const char *)es;
			while (*str) {
				size_t len = std::strlen(str);
				_parse_env(str, len, proc, args);
				str += len;
				++str;
			}

			::FreeEnvironmentStrings(es);
		}
		catch (...) {
			::FreeEnvironmentStrings(es);
			throw;
		}

#endif
	}

}

#include "y/string/convert.hpp"
#include <iostream>

namespace upsylon
{


	static inline bool env_get_clean(string &content, const string &name)
	{
		if (environment::get(content, name))
		{
			content.clean_with(" \t");
			return true;
		}
		else
		{
			return false;
		}

	}

	template <>
	bool environment::check<bool>(bool &value, const string &name)
	{
		static const char *ok[] = { "yes", "on", "true" , "1" };
		static const char *no[] = { "no", "off", "false" , "0" };
		value = false;
		string content;
		if (env_get_clean(content, name))
		{
			string_convert::to_lower(content);
			for (unsigned i = 0; i < sizeof(ok) / sizeof(ok[0]); ++i)
			{
				if (ok[i] == content)
				{
					value = true;
					return true;
				}
			}

			for (unsigned i = 0; i < sizeof(no) / sizeof(no[0]); ++i)
			{
				if (no[i] == content)
				{
					value = false;
					return true;
				}
			}

			throw exception("environment::check<bool>: invalid '%s'='%s' )", *name, *content);

		}
		else
		{

		}

		// undefined, default value=false
		return false;
	}


	bool environment::flag(const string &name)
	{
		bool   value = false;
		return check<bool>(value, name) ? /* defined */ value : /* undefined */ false;
	}

	bool environment::flag(const char *name)
	{
		const string _(name);
		return flag(_);
	}

#define Y_ENV_CHECK_FOR(TYPE) \
template <>\
bool environment:: check<TYPE>( TYPE &value, const string &name )\
{\
string content;\
if( env_get_clean(content,name) )\
{\
value = string_convert::to<TYPE>(content,"environment::check<" #TYPE ">");\
return true;\
}\
else\
{\
return false;\
}\
}

	Y_ENV_CHECK_FOR(unit_t)
		Y_ENV_CHECK_FOR(size_t)
		Y_ENV_CHECK_FOR(float)
		Y_ENV_CHECK_FOR(double)



}



