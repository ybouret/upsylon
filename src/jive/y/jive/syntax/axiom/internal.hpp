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
                explicit Internal(const ID &id,const uint32_t u) :
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
                Internal(id,UUID)
                {
                    Y_JIVE_AXIOM(Dull);
                }




            private:
                Y_DISABLE_COPY_AND_ASSIGN(Dull);
                Y_JIVE_AXIOM_ACCEPT_DECL();
                Y_JIVE_AXIOM_ATTACH_DECL();
            };
        }

    }

}

#endif
