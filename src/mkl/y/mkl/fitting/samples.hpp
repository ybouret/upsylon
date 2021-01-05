//! \file

#ifndef Y_FITTING_SAMPLES_INCLUDED
#define Y_FITTING_SAMPLES_INCLUDED 1

#include "y/mkl/fitting/sample.hpp"

namespace upsylon
{
    namespace mkl
    {
        namespace fitting
        {
            
            //__________________________________________________________________
            //
            //
            //! common functions for samples
            //
            //__________________________________________________________________
            struct _samples
            {
                //! raise an exception
                static void throw_multiple_sample(const string &owner, const string &name);
                
                //! raise an exception
                static void throw_missing_sample(const string &owner, const string &name);
            };
            
            //__________________________________________________________________
            //
            //
            //! sample for a set of data
            //
            //__________________________________________________________________
            template <typename ABSCISSA, typename ORDINATE>
            class samples : public sample_api<ABSCISSA,ORDINATE>,
            public hash_set<string, typename sample<ABSCISSA,ORDINATE>::pointer>
            {
            public:
                //______________________________________________________________
                //
                // types and definitions
                //______________________________________________________________
                typedef sample_api<ABSCISSA,ORDINATE>               api_type;        //!< alias
                typedef sample<ABSCISSA,ORDINATE>                   single_sample;   //!< alias
                typedef typename single_sample::pointer             shared_sample;   //!< alias
                typedef hash_set<string,shared_sample>              samples_db;      //!< alias
                typedef typename samples_db::const_iterator         const_iterator;  //!< alias
                typedef typename samples_db::iterator               iterator;        //!< alias
                typedef typename api_type::sequential_type          sequential_type; //!< alias
                typedef typename api_type::v_gradient_type          v_gradient_type; //!< alias
                
                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                //! setup
                template <typename ID>
                inline explicit samples(const ID &id) :
                api_type(id),
                samples_db(),
                reserved(),
                __beta(),
                __alpha()
                {}
                
                //! cleanup
                inline virtual ~samples() throw() {}
                
                //______________________________________________________________
                //
                // sample_interface
                //______________________________________________________________
                
                //! sum of all counts
                inline virtual size_t count() const throw()
                {
                    size_t ans = 0;
                    for(const_iterator it=this->begin();it!=this->end();++it)
                    {
                        ans += (**it).count();
                    }
                    return ans;
                }
                
                //! setup each sample
                inline virtual void setup(const accessible<ORDINATE> &aorg)
                {
                    reserved.adjust(this->size(),0);
                    for(iterator it=this->begin();it!=this->end();++it)
                    {
                        (**it).setup(aorg);
                    }
                    const size_t dims = aorg.size();
                    __beta.adjust(dims,this->zero);
                    __alpha.make(dims,dims);
                    
                }

                //! update correlation
                inline virtual void update_correlation(correlation<ORDINATE> &corr) const
                {
                    for(const_iterator it=this->begin();it!=this->end();++it)
                    {
                        (**it).update_correlation(corr);
                    }
                }
                
                //______________________________________________________________
                //
                // design interface
                //______________________________________________________________
                
                //! record a new dynamic sample
                template <typename SAMPLE_TYPE>
                SAMPLE_TYPE & operator()( SAMPLE_TYPE *s )
                {
                    return use(s);
                }
                
                //! create a new sample in the database
                template <typename ID> inline
                single_sample & create(const ID &id,const size_t n=0)
                {
                    return (*this)( single_sample::create(id,n) );
                }
                
                //! create a filled sample
                template <typename ID> inline
                single_sample & create(const ID &id, const ABSCISSA *x, const ORDINATE *y, const size_t n)
                {
                    return (*this)( single_sample::create(id,x,y,n) );
                }
                
                //! create a clone sample
                template <typename ID> inline
                single_sample & create(const ID &id, const single_sample &source)
                {
                    return (*this)( source.clone_as(id) );
                }
                
                
                
                //______________________________________________________________
                //
                // query samples
                //______________________________________________________________
                
                //! query by string
                inline single_sample & operator[](const string &id)
                {
                    const shared_sample *pps = this->search(id);
                    if(!pps) _samples::throw_missing_sample(this->name,id);
                    shared_sample tmp(*pps);
                    return *tmp;
                }
                
                //! query by string
                inline const single_sample & operator[](const string &id) const
                {
                    const shared_sample *pps = this->search(id);
                    if(!pps) _samples::throw_missing_sample(this->name,id);
                    return **pps;
                }
                //! query by text
                inline single_sample & operator[](const char *id)
                {
                    const string _(id); return (*this)[_];
                }
                
                //! query by text
                inline const single_sample & operator[](const char *id) const
                {
                    const string _(id); return (*this)[_];
                }
                
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(samples);
                vector<ORDINATE> reserved;
                vector<ORDINATE> __beta;
                matrix<ORDINATE> __alpha;
                
                template <typename SAMPLE_TYPE>
                SAMPLE_TYPE & use( SAMPLE_TYPE *s )
                {
                    const shared_sample p = s;
                    if(!this->insert(p)) _samples::throw_multiple_sample(this->name,p->name);
                    return *s;
                }

                //______________________________________________________________
                //
                //! return D2, weighted sum of samples
                //______________________________________________________________
                inline virtual ORDINATE D2_only(sequential_type            &F,
                                                const accessible<ORDINATE> &A)
                {
                    assert(reserved.size()==this->size() || die("setup") );
                    size_t sum = 0;
                    {
                        size_t i=0;
                        for(iterator it=this->begin();it!=this->end();++it)
                        {
                            single_sample &s = (**it);
                            const size_t   n = s.count();

                            sum          += n;
                            reserved[++i] = n * s.D2(F,A);
                        }
                    }
                    return (sum>0) ? sorted_sum(reserved)/sum : this->zero;
                }
                
                //______________________________________________________________
                //
                //! return weighted D2, alpha and beta
                //______________________________________________________________
                virtual ORDINATE D2_full(matrix<ORDINATE>           &alpha,
                                         addressable<ORDINATE>      &beta,
                                         sequential_type            &F,
                                         v_gradient_type            &G,
                                         const accessible<ORDINATE> &A,
                                         const accessible<bool>     &used)
                {
                    const size_t dims = A.size();
                    assert(dims==alpha.rows);
                    assert(dims==alpha.cols);
                    assert(dims==beta.size());
                    
                    assert(dims==__alpha.rows || die("setup"));
                    assert(dims==__alpha.cols || die("setup"));
                    assert(dims==__beta.size()|| die("setup"));

                    tao::ld(beta,this->zero);
                    tao::ldm(alpha,this->zero);
                    
                    size_t sum = 0;
                    {
                        size_t i=0;
                        for(iterator it=this->begin();it!=this->end();++it)
                        {
                            single_sample &s = (**it);
                            const size_t   n = s.count();
                            
                            sum          += n;
                            reserved[++i] = n * s.D2(__alpha,__beta,F,G,A,used);
                            for(size_t j=dims;j>0;--j)
                            {
                                beta[j] += __beta[j] * n;
                                for(size_t k=j;k>0;--k)
                                {
                                    alpha[j][k] += n * __alpha[j][k];
                                }
                            }
                        }
                    }
                    if(sum>0)
                    {
                        
                        for(size_t j=dims;j>0;--j)
                        {
                            beta[j] /= sum;
                            for(size_t k=j;k>0;--k)
                            {
                                alpha[j][k] /= sum;
                            }
                        }
                        return sorted_sum(reserved)/sum;
                    }
                    else
                    {
                        return this->zero;
                    }
                }
            };
            
        }
        
    }
    
}
#endif

