//! \file

#ifndef Y_SPADE_GHOST_INCLUDED
#define Y_SPADE_GHOST_INCLUDED 1

#include "y/spade/types.hpp"

namespace upsylon {

    namespace Spade
    {

        namespace Kernel
        {
            //------------------------------------------------------------------
            //
            //! a Ghost is an array of indices
            //
            //------------------------------------------------------------------
            class Ghost : public accessible<size_t>
            {
            public:
                //--------------------------------------------------------------
                //
                // C++
                //
                //--------------------------------------------------------------
                virtual ~Ghost() throw();     //!< cleanup
                explicit Ghost(const size_t); //!< setup

                //--------------------------------------------------------------
                //
                // methods
                //
                //--------------------------------------------------------------
                virtual size_t         size()                   const throw(); //!< for accessible
                virtual const size_t & operator[](const size_t) const throw(); //!< for accessible

            private:
                size_t        count_;
                size_t        bytes_;
            protected:
                size_t       *indices; //!< [1..items]
            public:
                const size_t  items;   //!< sub layout items
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Ghost);
            };
        };

        //------------------------------------------------------------------
        //
        //! a Ghost zone is an array of indices
        //
        //------------------------------------------------------------------
        class Ghost :  public Kernel::Ghost
        {
        public:
            //------------------------------------------------------------------
            //
            // C++
            //
            //------------------------------------------------------------------

            //! cleanup
            virtual ~Ghost() throw();

            //! setup from a zone in a full layout
            template <typename LAYOUT> inline
            Ghost(const LAYOUT &zone, const LAYOUT &full) :
            Kernel::Ghost(zone.items)
            {
                assert( full.contains(zone) );
                typename LAYOUT::Loop loop(zone.lower,zone.upper);
                for(loop.boot();loop.good();loop.next())
                {
                    indices[loop.index] = full.indexOf(loop.value);
                }
            }
            

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Ghost);
        };



    }
}

#endif

