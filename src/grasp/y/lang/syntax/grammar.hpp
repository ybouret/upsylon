//! \file
#ifndef Y_LANG_SYNTAX_GRAMMAR_INCLUDED
#define Y_LANG_SYNTAX_GRAMMAR_INCLUDED 1

#include "y/lang/syntax/rrs.hpp"
#include "y/ptr/auto.hpp"

#include "y/lang/syntax/terminal.hpp"
#include "y/lang/syntax/compound.hpp"
#include "y/lang/syntax/joker.hpp"

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
                void   finalize() throw();                     //!< clean up

                //! wrapper to keep derived rule type
                template <typename T>
                inline T & decl( T *r ) { add(r); return *r; }

                //! accept
                Node *accept( Source &source, Lexer &lexer );

                //______________________________________________________________
                //
                // advanced rules management
                //______________________________________________________________
                Terminal  & terminal( const string &id )  { return decl( new Terminal(id) ); }
                Terminal  & terminal( const char   *id )  { const string _(id); return terminal(_); }

                Aggregate & aggregate( const string &id ) { return decl( new Aggregate(id) ); }
                Aggregate & aggregate( const char   *id ) { const string _(id); return aggregate(_); }


                Alternate   & alternate( const string &id );
                Alternate   & alternate( const char   *id );
                Alternate   & alternate(); //!< with automatic label
                const Rule  & choice( const Rule &a, const Rule &b);

                const Rule  & optional(   const Rule &r );
                const Rule  & zeroOrMore( const Rule &r );
                const Rule  & oneOrMore(  const Rule &r );

            private:
                Rule::List        rules;
                RuleReferenceSet *rrs;
                bool              verbose;
                unsigned          iAlt;
                unsigned          iOpt;
                unsigned          iZoM;
                unsigned          iOoM;

                Y_DISABLE_COPY_AND_ASSIGN(Grammar);
                void              no_rrs() throw();
            };

        }
    }
}


#endif


