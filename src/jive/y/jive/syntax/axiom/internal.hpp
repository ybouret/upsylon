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

            class Internal : public Axiom
            {
            public:
                
                virtual ~Internal() throw();

            protected:
                template <typename ID> inline
                explicit Internal(const uint32_t u, const ID &id) :
                Axiom(u,Tags::Make(id))
                {

                }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Internal);
            };

            class Dull : public Internal
            {
            public:
                static const uint32_t UUID = Y_FOURCC('D', 'U', 'L', 'L');
                virtual ~Dull() throw();

                template <typename ID> inline
                explicit Dull(const ID &id) :
                Internal(UUID,id)
                {

                }
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Dull);

            };
        }

    }

}

#endif
