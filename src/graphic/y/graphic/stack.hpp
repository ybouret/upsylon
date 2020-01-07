
//! \file

#ifndef Y_GRAPHIC_STACK_INCLUDED
#define Y_GRAPHIC_STACK_INCLUDED 1

#include "y/graphic/pixmap.hpp"
#include "y/sequence/slots.hpp"
#include "y/graphic/convert.hpp"

namespace upsylon {

    namespace Graphic {

        //! a stack of pixmaps
        template <typename T>
        class Stack : public Area, public slots< Pixmap<T> >
        {
        public:
            typedef slots< Pixmap<T> > Slots; //!< alias

            //! setup
            inline explicit Stack(const size_t W,
                                  const size_t H,
                                  const size_t n) :
            Area(W,H),
            Slots(n)
            {
                for(size_t i=0;i<n;++i)
                {
                    this->template build<size_t,size_t>(w,h);
                }
            }

            //! cleanup
            inline virtual ~Stack() throw()
            {
            }

            //! average on [ini..end-1]
            template <typename U, const size_t DIMS>
            inline void average_(Pixmap<T>   &target,
                                 const size_t ini,
                                 const size_t end) const
            {
                if(end>ini)
                {
                    const Slots &self  = *this;
                    const size_t count = end-ini;
                    float        sum[DIMS] = { 0 };
                    const Point  lo = target->lower;
                    const Point  up = target->upper;
                    for(unit_t y=up.y;y>=lo.y;--y)
                    {
                        typename Pixmap<T>::RowType &r = target[y];

                        for(unit_t x=up.x;x>=lo.x;--x)
                        {
                            Y_BZSET_STATIC(sum);
                            for(size_t i=ini;i<end;++i)
                            {
                                const T &src = self[i][y][x];
                                const U *p   = (const U *) &src;
                                for(size_t dim=0;dim<DIMS;++dim)
                                {
                                    sum[dim] += float( p[dim] );
                                }
                            }
                            T &tgt = r[x];
                            U *q   = (U *)&tgt;
                            for(size_t dim=0;dim<DIMS;++dim)
                            {
                                q[dim] = Convert::Get<U,float>( sum[dim] / count );
                            }
                        }

                    }
                }
            }

            //! type specific average
            void average(Pixmap<T> &target, const size_t ini, const size_t end) const;

            //! full average
            inline void average(Pixmap<T> &target)
            {
                average( target, 0, this->size() );
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Stack);
        };

    }

}

#endif

