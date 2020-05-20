
#include "y/spade/workspace.hpp"
#include "y/exception.hpp"

namespace upsylon {

    namespace Spade {

        namespace Kernel
        {
            Workspace:: ~Workspace() throw()
            {
            }

            Workspace:: Workspace(const unsigned d) throw() :
            dimensions(d),
            fdb(),
            fields()
            {
            }
            
            
            void Workspace:: add(const FieldHandle &P, const FieldClass cls )
            {
                const string &id = P->name;
                // into database
                if(! aliasing::_(fdb).insert(P))
                {
                    throw exception("Spade::Workspace(multiple field '%s')", *id);
                }

                // put it in fields if necessary
                switch (cls) {
                    case AsyncField:
                        try { aliasing::_(fields).push_back(P);      }
                        catch(...) { aliasing::_(fdb).no(id); throw; }
                        break;
                    case LocalField: break;
                }
            }

            bool Workspace:: owns(const FieldHandle &F) const throw()
            {
                const FieldHandle *p = fdb.search(F->name);
                return( NULL!=p) && (*p == F) ;
            }

            bool Workspace:: ownsAll(Fields &F) const throw()
            {
                for(size_t i=F.size();i>0;--i)
                {
                    if( ! owns(F[i]) ) return false;
                }
                return true;
            }


        }

    }
}

