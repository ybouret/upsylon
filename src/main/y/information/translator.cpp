
#include "y/information/translator.hpp"

namespace upsylon {

    namespace information {


        Translator:: Translator() throw()
        {
        }

        Translator:: ~Translator() throw()
        {
        }

        size_t Translator :: process(ios::ostream &target,
                                     ios::istream &source,
                                     size_t       *sourceLength)
        {
            size_t n_inp = 0;
            size_t n_out = 0;
            {
                char   C     = 0;
                while( source.query(C) )
                {
                    ++n_inp;
                    this->write(C);
                    while( this->query(C) )
                    {
                        ++n_out;
                        target.write(C);
                    }
                }
                this->flush();
                while( this->query(C) )
                {
                    ++n_out;
                    target.write(C);
                }
            }
            if(sourceLength) *sourceLength = n_inp;
            return n_out;
        }

    }
}

