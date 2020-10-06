

//! \file
#ifndef Y_JIVE_SYNTAX_REPEAT_INCLUDED
#define Y_JIVE_SYNTAX_REPEAT_INCLUDED 1

#include "y/jive/syntax/axiom/wildcard.hpp"

namespace upsylon
{
    namespace Jive
    {
        namespace Syntax
        {

            //__________________________________________________________________
            //
            //
            //! Repeat an Axiom >= atLeast
            //
            //__________________________________________________________________
            class Repeat : public Wildcard
            {
            public:
                static const uint32_t UUID = Y_FOURCC('R','E','P',' '); //!< UUID

                //! cleanup
                virtual ~Repeat() throw();

                //! setup
                template <typename ID> inline
                explicit Repeat(const ID    &i,
                                const Axiom &a,
                                const size_t n) :
                Wildcard(i,a,UUID),
                atLeast(n)
                {
                    Y_JIVE_AXIOM(Repeat);
                }

                const size_t atLeast; //!< validity threshold

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Repeat);
                Y_JIVE_AXIOM_ACCEPT_DECL();
            };


        }

    }

}

#endif
