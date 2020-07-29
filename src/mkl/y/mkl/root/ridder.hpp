//! \file

#ifndef Y_MKL_ROOT_RIDDER_INCLUDED
#define Y_MKL_ROOT_RIDDER_INCLUDED 1

#include "y/mkl/root/zfind.hpp"
#include "y/type/cswap.hpp"

namespace upsylon {

    namespace mkl {

        namespace kernel {

            //! common content
            struct ridder {
                static const char name[]; //!< "ridder"
            };
        }

        //______________________________________________________________________
        //
        //
        //! secant/false position
        //
        //______________________________________________________________________
        template <typename T>
        class ridder : public zroot<T>
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            Y_DECL_ARGS(T,type);                                  //!< aliases
            typedef typename zroot<T>::triplet_type triplet_type; //!< alias
            typedef zfind::sign_type                sign_type;    //!< alias

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________

            //! cleanup
            inline virtual ~ridder() throw() {}

            //! setup
            inline explicit ridder() : zroot<T>() {}

            //! name
            inline virtual const char * method() const throw() { return kernel::ridder::name; }

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! solve f.b=F(x.b)=0
            template <typename FUNC> inline
            bool operator()( FUNC &F, triplet_type &x, triplet_type &f )
            {
                static const_type half(0.5);

                // reordering for clamping
                if(x.c<x.a)
                {
                    cswap(x.a,x.c);
                    cswap(f.a,f.c);
                }
                // setup
                sign_type s_a = zfind::__zero__;
                sign_type s_c = zfind::__zero__;
                switch(this->setup(s_a,s_c,x,f))
                {
                    case zfind::success:      break;
                    case zfind::failure:      return false;
                    case zfind::early_return: return true;
                }
                assert(s_a!=zfind::__zero__);
                assert(s_c!=zfind::__zero__);
                assert(s_a!=s_c);
                mutable_type width = x.c - x.a;

                while(true)
                {
                    // sanity check
                    assert(s_a!=zfind::__zero__);
                    assert(s_c!=zfind::__zero__);
                    assert(s_a!=s_c);

                    // initialize@b and get
                    const_type             den = prepare(F,x,f);
                    const zfind::sign_type s_b = zfind::__sign(f.b);

                    // update
                    if(zfind::__zero__==s_b)
                    {
                        // early return
                        this->exactly(x.b,x,f); return true;
                    }
                    else
                    {
                        // Ridder's value
                        const_type   step = width * half * (f.b/den);
                        mutable_type x_r  = x.b;
                        switch(s_c)
                        {
                            case zfind::__zero__: return false; // shouldn't happen
                            case zfind::positive: x_r -= step; break;
                            case zfind::negative: x_r += step; break;
                        }
                        x_r = clamp(x.a,x_r,x.c);
                        const_type      f_r = F(x_r);
                        const sign_type s_r = zfind::__sign(f_r);

                        if(zfind::__zero__==s_r)
                        {
                            // early return
                            this->exactly(x_r,x,f); return true;
                        }
                        else
                        {
                            // update from all info
                            assert(x.is_increasing());
                            mutable_type xx[4] = { x.a, x.b, x_r, x.c };
                            mutable_type ff[4] = { f.a, f.b, f_r, f.c };
                            sign_type    ss[4] = { s_a, s_b, s_r, s_c };

                            if(x_r<x.b)
                            {
                                cswap(xx[1],xx[2]);
                                cswap(ff[1],ff[2]);
                                cswap(ss[1],ss[2]);
                            }
                            // sanity check
                            assert(xx[0]<=xx[1]);
                            assert(xx[1]<=xx[2]);
                            assert(xx[2]<=xx[3]);
                            assert(zfind::__zero__!=ss[0]);
                            assert(zfind::__zero__!=ss[1]);
                            assert(zfind::__zero__!=ss[2]);
                            assert(zfind::__zero__!=ss[3]);

                            // find the first alternating interval
                            size_t       j=0;
                            mutable_type w=-1;
                            for(size_t i=0;i<3;++i)
                            {
                                if(ss[i]!=ss[i+1])
                                {
                                    j=i;
                                    w=xx[i+1]-xx[i];
                                    break;
                                }
                            }
                            assert(w>=0);

                            // find the smallest alternating interval
                            for(size_t i=j+1;i<3;++i)
                            {
                                if(ss[i]!=ss[i+1])
                                {
                                    const_type tmp = xx[i+1]-x[i];
                                    if(tmp<w)
                                    {
                                        w=tmp;
                                        j=i;
                                    }
                                }
                            }
                            //std::cerr << "@" << j << ": w=" << w << std::endl;
                            const size_t jp = j+1;

                            // update status
                            x.a = xx[j];  f.a = ff[j];  s_a = ss[j];
                            x.c = xx[jp]; f.c = ff[jp]; s_c = ss[jp];
                            if(fabs_of(f.a)<fabs_of(f.c))
                            {
                                x.b = x.a;
                                f.b = f.a;
                            }
                            else
                            {
                                x.b = x.c;
                                f.b = f.c;
                            }
                            if(w>=width)
                            {
                                return true;
                            }
                            width = w;
                        }
                    }
                }

            }

            Y_ZROOT_API()

        private:
            Y_DISABLE_COPY_AND_ASSIGN(ridder);
            template <typename FUNC> static inline
            mutable_type prepare(FUNC &F, triplet_type &x, triplet_type &f)
            {
                assert(f.a*f.c<=0);
                x.b = const_type(0.5)*(x.a+x.c); assert( x.is_increasing() );
                f.b = F(x.b);
                return sqrt_of( f.b*f.b - f.a*f.c );
            }
        };

    }
}

#endif
