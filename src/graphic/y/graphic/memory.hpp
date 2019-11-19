//! \file

#ifndef Y_GRAPHIC_MEMORY_INCLUDED
#define Y_GRAPHIC_MEMORY_INCLUDED 1

#include "y/graphic/types.hpp"

namespace upsylon {

    namespace Graphic {

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
                RW,
                RO
            };

            virtual ~Memory() throw();
            explicit Memory(const size_t required, const Mode   = RW  );
            explicit Memory(void        *buffer,   const size_t length);
            explicit Memory(const void  *buffer,   const size_t length);

            Memory(const Memory &other ) throw();
            Memory(const Memory &other, const size_t shift );



            const Kind   kind;
            const Mode   mode;

            void displayMemoryInfo() const;

        private:
            size_t      *nref;
            size_t       size;
            void        *data;
            Y_DISABLE_ASSIGN(Memory);
            void checkStaticData();

        protected:
            void        *entry;
            const size_t count;
        };

    }

}


#endif


