#include "y/information/modulation/mtf.hpp"

#include <cstring>
#include <iostream>

namespace upsylon {

    namespace Information {

        MoveToFront:: MoveToFront() throw() :
        list_(),
        node_()
        {
            build();
        }

        MoveToFront:: ~MoveToFront() throw()
        {
            list_.reset();
            memset( node_, 0, sizeof(node_) );
        }

        void MoveToFront:: build() throw()
        {
            memset( node_, 0, sizeof(node_) );
            for(size_t i=0;i<256;++i)
            {
                list_.push_back( &node_[i] )->data = uint8_t(i);
            }
            assert( 256 == list_.size );
        }

        void MoveToFront:: duplicate( const MoveToFront &m ) throw()
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



        void MoveToFront::reset() throw()
        {
            list_.reset();
            build();
        }


    }

    namespace Information {

        Y_INFORMATION_MODULATION(MoveToFrontEncoder);


        MoveToFrontEncoder::  MoveToFrontEncoder() throw() {}
        MoveToFrontEncoder:: ~MoveToFrontEncoder() throw() {}

        uint8_t MoveToFrontEncoder:: fetch(const uint8_t x) throw()
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

        Modulation * MoveToFrontEncoder :: clone() const
        {
            MoveToFrontEncoder *m = new MoveToFrontEncoder();
            m->duplicate(*this);
            return m;
        }

    }


    namespace Information {

        Y_INFORMATION_MODULATION(MoveToFrontDecoder);


        MoveToFrontDecoder::  MoveToFrontDecoder() throw() {}
        MoveToFrontDecoder:: ~MoveToFrontDecoder() throw() {}

        uint8_t MoveToFrontDecoder:: fetch(const uint8_t x) throw()
        {
            assert(list_.size == 256 );
            node_t *node = list_.head;
            for(size_t i=x;i>0;--i) {
                node=node->next;
                assert(NULL!=node);
            }
            return list_.move_to_front(node)->data;
        }


        Modulation * MoveToFrontDecoder :: clone() const
        {
            MoveToFrontDecoder *m = new MoveToFrontDecoder();
            m->duplicate( *this );
            return m;
        }
    }

}

