//! \file

#ifndef Y_SPADE_GHOST_INCLUDED
#define Y_SPADE_GHOST_INCLUDED 1

#include "y/spade/types.hpp"
#include "y/sequence/vector.hpp"
#include "y/ptr/arc.hpp"

namespace upsylon {

    namespace Spade
    {

        typedef vector<const size_t> Indices;

        namespace Kernel {

            class Ghost : public Indices
            {
            public:
                explicit Ghost() throw();
                virtual ~Ghost() throw();

                template <typename LAYOUT>
                void assign(const LAYOUT &zone, const LAYOUT &full)
                {
                    assert( full.contains(zone) );
                    typename LAYOUT::Loop loop(zone.lower,zone.upper);
                    free();
                    ensure(loop.count);
                    for(loop.boot();loop.good();loop.next())
                    {
                        push_back_( full.indexOf( loop.value ) );
                    }
                }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Ghost);
            };

        }

        typedef arc_ptr<Kernel::Ghost> Ghost;



    }
}

#endif

