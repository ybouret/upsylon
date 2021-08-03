
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
            static const char CLID[];

            explicit Reactor(Library    &,
                             Equilibria &);

            virtual ~Reactor() throw();

            const Library    &lib; //!< support library
            const Equilibria &eqs; //!< support equlibria
            const size_t      N;   //!< number of equilibria
            const size_t      M;   //!< number of components

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Reactor);
            const Freezer lfrz;
            const Freezer efrz;
        };

    }

}

#endif
