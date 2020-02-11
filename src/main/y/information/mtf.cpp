#include "y/information/mtf.hpp"

#include <cstring>
#include <iostream>

namespace upsylon {

    namespace information {

        mtf_modulation:: mtf_modulation() throw() :
        list_(),
        node_()
        {
            build();
        }

        mtf_modulation:: ~mtf_modulation() throw()
        {
            list_.reset();
            memset( node_, 0, sizeof(node_) );
        }

        void mtf_modulation:: build() throw()
        {
            memset( node_, 0, sizeof(node_) );
            for(size_t i=0;i<256;++i)
            {
                list_.push_back( &node_[i] )->data = uint8_t(i);
            }
            assert( 256 == list_.size );
        }

        void mtf_modulation:: duplicate( const mtf_modulation &m ) throw()
        {
            list_.reset();
            for( const node_t *node = m.list_.head; node; node=node->next)
            {
                node_t *sub = &node_[node->data];
                assert(sub->data==node->data);
                sub->next = sub->prev = 0;
                list_.push_back(sub);
            }
            assert(256==list_.size);
        }



        void mtf_modulation::reset() throw()
        {
            list_.reset();
            build();
        }


    }

    namespace information {

        mtf_encoder::  mtf_encoder() throw() {}
        mtf_encoder:: ~mtf_encoder() throw() {}

        uint8_t mtf_encoder:: fetch(const uint8_t x) throw()
        {
            assert(list_.size == 256 );
            node_t *node = list_.head;
            int     indx = 0;
            while( node->data != x ) {
                ++indx;
                node = node->next;
                assert( node != NULL );
                assert( indx < 256   );
            }
            list_.move_to_front(node);
            assert(list_.size == 256 );
            return uint8_t(indx);
        }

        shaker * mtf_encoder :: clone() const
        {
            mtf_modulation *m = new mtf_encoder();
            m->duplicate(*this);
            return m;
        }

    }


    namespace information {

        mtf_decoder::  mtf_decoder() throw() {}
        mtf_decoder:: ~mtf_decoder() throw() {}

        uint8_t mtf_decoder:: fetch(const uint8_t x) throw()
        {
            assert(list_.size == 256 );
            node_t *node = list_.head;
            for(size_t i=x;i>0;--i) {
                node=node->next;
                assert(NULL!=node);
            }
            list_.move_to_front(node);
            return node->data;
        }


        shaker * mtf_decoder :: clone() const
        {
            mtf_modulation *m = new mtf_decoder();

            return m;
        }
    }

}

