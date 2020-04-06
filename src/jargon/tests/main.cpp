
#include "y/utest/driver.hpp"

Y_UTEST_INIT(32)
{
	Y_UTEST(jargon_types);
    Y_UTEST(jargon_module);
    Y_UTEST(jargon_source);
    Y_UTEST(jargon_pattern);
    Y_UTEST(jargon_posix);
    Y_UTEST(jargon_regexp);
    Y_UTEST(jargon_regcom);
    Y_UTEST(jargon_scanner);
    Y_UTEST(jargon_lexer);
    Y_UTEST(jargon_matching);
    
    Y_UTEST(types);    
    Y_UTEST(xnode);
    Y_UTEST(grammar);
    Y_UTEST(parser);
}
Y_UTEST_EXEC()
