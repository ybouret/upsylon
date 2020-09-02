//! \file

#ifndef Y_JIVE_LEXICAL_SCANNER_INCLUDED
#define Y_JIVE_LEXICAL_SCANNER_INCLUDED 1

#include "y/jive/lexical/rule.hpp"
#include "y/jive/lexical/events.hpp"
#include "y/ptr/intr.hpp"

namespace upsylon
{
    namespace Jive
    {
        namespace Lexical
        {

            //! directive for Lexer during probe
            /**
             a directive is emitted when a control event (jump/call/back) is
             triggered
             */
            typedef const ControlEvent *Directive;

            class Scanner : public CountedObject
            {
            public:

                static  bool                     Verbose; //!< global lexical verbosity
                typedef intr_ptr<string,Scanner> Pointer;  //!< for database
                typedef suffix_tree<Rule*>       RulesDB;  //!< rules database

                //! what happens when End of Stream if met during a call
                enum AtEOS
                {
                    RejectEOS, //!< will fail (and this is the default behavior!)
                    AcceptEOS  //!< ok, may stop
                };

                //! return text
                static const char *AtEOSText(const AtEOS) throw();

                //--------------------------------------------------------------
                //
                // C++
                //
                //------------------------------------------------------------------
                virtual ~Scanner() throw();                                   //!< cleanup

                template <typename ID> inline
                explicit Scanner(const ID &id, const AtEOS which=RejectEOS) :
                label(Tags::Make(id)),
                atEOS(which),
                rules(),
                rdb(),
                rfc()
                {
                }


                void           add(Rule *r);
                const string & key() const throw(); //!< for Pointer

                //--------------------------------------------------------------
                //
                // generic method
                //
                //------------------------------------------------------------------
                const char   *getEOS() const throw();                //!< named endOfStream

                const Tag    label;
                const AtEOS  atEOS;
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Scanner);
                Rules        rules; //!< list of rules
                RulesDB      rdb;   //!< database of rules
                Leading      rfc;   //!<
            };
        }
    }

}

#endif

