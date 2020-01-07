
//! \file

#ifndef Y_GRAPHICS_PARALLEL_TILES_INCLUDED
#define Y_GRAPHICS_PARALLEL_TILES_INCLUDED 1

#include "y/graphic/parallel/tile.hpp"
#include "y/sequence/slots.hpp"
#include "y/concurrent/scheme/for-each.hpp"
#include "y/randomized/bits.hpp"
#include "y/ptr/auto.hpp"

namespace upsylon {

    namespace Graphic {

        typedef slots<Tile>                   Tiles_;   //!< base class for Tiles
        typedef concurrent::for_each          ForEach_; //!< alias
        typedef concurrent::for_each::pointer ForEach;  //!< alias for shared loop


        //! Tiles to partition an area for parallel tasks
        class Tiles : public Tiles_
        {
        public:
            const Area   area; //!< original area

            //! setup for a given area and and a level of parallelism
            explicit Tiles(const Area    &full,
                           const ForEach &devs );

            //! cleanup
            virtual ~Tiles() throw();

            //! randomize to change CPU addressing
            void randomize( randomized::bits &ran ) throw();

            //! get internal loop
            ForEach_ & loop() throw();

            
            //! cache of C memory for each tile
            void cacheAcquire(const size_t BytesPerTile);


            //! cache of C memory for each tile, sizeof(T)
            template <typename T> inline
            void cacheAcquireFor() { cacheAcquire( sizeof(T) ); }

            //! cache of C memory for each tile, n*sizeof(T)
            template <typename T> inline
            void cacheAcquireFor(const size_t n) { cacheAcquire( n*sizeof(T) ); }


            //! assume a local max is stored in local memory
            template <typename T>
            T globalMax() const throw()
            {
                const Tiles_ &self = *this;
                const size_t  num  = self.size();
                T             vmax = self[0].get<T>();
                for(size_t i=1;i<num;++i)
                {
                    const T tmp = self[i].get<T>();
                    if(tmp>vmax)
                    {
                        vmax = tmp;
                    }
                }
                return vmax;
            }

            //! assume a pair of min/max is stored in local memory
            template <typename T>
            void globalMinMax( T &vmin, T &vmax ) const throw()
            {
                const Tiles_ &self = *this;
                const size_t  num  = self.size();
                vmin = self[0].get<T>(0);
                vmax = self[0].get<T>(1);
                for(size_t i=1;i<num;++i)
                {
                    {
                        const T tmp = self[i].get<T>(0);
                        if(tmp<vmin)
                        {
                            vmin = tmp;
                        }
                    }
                    {
                        const T tmp = self[i].get<T>(1);
                        if(tmp>vmax)
                        {
                            vmax = tmp;
                        }
                    }
                }
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Tiles);
            ForEach                     device;
            //auto_ptr<Kernel::DataBlock> dataBlock;
        };

    }
}

#endif

