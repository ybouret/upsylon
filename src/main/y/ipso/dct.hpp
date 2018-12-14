//! \file
#ifndef Y_IPSO_DCT_INCLUDED
#define Y_IPSO_DCT_INCLUDED 1

#include "y/ipso/field2d.hpp"

namespace upsylon
{
    namespace ipso
    {
        class DCT 
        {
        public:
            typedef field2D<double> Table;

            virtual ~DCT() throw();

        protected:
            explicit DCT( const unit_t W, const unit_t H);
            Table LAMBDA;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(DCT);
        };

        class CommonDCT : public DCT
        {
        public:
            virtual ~CommonDCT() throw();
            explicit CommonDCT(const unit_t W,const unit_t H);

        private:
            Y_DISABLE_COPY_AND_ASSIGN(CommonDCT);
            Table XCOS;
            Table YCOS;
        };

        class SquareDCT : public DCT
        {
        public:
            virtual ~SquareDCT() throw();
            explicit SquareDCT(const unit_t W);
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(SquareDCT);
            Table COS;
        };


    }
}

#endif

