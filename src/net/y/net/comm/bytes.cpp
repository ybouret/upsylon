#include "y/net/comm/bytes.hpp"
#include "y/type/aliasing.hpp"
#include <cstring>

namespace upsylon
{
    namespace net
    {

        comm_bytes:: comm_bytes() throw() : comm_bytes_type(), pool()
        {
        }

        comm_bytes::  comm_bytes(size_t n, const as_capacity_t &) :
        comm_bytes_type(), pool()
        {
            reserve(n);
        }


        comm_bytes:: ~comm_bytes() throw()
        {
        }

        void comm_bytes:: reserve(size_t n)
        {
            static comm_byte::supply &mgr = comm_byte::instance();
            while(n-- > 0)
            {
                pool.push_back( mgr.acquire() );
            }
        }

        comm_byte * comm_bytes:: rig(const uint8_t code)
        {
            static comm_byte::supply &mgr = comm_byte::instance();
            if(pool.size)
            {
                comm_byte *node = pool.pop_back();
                aliasing::_(node->code) = code;
                return node;
            }
            else
            {
                return mgr.acquire<uint8_t>(code);
            }
        }


        void comm_bytes::push(const uint8_t code)
        {
            (void)push_back( rig(code) );
        }

        void comm_bytes:: push(const void *buffer, const size_t buflen)
        {
            assert( !(0==buffer&&buflen>0) );

            static comm_byte::supply &mgr  = comm_byte::instance();
            const uint8_t            *code = static_cast<const uint8_t *>(buffer);
            size_t                    todo = buflen;

            if(pool.size>=todo)
            {
                while(todo-- >0)
                {
                    aliasing::_(push_back( pool.pop_back() )->code )= *(code++);
                }
            }
            else
            {
                assert(todo>pool.size);
                for(size_t delta=todo-pool.size;delta>0;--delta, --todo)
                {
                    push_back( mgr.acquire<uint8_t>( *(code++) ) );
                }
                assert(todo==pool.size);
                while(todo-- >0)
                {
                    aliasing::_(push_back( pool.pop_back() )->code )= *(code++);
                }
            }

        }

        void comm_bytes:: push(const char *text)
        {
            if(text) push(text, strlen(text));
        }

        void comm_bytes::push(const memory::ro_buffer &buff)
        {
            push( buff.ro(), buff.length() );
        }

        void comm_bytes:: clear() throw()
        {
            while(size>0 )
            {
                aliasing::_( pool.push_back( pop_back() )->code ) = 0;
            }
        }

        uint8_t comm_bytes:: pop() throw()
        {
            assert(size>0);
            assert(head!=NULL);
            const uint8_t code = head->code;
            aliasing::_(pool.push_back( pop_front() )->code) = 0;
            return code;
        }

        void comm_bytes:: prune() throw()
        {
            pool.release();
        }

    }

}
