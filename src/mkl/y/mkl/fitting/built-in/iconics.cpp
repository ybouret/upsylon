
#include "y/mkl/fitting/built-in/conics.hpp"
#include "y/mkl/kernel/diagonalize.hpp"
#include "y/mkl/kernel/eigen.hpp"

namespace upsylon
{
    namespace mkl
    {
        namespace fitting
        {
            namespace built_in
            {
                iConics:: iConics() : conics_type(),
                _W(nvar,nvar),
                _C(nvar,nvar)
                {
                }
                
                iConics:: ~iConics() throw()
                {
                }
                
                void iConics:: ellipse_replica()
                {
                    _C.ld(zero);
                    const apq two(2);
                    const apq minus_one(-1);
                    _C[1][3] = two;
                    _C[3][1] = two;
                    _C[2][2] = minus_one;
                }
                
                
                bool iConics:: build_shape()
                {
                    S.ld(zero);
                    assemble();
                    regularize();
                    std::cerr << "S=" << S << std::endl;
                    if( !LU::build(S) )
                    {
                        return false;
                    }
                    else
                    {
                        _W.assign(_C);
                        LU::solve(S,_W); // _W = inv(S)*C
                        for(size_t i=1;i<=nvar;++i)
                        {
                            for(size_t j=1;j<=nvar;++j)
                            {
                                W[i][j] = _W[i][j].to_double();
                            }
                        }
                        return true;
                    }
                }
                
                void iConics:: assemble()
                {
                    size_t n = x.size();
                    // Z = [x^2 xy y^2 x y 1]
                    S[6][6] = n;
                    for( list<apq>::iterator i=x.begin(),j=y.begin();n>0;--n,++i,++j)
                    {
                        const apq &X = *i;
                        const apq &Y = *j;
                        const apq  X2 = square_of(X);
                        const apq  Y2 = square_of(Y);
                        const apq  XY = X*Y;
                        S[1][1] += square_of(X2);
                        S[1][2] += X2*XY;
                        S[1][3] += X2*Y2;
                        S[1][4] += X2*X;
                        S[1][5] += X2*Y;
                        S[1][6] += X2;
                        
                        S[2][2] += square_of(XY);
                        S[2][3] += XY * Y2;
                        S[2][4] += XY * X;
                        S[2][5] += XY * Y;
                        S[2][6] += XY;
                        
                        S[3][3] += square_of(Y2);
                        S[3][4] += Y2 * X;
                        S[3][5] += Y2 * Y;
                        S[3][6] += Y2;
                        
                        S[4][4] += square_of(X);
                        S[4][5] += X*Y;
                        S[4][6] += X;
                        
                        S[5][5] += square_of(Y);
                        S[5][6] += Y;
                    }
                }
                
                
                void iConics:: add(const unit_t X, const unit_t Y)
                {
                    assert(x.size()==y.size());
                    
                    const apq qx = X;
                    const apq qy = Y;
                    x.push_back(qx);
                    try
                    {
                        y.push_back(qy);
                    }
                    catch(...)
                    {
                        x.pop_back();
                        throw;
                    }
                }
                
                
            }
            
        }
        
    }
    
}

