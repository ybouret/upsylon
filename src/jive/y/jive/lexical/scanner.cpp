#include "y/jive/lexical/scanner.hpp"
#include "y/type/aliasing.hpp"
#include "y/exception.hpp"

namespace upsylon {

    namespace Jive {

        namespace Lexical {

            const char * Scanner:: AtEOSText(const AtEOS eos) throw()
            {
                switch (eos) {
                    case RejectEOS: return "RejectEOS";
                    case AcceptEOS: return "AcceptEOS";
                }
                return "";
            }

            const char *Scanner:: getEOS() const throw()
            {
                return AtEOSText(atEOS);
            }


            Scanner:: ~Scanner() throw()
            {
            }

            const string & Scanner:: key() const throw()
            {
                return *label;
            }

            bool Scanner::Verbose = false;

#define YJS_PRINTLN(CODE) do { if(Scanner::Verbose) { std::cerr<< CODE << std::endl; } } while(false)

            void Scanner:: add(Rule *r)
            {
                //--------------------------------------------------------------
                //
                // store rule in list
                //
                //--------------------------------------------------------------
                assert(r);
                rules.push_back(r);
                const string & ruleName = *(r->label);
                try
                {
                    //----------------------------------------------------------
                    // insert into database
                    //----------------------------------------------------------
                    if(!rdb.insert_by(ruleName,r))
                    {
                        throw exception("Jive::Lexical::Scanner(multiple rule '%s')", *ruleName);
                    }
                }
                catch(...)
                {
                    //----------------------------------------------------------
                    // cleanup
                    //----------------------------------------------------------
                    delete rules.pop_back();
                    throw;
                }

                //--------------------------------------------------------------
                //
                // at that point, in list and database
                //
                //--------------------------------------------------------------
                try
                {
                    Leading cfc(rfc);     // current fc
                    Leading lfc;          // local first chars
                    r->motif->start(lfc); // find them
                    cfc.include(lfc);     // include to current first chars
                    cfc.commute(rfc);     // done, no-throw
                }
                catch(...)
                {
                    (void)rdb.remove_by(ruleName);
                    delete rules.pop_back();
                    throw;
                }
            }


            
            

        }

    }
}


