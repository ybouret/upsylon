
//! \file

#ifndef Y_MKL_SOLVE_ZALGO_INCLUDED
#define Y_MKL_SOLVE_ZALGO_INCLUDED 1

#include "y/mkl/solve/zseek.hpp"
#include "y/mkl/triplet.hpp"

namespace upsylon
{
    namespace mkl
    {

        //______________________________________________________________________
        //
        //
        //! common interface for algorithms
        //
        //______________________________________________________________________
        template <typename T>
        class zalgo : public zseek
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            Y_DECL_ARGS(T,type);                        //!< aliases
            typedef triplet<mutable_type> triplet_type; //!< alias


            //__________________________________________________________________
            //
            // static methods
            //__________________________________________________________________

            //! prepare result for an exact zero return
            static inline
            void exactly(param_type z, triplet_type &x, triplet_type &f)
            {
                x.ld(z);
                f.ld(0);
            }

            //! setup initial points
            static inline
            result setup(sign_t       &s_a,
                         sign_t       &s_c,
                         triplet_type &x,
                         triplet_type &f)
            {
                s_a = sign_of(f.a);
                s_c = sign_of(f.c);
                switch(s_a)
                {
                    case __zero__:
                        switch(s_c)
                        {
                            case __zero__: return failure; //! cannot decide
                            default      : break;
                        }
                        exactly(x.a, x, f);
                        return early_return;

                    case positive:
                        switch(s_c)
                        {
                            case __zero__: exactly(x.c, x, f); return early_return;
                            case positive: return failure;
                            case negative: return success;
                        }
                        // never get here
                        break;

                    case negative:
                        switch(s_c)
                        {
                            case __zero__: exactly(x.c, x, f); return early_return;
                            case positive: return success;
                            case negative: return failure;
                        }
                        // never get here
                        break;
                }
                // never get here...
                return failure;
            }



            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            
            //! cleanup
            inline virtual ~zalgo() throw() {}



        protected:
            //! setup
            inline explicit zalgo() throw() : zseek() {}

            //! check convergence
            static inline bool stop(mutable_type       &width,
                                    const triplet_type &x) throw()
            {
                const_type new_width = fabs_of(x.c-x.a);
                if(new_width>=width)
                {
                    return true;
                }
                else
                {
                    return (width=new_width) <= fabs_of(x.b) * numeric<mutable_type>::ftol ;
                }

            }
        private:
            Y_DISABLE_COPY_AND_ASSIGN(zalgo);

        };


    }

}

#endif
