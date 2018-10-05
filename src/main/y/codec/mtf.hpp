//! \file
#ifndef Y_CODEC_MTF_INCLUDED
#define Y_CODEC_MTF_INCLUDED 1

#include "y/core/list.hpp"

namespace upsylon
{

    //! move to fron algorithm
    class move_to_front
    {
    public:
        explicit move_to_front() throw();   //!< uinitialize
        virtual ~move_to_front() throw();   //!< destructor
        uint8_t  encode( uint8_t ) throw(); //!< encoding
        uint8_t  decode( uint8_t ) throw(); //!< decoding
        void     reset(void)   throw();     //!< reset

        //! possible output=input
        void     encode(void *output,const void *input, size_t length) throw();

        //! possible output=input
        void     decode(void *output,const void *input, size_t length) throw();


    private:
        Y_DISABLE_COPY_AND_ASSIGN(move_to_front);
        struct node_t
        {
            node_t *next;
            node_t *prev;
            uint8_t data;
        };
        core::list_of<node_t>  list_;
        node_t                 node_[256];
        void build() throw();
    };
    
}


#endif
