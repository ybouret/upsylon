#include "y/oxide/grid/structured.hpp"



namespace upsylon {

    namespace Oxide {

        const char StructuredGrid_::Name[]    = "StructuredGrid";
        const char StructuredGrid_::Spacing[] = "spacing";

        const char StructuredGrid_:: VTK_DATASET[]    = "DATASET STRUCTURED_POINTS";
        const char StructuredGrid_:: VTK_DIMENSIONS[] = "DIMENSIONS";
        const char StructuredGrid_:: VTK_ORIGIN[]     = "ORIGIN";
        const char StructuredGrid_:: VTK_SPACING[]    = "SPACING";

        StructuredGrid_::  StructuredGrid_() throw() {}
        StructuredGrid_:: ~StructuredGrid_() throw() {}

    }

}
