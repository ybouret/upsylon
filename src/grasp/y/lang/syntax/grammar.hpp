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
                explicit RuleReferenceSet();          //!< initialize
                virtual ~RuleReferenceSet() throw();  //!< desctructor
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
                void add( Rule *r );

            private:
                Rule::List rules;
                bool       verbose;
                Y_DISABLE_COPY_AND_ASSIGN(Grammar);
            };

        }
    }
}


#endif


