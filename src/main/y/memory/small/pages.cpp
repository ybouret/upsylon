
#include "y/memory/small/pages.hpp"
#include "y/type/utils.hpp"
#include "y/type/aliasing.hpp"
#include "y/sort/merge.hpp"
#include "y/memory/allocator/global.hpp"
#include "y/exceptions.hpp"

#include <cstring>
#include <cerrno>

#include <iostream>

namespace upsylon {

    namespace memory {

        namespace small {

            pages::deposit:: deposit(const size_t usr_iln2) throw() :
            deposit_type(),
            page_iln2(usr_iln2),
            page_size(one<<page_iln2),
            bytes(0)
            {
            }


            pages:: pages(const size_t large_page_size) :
            max_page_size(next_power_of_two(max_of(large_page_size,min_page_size))),
            max_page_iln2(integer_log2(max_page_size)),
            deposit_count(1+max_page_iln2-min_page_iln2),
            size(0),
            depo(0),
            wksp()
            {
                memset(wksp,0,sizeof(wksp));

                size_t *sz = aliasing::forward<size_t> (wksp,0);
                depo       = aliasing::forward<deposit>(wksp,size_bytes);
                size       = sz;

                for(size_t i=0;i<full_deposits;++i)
                {
                    sz[i] = one<<i;
                }

                for(size_t i=min_page_iln2;i<=max_page_iln2;++i)
                {
                    new (depo+i) deposit();
                }
            }

            pages:: ~pages() throw()
            {
                for(size_t i=min_page_iln2;i<=max_page_iln2;++i)
                {
                    deposit     &d = depo[i];
                    const size_t s = size[i];
                    merging<page>::sort_by_addr(d);
                    while(d.size)
                    {
                        static global & mgr = global::location();
                        mgr.__free(d.pop_back(),s);
                    }
                }

                aliasing::_(max_page_iln2) = 0;
                aliasing::_(max_page_size) = 0;

            }

            void * pages:: acquire(const size_t iln2)
            {
                static global &mgr = global::instance();
                if(iln2<min_page_iln2)
                {
                    return mgr.__calloc(1,size[iln2]);
                }
                else if(max_page_iln2<iln2)
                {
                    if(iln2>=full_deposits)
                    {
                        throw libc::exception(ERANGE,"memory::small::pages");
                    }
                    return mgr.__calloc(1,size[iln2]);
                }
                else
                {
                    deposit &d = depo[iln2];
                    if(d.size)
                    {
                        return d.pop_front();
                    }
                    else
                    {
                        return mgr.__calloc(1,size[iln2]);
                    }
                }
            }

            void   pages:: release(void *addr, const size_t iln2) throw()
            {
                assert(addr);
                static global &mgr = global::location();
                assert(iln2<full_deposits);

                if(iln2>=min_page_iln2&&iln2<=max_page_iln2)
                {
                    deposit     &d = depo[iln2];
                    //const size_t s = size[iln2];

                    // prepare address as page
                    memset(addr,0,sizeof(page));
                    page *p = aliasing::as<page>(addr);

                    // save page and move it into position
                    d.push_front(p);
                    while(p->next&&p>p->next)
                    {
                        d.towards_tail(p);
                    }
                }
                else
                {
                    mgr.__free(addr,size[iln2]);
                }


            }



        }

    }

}
