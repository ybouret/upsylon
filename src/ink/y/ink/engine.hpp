#ifndef Y_INK_ENGINE_INCLUDED
#define Y_INK_ENGINE_INCLUDED 1

#include "y/concurrent/scheme/for-each.hpp"
#include "y/ink/area.hpp"
#include "y/ptr/arc.hpp"

namespace upsylon
{
    namespace ink
    {
        typedef arc_ptr<concurrent::for_each> workers;

        //! parallel engine for a given area
        class engine : public area
        {
        public:
            explicit engine(const workers &shared,
                            const area    &full);
            virtual ~engine() throw();
        private:
            workers    agents;
        public:
            const areas tiles;
        };

    }
}


#endif
