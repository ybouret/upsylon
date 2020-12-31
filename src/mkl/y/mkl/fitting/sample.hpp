//! \file

#ifndef Y_FITTING_SAMPLE_INCLUDED
#define Y_FITTING_SAMPLE_INCLUDED 1

#include "y/mkl/fitting/sample/api.hpp"
#include "y/sequence/vector.hpp"
#include "y/ptr/arr.hpp"
#include "y/ptr/auto.hpp"
#include "y/sort/sorted-sum.hpp"
#include "y/type/utils.hpp"

namespace upsylon
{
    namespace mkl
    {
        namespace fitting
        {
            //__________________________________________________________________
            //
            //
            //! helper to define series
            //
            //__________________________________________________________________
            template <typename T>
            struct series
            {
                typedef arr_ptr< sequence<T>, arc_ptr> type; //!< alias
            };

            //__________________________________________________________________
            //
            //
            //! sample for a set of data
            //
            //__________________________________________________________________
            template <typename ABSCISSA, typename ORDINATE>
            class sample: public sample_api<ABSCISSA,ORDINATE>
            {
            public:
                //______________________________________________________________
                //
                // types and definitions
                //______________________________________________________________
                typedef          sample_api<ABSCISSA,ORDINATE>      api_type;        //!< alias
                typedef          intr_ptr<string,sample>            pointer;         //!< for samples
                typedef typename series<ABSCISSA>::type             abscissa_type;   //!< alias
                typedef typename series<ORDINATE>::type             ordinate_type;   //!< alias
                typedef typename api_type::sequential_type          sequential_type; //!< alias
                typedef typename api_type::sequential_grad          sequential_grad; //!< alias

                //______________________________________________________________
                //
                // C++
                //______________________________________________________________

                //! cleanup
                inline virtual ~sample() throw() {}

                //! setup with user's series
                template <typename ID>
                inline explicit sample(const ID            &the_name,
                                       const abscissa_type &the_abscissa,
                                       const ordinate_type &the_ordinate,
                                       const ordinate_type &the_adjusted) :
                api_type(the_name),
                abscissa(the_abscissa),
                ordinate(the_ordinate),
                adjusted(the_adjusted),
                reserved(),
                dFdA(),
                zero(0)
                {}

                //______________________________________________________________
                //
                // methods
                //______________________________________________________________
                //! create an empty sample with capacity
                template <typename ID> static inline
                sample * create(const ID &id, const size_t n)
                {
                    const abscissa_type the_abscissa = new vector<ABSCISSA>(n,as_capacity);
                    const ordinate_type the_ordinate = new vector<ORDINATE>(n,as_capacity);
                    const ordinate_type the_adjusted = new vector<ORDINATE>(n,as_capacity);
                    return new sample(id,the_abscissa,the_ordinate,the_adjusted);
                }

                //! create a filled sample
                template <typename ID> static inline
                sample * create(const ID &id, const ABSCISSA *x, const ORDINATE *y, const size_t n)
                {
                    assert(!(NULL==x&&n>0));
                    assert(!(NULL==y&&n>0));

                    auto_ptr<sample> self( create(id,n) );
                    self->add(x,y,n);
                    return self.yield();
                }

                //! create a clone with shared data, new adjusted
                template <typename ID> inline
                sample * clone_as(const ID &id)
                {
                    const size_t        n            = this->count();
                    const ordinate_type the_adjusted = new vector<ORDINATE>(n,zero);
                    return new sample(id,abscissa,ordinate,the_adjusted);
                }

                //! add x,y,__zero__
                inline void add(const ABSCISSA x, const ABSCISSA y)
                {
                    assert( abscissa.size() == ordinate.size() );
                    assert( abscissa.size() == adjusted.size() );
                    abscissa->push_back(x);
                    try { ordinate->push_back(y); } catch(...) { abscissa->pop_back(); throw; }
                    try { adjusted->push_back(zero); }
                    catch(...) {
                        ordinate->pop_back();
                        abscissa->pop_back();
                        throw;
                    }
                }

                //! add x[n],y[n],__zero__[n]
                inline void add(const ABSCISSA *x, const ORDINATE *y, const size_t n)
                {
                    assert(!(NULL==x&&n>0));
                    assert(!(NULL==y&&n>0));
                    for(size_t i=0;i<n;++i)
                    {
                        add(x[i],y[i]);
                    }
                }



                //______________________________________________________________
                //
                // interface
                //______________________________________________________________

                //! data within
                inline virtual size_t count() const throw()
                {
                    assert( abscissa.size() == ordinate.size() );
                    assert( abscissa.size() == adjusted.size() );

                    return abscissa.size();
                }

                //! setup for a cycle
                inline virtual void setup(const accessible<ORDINATE> &aorg)
                {
                    reserved.adjust(count(),0);
                    dFdA.adjust(aorg.size(),0);
                }
                
                //! D2
                inline virtual ORDINATE D2(sequential_type            &F,
                                           const accessible<ORDINATE> &A) 
                {
                    assert(reserved.size()==count() || die("setup!") );
                    const accessible<ABSCISSA> &X = *abscissa;
                    const accessible<ORDINATE> &Y = *ordinate;
                    addressable<ORDINATE>      &Z = *adjusted;
                    const variables            &V = this->vars;
                    const size_t                N = count();
                    if(N>0)
                    {
                        reserved[1] = square_of(Y[1]-(Z[1]=F.start(X[1],A,V)));
                        for(size_t i=2;i<=N;++i)
                        {
                            reserved[i] = square_of(Y[i]-(Z[i]=F.reach(X[i],A,V)));
                        }
                        return sorted_sum(reserved);
                    }
                    else
                    {
                        return 0;
                    }
                }



                //! D2, beta and alpha
                inline virtual ORDINATE D2(matrix<ORDINATE>           &alpha,
                                           addressable<ORDINATE>      &beta,
                                           sequential_type            &F,
                                           sequential_grad            &G,
                                           const accessible<ORDINATE> &A,
                                           const accessible<bool>     &used)
                {
                    assert(dFdA.size()==A.size());
                    assert(beta.size()==A.size());
                    assert(beta.size()==alpha.rows);
                    assert(beta.size()==alpha.cols);

                    const accessible<ABSCISSA> &X = *abscissa;
                    const accessible<ORDINATE> &Y = *ordinate;
                    addressable<ORDINATE>      &Z = *adjusted;
                    const variables            &V = this->vars;
                    const size_t                N = count();
                    tao::ld(beta,0);
                    alpha.ld(0);

                    if(N>0)
                    {
                        {
                            const ABSCISSA &Xi = X[1];
                            const ORDINATE dY  = Y[1]-(Z[1]=F.start(Xi,A,V));
                            reserved[1] = square_of(dY);
                            G(dFdA,F,Xi,A,V,used);
                            tao::muladd(beta,dY,dFdA);
                            add_to(alpha);
                        }
                        for(size_t i=2;i<=N;++i)
                        {
                            const ABSCISSA &Xi = X[i];
                            const ORDINATE dY  = Y[i]-(Z[i]=F.reach(Xi,A,V));
                            reserved[1] = square_of(dY);
                            G(dFdA,F,Xi,A,V,used);
                            tao::muladd(beta,dY,dFdA);
                            add_to(alpha);
                        }
                        return sorted_sum(reserved);
                    }
                    else
                    {
                        return 0;
                    }
                }
                
                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                abscissa_type    abscissa; //!< abscissa, NDim
                ordinate_type    ordinate; //!< ordinate, 1Dim
                ordinate_type    adjusted; //!< adjusted, 1Dim
                vector<ORDINATE> reserved; //!< memory,   1Dim
                vector<ORDINATE> dFdA; //!< memory for gradient
                const ORDINATE   zero; //!< a zero ordinate value


            private:
                Y_DISABLE_COPY_AND_ASSIGN(sample);
                
                inline void add_to(matrix<ORDINATE> &alpha) const throw()
                {
                    const accessible<ORDINATE> &df = dFdA;
                    for(size_t i=df.size();i>0;--i)
                    {
                        const ORDINATE         df_i    = df[i];
                        addressable<ORDINATE> &alpha_i = alpha[i];
                        alpha_i[i] += df_i * df_i;
                        for(size_t j=i-1;j>0;--j)
                        {
                            alpha_i[j] += df_i * df[j];
                        }
                    }
                }
            };
        }

    }

}

#endif
