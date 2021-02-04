#include "y/utest/run.hpp"
#include "y/exceptions.hpp"
#include <cstdio>
#include <cstring>

#if defined(Y_BSD)
#include <sys/mman.h>
#endif

#if defined(Y_WIN)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif



using namespace upsylon;


Y_UTEST(mlock)
{

	int block_size = 1024;
	if (argc > 1)
	{
		block_size = atoi(argv[1]);
	}

#if defined(Y_BSD)
	std::cerr << "+" << block_size << std::endl;
	void * addr = calloc(block_size, 1);
	if (addr)
	{
		const int status = mlock(addr, block_size);
		if (status != 0)
		{
			perror("mlock");
		}
		else
		{
			munlock(addr, block_size);
		}
		free(addr);
	}
	else
	{
		perror("no memory");
	}
#endif

#if defined(Y_WIN)
	SYSTEM_INFO sSysInfo;         // useful information about the system
	GetSystemInfo(&sSysInfo);     // initialize the structure
	printf("This computer has page size %u.\n", sSysInfo.dwPageSize);
	//block_size = sSysInfo.dwPageSize;
	void *addr = VirtualAlloc(NULL, block_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	if (!addr)
	{
		throw win32::exception(GetLastError(), "VirtualAlloc");
	}

	memset(addr, 0, block_size);
	char *tgt = (char *)addr;
	const size_t num = alea.leq(block_size);
	for (size_t i = 0; i < num; ++i)
	{
		tgt[i] = alea.range<char>('a', 'z');
	}
	std::cerr << tgt << std::endl;

	if (!VirtualLock(addr, block_size))
	{
		win32::exception e(GetLastError(), "VirtualLock");
		std::cerr << e.what() << " " << e.when() << std::endl;
	}
	else
	{
		if (!VirtualUnlock(addr, block_size))
		{
			win32::exception e(GetLastError(), "VirtualUnlock");
			std::cerr << e.what() << " " << e.when() << std::endl;
		}
	}

	if (!VirtualFree(addr, 0, MEM_RELEASE))
	{
		throw win32::exception(GetLastError(), "VirtualFree");
	}

#endif

}
Y_UTEST_DONE()

