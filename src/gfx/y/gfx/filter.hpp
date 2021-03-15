//! \file

#ifndef Y_GFX_FILTER_INCLUDED
#define Y_GFX_FILTER_INCLUDED 1

#include "y/gfx/pixmap.hpp"
#include "y/memory/embed.hpp"
#include "y/memory/allocator/dyadic.hpp"
#include <iomanip>

namespace upsylon
{

    namespace graphic
    {


        namespace crux
        {
            template <typename T>
            class filter_weight
            {
            public:
                const unit_t x;
                const T      value;

                inline filter_weight(const unit_t X, const T V) throw() : x(X), value(V) { }

            private:
                ~filter_weight() throw(); Y_DISABLE_COPY_AND_ASSIGN(filter_weight);
            };


            template <typename T>
            class filter_weights
            {
            public:
                const unit_t                    y;
                const filter_weight<T>  * const w;
                const size_t                    n;

                explicit filter_weights(const unit_t            Y,
                                        const filter_weight<T> *W,
                                        const size_t            N) throw() :
                y(Y), w(W), n(N)
                {
                    assert(W);
                    assert(n>0);
                }
            private:
                ~filter_weights() throw(); Y_DISABLE_COPY_AND_ASSIGN(filter_weights);
            };


        }

        template <typename T>
        class filter : public entity, public accessible< const crux::filter_weights<T> >
        {
        public:
            typedef crux::filter_weight<T>  weight_type;
            typedef crux::filter_weights<T> weights_type;


            inline virtual ~filter() throw()
            {
                if(wksp)
                {
                    static memory::allocator &mgr = memory::dyadic::location();
                    out_of_reach::fill(wksp,0,wlen);
                    mgr.release(wksp,wlen);
                    wline=0;
                    aliasing::_(lines)=0;
                }
            }

            template <typename U>
            inline explicit filter(const U     *coeff,
                                   const unit_t width,
                                   const bool   trans) :
            wline(0), lines(0), wksp(0), wlen(0)
            {
                compile(coeff,width,trans);
            }

            virtual size_t size() const throw() { return lines; }

            virtual const weights_type & operator[](const size_t iline) const throw()
            {
                assert(wline);
                assert(iline>0);
                assert(iline<=lines);
                return wline[iline];
            }

            inline friend std::ostream & operator<<(std::ostream &os, const filter &f)
            {
                for(size_t j=f.lines;j>0;--j)
                {
                    const weights_type &W = f[j];
                    os << "line[" << j << "]@y=" << std::setw(3) << W.y << "|";
                    for(size_t i=0;i<W.n;++i)
                    {
                        const weight_type &w = W.w[i];
                        os << " (" << std::setw(3) << w.value << ")@x=" << std::setw(3) << w.x << "|";
                    }
                    os << std::endl;
                }
                return os;
            }

            template <typename U, typename V> inline
            void operator()( U &target, const pixmap<V> &source, const coord p) const throw()
            {
                U sum = 0;
                for(size_t j=lines;j>0;--j)
                {
                    const weights_type &W = (*this)[j];
                    const pixrow<V>    &r = source[W.y+p.y];
                    for(size_t i=W.n;i>0;--i)
                    {

                    }
                }
                target = sum;

            }



        private:
            Y_DISABLE_COPY_AND_ASSIGN(filter);
            weights_type  *wline;
            const size_t   lines;

            void          *wksp;
            size_t         wlen;

            template <typename U> inline
            void compile(const U     *coeff,
                         const unit_t width,
                         const bool   trans)
            {
                const unit_t delta=(width-1)/2;
                size_t       total = 0;

                assert(coeff);
                assert(delta>0);

                for(unit_t y=-delta,j=0;y<=delta;++y,++j)
                {
                    size_t count = 0;
                    for(unit_t x=-delta,i=0;x<=delta;++x,++i)
                    {
                        const U &value = coeff[trans? i*width+j : j*width+i];
                        if(value!=0)
                        {
                            ++count;
                        }
                    }
                    if(count)
                    {
                        ++aliasing::_(lines);
                        total += count;
                    }
                }
                std::cerr << "lines=" << lines << "/total=" << total << std::endl;

                if(lines)
                {
                    weight_type  *w     = 0;
                    memory::embed emb[] =
                    {
                        memory::embed::as(wline,lines),
                        memory::embed::as(w,total)
                    };

                    wksp = memory::embed::create(emb, sizeof(emb)/sizeof(emb[0]), memory::dyadic::instance(), wlen);
                    std::cerr << "wlen=" << wlen << std::endl;

                    {
                        size_t iline = 0;
                        for(unit_t y=-delta,j=0;y<=delta;++y,++j)
                        {
                            weight_type *start = w;
                            for(unit_t x=-delta,i=0;x<=delta;++x,++i)
                            {
                                const U &value = coeff[trans? i*width+j : j*width+i];
                                if(value!=0)
                                {
                                    new ( w++ ) weight_type(x,T(value));
                                }
                            }
                            const size_t count = w-start;
                            if(count)
                            {
                                new ( &wline[iline++] ) weights_type(y,start,count);
                                std::cerr << " line:" << iline << " @" << y << " (+" << count << ")" << std::endl;
                            }
                        }
                    }

                    --wline;
                }

            }
        };

    }

}

#endif

