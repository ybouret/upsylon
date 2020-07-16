//! \file
#ifndef Y_COUNTING_MLOOP_INCLUDED
#define Y_COUNTING_MLOOP_INCLUDED 1

#include "y/counting/counting.hpp"
#include "y/sequence/accessible.hpp"
#include "y/type/args.hpp"
#include "y/memory/embed.hpp"
#include "y/type/ints-display.hpp"
#include "y/type/aliasing.hpp"
#include <iostream>
#include <cstring>

namespace upsylon
{

    namespace core
    {
        //----------------------------------------------------------------------
        //
        //! auxiliary stuff for mloop
        //
        //----------------------------------------------------------------------
        class mloop_ : public upsylon::counting
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            static const char identifier[];  //!< "mloop: "
            static const char separators[2]; //!< ',', '\0'

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const size_t dimensions; //!< dimensions to loop over
            const bool   secured;    //!< no reset  but for first and last index

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~mloop_() throw();                            //!< cleanup
        protected:                                                //
            explicit mloop_(const size_t,const bool)     throw(); //!< set checked dimensions
            explicit mloop_(const mloop_ &)              throw(); //!< copy
            void     overrule()                          const;   //!< check current can be changed


        private:
            Y_DISABLE_ASSIGN(mloop_);
        };

        //! constructor setup
#define Y_MLOOP_CTOR( )         \
accessible<T>(),                \
curr(0),                        \
item(0),                        \
head(0),                        \
tail(0),                        \
quit(0),                        \
move(0),                        \
iter(0),                        \
wksp(0),                        \
wlen(0),                        \
data(0)

        //______________________________________________________________________
        //
        //
        //! context for a multidimensional loop
        //
        //______________________________________________________________________
        template <typename T>
        class mloop : public mloop_,  public accessible<T>
        {
        public:
            //------------------------------------------------------------------
            // types and definitions
            //------------------------------------------------------------------
            Y_DECL_ARGS(T,type);      //!< alias

            //------------------------------------------------------------------
            // C++
            //------------------------------------------------------------------
            //! setup with dimensions and boundaries
            inline explicit mloop(const size_t dim,
                                  const_type  *ini,
                                  const_type  *end,
                                  const bool   safe=true) :
            mloop_(chkdim(dim),safe),
            Y_MLOOP_CTOR()
            {
                assert(ini); assert(end);
                setup_memory();
                setup(ini,end);
                boot();
            }

            //! hard copy
            inline mloop( const mloop &other ) : collection(),
            mloop_(other),
            Y_MLOOP_CTOR()
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
                static memory::allocator &mem = upsylon::counting::mem_location();
                memset(wksp,0,wlen);
                mem.release(wksp,wlen);
                curr = 0;
                item = head = tail = quit = 0;
                move = 0; iter = 0;
            }

            //------------------------------------------------------------------
            // accessible interface
            //------------------------------------------------------------------
            //! access [1..dimensions]
            inline virtual const_type & operator[](const size_t dim) const throw()
            {
                assert(dim>0);
                assert(dim<=dimensions);
                return item[dim];
            }

            //! size=dimensions
            inline virtual size_t size() const throw() { return dimensions; }

            //------------------------------------------------------------------
            // helpers
            //------------------------------------------------------------------
            //! memory check
            inline static void memchk(const mloop &lhs, const mloop &rhs)
            {
                assert(lhs.dimensions==rhs.dimensions);
                assert(lhs.count==rhs.count);
                assert(lhs.data==rhs.data);
                check_contents(identifier, lhs, lhs.wksp, rhs, rhs.wksp, lhs.data );
            }

            //! show
            virtual std::ostream & show( std::ostream &os ) const
            {
                return display_int::to(os<< '{',curr,dimensions,separators) << '}';
            }

            //! workspace memory
            inline size_t workspace() const throw() { return wlen; }

            //! used data
            inline size_t allocated() const throw() { return data; }

        private:
            typedef void (*proc)(mutable_type &);
        protected:
            mutable_type *curr; //!< current indices
            const_type   *item; //!< for accessible: curr-1
            const_type   *head; //!< head value: starting
            const_type   *tail; //!< tail value: finishing
            const_type   *quit; //!< value to quit local loop
        private:
            const bool   *move; //!< true if more than one step
            const proc   *iter; //!< incr/decr
            void         *wksp; //!< internal data
            size_t        wlen; //!< allocated memory
            size_t        data; //!< effective data size
            Y_DISABLE_ASSIGN(mloop);


            //__________________________________________________________________
            //
            //! prepare loop
            //__________________________________________________________________
            virtual void onBoot() throw()
            {
                assert(1==index);
                for(size_t i=0;i<dimensions;++i)
                {
                    curr[i] = head[i];
                }
            }

            //__________________________________________________________________
            //
            //! next indices
            //__________________________________________________________________
            virtual void onNext() throw()
            {
                assert(index<=count);
                recursive_update(0);
            }

            //__________________________________________________________________
            //
            //! find an index to move
            //__________________________________________________________________
            void recursive_update( const size_t odim ) throw()
            {
                assert(odim<dimensions);
                size_t idim = odim;
            FIND_DOF:
                if(move[idim])
                {
                    //----------------------------------------------------------
                    // looping on movable coors
                    //----------------------------------------------------------
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
                    //----------------------------------------------------------
                    // look for another D.O.F
                    //----------------------------------------------------------
                    idim = next_dim(idim);
                    if(odim==idim) return; //!< no more d.o.f
                    goto FIND_DOF;
                }

            }

            //__________________________________________________________________
            //
            //! next dimension to probe
            //__________________________________________________________________
            inline size_t next_dim(size_t dim) const throw()
            {
                return (++dim>=dimensions) ? 0 : dim;
            }


            //__________________________________________________________________
            //
            //! allocated all embedded memory
            //__________________________________________________________________
            inline void setup_memory()
            {
                static memory::allocator &mem = this->mem_instance();
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
                    item = curr-1;
                }
            }

            static inline void incr(mutable_type&i) throw() { ++i; }
            static inline void decr(mutable_type&i) throw() { --i; }

        protected:
            //! prepare loop parameters
            inline void setup(const_type *ini, const_type *end) throw()
            {
                size_t &num = aliasing::_(this->count);
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
                }
            }
            

        };


    }

    //--------------------------------------------------------------------------
    //
    //! embedding mloop with a vector type
    //
    //--------------------------------------------------------------------------
    template <typename T, typename COORD>
    class mloop : public core::mloop<T>
    {
    public:
        //----------------------------------------------------------------------
        //
        // types and definitions
        //
        //----------------------------------------------------------------------
        Y_DECL_ARGS(T,type);                                            //!< alias
        typedef typename type_traits<COORD>::mutable_type coord;        //!< alias
        typedef const coord                               const_coord;  //!< alias

        //----------------------------------------------------------------------
        //
        // C++
        //
        //----------------------------------------------------------------------

        //! setup
        inline explicit mloop( const_coord &ini, const_coord &end ) :
        core::mloop<type>( sizeof(COORD)/sizeof(type),
                          (const type *)&ini,
                          (const type *)&end ),
        value( *(const_coord *)(this->curr) ),
        lower( *(const_coord *)(this->head) ),
        upper( *(const_coord *)(this->tail) )
        {
        }

        //! cleanup
        inline virtual ~mloop() throw() {}

        //----------------------------------------------------------------------
        //
        // methods
        //
        //----------------------------------------------------------------------
        //! reset/start a loop with different coordinates
        inline void reset(const_coord &ini, const_coord &end)
        {
            this->overrule();
            this->setup( (const type *)&ini, (const type *)&end );
            this->boot();
        }

        //! access value
        inline const_coord & operator*() const throw()
        {
            return value;
        }

        //----------------------------------------------------------------------
        //
        // members
        //
        //----------------------------------------------------------------------
        const_coord &value; //!< apparent value
        const_coord &lower; //!< apparent lower bound
        const_coord &upper; //!< apparent upper bound

    private:
        Y_DISABLE_COPY_AND_ASSIGN(mloop);
    };



}

#endif

