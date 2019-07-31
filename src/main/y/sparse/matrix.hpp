//! \file
#ifndef Y_SPARSE_MATRIX_INCLUDED
#define Y_SPARSE_MATRIX_INCLUDED 1

#include "y/sparse/dok.hpp"
#include "y/container/const-field.hpp"
#include "y/comparison.hpp"

namespace upsylon
{

    namespace sparse
    {
        class matrix_key
        {
        public:
            const size_t r;
            const size_t c;
            matrix_key(const size_t R, const size_t C) throw();
            ~matrix_key() throw();
            matrix_key(const matrix_key &other) throw();
            friend bool operator==(const matrix_key &, const matrix_key &) throw();
            friend bool operator!=(const matrix_key &, const matrix_key &) throw();
            friend bool operator<(const matrix_key &, const matrix_key &) throw();
        private:
            Y_DISABLE_ASSIGN(matrix_key);
        };

        class matrix_info
        {
        public:
            virtual ~matrix_info() throw();
            explicit matrix_info(const size_t nr, const size_t nc);

            const size_t rows;
            const size_t cols;
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(matrix_info);
        };
    }

}

#endif

