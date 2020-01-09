//! \file

#ifndef Y_GRAPHIC_HISTOGRAM_INCLUDED
#define Y_GRAPHIC_HISTOGRAM_INCLUDED 1

#include "y/graphic/parallel/tiles.hpp"
#include "y/graphic/pixmaps.hpp"
#include "y/graphic/pixel.hpp"
#include "y/container/tuple.hpp"

namespace upsylon {

    namespace Graphic {

        //! which part to keep for thresholding
        enum KeepMode
        {
            KeepForeground, //!< shall keep foreground
            KeepBackground  //!< shall keep background
        };


        //! histogram of bytes
        class Histogram : public Object
        {
        public:
            static const size_t BINS = 256; //!< number of bins

            //! declare metrics info
            Y_PAIR_DECL(STANDARD,Metrics,double,average,double,variance);

            //! end of metrics
            Y_PAIR_END();
            

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

            //! compute metrics from a to b
            Metrics getMetrics( const uint8_t a, const uint8_t b ) const throw();

            //! reserve and clean memory
            void prolog( Tiles &tiles );

            //! sum local bins
            void epilog(Tiles &tiles) throw();

            //! build histogram in parallel
            template <typename T, typename PROC>
            inline void build(const Pixmap<T> &source,
                              PROC            &proc,
                              Tiles           &tiles )
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
                        Histogram::Build<T,PROC>(*task.source_,*task.proc_,tile);
                    }


                };

                Task task = { &source, &proc, &tiles };
                tiles.loop().run( Task::Run, &task );

                // collect data
                epilog(tiles);
            }

            //! keep into target the valid part of source
            template <typename T, typename PROC>
            inline void keep(Pixmap<T>       &target,
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
                        switch(task.keep_)
                        {
                            case KeepForeground: Histogram::KeepFG(*task.target_, *task.source_, task.t_, *task.proc_, tile); break;
                            case KeepBackground: Histogram::KeepBG(*task.target_, *task.source_, task.t_, *task.proc_, tile); break;
                        }
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
                assert( tile.size >= BINS*sizeof(T) );
                size_t     *H  = & tile.get<size_t>(0);
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


            template <typename T, typename PROC>
            static inline
            void KeepFG(Pixmap<T>       &target,
                        const Pixmap<T> &source,
                        const uint8_t    t,
                        PROC            &proc,
                        const Tile      &tile )
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
                        if(value>=t)
                        {
                            tgt[x] = data;
                        }
                        else
                        {
                            Pixel::Zero( tgt[x] );
                        }
                    }
                }
            }

            template <typename T, typename PROC> static inline
            void KeepBG(Pixmap<T>       &target,
                        const Pixmap<T> &source,
                        const uint8_t    t,
                        PROC            &proc,
                        const Tile      &tile )
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
                        if(value<=t)
                        {
                            tgt[x] = Pixel::Invert( data );
                        }
                        else
                        {
                            Pixel::Zero( tgt[x] );
                        }
                    }
                }
            }

            
        };

    }

}

#endif

