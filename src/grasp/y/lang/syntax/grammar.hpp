//! \file
#ifndef Y_LANG_SYNTAX_GRAMMAR_INCLUDED
#define Y_LANG_SYNTAX_GRAMMAR_INCLUDED 1

#include "y/lang/syntax/rule.hpp"

namespace upsylon
{
    namespace Lang
    {
        namespace Syntax
        {

            //! lightweight wapper to manager database of rules
            class RuleReference
            {
            public:
                typedef set<string,RuleReference>            Set;     //!< alias

                const Rule &rule;
                explicit RuleReference(const Rule &r) throw();
                RuleReference( const RuleReference &other) throw();
                ~RuleReference() throw();
                const string & key() const throw();


            private:
                Y_DISABLE_ASSIGN(RuleReference);
            };

            //! Database of rules
            class RuleReferenceSet : public RuleReference::Set
            {
            public:
                explicit RuleReferenceSet() throw();  //!< initialize
                virtual ~RuleReferenceSet() throw();  //!< destructor
                bool     declare(const Rule *r);

            private:
                Y_DISABLE_COPY_AND_ASSIGN(RuleReferenceSet);
            };


            class Grammar : public Object
            {
            public:
                const Tag name; //!< shared tag

                explicit Grammar(const Tag &tid); //!< initialize
                virtual ~Grammar() throw();       //!< destructor

                void setVerbose(const bool flag) throw(); //!< propagate flag to all rules

                //______________________________________________________________
                //
                // rules management
                //______________________________________________________________
                void add( Rule *r ); //!< add a rule with memory management
                Rule & getRuleByName(const string &id);        //!< rule Look up
                Rule & topLevel();                             //!< get top level rule
                void   topLevel( Rule &r );                    //!< set top level rule
                bool   ownsRule( const Rule & ) const throw(); //!< check ownership

                //! wrapper to keep derived rule type
                template <typename T>
                inline T & decl( T *r ) { add(r); return *r; }


                void finalize() throw(); //!< clean up

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


