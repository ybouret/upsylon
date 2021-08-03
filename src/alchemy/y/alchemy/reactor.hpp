
//! \file

#ifndef Y_ALCHEMY_REACTOR_INCLUDED
#define Y_ALCHEMY_REACTOR_INCLUDED 1

#include "y/alchemy/library.hpp"
#include "y/alchemy/equilibria.hpp"

namespace upsylon
{
    namespace Alchemy
    {

        //______________________________________________________________________
        //
        //
        //! chemical reactor
        //
        //______________________________________________________________________
        class Reactor
        {
        public:
            explicit Reactor(Library    &,
                             Equilibria &);

            virtual ~Reactor() throw();

            const size_t M; //!< number of components
            const size_t N; //!< number of equilibria


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Reactor);
        };

    }

}

#endif
