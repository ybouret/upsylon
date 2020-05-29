
//! file

#ifndef Y_TENSOR_INCLUDED
#define Y_TENSOR_INCLUDED 1

#include "y/memory/embed.hpp"

namespace upsylon {

    namespace core {

        class tensor
        {
        public:
            virtual ~tensor() throw();

            const   size_t *lower()      const throw();
            const   size_t *upper()      const throw();

            size_t allocated() const throw(); //!< private bytes

            const size_t dimensions;


        protected:
            explicit tensor() throw(); //!< setup
            void     create(memory::embed [],const size_t); //!< create and plug global private memory
            void     record(const size_t num) throw();      //!< create a new dimension

            size_t   built;                    //!< shared construction level


        private:
            Y_DISABLE_COPY_AND_ASSIGN(tensor);
            size_t              bytes;
            void               *where;
            size_t              up[4];
            static const size_t lo[4];
            void set_upper(const size_t dim, const size_t num) throw();
        public:
            const size_t &cols;
            const size_t &rows;
            const size_t &slices;
            const size_t &frames;
        };

    }

}

#endif
