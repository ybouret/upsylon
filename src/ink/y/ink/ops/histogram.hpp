//! \file
#ifndef Y_INK_HIST_INCLUDED
#define Y_INK_HIST_INCLUDED

#include "y/ink/pixmaps.hpp"

namespace upsylon
{
    namespace Ink
    {
        //! Histogram manipulation
        class Histogram : public Object
        {
        public:
            typedef size_t count_t;             //!< bins content
            static  const  size_t   BINS = 256; //!< 8-bit version
            static  const  size_t   BYTES = sizeof(count_t)*BINS; //!< for local engine memory

            count_t bins[BINS]; //!< bins with their count

            explicit Histogram() throw(); //!< set bins to 0
            virtual ~Histogram() throw(); //!< destructor

            void reset() throw(); //!< set bins to 0
            void append( const count_t *other_bins ) throw(); //!< low level append

            //! build histogram with projection function
            template <typename T,typename FUNC>
            inline void build( const Pixmap<T> &source, FUNC &type2byte, Engine &E, const bool init=true )
            {
                E.acquire_all( sizeof(bins) );
                if(init)
                {
                    reset();
                }
                __build<T,FUNC> proxy = { &source, &type2byte };
                E.run(proxy);
                append_from(E);
            }

            //! append from local collection
            void append_from( const Engine &E ) throw();

            //! zero-th order moment (total)
            inline count_t moment0() const throw()
            {
                count_t ans = 0;
                for(size_t i=0;i<BINS;++i) ans += bins[i];
                return ans;
            }

            //! first order moment
            inline count_t moment1() const throw()
            {
                count_t ans = 0;
                for(size_t i=1;i<BINS;++i) ans += i*bins[i];
                return ans;
            }

            //! both moment0 and moment1
            inline void moments(count_t &mu0, count_t &mu1) const throw()
            {
                mu0 = bins[0];
                mu1 = 0;
                for(size_t i=1;i<BINS;++i)
                {
                    const count_t b = bins[i];
                    mu0 += b;
                    mu1 += i*b;
                }
            }

            //! Otsu's threshold
            size_t threshold() const throw();

            //! Otsu's level computing
            template <typename T,typename FUNC> static inline
            size_t Level(const Pixmap<T> &pxm, FUNC &func, Engine &E )
            {
                Histogram H;
                H.build<T,FUNC>(pxm,func,E,true);
                return H.threshold();
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Histogram);
            template <typename T,typename FUNC>
            struct __build
            {
                const Pixmap<T> * _source;
                FUNC            * _func;

                inline void operator()( const Tile &tile, lockable & )
                {
                    assert(_source);
                    assert(_func);
                    const Pixmap<T> &source = *_source;
                    FUNC            &func   = *_func;
                    count_t         *b      = & tile.cache.get<count_t>();
                    Y_INK_AREA_LIMITS(tile);
                    for(unit_t y=ymax;y>=ymin;--y)
                    {
                        const typename Pixmap<T>::Row &src = source[y];
                        for(unit_t x=xmax;x>=xmin;--x)
                        {
                            ++b[ uint8_t(func( src[x] )) ];
                        }
                    }
                }
            };


        };

        //! build pixmap from a threshold level
        struct Threshold
        {
            //! generic keep function
            template <typename T,typename FUNC,typename ACCEPT,typename OP>
            static inline
            void Keep(Pixmap<T>       &target,
                      const Pixmap<T> &source,
                      FUNC            &type2byte,
                      ACCEPT          &acc,
                      const size_t     level,
                      OP              &op,
                      Engine          &E)
            {
                struct __keep<T,FUNC,ACCEPT,OP> proxy =
                {
                    &target,
                    &source,
                    &type2byte,
                    &acc,
                    level,
                    &op
                };
                E.run(proxy);
            }

            typedef bool (*AcceptProc)(const size_t,const size_t); //!< accept a pixel prototype
            static inline bool __gt__( const size_t x, const size_t level) { return x>level;  } //!< true if x>level
            static inline bool __geq__(const size_t x, const size_t level) { return x>=level; } //!< true if x>=level
            static inline bool __lt__( const size_t x, const size_t level) { return x<level;  } //!< true if x<level
            static inline bool __leq__(const size_t x, const size_t level) { return x<=level; } //!< true if x<=level

            //! Keep Foreground
            template <typename T, typename FUNC, typename OP> static inline
            void Foreground(Pixmap<T>       &fg,
                            const Pixmap<T> &source,
                            FUNC            &func,
                            const size_t     level,
                            Engine          &E,
                            OP              &op,
                            const bool       accept_strict=false)
            {
                AcceptProc acc = __geq__;
                if(accept_strict) acc = __gt__;
                Keep(fg,source,func,acc,level,op,E);
            }

            //! Keep Background
            template <typename T, typename FUNC, typename OP> static inline
            void Background(Pixmap<T>       &fg,
                            const Pixmap<T> &source,
                            FUNC            &func,
                            const size_t     level,
                            Engine          &E,
                            OP              &op,
                            const bool       accept_strict=false)
            {
                AcceptProc acc = __leq__;
                if(accept_strict) acc = __lt__;
                Keep(fg,source,func,acc,level,op,E);
            }
            


        private:
            template <typename T,typename FUNC,typename ACCEPT,typename OP>
            struct __keep
            {
                Pixmap<T>       * _target;
                const Pixmap<T> * _source;
                FUNC            * _func;
                ACCEPT          * _acc;
                size_t            level;
                OP              * _op;

                inline void operator()( const Tile &tile, lockable & )
                {
                    assert(_source);
                    assert(_func);
                    assert(_acc);
                    assert(_op);
                    assert(_target);

                    Pixmap<T>       &target = *_target;
                    const Pixmap<T> &source = *_source;
                    FUNC            &func   = *_func;
                    ACCEPT          &acc    = *_acc;
                    OP              &op     = *_op;
                    Y_INK_AREA_LIMITS(tile);

                    for(unit_t y=ymax;y>=ymin;--y)
                    {
                        typename       Pixmap<T>::Row &tgt = target[y];
                        const typename Pixmap<T>::Row &src = source[y];
                        for(unit_t x=xmax;x>=xmin;--x)
                        {
                            const T      &org = src[x];
                            const uint8_t lvl = uint8_t(func(org));
                            T             pix = Pixel<T>::Zero;
                            if( acc(lvl,level) )
                            {
                                pix = org;
                            }
                            tgt[x] = op(pix);
                        }
                    }
                }
            };
        };

    }
}

#endif

