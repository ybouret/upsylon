//! \file

#ifndef Y_SPADE_LAYOUT_METRICS_INCLUDED
#define Y_SPADE_LAYOUT_METRICS_INCLUDED 1

#include "y/spade/types.hpp"

namespace upsylon {
 
    namespace Spade
    {
        
        //----------------------------------------------------------------------
        //
        //! basic layout metrics
        //
        //----------------------------------------------------------------------
        class LayoutMetrics
        {
            //------------------------------------------------------------------
            //
            // C++
            //
            //------------------------------------------------------------------
        public:
            //! cleanup
            virtual ~LayoutMetrics() throw();
        protected:
            //! setup dimensions, items are setup later
            explicit LayoutMetrics(const unsigned d) throw();
           
            //! copy
            LayoutMetrics(const LayoutMetrics&) throw();

            //! true is all w[0..dimensions-1]>1
            bool _isThick(const Coord1D *w) const throw();

        public:
            
            //------------------------------------------------------------------
            //
            // methods
            //
            //------------------------------------------------------------------
            
            //! split in 1D, check that length is valid
            static void Split1D(Coord1D       &length,
                                Coord1D       &offset,
                                const Coord1D  size,
                                const Coord1D  rank,
                                const unsigned dim);
            
            
            //------------------------------------------------------------------
            //
            // members
            //
            //------------------------------------------------------------------
            const unsigned dimensions;        //!< dimensions
            const size_t   items;             //!< linear items for this layout
            
        private:
            Y_DISABLE_ASSIGN(LayoutMetrics);
        };
        
    }
    
}

#endif

