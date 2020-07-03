
#include "y/net/socket/set.hpp"
#include "y/core/locate.hpp"
#include "y/type/block/zset.hpp"
#include "y/type/aliasing.hpp"

#if defined(Y_BSD)
#include <sys/select.h>
#endif

#if !defined(NDEBUG)
#include "y/alea.hpp"
#endif

namespace upsylon
{
    namespace net
    {
        socket_set:: ~socket_set() throw()
        {
            memory::global::location().release(workspace,aliasing::_(allocated));
        }

        void socket_set:: free() throw()
        {
            _bzset(size);
            memset(workspace,0,allocated);
        }

        socket_set:: socket_set():
        size(0),
        allocated( capacity * sizeof(socket_type) + 4 * memory::align(sizeof(fd_set))  ),
        workspace( memory::global::instance().acquire( aliasing::_(allocated)) ),
        sock( memory::io::cast<socket_type>(workspace,0) ),
        ufd(  memory::io::cast<fd_set>(workspace,capacity*sizeof(socket_type) ) ),
        wfd(  memory::io::cast<fd_set>(ufd, Y_MEMORY_ALIGN(sizeof(fd_set)) ) ),
        rfd(  memory::io::cast<fd_set>(wfd, Y_MEMORY_ALIGN(sizeof(fd_set)) ) ),
        xfd(  memory::io::cast<fd_set>(rfd, Y_MEMORY_ALIGN(sizeof(fd_set)) ) )
        {

#if !defined(NDEBUG)
            {
                uint8_t *b = static_cast<uint8_t *>(workspace);

                for(size_t i=0;i<allocated;++i)
                {
                    b[i] = alea.full<uint8_t>();
                }

            }

#define Y_NET_S32 crc32(sock,capacity*sizeof(socket_type))
#define Y_NET_U32 crc32(ufd,sizeof(fd_set))
#define Y_NET_W32 crc32(wfd,sizeof(fd_set))
#define Y_NET_R32 crc32(rfd,sizeof(fd_set))
#define Y_NET_X32 crc32(xfd,sizeof(fd_set))
#define Y_NET_CHK() assert(Y_NET_S32==s32); assert(Y_NET_U32==u32); assert(Y_NET_W32==w32); assert(Y_NET_R32==r32); assert(Y_NET_X32==x32)
#define Y_NET_OUT() std::cerr << s32 << "." << u32 << "." << w32 << "." << r32 << "." << x32 << std::endl

            uint32_t s32 = Y_NET_S32;
            uint32_t u32 = Y_NET_U32;
            uint32_t w32 = Y_NET_W32;
            uint32_t r32 = Y_NET_R32;
            uint32_t x32 = Y_NET_X32;
            // Y_NET_OUT();
#endif
            FD_ZERO(xfd);
#if !defined(NDEBUG)
            x32 = Y_NET_X32;
            //Y_NET_OUT();
            Y_NET_CHK();
#endif

            FD_ZERO(rfd);
#if !defined(NDEBUG)
            r32 = Y_NET_R32;
            //Y_NET_OUT();
            Y_NET_CHK();
#endif

            FD_ZERO(wfd);
#if !defined(NDEBUG)
            w32 = Y_NET_W32;
            //Y_NET_OUT();
            Y_NET_CHK();
#endif

            FD_ZERO(ufd);
#if !defined(NDEBUG)
            u32 = Y_NET_U32;
            //Y_NET_OUT();
            Y_NET_CHK();
            memset(sock,0,capacity*sizeof(socket_type));
            s32 = Y_NET_S32;
            //Y_NET_OUT();
            Y_NET_CHK();
#endif

        }


        static inline
        int __compare_socks( const socket_type &lhs, const socket_type &rhs ) throw()
        {
            const ptrdiff_t L = static_cast<ptrdiff_t>(lhs);
            const ptrdiff_t R = static_cast<ptrdiff_t>(rhs);
            return comparison::increasing(L,R);
        }

        void socket_set:: insert( const bsd_socket &s )
        {
            static const char fn[] = "net::socket_set::insert";

            if(size>=capacity)
            {
                throw upsylon::exception("%s(exceeded capacity=%u)",fn,unsigned(capacity));
            }


            const socket_type lhs = s.sock;

            // low-level check
            if( FD_ISSET(lhs,ufd) )
            {
                throw upsylon::exception("%s(multiple system descriptor)",fn);
            }
            
            // high-level check
            size_t            idx = 0;
            if( core::locate(lhs,sock,size,__compare_socks, idx) )
            {
                throw upsylon::exception("%s(multiple high-level descriptor)",fn);
            }
            socket_type *target = sock+idx;
            size_t      &length = aliasing::_(size);
            memmove(target+1,target,( (length++) - idx )*sizeof(socket_type));
            *target = lhs;
            FD_SET(lhs,ufd);
            assert(FD_ISSET(lhs,ufd));
        }

        void socket_set::remove(const bsd_socket &s) throw()
        {
            const socket_type lhs = s.sock;

            // remove low-level
            FD_CLR(lhs,ufd);
            assert( !FD_ISSET(lhs,ufd) );

            // remove high-level
            size_t            idx = 0;
            if( core::locate(lhs,sock,size,__compare_socks, idx) )
            {
                assert(size>0);
                socket_type *target = sock+idx;
                size_t      &length = aliasing::_(size);
                memmove(target,target+1,( (--length)-idx ) * sizeof(socket_type) );
            }
            assert( !core::locate(lhs,sock,size,__compare_socks, idx) );
        }
        


        size_t socket_set:: call_select(fd_set *r, fd_set *w, fd_set *x, struct timeval *tv) const
        {
            Y_GIANT_LOCK();
#if defined(Y_BSD)
            const socket_type fmx = sock[size-1]+1;
            int               ans = 0;
            while( (ans= ::select(fmx, r, w, x, tv))<0 )
            {
                const int err = Y_NET_LAST_ERROR();
                switch(err)
                {
                    case EINTR: continue;
                    default:    throw net::exception( err, "::select()");
                }
            }
            return ans;
#endif

#if defined(Y_WIN)
            const int ans = ::select(0,r, w, x, tv);
            if(ans==SOCKET_ERROR)
            {
                throw net::exception( Y_NET_LAST_ERROR(), "::select()");
            }
            return ans;
#endif
        }

        size_t socket_set:: incoming( socket_delay &d )
        {
            Y_NET_VERBOSE(std::cerr << "[network.socket_set.incoming(delay=" << d.wait_for() << ")]" << std::endl);
            if(size>0)
            {

                {
                    const uint8_t *u = (uint8_t *)ufd;
                    uint8_t       *r = (uint8_t *)rfd;
                    uint8_t       *x = (uint8_t *)xfd;
                    for(size_t i=0;i<sizeof(fd_set);++i)
                    {
                        r[i] =  x[i] = u[i];
                    }
                }
                return call_select(rfd,NULL,xfd,d.time_out());

            }
            else
            {
                return 0;
            }
        }

        size_t socket_set:: outgoing()
        {
            Y_NET_VERBOSE(std::cerr << "[network.socket_set.outgoing)]" << std::endl);
            if(size>0)
            {
                {
                    const uint8_t *u = (uint8_t *)ufd;
                    uint8_t       *w = (uint8_t *)wfd;
                    uint8_t       *x = (uint8_t *)xfd;
                    for(size_t i=0;i<sizeof(fd_set);++i)
                    {
                        w[i] = x[i] = u[i];
                    }
                }
                struct timeval no_wait = {0,0};
                return call_select(NULL,wfd,xfd,&no_wait);
            }
            else
            {
                return 0;
            }
        }


        static inline bool __test_fd_set(const socket_type fd,
                                         fd_set           *fds) throw()
        {
            assert(fds);
            if( FD_ISSET(fd,fds) )
            {
                FD_CLR(fd,fds);
                return true;
            }
            else
            {
                return false;
            }
        }

        bool socket_set::is_readable(const bsd_socket &s) throw()
        {
            return __test_fd_set(s.sock,rfd);
        }


        bool socket_set::is_writable(const bsd_socket &s) throw()
        {
            return __test_fd_set(s.sock,wfd);
        }

        bool socket_set::is_exception(const bsd_socket &s) throw()
        {
            return __test_fd_set(s.sock,xfd);
        }

    }
}
