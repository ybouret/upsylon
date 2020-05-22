
//! \file

#ifndef Y_JARGON_MOTIFS_INCLUDED
#define Y_JARGON_MOTIFS_INCLUDED 1

#include "y/jargon/pattern.hpp"

namespace upsylon {

    namespace Jargon {


        class Motifs : public Motif
        {
        public:
            Motifs(const string &);
            Motifs(const char   *);
            Motifs(const Motifs &) throw();
            virtual ~Motifs() throw();
            
        private:
            Y_DISABLE_ASSIGN(Motifs);
        };


    }

}

#endif

