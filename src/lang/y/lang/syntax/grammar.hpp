//! \file
#ifndef Y_LANG_SYNTAX_GRAMMAR_INCLUDED
#define Y_LANG_SYNTAX_GRAMMAR_INCLUDED 1

#include "y/lang/syntax/terminal.hpp"
#include "y/lang/syntax/compound.hpp"

namespace upsylon
{
    namespace Lang
    {
        namespace Syntax
        {
            //! for building set of rules
            class Grammar
            {
            public:

                const Origin name; //!< shared name

                //______________________________________________________________
                //
                // Setup API
                //______________________________________________________________
                explicit Grammar(const Origin &id); //!< initialize
                virtual ~Grammar() throw();         //!< desctructor


                const Rule *top() const throw(); //!< get top rule, maybe NULL
                void        top(const Rule &);   //!< set a valid top rule
                void        add( Rule *rule );   //!< add a valid rule

                //! wrapper for derived rules
                template <typename RULE>
                inline RULE & __add( RULE *rule ) { add(rule); return *rule; }

                //! rule look up
                const Rule * getRuleByName(const string &id) const;

                //! rule look up
                inline
                const Rule * getRuleByName(const char   *id) const
                {
                    const string _ = id; return getRuleByName(_);
                }

                //! run
                Node *run(Lexer &lexer, Source &source);


                //______________________________________________________________
                //
                // Building API
                //______________________________________________________________

                //! a new terminal
                inline Rule & terminal( const string &id, const Terminal::Type attr=Terminal::Standard ) { return __add( new Terminal(id,attr) ); }

                //! a new terminal
                inline Rule & terminal( const char   *id, const Terminal::Type attr=Terminal::Standard ) { const string _(id); return terminal(_,attr); }

                Rule & optional( const Rule &r );                    //!< optional rule
                Rule & repeating( const Rule &r, const size_t nmin); //!< repeating >= nmin

                //! '*'
                inline
                Rule & zeroOrMore( const Rule &r ) { return repeating(r,0); }

                //! '+'
                inline
                Rule & oneOrMore( const Rule &r ) { return repeating(r,1); }

                //! new aggregate
                inline Aggregate & agg( const string &id ) { return __add( new Aggregate(id) ); }

                //! new aggregate
                inline Aggregate & agg( const char   *id ) { const string _(id); return agg(_); }

                //! new alternation
                inline Alternate & alt( const string &id ) { return __add( new Alternate(id) ); }

                //! new alternation
                inline Alternate & alt( const char   *id ) { const string _(id); return alt(_); }

                //! automatic alternation
                inline Alternate & alt() { const string id = nextAltID(); return __add( new Alternate(id) ); }

                //! choice
                const Rule & choice(const Rule &a1, const Rule &a2);

                //! check the rule belongs to the grammar
                bool  owns( const Rule &r ) const throw();

                //! output as GraphViz
                void GraphViz( const string &filename ) const;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Grammar);
                class MetaRule
                {
                public:
                    const Rule          & rule;
                    inline const string & key() const throw() { return rule.name; }
                    inline MetaRule(const Rule     &r    ) throw() : rule(r) {}
                    inline MetaRule(const MetaRule &other) throw() : rule(other.rule) {}
                    inline ~MetaRule() throw() {}

                    typedef set<string,MetaRule> Set;
                private:
                    Y_DISABLE_ASSIGN(MetaRule);
                };

                Rule::List    rules;
                MetaRule::Set rdb;
                unsigned      altID;
                string        nextAltID();
            };
        }
    }
}

#endif

