//! \file
#ifndef Y_LANG_SYNTAX_GRAMMAR_INCLUDED
#define Y_LANG_SYNTAX_GRAMMAR_INCLUDED 1

#include "y/lang/syntax/rrs.hpp"
#include "y/ptr/auto.hpp"
#include "y/sequence/vector.hpp"

#include "y/lang/syntax/terminal.hpp"
#include "y/lang/syntax/compound.hpp"
#include "y/lang/syntax/joker.hpp"

namespace upsylon
{
    namespace Lang
    {
        namespace Syntax
        {

            typedef vector<string,memory::pooled> Strings;         //!< alias, to store some strings
            typedef array<string>                 ArrayOfStrings;  //!< alias, interface for functions

            //! a Grammar is a set of rules
            class Grammar : public Object
            {
            public:
                const Tag name; //!< shared name

                explicit Grammar(const Tag &tid); //!< initialize
                virtual ~Grammar() throw();       //!< destructor


                //______________________________________________________________
                //
                // basic rules management
                //______________________________________________________________
                void setVerbose(const bool flag) throw();      //!< propagate flag to all rules
                void add( Rule *r );                           //!< add a rule with memory management
                Rule & getRuleByName(const string &id);        //!< rule Look up
                Rule & topLevel();                             //!< get top level rule
                void   topLevel( const Rule &r );              //!< set top level rule
                bool   ownsRule( const Rule & ) const throw(); //!< check ownership
                void   finalize();                             //!< clean up

                //! wrapper to keep derived rule type
                template <typename T>
                inline T & decl( T *r ) { add(r); return *r; }

                //! accept
                Node *accept( Source &source, Lexer &lexer );

                //______________________________________________________________
                //
                // Terminals
                //______________________________________________________________
                Terminal  & terminal( const string &id );   //!< make a terminal recognition
                Terminal  & terminal( const char   *id );   //!< make a terminal recognition, wrapper

                //______________________________________________________________
                //
                // Aggregates
                //______________________________________________________________
                Aggregate  & aggregate( const string &id );                      //!< create an aggregate rule
                Aggregate  & aggregate( const char   *id );                      //! create an aggregate rule, wrapper
                Aggregate  & aggregate();                                        //!< create an aggregate with automatic name
                const Rule & join( const Rule &a, const Rule &b);                //!< create a small aggregate
                const Rule & join( const Rule &a, const Rule &b, const Rule &c); //!< create a small aggregate

                //______________________________________________________________
                //
                // Alternates
                //______________________________________________________________
                Alternate   & alternate( const string &id );                        //!< create a named alternate
                Alternate   & alternate( const char   *id );                        //!< create a named alternate
                Alternate   & alternate();                                          //!< with automatic label
                const Rule  & choice( const Rule &a, const Rule &b);                //!< choice between two rules
                const Rule  & choice( const Rule &a, const Rule &b, const Rule &c); //!< choice between three rules

                //______________________________________________________________
                //
                // Jokers
                //______________________________________________________________
                const Rule  & optional(   const Rule &r ); //!< make an Optional   rule
                const Rule  & zeroOrMore( const Rule &r ); //!< make a  ZeroOrMore rule
                const Rule  & oneOrMore(  const Rule &r ); //!< make a  OneOrMore  rule

                //______________________________________________________________
                //
                // other methods
                //______________________________________________________________
                string MakeAlternateName( const ArrayOfStrings &) const; //!< make name for an alternate
                string MakeAggregateName( const ArrayOfStrings &) const; //!< make name for an aggregate

                void graphViz( const string &dotfile ) const;
                void graphViz( const char   *dotfile ) const;

            private:
                Rule::List        rules;
                RuleReferenceSet *rrs;
                bool              verbose;
                unsigned          iAlt;
                unsigned          iAgg;

                Y_DISABLE_COPY_AND_ASSIGN(Grammar);
                string MakeCompoundName(  const ArrayOfStrings &strings, const char sep) const;
                void              no_rrs() throw(); //!< safely remove rrs
                void validate() const; //!< check all rules are used
            };

        }
    }
}


#endif


