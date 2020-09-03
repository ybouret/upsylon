
#include "y/jive/lexical/scanner.hpp"
#include "y/exception.hpp"

namespace upsylon {

    namespace Jive {

        namespace Lexical {


#define YJS_PRINTLN(CODE) do { if(Scanner::Verbose) { std::cerr << "[" << label << "] " << CODE << std::endl; } } while(false)

            Unit * Scanner:: probe(Source &source, Directive &directive)
            {
                assert(0==directive);
                YJS_PRINTLN("probe");
                //--------------------------------------------------------------
                //
                // find a valid first char
                //
                //--------------------------------------------------------------
                if( !source.find(intro) )
                {

                    //----------------------------------------------------------
                    //
                    //
                    //
                    //----------------------------------------------------------
                }
                else
                {
                    //----------------------------------------------------------
                    //
                    // found!
                    //
                    //----------------------------------------------------------
                }


                return 0;
            }


        }

    }

}


