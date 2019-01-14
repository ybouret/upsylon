#ifndef Y_GEOMETRY_ISO2D_INCLUDED
#define Y_GEOMETRY_ISO2D_INCLUDED 1


#include "y/type/point2d.hpp"
#include "y/associative/set.hpp"
#include "y/ptr/arc.hpp"
#include "y/ptr/intr.hpp"

namespace upsylon
{
    namespace geometry
    {

        //! findind iso levels in 2d
        /**
         Each square is divided in four triangles
         4@(i  ,j+1)------3@(i+1,j+1)
         | \           /  |
         |   \       /    |
         |     \   /      |
         |       0        |
         |     /   \      |
         |   /       \    |
         | /           \  |
         1@(i ,j   )-----2@(i+1,j  )

         For a given level, a matching value can be found on a vertex or
         on within an edge, which defines a set of unique coordinates
         for any point of the isoline
         */
        struct iso2d
        {
            typedef point2d<double> vertex;

            //__________________________________________________________________
            //
            // iso level is on a vertex
            //__________________________________________________________________
            static const unsigned vertex0 = 0x01<<0; //!< bit 1
            static const unsigned vertex1 = 0x01<<1; //!< bit 2
            static const unsigned vertex2 = 0x01<<2; //!< bit 3
            static const unsigned vertex3 = 0x01<<3; //!< bit 4
            static const unsigned vertex4 = 0x01<<4; //!< bit 5

            //__________________________________________________________________
            //
            // iso level is on an edge
            //__________________________________________________________________
            static const unsigned edge01  = (vertex0|vertex1);
            static const unsigned edge02  = (vertex0|vertex2);
            static const unsigned edge03  = (vertex0|vertex3);
            static const unsigned edge04  = (vertex0|vertex4);

            static const unsigned edge12  = (vertex1|vertex2);
            static const unsigned edge14  = (vertex1|vertex4);

            static const unsigned edge23  = (vertex2|vertex3);
            static const unsigned edge34  = (vertex3|vertex4);

            //! identifier for a point of an iso level
            class identifier
            {
            public:
                const unit_t   i; //!< index of the square
                const unit_t   j; //!< index of the square
                const unsigned p; //!< position within the square

                identifier(const unit_t ii, const unit_t jj, const unsigned pp) throw();
                identifier(const identifier &other) throw();
                ~identifier() throw();

                friend bool operator==(const identifier &lhs, const identifier &rhs) throw()
                {
                    return (lhs.i == rhs.i) && (lhs.j == rhs.j) && (lhs.p == rhs.p);
                }
            private:
                Y_DISABLE_ASSIGN(identifier);
            public:
                class hasher
                {
                public:
                    hashing::fnv H;
                    hasher() throw();
                    ~hasher() throw();
                    size_t operator()(const identifier &id) throw();

                private:
                    Y_DISABLE_COPY_AND_ASSIGN(hasher);
                };
            };

            //! a unique point is an identifier and its position
            class unique_point : public counted_object
            {
            public:
                const identifier tag; //!< logical  identifier
                const vertex     vtx; //!< physical position

                explicit unique_point(const identifier &id, const vertex &v) throw();
                virtual ~unique_point() throw();
                inline const identifier & key() const throw() { return tag; }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(unique_point);
            };

            typedef intr_ptr<identifier,unique_point>               shared_point;
            typedef set<identifier,shared_point,identifier::hasher> unique_points;

            class segment : public object
            {
            public:
                typedef core::list_of_cpp<segment> list;
                segment           *next;
                segment           *prev;
                const shared_point a;
                const shared_point b;
                explicit segment(const shared_point &A, const shared_point &B) throw();
                virtual ~segment() throw();

            private:
                Y_DISABLE_COPY_AND_ASSIGN(segment);
            };

            class shared_points : public unique_points, public counted
            {
            public:
                explicit shared_points() throw() {}
                virtual ~shared_points() throw() {}

            private:
                Y_DISABLE_COPY_AND_ASSIGN(shared_points);
            };

        };

    }
}

#endif

