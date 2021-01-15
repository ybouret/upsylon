
#include "y/mkl/fitting/built-in/circle.hpp"
 
namespace upsylon
{
    namespace mkl
    {
        namespace fitting
        {

            namespace built_in
            {

                const char * const circle:: name[3] =
                {
                    "center.x",
                    "center.y",
                    "radius"
                };

                circle:: ~circle() throw()
                {
                }

                circle:: circle(const size_t n) :
                data( sample_type::create("circle",n) ),
                dptr(NULL),
                iptr(NULL)
                {
                    **data << name[0] << name[1] << name[2];
                }

                double circle:: get2D(const vertex &v, const accessible<double> &aorg, const variables &vars)
                {
                    const double xc = vars(aorg,name[0]);
                    const double yc = vars(aorg,name[1]);
                    const double rr = vars(aorg,name[2]);
                    return sqrt( square_of(v.x-xc) + square_of(v.y-yc) ) - rr;
                }

                double circle:: onStart(const vertex v, const accessible<double> &aorg, const variables &vars)
                {
                    return get2D(v,aorg,vars);
                }

                double circle:: onReach(const vertex v, const accessible<double> &aorg, const variables &vars)
                {
                    return get2D(v,aorg,vars);
                }

                void  circle:: compute(addressable<double>        &dF,
                                       const vertex                v,
                                       const accessible<double>   &aorg,
                                       const variables            &vars,
                                       const accessible<bool>     &used)
                {
                    static const double tiny = numeric<double>::tiny;

                    const variable &xc = vars[name[0]];
                    const variable &yc = vars[name[1]];
                    const variable &rr = vars[name[2]];

                    const vertex center(xc(aorg),yc(aorg));
                    const vertex delta = center - v;
                    const double den   = sqrt( delta.norm2() ) + tiny;

                    if( xc(used) ) xc(dF) = delta.x/den;
                    if( yc(used) ) yc(dF) = delta.y/den;
                    if( rr(used) ) rr(dF) = -1;


                }

                const variables & circle:: operator*() const throw()
                {
                    return data->vars;
                }

                bool circle:: operator()(ls_type                  &ls,
                                         const accessible<double> &X,
                                         const accessible<double> &Y,
                                         addressable<double>      &aorg,
                                         const accessible<bool>   &used,
                                         addressable<double>      &aerr,
                                         const method              how)
                {
                    assert(X.size()==Y.size());

                    // initialize
                    data->free();
                    const size_t n       = X.size();
                    vertex       center;
                    double       radius  = 0;
                    bool         success = false;


                    // starting point: algebraic circle
                    switch(how)
                    {
                        case with_ints:
                            if(iptr.is_empty()) iptr = new iCircle();
                            iptr->free();
                            for(size_t i=n;i>0;--i) iptr->add( unit_t(X[i]), unit_t(Y[i]));
                            success = iptr->solve(center,radius);
                            break;

                        case with_reals: if(dptr.is_empty()) dptr = new dCircle();
                            for(size_t i=n;i>0;--i) dptr->add(X[i],Y[i]);
                            success = dptr->solve(center,radius);
                            break;
                    }
                    if(!success)
                    {
                        return false;
                    }

                    // prepare sample
                    for(size_t i=n;i>0;--i)
                    {
                        const vertex v(X[i],Y[i]);
                        data->add(v,0);
                    }

                    // prepare aorg
                    const variables &vars = **this;
                    const variable  &xc   = vars[name[0]];
                    const variable  &yc   = vars[name[1]];
                    const variable  &rr   = vars[name[2]];

                    if(xc(used)) xc(aorg) = center.x;
                    if(yc(used)) yc(aorg) = center.y;
                    if(rr(used)) rr(aorg) = radius;

                    __circle::sequential_type &F = *this;
                    __circle::v_gradient_type &G = *this;
                    return ls.fit(*data, F, G, aorg, used, aerr);

                }

                void circle:: free() throw()
                {
                    data->free();
                    if(iptr.is_valid()) iptr->free();
                    if(dptr.is_valid()) dptr->free();
                }

            }

        }

    }
}

