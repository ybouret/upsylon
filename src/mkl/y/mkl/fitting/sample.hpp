//! \file

#ifndef Y_FITTING_SAMPLE_INCLUDED
#define Y_FITTING_SAMPLE_INCLUDED 1

#include "y/mkl/fitting/sample/api.hpp"
#include "y/sequence/vector.hpp"
#include "y/ptr/arr.hpp"
#include "y/ptr/auto.hpp"

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
            template <
            typename T,
            typename ABSCISSA,
            typename ORDINATE
            >
            class sample: public sample_api<T,ABSCISSA,ORDINATE>
            {
            public:
                //______________________________________________________________
                //
                // types and definitions
                //______________________________________________________________
                typedef          sample_api<T,ABSCISSA,ORDINATE> api_type;      //!< alia
                typedef          intr_ptr<string,sample>         pointer;       //!< for samples
                typedef typename series<ABSCISSA>::type          abscissa_type; //!< alias
                typedef typename series<ORDINATE>::type          ordinate_type; //!< alias

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
                __zero__(0)
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

                //! add x,y,__zero__
                inline void add(const ABSCISSA x, const ABSCISSA y)
                {
                    assert( abscissa.size() == ordinate.size() );
                    assert( abscissa.size() == adjusted.size() );
                    abscissa->push_back(x);
                    try { ordinate->push_back(y); } catch(...) { abscissa->pop_back(); throw; }
                    try { adjusted->push_back(__zero__); }
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

                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                abscissa_type  abscissa; //!< abscissa, NDim
                ordinate_type  ordinate; //!< ordinate, 1Dim
                ordinate_type  adjusted; //!< adjusted, 1Dim
                const ORDINATE __zero__; //!< a zero ordinate value


            private:
                Y_DISABLE_COPY_AND_ASSIGN(sample);
            };
        }

    }

}

#endif
