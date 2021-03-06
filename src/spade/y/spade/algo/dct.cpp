#include "y/spade/algo/dct.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace Spade
    {

        DCT:: ~DCT() throw()
        {
        }

        static inline unit_t __check(const unit_t X, const char *id)
        {
            assert(id);
            if(X<=0) throw exception("invalid DCT  %s=%d", id, int(X));
            return X;
        }

        DCT:: DCT(const unit_t W, const unit_t H) :
        w(__check(W,"W") ),
        h(__check(H,"H") ),
        LAMBDA("LAMBDA",W,H)
        {
        }

        CommonDCT:: CommonDCT(const unit_t W,const unit_t H) :
        DCT(W,H),
        XCOS( "XCOS", W,W),
        YCOS( "YCOS", H,H)
        {
            const size_t N  = size_t(W);
            const size_t NN = N<<1;
            for(size_t i=0;i<N;++i)
            {
                for(size_t j=0;j<N;++j)
                {
                    const double arg = (((2*i+1)*j)*3.141592653589793115997963468544185161590576171875)/NN;
                    XCOS[i][j]        = cos(arg);
                }
            }

            const size_t M  = size_t(H);
            const size_t MM = M<<1;
            for(size_t i=0;i<M;++i)
            {
                for(size_t j=0;j<M;++j)
                {
                    const double arg = (((2*i+1)*j)*3.141592653589793115997963468544185161590576171875)/MM;
                    YCOS[i][j]        = cos(arg);
                }
            }




            const double den          = sqrt( double(N) * double(M) );
            const double two_over_den = 2.0/den;
            const double sq2_over_den = sqrt(2.0)/den;
            LAMBDA[0][0] = 1.0/den;
            for(size_t j=1;j<M;++j)
            {
                for(size_t i=1;i<N;++i)
                {
                    LAMBDA[j][i] = two_over_den;
                }
            }

            for(size_t i=1;i<N;++i)
            {
                LAMBDA[0][i] = sq2_over_den;
            }

            for(size_t j=1;j<M;++j)
            {
                LAMBDA[j][0] = sq2_over_den;
            }
        }

        CommonDCT:: ~CommonDCT() throw()
        {
        }


        void CommonDCT:: reverse(Table &pix, const Table &dct) throw()
        {
            const size_t N = w;
            const size_t M = h;
            for(size_t y=0;y<M;++y)
            {
                for(size_t x=0;x<N;++x)
                {

                    double q = 0;
                    for(size_t j=0;j<M;++j)
                    {
                        const double Cyj    = YCOS[y][j];
                        for(size_t i=0;i<N;++i)
                        {
                            const double Cxi    = XCOS[x][i];
                            const double weight = LAMBDA[j][i] * Cxi * Cyj;
                            q += dct[j][i] * weight;
                        }
                    }
                    pix[y][x] = q;
                }
            }

        }

        SquareDCT:: SquareDCT(const unit_t W) :
        DCT(W,W),
        COS( "COS", W,W)
        {
            const size_t N  = size_t(W);
            const size_t NN = N<<1;
            for(size_t i=0;i<N;++i)
            {
                for(size_t j=0;j<N;++j)
                {
                    const double arg = (((2*i+1)*j)*3.141592653589793115997963468544185161590576171875)/NN;
                    COS[i][j]        = cos(arg);
                }
            }

            const double two_over_N = 2.0 / N;
            const double sq2_over_N = sqrt(2.0)/N;
            for(size_t i=1;i<N;++i)
            {
                LAMBDA[0][i] = sq2_over_N;
                LAMBDA[i][0] = sq2_over_N;
                for(size_t j=1;j<N;++j)
                {
                    LAMBDA[i][j] = two_over_N;
                }
            }

            LAMBDA[0][0] = 1.0/N;
        }

        SquareDCT:: ~SquareDCT() throw()
        {
        }

        void SquareDCT:: reverse(Table &pix, const Table &dct) throw()
        {
            const size_t N = w;
            for(size_t y=0;y<N;++y)
            {
                for(size_t x=0;x<N;++x)
                {
                    double q = 0;
                    for(size_t j=0;j<N;++j)
                    {
                        const double Cyj    = COS[y][j];
                        for(size_t i=0;i<N;++i)
                        {
                            const double Cxi    = COS[x][i];
                            const double weight = LAMBDA[j][i] * Cxi * Cyj;
                            q += dct[j][i] * weight;
                        }
                    }
                    pix[y][x] = q;
                }
            }

        }

    }
}


