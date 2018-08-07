//! \file
#ifndef Y_PROGRAM_INCLUDED
#define Y_PROGRAM_INCLUDED 1

#include "y/fs/vfs.hpp"
#include "y/exception.hpp"
#include <iostream>

//! wrapper to start program
#define Y_PROGRAM_START()                                         \
int main(int argc, char *argv[]) {                                \
static const char *program = upsylon::vfs::get_base_name(argv[0]);\
(void)argc;                                                       \
try {

//! wrapper to end program
#define Y_PROGRAM_END()                                         \
return 0;                                                       \
}                                                               \
catch( const upsylon::exception &e )                            \
{                                                               \
std::cerr << "in " << program << ": " << e.what() << std::endl; \
std::cerr << e.when() << std::endl;                             \
}                                                               \
catch( const std::exception &e )                                \
{                                                               \
std::cerr << "std::exception in " << program << std::endl;      \
std::cerr << e.what()             << std::endl;                 \
}                                                               \
catch(...)                                                      \
{                                                               \
std::cerr << "uncaught exception in " << program << std::endl;  \
}                                                               \
return 1;                                                       \
}
#endif
