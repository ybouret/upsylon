//! \file
#ifndef Y_OXIDE_WORKSPACE_INCLUDED
#define Y_OXIDE_WORKSPACE_INCLUDED 1

#include "y/oxide/layouts.hpp"
#include "y/oxide/field/set.hpp"

namespace upsylon
{
    namespace Oxide
    {

        typedef set<string,FieldHandle> Fields;

        template <typename COORD>
        class Workspace : public Layouts<COORD>, public Fields
        {
        public:
            //------------------------------------------------------------------
            //
            // types and definition
            //
            //------------------------------------------------------------------
            typedef Layout<COORD>                    LayoutType;
            typedef typename LayoutType::coord       coord;
            typedef typename LayoutType::const_coord const_coord;
            typedef Layouts<COORD>                   LayoutsType;
            static const size_t                      Dimensions = LayoutType::Dimensions;


            //------------------------------------------------------------------
            //
            // methods
            //
            //------------------------------------------------------------------
            virtual ~Workspace() throw()
            {
            }

            //! setup
            explicit Workspace(const LayoutType &full,
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

            //! access a created field
            template <typename FIELD>
            FIELD & as( const string &name )
            {
                return (FIELD &)(__Fields::LookUp<FIELD>(*this,name));
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Workspace);
        };
    }
}

#endif

