

//! \file

#ifndef Y_ALCHEMY_GUARD_INCLUDED
#define Y_ALCHEMY_GUARD_INCLUDED 1

#include "y/alchemy/leading.hpp"

namespace upsylon
{
    namespace Alchemy
    {
        //! guard leading conditions for an equilibrium
        class Guard : public object, public counted
        {
        public:
            typedef arc_ptr<Guard>            Pointer;
            typedef vector<Pointer,Allocator> Array_;

            //! class of guard
            enum Class
            {
                HasNoBound,   //!< no leading condition
                HasOnlyLEQ,   //!< leq.size>0, geq.size==0
                HasOnlyGEQ,   //!< leq.size==0, geq.size>0
                IsBothWays    //!< leq.size>0, geq.size>0
            };
            
            //! pre-allocated arrays
            class Array : public Array_
            {
            public:
                explicit Array(size_t);   //!< setup
                virtual ~Array() throw(); //!< cleanup
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Array);
            };

            explicit Guard() throw();
            virtual ~Guard() throw();

            const Leading::Array leq;
            const Leading::Array geq;
            const Class          cls;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Guard);
        };

    }

}

#endif
