//! \file
#ifndef Y_RANDOM_BITS_INCLUDED
#define Y_RANDOM_BITS_INCLUDED 1

#include "y/type/ints.hpp"
#include "y/type/bswap.hpp"
#include "y/lockable.hpp"
#include <cstdlib>
#include <cmath>

namespace upsylon
{

    namespace randomized
    {
        //! interface to bits generation
        class bits
        {
        public:
            virtual ~bits() throw(); //!< destructor
            const uint32_t span; //!< next32 in 0..span
            const size_t   nbit; //!< bits_for(span)
            const uint32_t half; //!< span/2
            const double   denD; //!< 1.0+double(span)
            const float    denF; //!< 1.0f+float(span)
            const double   symD; //!< 0.5*denD
            const float    symF; //!< 0.5*denF
            const double   midD; //!< span * 0.5
            const double   midF; //!< span * 0.5f
            
            virtual uint32_t next32() throw() = 0; //!< next 32-bits value in 0..span
            virtual void     reseed( bits &bits ) throw() = 0; //!< initialize state with other bits
            
            template <typename T> T to()   throw(); //!< in 0:1 exclusive [float|double]
            template <typename T> T symm() throw(); //!< in -1:1 exclusive [float|double]

            //! random full integral type
            template <typename T> inline
            T full() throw()
            {

                static const size_t full_bits = sizeof(T)*8;
                const  size_t       self_bits = nbit;
                size_t              read_bits = 0;
                T ans(0);
                do
                {
                    (ans <<= self_bits) |= T(next32());
                    read_bits += self_bits;
                }
                while(read_bits<full_bits);
                return ans;
            }

            //! random partial integral type on given bits
            template <typename T> inline
            T partial(size_t nbits) throw()
            {
                T ans(0);
                while(nbits-->0)
                {
                    (ans <<= 1);
                    if(choice())
                    {
                        ans |= 1;
                    }
                }
                return ans;
            }

            //! slow partial type
            template <typename T> inline
            T partial() throw()
            {
                return partial<T>( leq(8*sizeof(T)) );
            }


            //! random unsigned integral in 0..X-1
            template <typename T> inline
            T __lt(const T X) throw()
            {
                return ( (X<=0) ? T(0) : ( full<T>() % X ) );
            }

            //! random value in integral range
            template <typename T> inline
            T range(const T a, const T b) throw()
            {
                const double r = floor( 0.5 + (to<double>() * (double(b)-double(a))) );
                return a + T(r);
            }

            //! 0..n-1
            inline size_t lt(const size_t n) throw()
            {
                return __lt<size_t>(n);
            }

            //! returns in 0..n
            inline size_t leq( size_t n ) throw()
            {
                return full<size_t>() % (++n);
            }

            //! Knuth shuffle of a[0..n-1]
            template <typename T>
            inline void shuffle( T *a, const size_t n ) throw()
            {
                assert(!(NULL==a&&n>0));
                if( n > 1 )
                {
                    for( size_t i=n-1;i>0;--i)
                    {
                        const size_t j   = leq(i);
                        bswap( a[i], a[j] );
                    }
                }
            }

            //! Knuth co shuffle of a[0..n-1] and b[0..n-1]
            template <typename T, typename U>
            inline void shuffle( T *a, U *b, const size_t n) throw()
            {
                assert(!(NULL==a&&n>0));
                if( n > 1 )
                {
                    for( size_t i=n-1;i>0;--i)
                    {
                        const size_t j   = leq(i);
                        bswap( a[i], a[j] );
                        bswap( b[i], b[j] );
                    }
                }
            }

            //! a random boolean
            inline bool choice() throw()
            {
                return (next32() <= half);
            }

            //! +/-
            inline int pm() throw()
            {
                return ( choice() ) ? 1:-1;
            }

            //! shuffle LIST type
            template <typename LIST>
            inline void shuffle( LIST &l ) throw()
            {
                LIST tmp;
                while(l.size>0)
                {
                    if(choice())
                    {
                        tmp.push_back( l.pop_back() );
                    }
                    else
                    {
                        tmp.push_front( l.pop_back() );
                    }
                }
                l.swap_with(tmp);
            }

            static bits     & simple(); //!< global isaac<4> bits generator
            static bits     & crypto(); //!< global isaac<8> bits generator
            static lockable & access(); //!< to lock access to global generators

            //! generate point on unit circle
            template <typename T>
            void on_circle( T &x, T &y ) throw()
            {
                while(true)
                {
                    const T x1 = symm<T>();
                    const T x2 = symm<T>();
                    const T x12 = x1*x1;
                    const T x22 = x2*x2;
                    const T den = x12+x22;
                    if(den>0&&den<1)
                    {
                        const T h = x1*x2;
                        x = (x12-x22)/den;
                        y = (h+h)/den;
                        return;
                    }
                }
            }

            //! generate point within unit disk, excluded
            template <typename T>
            void in_disk(T &x, T &y) throw()
            {
                while(true)
                {
                    const T x1 = symm<T>();
                    const T x2 = symm<T>();
                    const T x12 = x1*x1;
                    const T x22 = x2*x2;
                    const T d2  = x12+x22;
                    if(d2<1)
                    {
                        x=x1;
                        y=x2;
                        return;
                    }
                }
            }

            //! generate point on unit sphere
            template <typename T>
            void on_sphere(T &x, T &y, T&z ) throw()
            {
                while(true)
                {
                    const T x1 = symm<T>();
                    const T x2 = symm<T>();
                    const T x12 = x1*x1;
                    const T x22 = x2*x2;
                    const T ssq = x12+x22;
                    if(ssq<1)
                    {
                        const T del = 1-ssq;
                        const T fac = sqrt(1-ssq);
                        z = del-ssq;
                        const T hx  = x1 * fac;
                        const T hy  = x2 * fac;
                        x=hx+hx;
                        y=hy+hy;
                        return;
                    }
                }
            }

            //! generate point strictly within unit ball
            template <typename T>
            void in_ball(T &x, T &y, T &z) throw()
            {
                while(true)
                {
                    const T x1 = symm<T>();
                    const T x2 = symm<T>();
                    const T x3 = symm<T>();
                    const T d2 = x1*x1 + x2*x2 + x3*x3;
                    if(d2<1)
                    {
                        x=x1;
                        y=x2;
                        z=x3;
                        return;
                    }
                }
            }

        protected:
            //! sets span and auxiliary values
            explicit bits(const uint32_t maxValue) throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(bits);
        };



        //! fast demo based on rand()
        class cstdbits : public bits
        {
        public:
            //! constructor with RAND_MAX
            inline explicit cstdbits() throw() : bits(RAND_MAX) {}

            //!desctructor
            inline virtual ~cstdbits() throw() {}

            //! call rand
            inline virtual uint32_t next32() throw() { return rand(); }
            inline virtual void     reseed(bits &other) throw() { srand( other.full<unsigned>() ); }
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(cstdbits);
        };


    }

}

#endif

