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
                template <
                typename UNIT,
                typename ORDINATE>
                class circle
                {
                public:
                    
                    
                    typedef point2d<UNIT>              ABSCISSA;
                    typedef sample<ABSCISSA,ORDINATE>  sample_type;
                    typedef samples<ABSCISSA,ORDINATE> samples_type;
                    typedef vector<ABSCISSA>           abscissae;
                    typedef vector<ORDINATE>           ordinates;
                    typedef sequential<ABSCISSA,ORDINATE>          sequential_type;
                    typedef sequential_gradient<ABSCISSA,ORDINATE> sequential_grad;
                    typedef typename sequential_type::function     sequential_func;
                   
                    
                    inline ORDINATE call(const ABSCISSA             &p,
                                         const accessible<ORDINATE> &A,
                                         const variables            &V)
                    {
                        return V(A,"a") * p.x + V(A,"b") * p.y + V(A,"c");
                    }
                    
                    class sampling : public sample_type
                    {
                    public:
                        inline virtual ~sampling() throw() {}
                        
                        template <typename ID>
                        static inline
                        sampling *create(const ID &id, const size_t n=0)
                        {
                            const typename sample_type::abscissa_type the_abscissa = new abscissae(n,as_capacity);
                            const typename sample_type::ordinate_type the_ordinate = new ordinates(n,as_capacity);
                            const typename sample_type::ordinate_type the_adjusted = new ordinates(n,as_capacity);
                            return new sampling(id,the_abscissa,the_ordinate,the_adjusted);
                        }
                        
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
                    
                    class gradient : public sequential_grad
                    {
                    public:
                        inline explicit gradient() throw() : sequential_grad() {}
                        inline virtual ~gradient() throw() {}
                        
                    private:
                        Y_DISABLE_COPY_AND_ASSIGN(gradient);
                       
                    };
                    
                    
                    inline explicit circle() : func(this, & circle::call) {}
                    inline virtual ~circle() throw()
                    {
                    }
                    sequential_func func;

                };
                
            }
            
        }
        
    }
    
}

#endif

