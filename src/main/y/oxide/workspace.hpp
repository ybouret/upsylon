//! \file
#ifndef Y_OXIDE_WORKSPACE_INCLUDED
#define Y_OXIDE_WORKSPACE_INCLUDED 1

#include "y/oxide/layouts.hpp"
#include "y/oxide/field/set.hpp"

namespace upsylon
{
    namespace Oxide
    {
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
            LayoutsType(full,localSizes,globalRank,PBC,ng),
            Fields()
            {

            }

            //! create and register a field
            template <typename FIELD> inline FIELD & create( const string &name )
            {
                FIELD *F = new FIELD(name,this->outer);
                __Fields::Enroll<FIELD>(*this,F);
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
                return __Fields::LookUp<FIELD>(*this,name);
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
                return (FIELD &)(__Fields::LookUp<FIELD>(*this,name));
            }

            //! access a created field, wrapper
            template <typename FIELD>
            FIELD & as( const char  *name )
            {
                const string _(name);
                return as<FIELD>(_);
            }

            //! test really owned
            bool owns( const FieldType &F ) const
            {
                const FieldHandle *h = this->search(F.name);
                if(!h)
                {
                    return false;
                }
                else
                {
                    const FieldPointer &p = h->field;
                    if( & *p != &F )
                    {
                        return false;
                    }
                    else
                    {
                        return true;
                    }
                }
            }

            void exchangeLocal( FieldType &F  )
            {
                //std::cerr << "exchanging Local in " << F.name << std::endl;
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
                        //std::cerr << "@orientation#" << i << " : " << fwd_inner.size() << std::endl;

                        for(size_t j=fwd_inner.size();j>0;--j)
                        {
                            F.copyObject(fwd_outer[j],rev_inner[j]);
                            F.copyObject(rev_outer[j],fwd_inner[j]);
                        }
                    }

                }
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Workspace);
        };
    }
}

#endif

