
//! \file

#ifndef Y_FITTING_SAMPLE_INFO_INCLUDED
#define Y_FITTING_SAMPLE_INFO_INCLUDED 1

#include "y/mkl/fitting/variables.hpp"
#include "y/ios/ostream.hpp"

namespace upsylon
{
    namespace mkl
    {
        namespace fitting
        {

            //__________________________________________________________________
            //
            //
            //! sample info
            //
            //__________________________________________________________________
            class sample_info : public counted, public object
            {
            public:
                //______________________________________________________________
                //
                // virtual inteface
                //______________________________________________________________
                virtual       ~sample_info() throw();     //!< cleanup
                virtual size_t count() const throw() = 0; //!< number of points

                //______________________________________________________________
                //
                // non-virtual interface
                //______________________________________________________________
                const string    &key()        const throw(); //!< name
                variables       & operator*()       throw(); //!< access
                const variables & operator*() const throw(); //!< access

                //! helper to quick save
                static void       save_triplet( ios::ostream &fp, const double, const double, const double);

                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                const string name; //!< unique identifier
                variables    vars; //!< local or global variables

            protected:
                //! setup
                template <typename ID> inline
                explicit sample_info(const ID &id) :
                counted(),
                object(),
                name(id),
                vars()
                {
                }


            private:
                Y_DISABLE_COPY_AND_ASSIGN(sample_info);
            };
        }

    }

}

#endif
