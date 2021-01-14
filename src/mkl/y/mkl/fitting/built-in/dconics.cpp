
#include "y/mkl/fitting/built-in/conics.hpp"
#include "y/sequence/arrays.hpp"
#include "y/sort/sorted-sum.hpp"

namespace upsylon
{
    namespace mkl
    {
        namespace fitting
        {
            
            namespace built_in
            {
                dConics:: dConics() : conics_type()
                {
                }
                
                dConics:: ~dConics() throw()
                {
                }
                
                void dConics:: ellipse_replica()
                {
                    
                }
                
                void dConics:: assemble()
                {
                    size_t n = x.size();
                    // Z = [x^2 xy y^2 x y 1]
                    if(n>0)
                    {
                        arrays<double> arr(6,n);
                        array<double>
                        &X   = arr.next(),
                        &Y   = arr.next(),
                        &X2  = arr.next(),
                        &Y2  = arr.next(),
                        &XY  = arr.next(),
                        &tmp = arr.next();
                        
                        S[6][6] = double(n);
                        
                        {
                            list<double>::iterator it = x.begin();
                            list<double>::iterator jt = y.begin();
                            for(size_t i=n;i>0;--i,++it,++jt)
                            {
                                const double xx = *it;
                                const double yy = *jt;
                                X[i]  = xx;
                                Y[i]  = yy;
                                X2[i] = xx * xx;
                                Y2[i] = yy * yy;
                                XY[i] = xx * yy;
                            }
                        }
                        
#define Y_CONIC(tgt,expr) do { for(size_t i=n;i>0;--i) tmp[i] = (expr(i)); tgt = sorted_sum_by_abs(tmp); } while(false)

#define  _X2X2(i) square_of(X2[i])
#define  _X2XY(i) X2[i] * XY[i]
#define  _X2Y2(i) X2[i] * Y2[i]
#define  _X2X(i)  X2[i] * X[i]
#define  _X2Y(i)  X2[i] * Y[i]
#define  _X2(i)   X2[i]
                        
                        Y_CONIC(S[1][1],_X2X2);
                        Y_CONIC(S[1][2],_X2XY);
                        Y_CONIC(S[1][3],_X2Y2);
                        Y_CONIC(S[1][4],_X2X);
                        Y_CONIC(S[1][5],_X2Y);
                        Y_CONIC(S[1][6],_X2);

#define _XYXY(i) square_of(XY[i])
#define _XYY2(i) XY[i] * Y2[i]
#define _XYX(i)  XY[i] * X[i]
#define _XYY(i)  XY[i] * Y[i]
#define _XY(i)   XY[i]
                        Y_CONIC(S[2][2],_XYXY);
                        Y_CONIC(S[2][3],_XYY2);
                        Y_CONIC(S[2][4],_XYX);
                        Y_CONIC(S[2][5],_XYY);
                        Y_CONIC(S[2][6],_XY);
                        
#define _Y2Y2(i) square_of(Y2[i])
#define _Y2X(i)  Y2[i] * X[i]
#define _Y2Y(i)  Y2[i] * Y[i]
#define _Y2(i)   Y2[i]

                        Y_CONIC(S[3][3],_Y2Y2);
                        Y_CONIC(S[3][4],_Y2X);
                        Y_CONIC(S[3][5],_Y2Y);
                        Y_CONIC(S[3][6],_Y2);

 #define _X(i)  X[i]
                        Y_CONIC(S[4][4],_X2);
                        Y_CONIC(S[4][5],_XY);
                        Y_CONIC(S[4][6],_X);
                    
#define _Y(i)  Y[i]
                        Y_CONIC(S[5][5],_Y2);
                        Y_CONIC(S[5][6],_Y);
                        
                    }
                }
                
               
                //! build shape
                bool dConics:: build_shape()
                {
                    assert(x.size()==y.size());
                    S.ld(zero);
                    assemble();
                    regularize();
                    std::cerr << "Sd=" << S << std::endl;
                    if( !LU::build(S) )
                    {
                        return false;
                    }
                    else
                    {
                        W.assign(C);
                        LU::solve(S,W); // W = inv(S)*C
                        return true;
                    }
                }
                
                void dConics:: add(const double X, const double Y)
                {
                    x.push_back(X);
                    try
                    {
                        y.push_back(Y);
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
