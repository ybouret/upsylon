//! \file
#ifndef Y_OXIDE_WORKSPACE_INCLUDED
#define Y_OXIDE_WORKSPACE_INCLUDED 1

#include "y/oxide/layouts.hpp"
#include "y/oxide/field/active.hpp"
#include "y/ios/ovstream.hpp"
#include "y/ios/imstream.hpp"

namespace upsylon
{
    namespace Oxide
    {

        //! common workspaces operations
        struct __Workspace
        {
            //! check all coordinates are greater than zero
            static void CheckLocalSizes( const Coord1D *sizes, const unsigned dim );

            //! check all local sizes are greater than zero
            template <typename COORD> static inline
            const COORD & CheckLocalSizes( const COORD &localSizes )
            {
                CheckLocalSizes((const Coord1D *)&localSizes, Coord::Get<COORD>::Dimensions);
                return localSizes;
            }
        };

        typedef ios::ovstream IOBlock; //!< alias for a variable-length I/O block

        //! a workspace is some layouts and some fields
        template <typename COORD>
        class Workspace : public Layouts<COORD>, public Fields
        {
        public:
            //------------------------------------------------------------------
            //
            // types and definition
            //
            //------------------------------------------------------------------
            typedef Layout<COORD>                    LayoutType;                               //!< alias
            typedef typename LayoutType::coord       coord;                                    //!< alias
            typedef typename LayoutType::const_coord const_coord;                              //!< alias
            typedef Layouts<COORD>                   LayoutsType;                              //!< alias
            typedef typename LayoutsType::GIO        GIO;                                      //!< alias
            static const size_t                      Dimensions   = LayoutType::Dimensions;    //!< alias
            static const size_t                      Orientations = LayoutsType::Orientations; //!< alias
            typedef typename LayoutsType::GhostsType GhostsType;                               //!< alias
            typedef typename LayoutsType::Peer       Peer;                                     //!< alias

            //------------------------------------------------------------------
            //
            // members
            //
            //------------------------------------------------------------------
            IOBlock sendBlock; //!< memory to store inner ghosts data, to be sent
            IOBlock recvBlock; //!< memory where outer ghosts data is received

            //------------------------------------------------------------------
            //
            // C++ methods
            //
            //------------------------------------------------------------------
            //! cleaunp
            inline virtual ~Workspace() throw() {}

            //! setup
            inline explicit Workspace(const LayoutType &full,
                                      const_coord      &localSizes,
                                      const Coord1D     globalRank,
                                      const_coord      &PBC,
                                      const Coord1D     ng) :
            LayoutsType(full,
                        __Workspace::CheckLocalSizes(localSizes),
                        globalRank,
                        PBC,
                        ng),
            Fields(), sendBlock(), recvBlock()
            {

            }

            //------------------------------------------------------------------
            //
            // Fields creation/access
            //
            //------------------------------------------------------------------

            //! create and register a field
            template <typename FIELD> inline FIELD & create( const string &name )
            {
                FIELD *F = new FIELD(name,this->outer);
                enroll(F);
                return *F;
            }

            //! create and register a field, wrapper
            template <typename FIELD> inline FIELD & create( const char *name )
            {
                const string _(name); return create<FIELD>(_);
            }

            //! access a created field, const
            template <typename FIELD>
            const FIELD & as( const string &name ) const
            {
                return lookUp<FIELD>(name);
            }

            //! access a created field, const, wrapper
            template <typename FIELD>
            const FIELD & as( const char *name ) const
            {
                const string _(name);
                return as<FIELD>(_);
            }

            //! access a created field
            template <typename FIELD>
            FIELD & as( const string &name )
            {
                return (FIELD &)(lookUp<FIELD>(name));
            }

            //! access a created field, wrapper
            template <typename FIELD>
            FIELD & as( const char  *name )
            {
                const string _(name);
                return as<FIELD>(_);
            }


            //------------------------------------------------------------------
            //
            // local exchanges
            //
            //------------------------------------------------------------------

            //! access ghosts
            const GIO & getGhosts( const size_t orientation ) const throw()
            {
                assert(orientation<Orientations);
                return this->ghosts[orientation];
            }

            //!  exchange of local ghosts pairs
            inline void localExchange1( Field &F  )
            {
                assert(owns(F));
                for(size_t i=0;i<Orientations;++i)
                {
                    const GIO &gio = this->ghosts[i];
                    if(gio.local)
                    {
                        assert(GhostsInfo::Both == gio.status);
                        assert(gio.forward);
                        assert(gio.reverse);

                        const array<Coord1D> &fwd_inner = gio.forward->inner.indices;
                        const array<Coord1D> &fwd_outer = gio.forward->outer.indices;
                        const array<Coord1D> &rev_inner = gio.reverse->inner.indices;
                        const array<Coord1D> &rev_outer = gio.reverse->outer.indices;

                        assert( fwd_inner.size() == fwd_outer.size());
                        assert( fwd_inner.size() == rev_inner.size());
                        assert( fwd_inner.size() == rev_outer.size());

                        for(size_t j=fwd_inner.size();j>0;--j)
                        {
                            F.copyObject(fwd_outer[j],rev_inner[j]);
                            F.copyObject(rev_outer[j],fwd_inner[j]);
                        }
                    }
                }
            }



            //! exchange a full SEQUENCE<FieldPointer>
            template <typename SEQUENCE>
            inline void localExchange( SEQUENCE &fields )
            {
                localExchangeRange(fields.begin(), fields.size());
            }


            //------------------------------------------------------------------
            //
            // asynchronous exchange
            //
            //------------------------------------------------------------------
            //! extract matching ghosts
            Peer getAsyncPeer(const Conn::Way way,
                              const size_t    orientation) const throw()
            {
                assert(orientation<Orientations);
                const GIO &gio = this->ghosts[orientation];
                if(gio.async)
                {
                    switch(way)
                    {
                        case Conn::Forward: return gio.forward;
                        case Conn::Reverse: return gio.reverse;
                    }
                }
                return 0;
            }

            //! save aynchronous content for way+orientation into block
            inline size_t asyncSave1(const Conn::Way     way,
                                     const size_t        orientation,
                                     ios::ostream       &block,
                                     const Field        &F,
                                     Peer               &G) const
            {
                assert(owns(F));
                G = getAsyncPeer(way,orientation);
                if(G)
                {
                    return F.save(G->inner.indices,block);
                }
                else
                {
                    return 0;
                }
            }
            
            //! save some fields, with sendBlock reinitialization
            template <typename SEQUENCE>
            inline size_t asyncSave(const Conn::Way     way,
                                    const size_t        orientation,
                                    SEQUENCE           &fields,
                                    Peer               &G)
            {
                sendBlock.free();
                G = getAsyncPeer(way,orientation);
                if(G)
                {
                    return asyncSave( fields.begin(), fields.size(), *G );
                }
                else
                {
                    return 0;
                }
            }



            //! load asynchronous content for way+orientation from input
            inline size_t asyncLoad1(const Conn::Way     way,
                                     const size_t        orientation,
                                     ios::istream       &input,
                                     Field              &F,
                                     Peer               &G)
            {
                assert(owns(F));
                G = getAsyncPeer(way,orientation);
                if(G)
                {
                    return F.load(G->outer.indices,input);
                }
                else
                {
                    return 0;
                }
            }

            //! load some fields, assuming recvBlock is filled
            template <typename SEQUENCE>
            inline size_t asyncLoad(const Conn::Way     way,
                                    const size_t        orientation,
                                    SEQUENCE           &fields,
                                    Peer               &G)
            {
                G = getAsyncPeer(way,orientation);
                if(G)
                {
                    ios::imstream input(recvBlock);
                    return  asyncLoad( fields.begin(), fields.size(), *G, input);
                }
                else
                {
                    return 0;
                }
            }



        private:
            Y_DISABLE_COPY_AND_ASSIGN(Workspace);


            
            //! *ITERATOR = FieldPointer, **ITERATOR=Field
            template <typename ITERATOR>
            inline void localExchangeRange( ITERATOR it, size_t n )
            {
                while(n-->0)
                {
                    localExchange1( (Field&)(**it) );
                    ++it;
                }
            }

            template <typename ITERATOR>
            inline size_t asyncSave(ITERATOR                it,
                                    size_t                  n,
                                    const GhostsType       &G)
            {
                size_t sendBytes = 0;
                while(n-->0)
                {
                    sendBytes += (**it).save(G.inner.indices,sendBlock);
                    ++it;
                }
                // TODO : CHECK
                return sendBytes;
            }

            template <typename ITERATOR>
            inline size_t asyncLoad(ITERATOR                it,
                                    size_t                  n,
                                    const GhostsType       &G,
                                    ios::istream           &input)
            {
                size_t recvBytes = 0;
                while(n-->0)
                {
                    const Field  &F = **it;
                    ((Field&)F).load(G.outer.indices,input);
                    ++it;
                }
                // TODO: CHECK
                return recvBytes;
            }



        };
    }
}

#endif

