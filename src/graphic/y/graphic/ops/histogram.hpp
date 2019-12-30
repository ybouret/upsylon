//! \file

#ifndef Y_GRAPHIC_HISTOGRAM_INCLUDED
#define Y_GRAPHIC_HISTOGRAM_INCLUDED 1

#include "y/graphic/parallel/tiles.hpp"
#include "y/graphic/pixmaps.hpp"
#include "y/graphic/pixel.hpp"

namespace upsylon {

    namespace Graphic {

        enum KeepMode
        {
            KeepForeground,
            KeepBackground
        };

        //! histogram of bytes
        class Histogram : public Object
        {
        public:
            static const size_t BINS = 256; //!< number of bins

            explicit Histogram() throw(); //!< setup
            virtual ~Histogram() throw(); //!< cleanup

            void zero() throw(); //!< set to zero

            size_t &       operator[]( const uint8_t u ) throw();       //!< access
            const size_t & operator[]( const uint8_t u ) const throw(); //!< const access

            //! bins[BINS], unsafe
            void set_(const size_t *bins) throw();

            //! bins[BINS], unsafe
            void add_(const size_t *bins) throw();

            //! set to another histogram
            void set( const Histogram &H ) throw();

            //! add another histogram
            void add( const Histogram &H ) throw();

            //! Otsu 1D thresholding
            uint8_t Otsu1D() const throw();

            //! total count
            size_t count() const throw();

            //! reserve and clean memory
            void prolog( Tiles &tiles );

            //! sum local bins
            void epilog(Tiles &tiles) throw();

            template <typename T,
            typename PROC>
            void build( const Pixmap<T> &source, PROC &proc, Tiles &tiles )
            {
                // prepare memory
                prolog(tiles);

                // run in parallel
                struct Task
                {
                    const Pixmap<T> *source_;
                    PROC            *proc_;
                    Tiles           *tiles_;

                    static inline
                    void Run( void *args, parallel &ctx, lockable & ) throw()
                    {
                        Task      &task = *static_cast<Task *>(args);
                        Tile      &tile = (*task.tiles_)[ctx.rank];
                        Histogram::Build<T,PROC>( *task.source_,  *task.proc_, tile );
                    }


                };

                Task task = { &source, &proc, &tiles };
                tiles.loop().run( Task::Run, &task );

                // collect data
                epilog(tiles);
            }

            template <typename T, typename PROC>
            void keep(Pixmap<T>       &target,
                      const Pixmap<T> &source,
                      const uint8_t    t,
                      PROC            &proc,
                      const KeepMode   keep,
                      Tiles           &tiles )
            {
                struct Task
                {
                    Pixmap<T>       *target_;
                    const Pixmap<T> *source_;
                    uint8_t          t_;
                    PROC            *proc_;
                    KeepMode         keep_;
                    Tiles           *tiles_;

                    static inline
                    void Run( void *args, parallel &ctx, lockable & ) throw()
                    {
                        Task      &task = *static_cast<Task *>(args);
                        Tile      &tile = (*task.tiles_)[ctx.rank];
                        Histogram::Keep<T,PROC>( *task.target_, *task.source_, task.t_, *task.proc_, task.keep_, tile);
                    }
                };

                Task task = { &target, &source, t, &proc, keep, &tiles };
                tiles.loop().run( Task::Run, &task );

            }



        private:
            Y_DISABLE_COPY_AND_ASSIGN(Histogram);
            size_t bin[BINS];
            template <typename T, typename PROC> static inline
            void Build( const Pixmap<T> &source, PROC &proc, Tile &tile ) throw()
            {
                assert( tile.localMemory() >= BINS*sizeof(T) );
                size_t     *H  = & tile.as<size_t>(0);
                const Point up = tile.upper;
                const Point lo = tile.lower;
                 for(unit_t y=up.y;y>=lo.y;--y)
                {
                    const typename Pixmap<T>::RowType &src = source[y];
                    for(unit_t x=up.x;x>=lo.x;--x)
                    {
                        ++H[ uint8_t( proc(src[x]) ) ];
                    }
                }
            }

            template <typename T, typename PROC> static inline
            void Keep(Pixmap<T>       &target,
                      const Pixmap<T> &source,
                      const uint8_t    t,
                      PROC            &proc,
                      const KeepMode   keep,
                      const Tile      &tile ) throw()
            {
                const Point up = tile.upper;
                const Point lo = tile.lower;
                for(unit_t y=up.y;y>=lo.y;--y)
                {
                    typename Pixmap<T>::RowType       &tgt = target[y];
                    const typename Pixmap<T>::RowType &src = source[y];
                    for(unit_t x=up.x;x>=lo.x;--x)
                    {
                        const T      &data  = src[x];
                        const uint8_t value = uint8_t( proc(data) );
                        switch(keep)
                        {
                            case KeepForeground:
                                if(value>=t)
                                {
                                    tgt[x] = data;
                                }
                                else
                                {
                                    Pixel::Zero( tgt[x] );
                                }
                                break;

                            case KeepBackground:
                                if(value<=t)
                                {
                                    tgt[x] = Pixel::Invert( data );
                                }
                                else
                                {
                                    Pixel::Zero( tgt[x] );
                                }
                                break;
                        }
                    }
                }
            }

        };

    }

}

#endif

