

//! \file

#ifndef Y_CHEMICAL_GVOBJ_INCLUDED
#define Y_CHEMICAL_GVOBJ_INCLUDED 1

#include "y/chemical/object.hpp"
#include "y/ios/tools/vizible.hpp"

namespace upsylon
{
    namespace Chemical
    {
        class gvObject : public Object
        {
        public:
            virtual ~gvObject() throw();
            virtual const char * label() const throw() = 0;
            
            
        protected:
            explicit gvObject() throw();
        private:
            Y_DISABLE_COPY_AND_ASSIGN(gvObject);
        };
    }

}

#endif

