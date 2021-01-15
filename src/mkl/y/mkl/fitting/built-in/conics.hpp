
//! \file

#ifndef Y_MKL_FITTING_CONICS_INCLUDED
#define Y_MKL_FITTING_CONICS_INCLUDED 1

#include "y/sequence/list.hpp"
#include "y/sequence/vector.hpp"
#include "y/yap/rational.hpp"
#include "y/mkl/fitting/v-gradient.hpp"
#include "y/mkl/fitting/sequential.hpp"
#include "y/mkl/fitting/sample.hpp"
#include "y/mkl/fitting/least-squares.hpp"
#include "y/type/point2d.hpp"
#include "y/mkl/kernel/lu.hpp"

namespace upsylon
{
    namespace mkl
    {
        namespace fitting
        {

            namespace built_in
            {

                //______________________________________________________________
                //
                //
                //! common parts for conics fitting
                //
                //______________________________________________________________
                class conics
                {
                public:
                    //__________________________________________________________
                    //
                    // types and definitions
                    //__________________________________________________________
                    static const size_t     nvar = 6; //!< number of variables
                    typedef point2d<double> vertex;   //!< alias
                    //__________________________________________________________
                    //
                    // virtual interface
                    //__________________________________________________________
                    virtual     ~conics() throw();  //!< cleanup
                    virtual bool build_shape() = 0; //!< build and transfer W
                    
                    //__________________________________________________________
                    //
                    // non virtual interface
                    //__________________________________________________________
                    bool find_values(); //!< compute 6 values after build_shape()
                    void ellipse();     //!< ellipse_primary() and ellipse_replica()
                    
                    //! build_shape && find_values && extract parameters
                    bool ellipse(vertex         &center,
                                 vertex         &radius,
                                 matrix<double> &rotate);

                    //! evaluate conic form
                    double at(const double x, const double y) const throw();
                    
                protected:
                    explicit conics();

                    matrix<double> W;   //!< workspace   matrix
                    matrix<double> C;   //!< constraints matrix
                    vector<double> wr;  //!< eigv re
                    vector<double> wi;  //!< eigv im, holds final result
                    matrix<double> Wd;  //!< temporary
                    matrix<double> Q;   //!< quadratic form
                    matrix<double> R;   //!< rotation
                    vector<double> L;   //!< linear  form
                    vector<double> J;   //!< center
                    vector<double> lam; //!< 2D scalings
                    double         rhs; //!< rhs for reduced form

                private:
                    Y_DISABLE_COPY_AND_ASSIGN(conics);

                    //! using wi as temporary
                    double compute_UCU(const accessible<double> &u) throw();
                    
                    void         ellipse_primary() throw();
                    virtual void ellipse_replica() = 0;
                    
                };

                //______________________________________________________________
                //
                //
                //! common parts to compute shape matrix
                //
                //______________________________________________________________
                template <typename T>
                class _conics  : public conics
                {
                public:
                    //__________________________________________________________
                    //
                    // types and definitions
                    //__________________________________________________________
                    typedef _conics<T> conics_type; //!< alias

                    //__________________________________________________________
                    //
                    // interface
                    //__________________________________________________________
                    //! cleanup
                    inline virtual ~_conics() throw() {}
                    
                    
                    //__________________________________________________________
                    //
                    // members
                    //__________________________________________________________
                    const T        zero; //!< a zero value



                protected:
                    //! setup
                    inline explicit _conics() :
                    zero(0), x(), y(), S(nvar,nvar)
                    {
                    }

                    list<T>        x;    //!< store coordinate
                    list<T>        y;    //!< store coordinate
                    matrix<T>      S;    //!< shape matrix
                    

                    //! make symmetric matrix
                    inline void regularize()
                    {
                        S[2][1] = S[1][2];

                        S[3][1] = S[1][3];
                        S[3][2] = S[2][3];

                        S[4][1] = S[1][4];
                        S[4][2] = S[2][4];
                        S[4][3] = S[3][4];

                        S[5][1] = S[1][5];
                        S[5][2] = S[2][5];
                        S[5][3] = S[3][5];
                        S[5][4] = S[4][5];


                        S[6][1] = S[1][6];
                        S[6][2] = S[2][6];
                        S[6][3] = S[3][6];
                        S[6][4] = S[4][6];
                        S[6][5] = S[5][6];
                    }

                private:
                    Y_DISABLE_COPY_AND_ASSIGN(_conics);
                    virtual void assemble() = 0; //!< build S from x and y
                };

                //______________________________________________________________
                //
                //
                //! compute shape with rational, for integer coordinates
                //
                //______________________________________________________________
                class iConics : public _conics<apq>
                {
                public:
                    explicit iConics();          //!< setup
                    virtual ~iConics() throw();  //!< cleanup

                    //! add integer coordinates
                    void add(const unit_t X, const unit_t Y);
                    
                    //! with integers
                    virtual bool build_shape();
                    
                private:
                    Y_DISABLE_COPY_AND_ASSIGN(iConics);
                    virtual void assemble();
                    virtual void ellipse_replica();

                    matrix<apq> _W;
                    matrix<apq> _C;
                };

                //______________________________________________________________
                //
                //
                //! compute shape with reals
                //
                //______________________________________________________________
                class dConics : public _conics<double>
                {
                public:
                    explicit dConics();          //!< setup
                    virtual ~dConics() throw();  //!< cleanup
                    
                    //! add floating point coordinates
                    void add(const double X, const double Y);
                    
                    //! using floating point
                    virtual bool build_shape();
                    
                private:
                    Y_DISABLE_COPY_AND_ASSIGN(dConics);
                    virtual void assemble();
                    virtual void ellipse_replica(); //!< do nothing

                };
            }

        }

    }

}

#endif
