
//! \file

#ifndef Y_CHEMICAL_GVOBJ_INCLUDED
#define Y_CHEMICAL_GVOBJ_INCLUDED 1

#include "y/chemical/object.hpp"
#include "y/ios/tools/vizible.hpp"

namespace upsylon
{
    namespace Chemical
    {

        typedef ios::vizible Vizible;      //!< to encode as GraphViz

        //______________________________________________________________________
        //
        //
        //! Vizible based on Object
        //
        //______________________________________________________________________
        class gvObject : public Object, public Vizible
        {
        public:
            virtual ~gvObject() throw(); //!< cleanup

        protected:
            explicit gvObject() throw(); //!< setup
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(gvObject);
        };
    }

}

#endif
