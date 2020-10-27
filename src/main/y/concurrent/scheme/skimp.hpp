//! \file

#ifndef Y_CONCURRENT_SKIMP_INCLUDED
#define Y_CONCURRENT_SKIMP_INCLUDED 1

#include "y/concurrent/scheme/for-each.hpp"

namespace upsylon {

    namespace concurrent
    {

        //! Simple Kernel Invoked in MultiProcessing
        template <typename INDEX>
        class skimp
        {
        public:
            template <
            typename FUNCTION,
            typename TARGET,
            typename SOURCE> static inline
            void call(FUNCTION    &function,
                      TARGET      &target,
                      SOURCE      &source,
                      const INDEX  length,
                      const INDEX  offset,
                      for_each    &loop)
            {

                proxy<FUNCTION,TARGET,SOURCE> task(function,target,source,length,offset);
                loop.run(task.call, &task);
            }


        private:
            template <
            typename FUNCTION,
            typename TARGET,
            typename SOURCE>
            class proxy
            {
            public:
                FUNCTION       &function;
                TARGET         &target;
                SOURCE         &source;
                const INDEX    length;
                const INDEX    offset;
                inline proxy(const FUNCTION &f,
                             TARGET         &t,
                             SOURCE         &s,
                             const INDEX    l,
                             const INDEX    o) throw() :
                function(f), target(t), source(s), length(l), offset(o)
                {
                }

                inline ~proxy() throw()
                {
                }


                static void call(void *args, parallel &ctx, lockable &sync)
                {
                    proxy &self   = *static_cast<proxy *>(args);
                    INDEX length = self.length;
                    INDEX offset = self.offset;
                    ctx.split(length,offset);
                    while(length-- > 0)
                    {
                        self.function(self.target[offset],self.source[offset],ctx,sync);
                        ++offset;
                    }
                }
            };
            
        };

    }

}

#endif
