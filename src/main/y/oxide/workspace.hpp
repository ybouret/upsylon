//! \file
#ifndef Y_OXIDE_WORKSPACE_INCLUDED
#define Y_OXIDE_WORKSPACE_INCLUDED 1

#include "y/oxide/layouts.hpp"
#include "y/oxide/field/set.hpp"
#include "y/ios/ovstream.hpp"

namespace upsylon
{
    namespace Oxide
    {

        struct __Workspace
        {
            static void CheckLocalSizes( const Coord1D *sizes, const unsigned dim );

            template <typename COORD> static inline
            const COORD & CheckLocalSizes( const COORD &localSizes )
            {
                CheckLocalSizes((const Coord1D *)&localSizes, Coord::Get<COORD>::Dimensions);
                return localSizes;
            }
        };

        typedef ios::ovstream IOBlock;

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
            typedef _Ghosts<COORD>                   Ghosts;                                   //!< alias

            //------------------------------------------------------------------
            //
            // members
            //
            //------------------------------------------------------------------
            IOBlock sendBlock;
            IOBlock recvBlock;

            //------------------------------------------------------------------
            //
            // methods
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
            Fields()
            {

            }

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

           
            //!  exchange of local ghosts pairs
            inline void localExchange( FieldType &F  )
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

            //!  exchange of local ghosts pairs, wrapper
            inline void localExchange( const string &fieldName )
            {
                Fields &self = *this;
                localExchange(self[fieldName]);
            }

            //! exchange of local ghosts pairs, wrapper
            inline void localExchange( const char *fieldName )
            {
                Fields &self = *this;
                localExchange(self[fieldName]);
            }


            //! *ITERATOR = FieldPointer, **ITERATOR=FieldType
            template <typename ITERATOR>
            inline void localExchangeRange( ITERATOR it, size_t n )
            {
                while(n-->0)
                {
                    localExchange(**it);
                    ++it;
                }
            }

            //! exchange a full sequence<FieldPointer>
            template <typename SEQUENCE>
            inline void localExchangeAll( SEQUENCE &fields )
            {
                localExchangeRange(fields.begin(), fields.size());
            }


            //! save aynchronous content for way+orientation into block
            inline size_t asyncSave(const Connectivity::Way way,
                                    const size_t            orientation,
                                    ios::ostream           &block,
                                    const FieldType        &F) const
            {
                assert(owns(F));
                const Ghosts *G = getAsync(way,orientation);
                if(G)
                {
                    return F.save(G->inner.indices,block);
                }
                else
                {
                    return 0;
                }
            }

            //! load asynchronous content for way+orientation from block
            inline size_t asyncLoad(const Connectivity::Way way,
                                    const size_t            orientation,
                                    ios::istream           &block,
                                    FieldType              &F)
            {
                assert(owns(F));
                const Ghosts *G = getAsync(way,orientation);
                if(G)
                {
                    return F.load(G->outer.indices,block);
                }
                else
                {
                    return 0;
                }
            }



            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Workspace);
            const Ghosts *getAsync(const Connectivity::Way way,
                                   const size_t            orientation) const throw()
            {
                assert(orientation<Orientations);
                const GIO &gio = this->ghosts[orientation];
                if(gio.async)
                {
                    switch(way)
                    {
                        case Connectivity::Forward: return gio.forward;
                        case Connectivity::Reverse: return gio.reverse;
                    }
                }
                return 0;
            }
        };
    }
}

#endif

