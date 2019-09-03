//! \file
#ifndef Y_COUNTING_MLOOP_INCLUDED
#define Y_COUNTING_MLOOP_INCLUDED 1

#include "y/counting/counting.hpp"
#include "y/type/args.hpp"
#include "y/memory/embed.hpp"
#include <cstring>
#include <iostream>

namespace upsylon
{

    namespace core
    {
#define Y_MLOOP_CTOR(COUNT,DIM) \
counting(COUNT),               \
dimensions(DIM),             \
curr(0),                  \
head(0),                \
tail(0),              \
quit(0),            \
move(0),          \
iter(0),        \
wksp(0),      \
wlen(0),    \
data(0)

        template <typename T>
        class mloop : public counting
        {
        public:
            Y_DECL_ARGS(T,type);

            const size_t dimensions;

            inline explicit mloop(const size_t dim,
                                  const_type  *ini,
                                  const_type  *end) :
            Y_MLOOP_CTOR(0,chkdim(dim))
            {
                assert(ini);
                assert(end);
                setup(ini,end);
            }

            inline mloop( const mloop &other ) :
            Y_MLOOP_CTOR(other,other.dimensions)
            {
                setup_memory();
                assert(count==other.count);
                assert(index==other.index);
                assert(data==other.data);
                memcpy(wksp,other.wksp,data);
            }


            //! cleanup
            inline virtual ~mloop() throw()
            {
                static memory::allocator &mem = mem_location();
                mem.release(wksp,wlen);
                curr = 0;
                head = tail = quit = 0;
                move = 0; iter = 0;
            }


            inline const_type & operator[](const size_t dim) const throw()
            {
                assert(dim<dimensions);
                return curr[dim];
            }

            inline static void memchk(const mloop &lhs, const mloop &rhs)
            {
                assert(lhs.dimensions==rhs.dimensions);
                assert(lhs.count==rhs.count);
                assert(lhs.data==rhs.data);
                check_contents("mloop: ", lhs, lhs.wksp, rhs, rhs.wksp, lhs.data );
            }

            inline friend std::ostream & operator<<( std::ostream &os, const mloop &l )
            {
                os << '{' << int64_t(l.curr[0]);
                for(size_t i=1;i<l.dimensions;++i)
                {
                    os << ',' << int64_t(l.curr[i]);
                }
                os << '}';
                return os;
            }


        private:
            typedef void (*proc)(mutable_type &);
        protected:
            mutable_type *curr;
            const_type   *head; //!< head value
            const_type   *tail; //!< tail value
            const_type   *quit; //!< value to quit local loop
        private:
            const bool   *move;
            const proc   *iter;
            void         *wksp;
            size_t        wlen;
            size_t        data; //!< full data size
            Y_DISABLE_ASSIGN(mloop);


            virtual void start_() throw()
            {
                assert(1==index);
                for(size_t i=0;i<dimensions;++i)
                {
                    curr[i] = head[i];
                }
            }

            virtual void next_() throw()
            {
                assert(index<=count);
                recursive_update(0);
            }

            void recursive_update( const size_t odim ) throw()
            {
                assert(odim<dimensions);
                size_t idim = odim;
            FIND_DOF:
                if(move[idim])
                {
                    //--------------------------------------------------------------
                    // looping on movable coors
                    //--------------------------------------------------------------
                    mutable_type &value = curr[idim];
                    iter[idim](value);
                    if(quit[idim]==value)
                    {
                        value = head[idim];
                        recursive_update( next_dim(idim) );
                    }
                }
                else
                {
                    //--------------------------------------------------------------
                    // look for another D.O.F
                    //--------------------------------------------------------------
                    idim = next_dim(idim);
                    if(odim==idim) return; //!< no more d.o.f
                    goto FIND_DOF;
                }

            }

            inline size_t next_dim(size_t dim) const throw()
            {
                return (++dim>=dimensions) ? 0 : dim;
            }



            inline void setup_memory()
            {
                static memory::allocator &mem = mem_instance();
                {
                    memory::embed emb[] =
                    {
                        memory::embed::as<mutable_type>(curr,dimensions),
                        memory::embed::as<const_type>  (head,dimensions),
                        memory::embed::as<const_type>  (tail,dimensions),
                        memory::embed::as<const_type>  (quit,dimensions),
                        memory::embed::as<const bool>  (move,dimensions),
                        memory::embed::as<const proc>  (iter,dimensions)
                    };
                    wksp = memory::embed::create(emb, sizeof(emb)/sizeof(emb[0]), mem, wlen, &data);
                }
            }

            inline void setup(const_type *ini, const_type *end)
            {
                setup_memory();
                size_t &num = (size_t&)count;
                num = 1;
                for(size_t i=0;i<dimensions;++i)
                {
                    const_type         lo      = ini[i];
                    const_type         up      = end[i];
                    *(mutable_type *) &head[i] = lo;
                    *(mutable_type *) &tail[i] = up;
                    *(mutable_type *) &quit[i] = up;
                    *(bool         *) &move[i] = (lo!=up);
                    if(lo<=up)
                    {
                        *(proc         *) &iter[i] = incr;
                        num *= (up-lo)+1;
                    }
                    else
                    {
                        assert(lo>up);
                        *(proc         *) &iter[i] = decr;
                        num *= (lo-up)+1;
                    }
                    iter[i](*(mutable_type *) &quit[i]);
                    std::cerr << "dim#" << i;
                    std::cerr << " : " << int64_t(lo) << "->" << int64_t(up) << ", quit@" << int64_t(quit[i]);
                    std::cerr << " : move=" << move[i];
                    std::cerr << std::endl;
                }
                std::cerr << "count=" << count << " (data:" << data << "/" << wlen << ")" << std::endl;
            }
            
            static inline void incr(mutable_type&i) throw() { ++i; }
            static inline void decr(mutable_type&i) throw() { --i; }
        };


    }

    template <typename T, typename COORD>
    class mloop : public core::mloop<T>
    {
    public:
        Y_DECL_ARGS(T,type);
        typedef typename type_traits<COORD>::mutable_type coord;
        typedef const coord                               const_coord;

        inline explicit mloop( const_coord &ini, const_coord &end ) :
        core::mloop<type>( sizeof(COORD)/sizeof(type),
                          (const type *)&ini,
                          (const type *)&end ),
        value( *(const_coord *)(this->curr) )
        {
        }

        inline virtual ~mloop() throw()
        {
        }

        const_coord &value;







    private:
        Y_DISABLE_COPY_AND_ASSIGN(mloop);
    };



}

#endif

