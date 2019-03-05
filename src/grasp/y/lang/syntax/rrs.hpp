
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
                typedef key_hasher<string,hashing::fnv>            KeyHasher; //!< alias
                typedef memory::pooled                             Memory;    //!< alias
                typedef set<string,RuleReference,KeyHasher,Memory> Set;       //!< alias

                explicit RuleReference(const Rule &r) throw();      //!< setup
                RuleReference( const RuleReference &other) throw(); //!< copy
                ~RuleReference() throw();                           //! destructor
                const string & key() const throw();                 //!< rule.name

                const Rule &rule; //!< the reference

            private:
                Y_DISABLE_ASSIGN(RuleReference);
            };

            //! Database of rules
            class RuleReferenceSet : public RuleReference::Set
            {
            public:
                explicit RuleReferenceSet() throw();  //!< initialize
                explicit RuleReferenceSet(size_t n);  //!< initialize with capacity
                virtual ~RuleReferenceSet() throw();  //!< destructor
                bool     declare(const Rule *r);      //!< try to insert a new reference

            private:
                Y_DISABLE_COPY_AND_ASSIGN(RuleReferenceSet);
            };
        }
    }
}

#endif

