#include <iostream>

extern "C" void f() throw();

int main()
{
	std::cerr << "Calling f()" << std::endl;
	f();
	std::cerr << "Done" << std::endl;
	return 0;
}
