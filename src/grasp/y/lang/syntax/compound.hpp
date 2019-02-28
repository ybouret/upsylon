//! \file

#ifndef Y_LANG_SYNTAX_COMPOUND_INCLUDED
#define Y_LANG_SYNTAX_COMPOUND_INCLUDED 1

#include "y/lang/syntax/rule.hpp"

namespace upsylon
{
    namespace Lang
    {
        namespace Syntax
        {
            //! holds a reference to a Rule
            class Operand : public Object, public core::inode<Operand>
            {
            public:
                typedef core::inode<Operand>       Base; //!< alias
                typedef core::list_of_cpp<Operand> List; //!< alias

                explicit Operand( const Rule &r) throw(); //!< setup
                virtual ~Operand() throw();               //!< destructor

                const Rule &rule; //!< previously declared (persistent) rule

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Operand);
            };

            //! holds a list of operands
            class Compound : public Rule, public Operand::List
            {
            public:
                virtual ~Compound() throw(); //!< destructor

            protected:
                explicit Compound(const uint32_t, const string &); //!< setup

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Compound);
            };

            


        }
    }
}


#endif


