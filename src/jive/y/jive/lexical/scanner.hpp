//! \file

#ifndef Y_JIVE_LEXICAL_SCANNER_INCLUDED
#define Y_JIVE_LEXICAL_SCANNER_INCLUDED 1

#include "y/jive/lexical/rule.hpp"
#include "y/jive/lexical/events.hpp"
#include "y/jive/scatter.hpp"
#include "y/ptr/intr.hpp"

namespace upsylon
{
    namespace Jive
    {
        namespace Lexical
        {
            //__________________________________________________________________
            //
            //! directive for Lexer during probe
            /**
             a directive is emitted when a control event (jump/call/back) is
             triggered
             */
            //__________________________________________________________________

            typedef const ControlEvent *Directive;

            //__________________________________________________________________
            //
            //
            //! detect and apply best rule to produce a token or a directive
            //
            //__________________________________________________________________
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
                //--------------------------------------------------------------

                //! cleanup
                virtual ~Scanner() throw();

                //! setup using Tags
                template <typename ID> inline
                explicit Scanner(const ID &id, const AtEOS which=RejectEOS) :
                label(Tags::Make(id)),
                atEOS(which),
                rules(),
                rdb(),
                rfc()
                {
                }


                //--------------------------------------------------------------
                //
                // methods
                //
                //--------------------------------------------------------------

                //! rule full registration
                /**
                 - r is added to internal rules
                 - the first chars of r->motifs are included in first chars db
                 - r is scattered into the table for all its first chars
                 */
                void           add(Rule *r);


                const string & key()    const throw();  //!< get key for pointer
                const char   * getEOS() const throw();  //!< named atEOS

                //--------------------------------------------------------------
                //
                // methods
                //
                //--------------------------------------------------------------
                const Tag    label; //!< label for this rule
                const AtEOS  atEOS; //!< what happens if EOS is met
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Scanner);
                Rules        rules; //!< list of rules
                RulesDB      rdb;   //!< database of rules
                Leading      rfc;   //!< database of first chars
                Scatter      table; //!< table of rules
            };
        }
    }

}

#endif

