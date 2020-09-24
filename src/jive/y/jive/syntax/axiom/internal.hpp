//! \file
#ifndef Y_JIVE_SYNTAX_INTERNAL_INCLUDED
#define Y_JIVE_SYNTAX_INTERNAL_INCLUDED 1

#include "y/jive/syntax/axiom.hpp"

namespace upsylon
{
    namespace Jive
    {
        namespace Syntax
        {

            //__________________________________________________________________
            //
            //
            //! base class for internal Axiom
            //
            //__________________________________________________________________
            class Internal : public Axiom
            {
            public:
                virtual ~Internal() throw(); //!< cleanup

            protected:
                //! setup
                template <typename ID> inline
                explicit Internal(const uint32_t u, const ID &id) :
                Axiom(u,Tags::Make(id))
                {
                    assert(isInternal());
                }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Internal);
            };

            //__________________________________________________________________
            //
            //
            //! a dull internal class
            //
            //__________________________________________________________________
            class Dull : public Internal
            {
            public:
                //! identifier
                static const uint32_t UUID = Y_FOURCC('D', 'U', 'L', 'L');

                //! cleanup
                virtual ~Dull() throw();

                //! setup
                template <typename ID> inline
                explicit Dull(const ID &id) :
                Internal(UUID,id)
                {
                    Y_JIVE_AXIOM(Dull);
                }
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Dull);

            };
        }

    }

}

#endif
