//! \file

#ifndef Y_GRAPHIC_OPS_FILTER_INCLUDED
#define Y_GRAPHIC_OPS_FILTER_INCLUDED 1

#include "y/graphic/pixmap.hpp"
#include "y/oxide/field2d.hpp"
#include "y/sequence/slots.hpp"

namespace upsylon {

    namespace Graphic {

        namespace Kernel {

            class Filter : public Object
            {
            public:
                class Weight
                {
                public:
                    const float  value;
                    const Point  point;
                    Weight(const float  v,
                           const unit_t dx,
                           const unit_t dy) throw();
                    ~ Weight() throw();
                private:
                    Y_DISABLE_ASSIGN(Weight);
                };
                typedef slots<Weight> Weights;

                virtual ~Filter() throw();
                Filter(const Filter &F);


            protected:
                explicit Filter(const size_t);
                void     update() throw();

                template <typename T> inline
                void compile( const Oxide::Field2D<T> &field )
                {
                    assert( weights.size() <= 0 );
                    assert(field.items==weights.count);

                    for(unit_t y=field.lower.y;y<=field.upper.y;++y)
                    {
                        for(unit_t x=field.lower.x;x<=field.upper.x;++x)
                        {
                            const float w = static_cast<float>( field[y][x] );
                            if( math::fabs_of(w) > 0 )
                            {
                                weights.build<float,unit_t,unit_t>(w,x,y);
                            }
                        }
                    }
                    update();
                }

            private:
                Weights weights;
                Y_DISABLE_ASSIGN(Filter);
            };

        }

        template <typename T>
        class Filter :  public Oxide::Field2D<T>, public Kernel::Filter
        {
        public:
            explicit Filter(const char            *id,
                            const Oxide::Coord2D   lo,
                            const Oxide::Coord2D   hi) :
            Oxide::Field2D<T>(id,lo,hi),
            Kernel::Filter(this->items)
            {

            }

            virtual ~Filter() throw()
            {
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Filter);
        };

    }
}


#endif

