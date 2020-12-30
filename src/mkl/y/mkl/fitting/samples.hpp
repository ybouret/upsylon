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

            struct _samples
            {
                static void throw_multiple_sample(const string &name);
            };

            //__________________________________________________________________
            //
            //
            //! sample for a set of data
            //
            //__________________________________________________________________
            template <
            typename ABSCISSA,
            typename ORDINATE
            >
            class samples : public sample_api<ABSCISSA,ORDINATE>,
            public hash_set<string, typename sample<ABSCISSA,ORDINATE>::pointer>
            {
            public:
                //______________________________________________________________
                //
                // types and definitions
                //______________________________________________________________
                typedef sample_api<ABSCISSA,ORDINATE>       api_type;       //!< alias
                typedef sample<ABSCISSA,ORDINATE>           single_sample;  //!< alias
                typedef typename single_sample::pointer     shared_sample;  //!< alias
                typedef hash_set<string,shared_sample>      samples_db;     //!< alias
                typedef typename samples_db::const_iterator const_iterator; //!< alias

                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                //! setup
                template <typename ID>
                inline explicit samples(const ID &id) : api_type(id), samples_db() {}

                //! cleanup
                inline virtual ~samples() throw() {}

                //______________________________________________________________
                //
                // sample_interface
                //______________________________________________________________
                inline virtual size_t count() const throw()
                {
                    size_t ans = 0;
                    for(const_iterator it=this->begin();it!=this->end();++it)
                    {
                        ans += (**it).count();
                    }
                    return ans;
                }


                //______________________________________________________________
                //
                // design interface
                //______________________________________________________________
                template <typename ID>
                single_sample & operator()(const ID &id,const size_t n=0)
                {
                    shared_sample p = single_sample::create(id,n);
                    if(!this->insert(p)) _samples::throw_multiple_sample(p->name);
                    return *p;
                }


            private:
                Y_DISABLE_COPY_AND_ASSIGN(samples);
            };

        }

    }

}
#endif

