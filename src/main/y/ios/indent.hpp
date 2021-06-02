

//! \file
#ifndef Y_IOS_ENDL_INCLUDED
#define Y_IOS_ENDL_INCLUDED 1

#include "y/os/platform.hpp"

namespace upsylon
{

    namespace ios
    {
        //______________________________________________________________________
        //
        //
        //! automatic indentation
        //
        //______________________________________________________________________
        class indent
        {
        public:
            const size_t width; //!< width
            const char   space; //!< printed

            indent(const size_t w, const char c=' ') throw(); //!< create
            indent(const indent &) throw();                   //!< copy
            ~indent() throw();                                //!< cleanup

            //! flexible output
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
