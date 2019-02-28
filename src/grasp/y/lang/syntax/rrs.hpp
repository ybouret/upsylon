
//! \file
#ifndef Y_LANG_SYNTAX_RRS_INCLUDED
#define Y_LANG_SYNTAX_RRS_INCLUDED 1

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
        }
    }
}

#endif

