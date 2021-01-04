//! \file

#ifndef Y_FITTING_BUILT_IN_CIRCLE_INCLUDED
#define Y_FITTING_BUILT_IN_CIRCLE_INCLUDED 1

#include "y/mkl/fitting/samples.hpp"
#include "y/type/point2d.hpp"

namespace upsylon
{
    namespace mkl
    {
        namespace fitting
        {
            
            namespace built_in
            {

                //! common data for circle
                struct __circle
                {
                    static const char   *name[3];                              //!< "a", "b", "c"
                    static const size_t  nvar = sizeof(name)/sizeof(name[0]);  //!< number of active vars
                };

                //______________________________________________________________
                //
                //
                //! Casa and Koope extended method
                //
                //______________________________________________________________
                template <
                typename UNIT,
                typename ORDINATE>
                class circle
                {
                public:
                    //__________________________________________________________
                    //
                    // types and definitions
                    //__________________________________________________________

                    typedef point2d<UNIT>                      ABSCISSA;        //!< base type
                    typedef sample<ABSCISSA,ORDINATE>          sample_type;     //!< alias
                    typedef samples<ABSCISSA,ORDINATE>         samples_type;    //!< alias
                    typedef vector<ABSCISSA>                   abscissae;       //!< alias
                    typedef vector<ORDINATE>                   ordinates;       //!< alias
                    typedef sequential<ABSCISSA,ORDINATE>      sequential_type; //!< alias
                    typedef v_gradient<ABSCISSA,ORDINATE>      v_gradient_type; //!< alias
                    typedef typename sequential_type::function sequential_func; //!< alias
                   

                    //__________________________________________________________
                    //
                    //! special sample to collect x,y and build z=x^2+y^2
                    //__________________________________________________________
                    class sampling : public sample_type
                    {
                    public:
                        //! cleanup
                        inline virtual ~sampling() throw() {}

                        //! create with name
                        template <typename ID>
                        static inline
                        sampling *create(const ID &id, const size_t n=0)
                        {
                            const typename sample_type::abscissa_type the_abscissa = new abscissae(n,as_capacity);
                            const typename sample_type::ordinate_type the_ordinate = new ordinates(n,as_capacity);
                            const typename sample_type::ordinate_type the_adjusted = new ordinates(n,as_capacity);
                            return new sampling(id,the_abscissa,the_ordinate,the_adjusted);
                        }

                        //! append a "pixel"
                        inline void append(const UNIT x, const UNIT y)
                        {
                            const ORDINATE z = square_of( ORDINATE(x) ) + square_of( ORDINATE(y) );
                            const ABSCISSA p(x,y);
                            this->add(p,z);
                        }
                        
                        
                    private:
                        Y_DISABLE_COPY_AND_ASSIGN(sampling);
                        template <typename ID>
                        inline sampling(const ID &id,
                                        const typename sample_type::abscissa_type &the_abscissa,
                                        const typename sample_type::ordinate_type &the_ordinate,
                                        const typename sample_type::ordinate_type &the_adjusted) :
                        sample_type(id,the_abscissa,the_ordinate,the_adjusted)
                        {
                        }
                        
                    };

                    //__________________________________________________________
                    //
                    //! compute gradient
                    //__________________________________________________________
                    class gradient : public v_gradient_type
                    {
                    public:
                        inline explicit gradient() throw() : v_gradient_type() {} //!< setup
                        inline virtual ~gradient() throw()                     {} //!< cleanup

                    private:
                        Y_DISABLE_COPY_AND_ASSIGN(gradient);
                        inline virtual void compute(addressable<ORDINATE>      &dFdA,
                                                    const ABSCISSA              p,
                                                    const accessible<ORDINATE> &,
                                                    const variables            &vars,
                                                    const accessible<bool>     &used)
                        {

                            const ORDINATE g[3] = { ORDINATE(p.x), ORDINATE(p.y), ORDINATE(1) };
                            for(size_t i=0;i<__circle::nvar;++i)
                            {
                                const size_t j = *vars[__circle::name[i]];
                                if(used[j]) dFdA[j] = g[i];
                            }
                        }
                    };


                    //__________________________________________________________
                    //
                    // C++
                    //__________________________________________________________
                    //! setup
                    inline explicit circle() : func(this, & circle::call), grad() {}

                    //! cleanup
                    inline virtual ~circle() throw() {}


                    //__________________________________________________________
                    //
                    // methods
                    //__________________________________________________________
                    //! base call to fit
                    inline ORDINATE call(const ABSCISSA             &p,
                                         const accessible<ORDINATE> &A,
                                         const variables            &V)
                    {
                        return V(A,__circle::name[0]) * p.x + V(A,__circle::name[0]) * p.y + V(A,__circle::name[2]);
                    }

                    //__________________________________________________________
                    //
                    // members
                    //__________________________________________________________
                    sequential_func func; //!< call()
                    gradient        grad; //!< to compute gradient

                private:
                    Y_DISABLE_COPY_AND_ASSIGN(circle);
                };

                
            }
            
        }
        
    }
    
}

#endif

