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
            static size_t Check( const Coord1D *, const unsigned, const size_t targetSize);

            template <typename COORD> static inline
            size_t Count( const COORD localSizes, const size_t targetSize )
            {
                return Check( (const Coord1D *) &localSizes, Coord::Get<COORD>::Dimensions, targetSize );
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
                               const_coord       localSizes,
                               const size_t      targetSize=0) :
            LayoutType(full),
            SlotsType( __Partition::Count(localSizes,targetSize) )
            {
                assert( this->count == __Partition::Count(localSizes,targetSize) );
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

