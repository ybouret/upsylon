//! \file

#ifndef Y_MKL_ROOT_ZSOLVER_INCLUDED
#define Y_MKL_ROOT_ZSOLVER_INCLUDED

#include "y/mkl/types.hpp"
#include "y/mkl/triplet.hpp"

namespace upsylon {

    namespace mkl {

        //______________________________________________________________________
        //
        //
        //! base class for zroot
        //
        //______________________________________________________________________
        class zfind
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________

            //! named sign
            enum sign_type
            {
                negative=-1, //!< f<0
                __zero__=0,  //!< f==0
                positive=1   //!< f>0
            };

            //! setup result
            enum result_type
            {
                success,       //!< bracketed
                failure,       //!< ill-bracketed
                early_return   //!< one point is zero
            };

            //! generic sign computation
            template <typename T> static inline
            sign_type __sign( const T x ) throw()
            {
                return (x<0) ? negative : ( (0<x) ? positive : __zero__ );
            }



            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~zfind() throw();  //!< cleanup

            //__________________________________________________________________
            //
            // inteface
            //__________________________________________________________________
            //! name of the method
            virtual const char *method() const throw() = 0;

        protected:
            explicit zfind() throw();             //!< setup
            void     error_not_bracketed() const; //!< raise exception

        private:
            Y_DISABLE_COPY_AND_ASSIGN(zfind);
        };

        //! complete the API with simpler calls
#define Y_ZROOT_API() \
/**/ template <typename FUNC> mutable_type operator()(FUNC &F, param_type a, param_type c)\
/**/ { return this->run(*this,F,a,c); }\
/**/ template <typename FUNC> mutable_type operator()(param_type y, FUNC &F, param_type a, param_type c)\
/**/ { return this->run(*this,y,F,a,c); }

        //______________________________________________________________________
        //
        //
        //! base class for generic solvers
        //
        //______________________________________________________________________
        template <typename T>
        class zroot : public zfind
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            Y_DECL_ARGS(T,type);                        //!< aliases
            typedef triplet<mutable_type> triplet_type; //!< alias

            //! helper to get F(x)=value
            template <typename FUNC>
            struct zcall
            {
                mutable_type value; //!< looked for
                FUNC        *pfunc; //!< function address

                //! wrapper call
                inline mutable_type operator()(param_type x)
                {
                    assert(pfunc);
                    return (*pfunc)(x)-value;
                }
            };

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

            //! prepare signs and configuration
            static inline
            result_type setup(sign_type    &s_a,
                              sign_type    &s_c,
                              triplet_type &x,
                              triplet_type &f)
            {
                s_a = __sign(f.a);
                s_c = __sign(f.c);
                switch(s_a)
                {
                    case __zero__:
                        switch(s_c)
                        {
                            case __zero__: return failure;
                            default     : break;
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
            inline virtual ~zroot() throw() {}

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            mutable_type  todo; //!< convergence criterion

            //! proc(F,[a,0,c],[F(a),0,F(c)])
            template <typename PROC, typename FUNC>
            mutable_type run(PROC &proc, FUNC &F, param_type a, param_type c)
            {
                triplet_type x = { a,      0, c      };
                triplet_type f = { F(x.a), 0, F(x.c) };
                if(!proc(F,x,f)) error_not_bracketed();
                return x.b;
            }

            //! proc(F-y,[a,0,c],[F(a)-y,0,F(c)-y])
            template <typename PROC, typename FUNC>
            mutable_type run(PROC &proc, param_type y, FUNC &F, param_type a, param_type c)
            {
                zcall<FUNC>  G = { y, &F };
                return run(proc,G,a,c);
            }


        protected:
            inline explicit zroot() throw() : zfind(), todo(0) {}

        private:
            Y_DISABLE_COPY_AND_ASSIGN(zroot);
        };
    }
}

#endif
