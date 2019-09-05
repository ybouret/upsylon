#include "y/oxide/topology.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace Oxide
    {

        Topology::Level Topology:: LevelFor( const Coord1D *addr, const size_t size ) throw()
        {
            assert(addr);
            assert(size<=3);
            int count = 0;
            for(size_t i=0;i<size;++i)
            {
                if( addr[i] ) ++count;
            }
            switch(count)
            {
                case 1: return Level1;
                case 2: return Level2;
                case 3: return Level3;
                default:
                    break;
            }
            return Level0;
        }


        static const char fn[] = "Oxide::Topology::";

        void Topology:: CheckSameLevels( const Level lhs, const Level rhs )
        {
            if(0==(lhs&Levels)||0==(rhs&Levels) || (lhs!=rhs) )
            {
                throw exception("%sCheckSameLevels(%u,%u) failure",fn,lhs,rhs);
            }
        }

        void Topology:: CheckGhosts( const Coord1D ng, const Coord1D width, const unsigned dim)
        {
            assert(ng>=0);
            if(ng>width) throw exception("%sCheckGhosts(%ld>%ld in dimension#%u)",fn,long(ng),long(width),dim);

            
        }

        void Topology::  BuildHubStatus(const Coord1D *size,
                                        const Coord1D *rank,
                                        bool          *head,
                                        bool          *tail,
                                        bool          *seq,
                                        bool          *par,
                                        bool          *bulk,
                                        const unsigned dims) throw()
        {
            assert(size);
            assert(rank);
            assert(head);
            assert(tail);
            assert(seq);
            assert(par);
            assert(bulk);
            assert(dims>=1);
            assert(dims<=3);
            for(unsigned dim=0;dim<dims;++dim)
            {
                const Coord1D ls = size[dim]; //!< local size
                const Coord1D lr = rank[dim]; //!< local rank
                assert(ls>0);
                assert(lr>=0);
                assert(lr<ls);
                const bool is_head = (  head[dim] = (0==lr)    );
                const bool is_tail = (  tail[dim] = (ls-1==lr) );
                bulk[dim] = (!is_head && !is_tail);
                seq[dim] = !( par[dim] = (ls>1) );
            }
        }


        void Topology:: Expand(Coord1D       *lower,
                               Coord1D       *upper,
                               const Coord1D *width,
                               const bool    *head,
                               const bool    *tail,
                               const bool    *bulk,
                               const bool    *par,
                               const bool    *pbc,
                               const Coord1D  ng,
                               const unsigned dims)
        {
            assert(lower);
            assert(upper);
            assert(width);
            assert(head);
            assert(tail);
            assert(bulk);
            assert(par);
            assert(pbc);
            assert(ng>0);
            assert(dims>=1);
            assert(dims<=3);
            for(unsigned dim=0;dim<dims;++dim)
            {
                if(ng>width[dim]) throw exception("%sExpand(ghosts=%ld>width=%ld in dimension#%u)",fn,long(ng),long(width[dim]),dim);

                Coord1D lo = 0;
                Coord1D up = 0;
                if(bulk[dim])
                {
                    //------------------------------------------------------
                    // in any bulk situatuion
                    //------------------------------------------------------
                    lo = up = ng;
                }
                else
                {
                    const bool periodic = pbc[dim];
                    const bool parallel = par[dim];
                    //------------------------------------------------------
                    // study head case
                    //------------------------------------------------------
                    if( head[dim] )
                    {
                        if(periodic)
                        {
                            lo = ng;
                        }

                        if(parallel)
                        {
                            up = ng;
                        }
                    }

                    //------------------------------------------------------
                    // study tail case, may be redundant, but safe
                    //------------------------------------------------------
                    if( tail[dim] )
                    {
                        if(periodic)
                        {
                            up = ng;
                        }

                        if(parallel)
                        {
                            lo = ng;
                        }
                    }

                }

                assert(lo>=0);
                assert(up>=0);
                lower[dim] -= lo;
                upper[dim] += up;
            }
        }

    }
}
