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
                    ~Weight() throw();
                    Weight(const Weight &) throw();

                private:
                    Y_DISABLE_ASSIGN(Weight);
                };
                typedef slots<Weight> Weights;

                virtual ~Filter() throw();
                Filter(const Filter &F);


                template <typename T,
                typename U>
                inline void apply(Graphic::Pixmap<T>       &target,
                                  const Graphic::Pixmap<U> &source,
                                  const Point               lower,
                                  const Point               upper)
                {
                    const size_t num = weights.size();

                    for(unit_t y=upper.y;y>=lower.y;--y)
                    {
                        typename Graphic::Pixmap<T>::RowType &tgt = target[y];

                        for(unit_t x=upper.x;x>=lower.x;--x)
                        {
                            float       sum = 0;
                            const Point org(x,y);
                            for(size_t i=0;i<num;++i)
                            {
                                const Weight &W     = weights[i];
                                const Point   probe = org + W.point;
                                sum += W.value * source(probe);
                            }
                            tgt[x] = sum;
                        }
                    }
                }



            protected:
                explicit Filter(const size_t);
                void     update();
                void     normalize() throw();

                template <typename T> inline
                void compile_( const Oxide::Field2D<T> &field )
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
                    for(size_t i=0;i<weights.size();++i)
                    {
                        std::cerr << weights[i].point << "=>" << weights[i].value << std::endl;
                    }
                    std::cerr << "\tsumOfWeights=" << sumOfWeights << std::endl;
                    std::cerr << "\tfactor      =" << factor       << std::endl;

                }

            private:
                Weights      weights;
                const float  sumOfWeights;
                const float  factor;
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

            inline void compile()
            {
                compile_<T>(*this);
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Filter);
        };

    }
}


#endif

