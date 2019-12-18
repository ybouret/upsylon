//! \file

#ifndef Y_GRAPHIC_OPS_FILTER_INCLUDED
#define Y_GRAPHIC_OPS_FILTER_INCLUDED 1

#include "y/graphic/pixmap.hpp"
#include "y/oxide/field2d.hpp"
#include "y/sequence/slots.hpp"
#include "y/graphic/parallel/tiles.hpp"
#include "y/ptr/intr.hpp"
namespace upsylon {

    namespace Graphic {

        namespace Kernel {

            class Filter : public Object
            {
            public:
                typedef intr_ptr<string,const Filter> Pointer;

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
                virtual const string & key() const throw() = 0;

                template <typename T,
                typename U>
                inline void applyRaw(Graphic::Pixmap<T>       &target,
                                     const Graphic::Pixmap<U> &source,
                                     const Point               lower,
                                     const Point               upper,
                                     float                    &globalVmin,
                                     float                    &globalVmax) const
                {
                    const size_t num   = weights.size();
                    float vmin  =0, vmax=0;
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
                                sum += W.value * static_cast<float>(source(probe));
                            }
                            tgt[x] = static_cast<T>(sum);
                            if(sum<vmin)
                            {
                                vmin = sum;
                            }
                            else if(sum>vmax)
                            {
                                vmax = sum;
                            }
                        }
                    }
                    globalVmin = vmin;
                    globalVmax = vmax;
                }

                template <typename T, typename U>
                void run(Graphic::Pixmap<T>       &target,
                         const Graphic::Pixmap<U> &source,
                         Tiles                    &tiles)
                {
                    tiles.localAcquire(2*sizeof(float));
                    struct Task
                    {
                        const Filter             *filter;
                        Graphic::Pixmap<T>       *target;
                        const Graphic::Pixmap<U> *source;
                        Tiles                    *tiles;

                        static void  Run( void *args, parallel &ctx, lockable &) throw()
                        {
                            Task  &task = *(Task*)args;
                            Tile  &tile = (*task.tiles)[ctx.rank];
                            float &vmin = tile.as<float>(0);
                            float &vmax = tile.as<float>(1);
                            task.filter->template applyRaw<T,U>( *task.target, *task.source, tile.lower, tile.upper, vmin, vmax);
                        }

                    };
                    Task task = { this, &target, &source, &tiles };
                    tiles.loop().run( Task::Run, &task);
                }

                void     normalize() throw();

            protected:
                explicit Filter(const size_t);
                void     update();


                template <typename T> inline
                void compile_( const Oxide::Field2D<T> &field )
                {
                    std::cerr << "Compiling " << key() << std::endl;
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
                    std::cerr << "\tweight =" << weight << std::endl;
                    std::cerr << "\tfactor =" << factor << std::endl;

                }

            private:
                Weights      weights;
                const float  weight;
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

            virtual const string & key() const throw() { return this->name; }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Filter);
        };

#define Y_GRAPHIC_FILTER_DECL(NAME) \
class NAME : public FilterType {\
public: explicit NAME(); virtual ~NAME() throw();\
private: Y_DISABLE_COPY_AND_ASSIGN(NAME); }

#define Y_GRAPHIC_FILTER_IMPL(STRUCT,NAME,LO,HI) \
STRUCT:: NAME:: ~NAME() throw() {}\
STRUCT:: NAME:: NAME() : FilterType( #STRUCT "::" #NAME, LO, HI ) {\
FilterType &self = *this; (void)self;

#define Y_GRAPHIC_FILTER_DONE() compile(); }


    }
}


#endif

