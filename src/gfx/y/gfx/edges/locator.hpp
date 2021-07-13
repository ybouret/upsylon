
//! \file

#ifndef Y_GFX_EDGES_LOCATOR_INCLUDED
#define Y_GFX_EDGES_LOCATOR_INCLUDED 1

#include "y/gfx/edges/detector.hpp"

namespace upsylon
{
    namespace graphic
    {

        namespace edges
        {

            //__________________________________________________________________
            //
            //
            //! detector with resources
            //
            //__________________________________________________________________
            class locator : public detector
            {
            public:
                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                virtual      ~locator() throw(); //!< cleanup

                //! setup with a named filter
                explicit      locator(const string       &F,
                                      const unit_t        W,
                                      const unit_t        H,
                                      const shared_knots &K);

                //! setup with a named filter
                explicit      locator(const char         *F,
                                      const unit_t        W,
                                      const unit_t        H,
                                      const shared_knots &K);


                //______________________________________________________________
                //
                // methods
                //______________________________________________________________

                //! find edges with default conversion
                template <typename T> inline
                size_t compat_find(blobs                &userBlobs,
                                   const pixmap<T>      &source,
                                   broker               &apply,
                                   const blur           *cloud,
                                   const size_t          cutSize)
                {
                    (void) prepare(source,apply,convert<float,rgba>::from,delta,cloud);
                    return finalize(userBlobs,cutSize);
                }

                //! find edges from prepared source
                size_t direct_find(blobs                &userBlobs,
                                   const pixmap<float>  &source,
                                   broker               &apply,
                                   const blur           *cloud,
                                   const size_t          cutSize);

                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                const shared_filters delta; //!< for gradient
                shared_knots         cache; //!< for blobs

            private:
                Y_DISABLE_COPY_AND_ASSIGN(locator);
                size_t finalize(blobs       &userBlobs,
                                const size_t cutSize);
            };

        }
    }
}

#endif

