//! \file

#ifndef Y_GFX_BROKER_INCLUDED
#define Y_GFX_BROKER_INCLUDED 1

#include "y/gfx/area/tiles.hpp"
#include "y/concurrent/loop/types.hpp"
#include "y/ptr/ref.hpp"

namespace upsylon
{
    namespace graphic
    {

        //______________________________________________________________________
        //
        //
        //! alias for a single instruction loop
        //
        //______________________________________________________________________
        typedef arc_ptr<concurrent::looper> engine;


        //______________________________________________________________________
        //
        //
        //! broker for instruction on multiple/single tile(s)
        //
        //______________________________________________________________________
        class broker :  public concurrent:: runnable
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            typedef void (*proc)(const tile &, void *, lockable &); //!< procedure for each tile

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit broker(const engine &, const area &); //!< setup
            virtual ~broker() throw();                     //!< cleanup

            //__________________________________________________________________
            //
            //! calling a procedure for each registerd tile
            //__________________________________________________________________
            void operator()(proc,void*) throw();

        private:
            proc                        func; //!< local function
            void                       *args; //!< local args

        public:
            ref_ptr<concurrent::looper> loop; //!< loop to apply for each
            const tiles                 tess; //!< tessellation


        private:
            void (broker::*call)(const concurrent::context &, lockable &) throw();

            Y_DISABLE_COPY_AND_ASSIGN(broker);
            virtual void run( const concurrent::context &, lockable &) throw();
            void         run_optimized(const concurrent::context &, lockable &) throw();
            void         run_downsized(const concurrent::context &, lockable &) throw();

            void setup() throw();
        };
        
    }

}

#endif

