#include "y/jive/lexical/scanner.hpp"
#include "y/type/aliasing.hpp"
#include "y/exception.hpp"

namespace upsylon {

    namespace Jive {

        const silent_t silent = {};
        const stated_t stated = {};

        namespace Lexical {

            void Scanner:: nothing(const Token &) const throw()
            {

            }

            void Scanner:: newLine(const Token &) throw()
            {
                assert(origin);
                origin->newLine();
            }

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

            const Rule &Scanner:: add(Rule *r)
            {
                //--------------------------------------------------------------
                //
                // store rule in list (safety first)
                //
                //--------------------------------------------------------------
                assert(r);
                aliasing::_(rules).push_back(r);
                const string & ruleName = *(r->label);
                try
                {
                    //----------------------------------------------------------
                    // insert into database
                    //----------------------------------------------------------
                    if(!aliasing::_(hoard).insert(ruleName,r))
                    {
                        throw exception("Jive::Lexical::Scanner(multiple rule <%s>)", *ruleName);
                    }
                }
                catch(...)
                {
                    //----------------------------------------------------------
                    // cleanup
                    //----------------------------------------------------------
                    delete aliasing::_(rules).pop_back();
                    throw;
                }

                //--------------------------------------------------------------
                //
                // at that point, in list and database
                //
                //--------------------------------------------------------------
                try
                {
                    Leading cfc(intro);              // current fc
                    Leading lfc;                     // local first chars
                    r->motif->start(lfc);            // find them
                    if(Verbose)
                    {
                        std::cerr << "[" << label << "] + " << r->event->kindText() << '.' << r->event->typeText() << " <" << ruleName << ">";
                        for(size_t i=ruleName.size();i<=12;++i)
                        {
                            std::cerr << ' ';
                        }
                        const string rxm = r->motif->toRegExp();
                        std::cerr << "= " << rxm << " => " << lfc;
                        std::cerr << std::endl;
                    }
                    cfc.include(lfc);                // include to current first chars
                    table.record(r,lfc);             // include in table of rules
                    cfc.commute(aliasing::_(intro)); // done, no-throw
                }
                catch(...)
                {
                    aliasing::_(hoard).remove(ruleName);
                    delete aliasing::_(rules).pop_back();
                    table.remove(r);
                    throw;
                }

                //--------------------------------------------------------------
                //
                // ok, full rule registration
                //
                //--------------------------------------------------------------
                {
                    const size_t l = r->label->size();
                    if(l>lmax) aliasing::_(lmax) = l;
                }
                return *r;
            }


            const Pattern & Scanner:: getPattern(const string &id) const
            {
                const Pattern *p = queryPattern(id);
                if(!p)
                {
                    throw exception("[[%s]] missing pattern for <%s>", **label, *id);
                }
                return *p;
            }

            const Pattern & Scanner:: getPattern(const char *id) const
            {
                const string _(id);
                return getPattern(_);
            }

        }

    }
}


