//! \file

#ifndef Y_GFX_FILTER_INCLUDED
#define Y_GFX_FILTER_INCLUDED 1

#include "y/gfx/pixmap.hpp"
#include "y/memory/embed.hpp"
#include "y/string.hpp"
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
            class filter_weights : public accessible< const filter_weight<T> >
            {
            public:
                const unit_t                    y;


                explicit filter_weights(const unit_t            yyy,
                                        const filter_weight<T> *ptr,
                                        const size_t            num) throw() :
                y(yyy), shift(ptr-1), count(num)
                {
                    assert(ptr!=NULL);
                    assert(num>0);
                }

                inline virtual size_t size() const throw() { return count; }

                inline const filter_weight<T> &  operator[](const size_t i) const throw()
                {
                    assert(i>0);
                    assert(i<=count);
                    return shift[i];
                }

            private:
                const filter_weight<T>  * const shift;
                const size_t                    count;

                virtual ~filter_weights() throw() {}
                Y_DISABLE_COPY_AND_ASSIGN(filter_weights);
            };


            class filter : public entity
            {
            public:
                virtual ~filter() throw();

                static void  suppress(void *&,size_t &) throw();
                static void *allocate(memory::embed emb[], const size_t num, size_t &);

                const string name;

            protected:
                explicit filter(const string&); //!< setup
                explicit filter(const char  *); //!< setup

            private:
                Y_DISABLE_COPY_AND_ASSIGN(filter);
            };
        }

        template <typename T>
        class filter : public crux::filter, public accessible< const crux::filter_weights<T> >
        {
        public:
            typedef crux::filter_weight<T>  weight_type;
            typedef crux::filter_weights<T> weights_type;

            inline virtual ~filter() throw()
            {
                suppress(wksp,wlen);
                wline=0; aliasing::_(lines)=0;
            }

            template <typename ID, typename U>
            inline explicit filter(const ID    &ident,
                                   const U     *coeff,
                                   const unit_t width,
                                   const bool   trans) :
            crux::filter(ident), wline(0), lines(0), wksp(0), wlen(0)
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
                    for(size_t i=1;i<=W.size();++i)
                    {
                        const weight_type &w = W[i];
                        os << " (" << std::setw(3) << w.value << ")@x=" << std::setw(3) << w.x << "|";
                    }
                    os << std::endl;
                }
                return os;
            }

            //! target = filter(source[p])
            template <typename U, typename V> inline
            void put( U &target, const pixmap<V> &source, const coord p) const throw()
            {
                U sum = 0;
                for(size_t j=lines;j>0;--j)
                {
                    const weights_type &W = (*this)[j];
                    const pixrow<V>    &r = source[W.y+p.y];
                    for(size_t i=W.size();i>0;--i)
                    {
                        const weight_type &w = W[i];
                        const V           &q = r[w.x+p.x];
                        sum += U(w.value) * U(q);
                    }
                }
                target = sum;
            }

            template <typename U, typename V> inline
            void operator()(pixmap<U>       &target,
                            const pixmap<V> &source,
                            broker          &apply) const throw()
            {
                assert( target.has_same_metrics_than(source) );
                assert( target.has_same_metrics_than(apply)  );

                struct ops
                {
                    pixmap<U>       &target;
                    const pixmap<V> &source;
                    const filter    &morpho;

                    static inline void run(const tile &t, void *args, lockable &) throw()
                    {
                        assert(args);
                        ops             &self   = *static_cast<ops *>(args);
                        pixmap<U>       &target = self.target;
                        const pixmap<V> &source = self.source;
                        const filter    &morpho = self.morpho;

                        for(size_t j=t.size();j>0;--j)
                        {
                            const segment &s = t[j];
                            const unit_t   xmin  = s.xmin;
                            const unit_t   y     = s.y;
                            pixrow<U>     &tgt_y = target(y);
                            for(unit_t x=s.xmax;x>=xmin;--x)
                            {
                                U &tgt = tgt_y(x);
                                morpho.put(tgt,source, coord(x,y));
                            }
                        }
                    }

                };

                ops todo = { target, source, *this };
                apply(ops::run,&todo);
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

                // pass 1: count active lines and total items
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

                //! pass 2: create structure
                if(lines)
                {
                    weight_type  *wcurr     = 0;

                    // allocate memory
                    {
                        memory::embed emb[] = {
                            memory::embed::as(wline,lines),
                            memory::embed::as(wcurr,total)
                        };
                        wksp = allocate(emb, sizeof(emb)/sizeof(emb[0]), wlen);
                    }

                    // setup addresses
                    {
                        size_t iline = 0;
                        for(unit_t y=-delta,j=0;y<=delta;++y,++j)
                        {
                            weight_type *start = wcurr;
                            for(unit_t x=-delta,i=0;x<=delta;++x,++i)
                            {
                                const U &value = coeff[trans? i*width+j : j*width+i];
                                if(value!=0)
                                {
                                    new ( wcurr++ ) weight_type(x,T(value));
                                }
                            }
                            const size_t count = wcurr-start;
                            if(count)
                            {
                                new ( &wline[iline++] ) weights_type(y,start,count);
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

