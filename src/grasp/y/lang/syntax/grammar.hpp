//! \file
#ifndef Y_LANG_SYNTAX_GRAMMAR_INCLUDED
#define Y_LANG_SYNTAX_GRAMMAR_INCLUDED 1

#include "y/lang/syntax/rrs.hpp"
#include "y/ptr/auto.hpp"

namespace upsylon
{
    namespace Lang
    {
        namespace Syntax
        {

            class Grammar : public Object
            {
            public:
                const Tag name; //!< shared tag

                explicit Grammar(const Tag &tid); //!< initialize
                virtual ~Grammar() throw();       //!< destructor

                void setVerbose(const bool flag) throw(); //!< propagate flag to all rules

                //______________________________________________________________
                //
                // basic rules management
                //______________________________________________________________
                void add( Rule *r ); //!< add a rule with memory management
                Rule & getRuleByName(const string &id);        //!< rule Look up
                Rule & topLevel();                             //!< get top level rule
                void   topLevel( Rule &r );                    //!< set top level rule
                bool   ownsRule( const Rule & ) const throw(); //!< check ownership
                void finalize() throw(); //!< clean up

                //! wrapper to keep derived rule type
                template <typename T>
                inline T & decl( T *r ) { add(r); return *r; }

                //! accept
                Node *accept( Source &source, Lexer &lexer );


            private:
                Rule::List        rules;
                RuleReferenceSet *rrs;
                bool              verbose;
                void              no_rrs() throw();
                Y_DISABLE_COPY_AND_ASSIGN(Grammar);
            };

        }
    }
}


#endif


