
//! file

#ifndef Y_TENSOR_INCLUDED
#define Y_TENSOR_INCLUDED 1

#include "y/memory/embed.hpp"

namespace upsylon {

    namespace core {

        //! base class for tensors
        class tensor
        {
        public:
            static const size_t max_dimensions = 4; //!< up to tensor4d

            virtual        ~tensor()           throw(); //!< cleanup
            const   size_t *lower()      const throw(); //!< lower indices
            const   size_t *upper()      const throw(); //!< upper indices
            size_t          allocated()  const throw(); //!< private bytes

            const size_t    dimensions; //!< current dimension


        protected:
            explicit tensor() throw();                      //!< setup
            void     create(memory::embed [],const size_t); //!< create and plug global private memory
            void     record(const size_t num) throw();      //!< create a new dimension
            size_t   built;                                 //!< shared construction level

        private:
            Y_DISABLE_COPY_AND_ASSIGN(tensor);
            size_t              bytes;
            void               *where;
            size_t              up[max_dimensions];
            static const size_t lo[max_dimensions];
            void set_upper(const size_t dim, const size_t num) throw();

        public:
            const size_t &cols;     //!< up[0]
            const size_t &rows;     //!< up[1]
            const size_t &slices;   //!< up[2]
            const size_t &frames;   //!< up[4]
        };

    }

}

#endif
