
#include "y/os/page-size.hpp"
#include "y/memory/locked.hpp"
#include "y/utest/run.hpp"




using namespace upsylon;


Y_UTEST(mlock)
{


	int block_size = 5000;
	if (argc > 1)
	{
		block_size = atoi(argv[1]);
	}

    std::cerr << "page_size  = " << page_size::get() << std::endl;
    std::cerr << "block_size = " << block_size << std::endl;
    memory::locked_area blk( block_size );
    std::cerr << "blk.bytes  = " << blk.bytes << std::endl;


#if 0
#if defined(Y_WIN)
	SYSTEM_INFO sSysInfo;         // useful information about the system
	GetSystemInfo(&sSysInfo);     // initialize the structure
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
#endif


}
Y_UTEST_DONE()

