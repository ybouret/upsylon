#include "y/net/io/bytes.hpp"
#include "y/type/aliasing.hpp"
#include <cstring>

namespace upsylon
{
    namespace net
    {

        io_bytes:: io_bytes() throw() : io_bytes_type(), pool()
        {
        }

        io_bytes::  io_bytes(size_t n, const as_capacity_t &) :
        io_bytes_type(), pool()
        {
            reserve(n);
        }


        io_bytes:: ~io_bytes() throw()
        {
        }

        void io_bytes:: reserve(size_t n)
        {
            static io_byte::supply &mgr = io_byte::instance();
            while(n-- > 0)
            {
                pool.push_back( mgr.acquire<uint8_t>(0) );
            }
        }

        io_bytes & io_bytes:: operator<<(const uint8_t code)
        {
            static io_byte::supply &mgr = io_byte::instance();
            if(pool.size)
            {
                aliasing::_(push_back( pool.pop_back() )->code )= code;
            }
            else
            {
                push_back( mgr.acquire<uint8_t>(code) );
            }
            return *this;
        }

        void io_bytes:: push(const void *buffer, const size_t buflen)
        {
            assert( !(0==buffer&&buflen>0) );

            static io_byte::supply &mgr = io_byte::instance();
            const uint8_t          *code = static_cast<const uint8_t *>(buffer);
            size_t                  todo = buflen;

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

        io_bytes & io_bytes:: operator<<(const char   *text)
        {
            if(text) push(text, strlen(text));
            return *this;
        }

        io_bytes & io_bytes:: operator<<(const memory::ro_buffer &buff)
        {
            push( buff.ro(), buff.length() );
            return *this;
        }

        void io_bytes:: clear() throw()
        {
            pool.merge_back(*this);
        }

        uint8_t io_bytes:: pop() throw()
        {
            assert(size>0);
            assert(head!=NULL);
            const uint8_t code = head->code;
            pool.push_back( pop_front() );
            return code;
        }



    }

}
