//! \file
#ifndef Y_OXIDE_PARTITION_INCLUDED
#define Y_OXIDE_PARTITION_INCLUDED 1

#include "y/oxide/layout.hpp"
#include "y/sequence/slots.hpp"

namespace upsylon
{
    namespace Oxide
    {

        struct __Partition
        {
            static size_t Count( const Coord1D *, const unsigned );
            template <typename COORD> static inline
            size_t Count( const COORD localSizes )
            {
                return Count( (const Coord1D *) &localSizes, Coord::Get<COORD>::Dimensions );
            }
        };
        template <typename COORD>
        class Partition : public Layout<COORD>, public slots< Layout<COORD> >
        {
        public:
            typedef Layout<COORD>                    LayoutType;
            typedef slots<LayoutType>                SlotsType;
            typedef typename LayoutType::coord       coord;
            typedef typename LayoutType::const_coord const_coord;

            explicit Partition(const LayoutType &full,
                               const_coord       localSizes) :
            LayoutType(full),
            SlotsType( __Partition::Count(localSizes) )
            {
                assert( this->count == __Partition::Count(localSizes) );
                while( this->size() < this->count )
                {
                    const_coord      localRanks = Coord::LocalRanks(localSizes,this->size());
                    const LayoutType partLayout = this->split(localSizes, localRanks );
                    this->push(partLayout);
                }
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Partition);
        };


    }
}

#endif

