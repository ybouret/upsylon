//! \file

#ifndef Y_GRAPHIC_MEMORY_INCLUDED
#define Y_GRAPHIC_MEMORY_INCLUDED 1

#include "y/graphic/metrics.hpp"

namespace upsylon {

    namespace Graphic {


        //! shared pointer for the memory part of a Metrics
        class Memory
        {
        public:
            enum Kind
            {
                Global,
                Static
            };

            enum Mode
            {
                ReadWrite,
                ReadOnly
            };


            explicit Memory( Metrics & );
            virtual ~Memory() throw();
            Memory(const Memory &) throw();

            const Kind kind;
            const Mode mode;

            void displayMemory() const;

        private:
            Y_DISABLE_ASSIGN(Memory);
            
            size_t *nref;
            void   *entry;
            size_t  count;
            void    release() throw();




        };

    }

}


#endif


