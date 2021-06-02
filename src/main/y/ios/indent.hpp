

//! \file
#ifndef Y_IOS_ENDL_INCLUDED
#define Y_IOS_ENDL_INCLUDED 1

#include "y/os/platform.hpp"

namespace upsylon
{

    namespace ios
    {

        class indent
        {
        public:
            const size_t width;
            const char   space;
            indent(const size_t w, const char c=' ') throw();
            indent(const indent &) throw();
            ~indent() throw();

            template <typename OSTREAM> inline
            friend OSTREAM & operator<<(OSTREAM &os, const indent &self)
            {
                for(size_t i=self.width;i>0;--i) { os << self.space; }
                return os;
            }

        private:
            Y_DISABLE_ASSIGN(indent);
        };

    }

}

#endif
