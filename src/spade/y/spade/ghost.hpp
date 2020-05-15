//! \file

#ifndef Y_SPADE_GHOST_INCLUDED
#define Y_SPADE_GHOST_INCLUDED 1

#include "y/spade/types.hpp"
#include "y/sequence/array.hpp"

namespace upsylon {

    namespace Spade
    {

        namespace Kernel
        {
            class Ghost : public accessible<size_t>
            {
            public:
                virtual ~Ghost() throw();
                explicit Ghost(const size_t);

                virtual size_t         size()                   const throw();
                virtual const size_t & operator[](const size_t) const throw();

            private:
                size_t count_;
                size_t bytes_;
            protected:
                size_t       *indices;

            public:
                const size_t  items;
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Ghost);
            };
        };

        class Ghost :  public Kernel::Ghost
        {
        public:
            virtual ~Ghost() throw();

            template <typename LAYOUT> inline
            Ghost(const LAYOUT &zone, const LAYOUT &full) :
            Kernel::Ghost(zone.items)
            {
                assert( full.contains(zone) );
                typename LAYOUT::Loop loop(zone.lower,zone.upper);
                for(loop.boot();loop.good();loop.next())
                {
                    indices[loop.index] = full.indexOf(loop.value);
                }
            }
            

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Ghost);
        };



    }
}

#endif

