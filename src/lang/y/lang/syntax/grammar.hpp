//! \file
#ifndef Y_LANG_SYNTAX_GRAMMAR_INCLUDED
#define Y_LANG_SYNTAX_GRAMMAR_INCLUDED 1

#include "y/lang/syntax/terminal.hpp"
#include "y/lang/syntax/compound.hpp"
#include "y/sequence/vector.hpp"

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
                typedef vector<string,memory::pooled> Strings; //!< internally used

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
                    const string _(id); return getRuleByName(_);
                }

                //! run
                Node *run(Lexer &lexer, Source &source);

                //! propagate verbose to all rules
                void  setVerbose( bool flag ) throw();

                //______________________________________________________________
                //
                // Building API
                //______________________________________________________________

                //! a new terminal
                inline Terminal & terminal( const string &id, const Terminal::Attribute attr=Terminal::Standard ) { return __add( new Terminal(id,attr) ); }

                //! a new terminal
                inline Rule & terminal( const char   *id, const Terminal::Attribute attr=Terminal::Standard ) { const string _(id); return terminal(_,attr); }

                Rule & optional( const Rule &r );                    //!< optional rule
                Rule & repeating( const Rule &r, const size_t nmin); //!< repeating >= nmin

                //! '*'
                inline
                Rule & zeroOrMore( const Rule &r ) { return repeating(r,0); }

                //! '+'
                inline
                Rule & oneOrMore( const Rule &r ) { return repeating(r,1); }

                //! new aggregate
                inline Aggregate & agg( const string &id, const Compound::Type flag=Compound::Normal) { return __add( new Aggregate(id,flag) ); }

                //! new aggregate
                inline Aggregate & agg( const char   *id, const Compound::Type flag=Compound::Normal) { const string _(id); return agg(_,flag); }

                //! new acting aggregate
                inline Aggregate & acting(const string &id) { return agg(id,Compound::Acting); }

                //! new acting aggregate
                inline Aggregate & acting(const char   *id) { const string _(id); return acting(_); }

                //! new acting aggregate
                inline Aggregate & design(const string &id) { return agg(id,Compound::Design); }

                //! new acting aggregate
                inline Aggregate & design(const char   *id) { const string _(id); return design(_); }

                //! new alternation
                inline Alternate & alt( const string &id ) { return __add( new Alternate(id) ); }

                //! new alternation
                inline Alternate & alt( const char   *id ) { const string _(id); return alt(_); }

                //! automatic alternation
                inline Alternate & alt() { const string id = nextAltID(); return __add( new Alternate(id) ); }

                //! choice
                const Rule & choice(const Rule &a1, const Rule &a2);

                //! choice
                const Rule & choice(const Rule &a1, const Rule &a2, const Rule &a3);


                //! check the rule belongs to the grammar
                bool  owns( const Rule &r ) const throw();

                //! output as GraphViz
                void GraphViz( const string &filename ) const;

                //! check the grammar
                /**
                 -- has a top level, non hollow rule
                 -- no undeclared rule
                 -- no standalone rule
                 */
                void checkValidity() const;

                //! check if terminal
                bool isStandardTerminal( const Lexeme *lx ) const throw();

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

                static string MakeAltNameFrom( array<string> &names );
            protected:
                bool          verbose;
            };
        }
    }
}

#endif

