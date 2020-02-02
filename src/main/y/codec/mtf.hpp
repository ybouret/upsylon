//! \file
#ifndef Y_CODEC_MTF_INCLUDED
#define Y_CODEC_MTF_INCLUDED 1

#include "y/codec/shaker.hpp"
#include "y/core/list.hpp"

namespace upsylon {

    namespace information {

        //! move to front base class
        class mtf_modulation : public shaker
        {
        public:
            virtual     ~mtf_modulation() throw(); //!< cleanup
            virtual void reset()          throw(); //!< rebuild state

        protected:
            explicit mtf_modulation() throw();     //!< setup

            //! basic node for list
            struct node_t
            {
                node_t *next; //!< for list
                node_t *prev; //!< for list
                uint8_t data; //!< initial node index
            };
            core::list_of<node_t>  list_;      //!< fast list
            node_t                 node_[256]; //!< built-in nodes

        private:
            Y_DISABLE_COPY_AND_ASSIGN(mtf_modulation);
            void build() throw();
        };

        //! move to front encoder
        class mtf_encoder : public mtf_modulation
        {
        public:
            virtual        ~mtf_encoder()        throw(); //!< cleanup
            explicit        mtf_encoder()        throw(); //!< setup
            virtual uint8_t fetch(const uint8_t) throw(); //!< encode

        private:
            Y_DISABLE_COPY_AND_ASSIGN(mtf_encoder);
        };

        //! move to front decoder
        class mtf_decoder : public mtf_modulation
        {
        public:
            virtual        ~mtf_decoder()        throw();//!< cleanup
            explicit        mtf_decoder()        throw();//!< setup
            virtual uint8_t fetch(const uint8_t) throw();//!< decode

        private:
            Y_DISABLE_COPY_AND_ASSIGN(mtf_decoder);
        };

    }

}


#endif
