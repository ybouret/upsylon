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
                static void throw_multiple_sample(const string &name);
            };

            //__________________________________________________________________
            //
            //
            //! sample for a set of data
            //
            //__________________________________________________________________
            template <
            typename T,
            typename ABSCISSA,
            typename ORDINATE
            >
            class samples : public sample_api<T,ABSCISSA,ORDINATE>,
            public hash_set<string, typename sample<T,ABSCISSA,ORDINATE>::pointer>
            {
            public:
                //______________________________________________________________
                //
                // types and definitions
                //______________________________________________________________
                typedef sample_api<T,ABSCISSA,ORDINATE>       api_type;       //!< alias
                typedef sample<T,ABSCISSA,ORDINATE>           single_sample;  //!< alias
                typedef typename single_sample::pointer       shared_sample;  //!< alias
                typedef hash_set<string,shared_sample>        samples_db;     //!< alias
                typedef typename samples_db::const_iterator   const_iterator; //!< alias
                typedef typename samples_db::iterator         iterator;       //!< alias

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
                //! create a new sample in the database
                template <typename ID> inline
                single_sample & operator()(const ID &id,const size_t n=0)
                {
                    shared_sample p = single_sample::create(id,n);
                    if(!this->insert(p)) _samples::throw_multiple_sample(p->name);
                    return *p;
                }

                //! create a filled sample
                template <typename ID> inline
                single_sample * create(const ID &id, const ABSCISSA *x, const ORDINATE *y, const size_t n)
                {
                    shared_sample p = single_sample::create(id,x,y,n);
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

