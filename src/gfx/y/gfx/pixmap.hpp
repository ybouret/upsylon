
//! \file

#ifndef Y_GFX_PIXMAP_INCLUDED
#define Y_GFX_PIXMAP_INCLUDED 1

#include "y/gfx/bitmap.hpp"
#include "y/gfx/async/broker.hpp"

namespace upsylon
{
    namespace GFX
    {
        //______________________________________________________________________
        //
        //
        //! named Pixmap
        //
        //______________________________________________________________________
        template <typename T>
        class Pixmap : public Bitmap
        {
        public:
            //! ABI of PixRow
            class Row
            {
            private:
                Row(); ~Row();
                Y_DISABLE_COPY_AND_ASSIGN(Row);
                T *p;
            public:
                const unit_t    w; //!< width
                const ZeroFlux &z; //!< zero flux width

                //! Zero Flux pixel[i], const
                inline T & operator()(const unit_t i)             throw() { assert(p); return p[z(i)]; }

                //! Zero Flux pixel[i]
                inline const T & operator()(const unit_t i) const throw() { assert(p); return p[z(i)]; }

                //! Raw pixel[i]
                inline T & operator[](const unit_t i) throw()
                {
                    assert(p); assert(i>=0); assert(i<w); return p[i];
                }

                //! Raw pixel[i], const
                inline const T & operator[](const unit_t i) const throw()
                {
                    assert(p); assert(i>=0); assert(i<w); return p[i];
                }


            };

            //! default constructor
            inline explicit Pixmap(const unit_t W, const unit_t H) :
            Bitmap(W,H,sizeof(T)),
            _row( static_cast<Row *>(oor_rows()) )
            {
            }

            //! cleanup
            inline virtual ~Pixmap() throw()
            {
            }

            //! hard copy
            inline explicit Pixmap(const Pixmap &pxm, Async::Broker &broker) :
            Bitmap(pxm,broker),
            _row( static_cast<Row *>(oor_rows()) )
            {

            }

            //! hard copy with transform
            template <typename U, typename FUNC>
            inline explicit Pixmap(const Pixmap<U> &pxm, FUNC &func, Async::Broker &broker) :
            Bitmap(pxm.w,pxm.h,sizeof(T)),
            _row( static_cast<Row *>(oor_rows()) )
            {
                apply(pxm,func,broker);
            }


            

            //! Zero Flux Row[j]
            inline Row & operator()(const unit_t j) throw()
            {
                return _row[ rows.zfh(j) ];
            }

            //! Zero Flux Row[j], const
            inline const Row & operator()(const unit_t j) const throw()
            {
                return _row[ rows.zfh(j) ];
            }

            //! Raw Row[j]
            inline Row & operator[](const unit_t j) throw()
            {
                assert(j>=0); assert(j<h);
                return _row[j];
            }

            //! Raw Row[j], const
            inline const Row & operator[](const unit_t j) const throw()
            {
                assert(j>=0); assert(j<h);
                return _row[j];
            }

            //! apply a 1:1 function
            template <typename U, typename FUNC>
            void apply(const Pixmap<U> &source,
                       FUNC            &func,
                       Async::Broker   &broker)
            {
                assert(this->equals(source));
                assert(this->equals( * broker.engine ));

                wrapper<U,FUNC> op = { *this, source, func };
                broker(op.run,&op);
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Pixmap);
            Row *_row;

            template <typename U,typename FUNC>
            struct wrapper
            {
                Pixmap          &target;
                const Pixmap<U> &source;
                FUNC            &func;

                static inline void run(Async::Worker &worker,
                                       lockable      &,
                                       void          *data)
                {
                    assert(data);
                    wrapper      &_    = *static_cast<wrapper *>(data);
                    const   Tile &tile = worker.tile;
                    for(size_t t=tile.size();t>0;--t)
                    {
                        const HScan &hs = tile[t];
                        Point        p  = hs.begin;
                        const Row   &src = _.source[p.y];
                        Row         &tgt = _.target[p.y];
                        for(unit_t i=hs.width;i>0;--i,++p.x)
                        {
                            tgt[p.x] = _.func(src[p.x]);
                        }
                    }
                }
            };
        };

    }

}

#endif
