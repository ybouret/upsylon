//! \file
#ifndef Y_OXIDE_PARTITION_INCLUDED
#define Y_OXIDE_PARTITION_INCLUDED 1

#include "y/oxide/layout.hpp"
#include "y/sequence/slots.hpp"

namespace upsylon
{
    namespace Oxide
    {
        
        //! helpers for partitions
        struct __Partition
        {
            //! check all sizes are valid and the product matches targetSize, if targetSize>0
            static size_t Check(const Coord1D *localSizes,
                                const unsigned dimensions,
                                const size_t   targetSize);

            //! templated check
            template <typename COORD> static inline
            size_t Count( const COORD localSizes, const size_t targetSize )
            {
                return Check( (const Coord1D *) &localSizes, Coord::Get<COORD>::Dimensions, targetSize );
            }
        };
        
        //! lighweight partitions for a current mapping=localSizes
        template <typename COORD>
        class Partition : public Layout<COORD>, public slots< Layout<COORD> >
        {
        public:
            //------------------------------------------------------------------
            //
            // types and definitions
            //
            //------------------------------------------------------------------
            typedef Layout<COORD>                    LayoutType;   //!< alias
            typedef slots<LayoutType>                SlotsType;    //!< alias
            typedef typename LayoutType::coord       coord;        //!< alias
            typedef typename LayoutType::const_coord const_coord;  //!< alias

            //------------------------------------------------------------------
            //
            // methods
            //
            //------------------------------------------------------------------
            
            //! setup all sub partitions
            explicit Partition(const LayoutType &fullLayout,
                               const_coord       localSizes,
                               const size_t      targetSize=0) :
            LayoutType(fullLayout),
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

            //! cleanup
            inline virtual ~Partition() throw() {}
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Partition);
        };


    }
}

#endif

