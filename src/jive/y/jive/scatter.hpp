#ifndef Y_JIVE_SCATTER_INCLUDED
#define Y_JIVE_SCATTER_INCLUDED 1

#include "y/jive/pattern.hpp"

namespace upsylon {

    namespace Jive
    {

        class Scatter
        {
        public:

            struct Node
            {
                const void *addr;
                Node       *next;
            };

            typedef Node *Slot;

            explicit Scatter();
            virtual ~Scatter() throw();

            void record(const void *, const uint8_t);
            void record(const void *, const Leading &);
            
            void display(std::ostream&, void (*proc)(std::ostream&,const void*) ) const;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Scatter);
            size_t  bytes;
            Slot   *slots;
        };



        

    }

}

#endif

