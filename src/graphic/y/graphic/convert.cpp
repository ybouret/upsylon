
#include "y/graphic/convert.hpp"

namespace upsylon {

    namespace Graphic {


#define _F(X) (X##.0f/255.0f)
        
        const float Convert::UnitFloat[256] =
        {
            _F(  0),_F(  1),_F(  2),_F(  3),_F(  4),_F(  5),_F(  6),_F(  7),_F(  8),_F(  9),_F( 10),_F( 11),_F( 12),_F( 13),_F( 14),_F( 15),
            _F( 16),_F( 17),_F( 18),_F( 19),_F( 20),_F( 21),_F( 22),_F( 23),_F( 24),_F( 25),_F( 26),_F( 27),_F( 28),_F( 29),_F( 30),_F( 31),
            _F( 32),_F( 33),_F( 34),_F( 35),_F( 36),_F( 37),_F( 38),_F( 39),_F( 40),_F( 41),_F( 42),_F( 43),_F( 44),_F( 45),_F( 46),_F( 47),
            _F( 48),_F( 49),_F( 50),_F( 51),_F( 52),_F( 53),_F( 54),_F( 55),_F( 56),_F( 57),_F( 58),_F( 59),_F( 60),_F( 61),_F( 62),_F( 63),
            _F( 64),_F( 65),_F( 66),_F( 67),_F( 68),_F( 69),_F( 70),_F( 71),_F( 72),_F( 73),_F( 74),_F( 75),_F( 76),_F( 77),_F( 78),_F( 79),
            _F( 80),_F( 81),_F( 82),_F( 83),_F( 84),_F( 85),_F( 86),_F( 87),_F( 88),_F( 89),_F( 90),_F( 91),_F( 92),_F( 93),_F( 94),_F( 95),
            _F( 96),_F( 97),_F( 98),_F( 99),_F(100),_F(101),_F(102),_F(103),_F(104),_F(105),_F(106),_F(107),_F(108),_F(109),_F(110),_F(111),
            _F(112),_F(113),_F(114),_F(115),_F(116),_F(117),_F(118),_F(119),_F(120),_F(121),_F(122),_F(123),_F(124),_F(125),_F(126),_F(127),
            _F(128),_F(129),_F(130),_F(131),_F(132),_F(133),_F(134),_F(135),_F(136),_F(137),_F(138),_F(139),_F(140),_F(141),_F(142),_F(143),
            _F(144),_F(145),_F(146),_F(147),_F(148),_F(149),_F(150),_F(151),_F(152),_F(153),_F(154),_F(155),_F(156),_F(157),_F(158),_F(159),
            _F(160),_F(161),_F(162),_F(163),_F(164),_F(165),_F(166),_F(167),_F(168),_F(169),_F(170),_F(171),_F(172),_F(173),_F(174),_F(175),
            _F(176),_F(177),_F(178),_F(179),_F(180),_F(181),_F(182),_F(183),_F(184),_F(185),_F(186),_F(187),_F(188),_F(189),_F(190),_F(191),
            _F(192),_F(193),_F(194),_F(195),_F(196),_F(197),_F(198),_F(199),_F(200),_F(201),_F(202),_F(203),_F(204),_F(205),_F(206),_F(207),
            _F(208),_F(209),_F(210),_F(211),_F(212),_F(213),_F(214),_F(215),_F(216),_F(217),_F(218),_F(219),_F(220),_F(221),_F(222),_F(223),
            _F(224),_F(225),_F(226),_F(227),_F(228),_F(229),_F(230),_F(231),_F(232),_F(233),_F(234),_F(235),_F(236),_F(237),_F(238),_F(239),
            _F(240),_F(241),_F(242),_F(243),_F(244),_F(245),_F(246),_F(247),_F(248),_F(249),_F(250),_F(251),_F(252),_F(253),_F(254),_F(255)
        };

        const uint8_t Convert::GreyScaleTable[] =
        {
            0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x02, 0x02, 0x02, 0x03, 0x03, 0x03, 0x04, 0x04, 0x04, 0x05,
            0x05, 0x05, 0x06, 0x06, 0x06, 0x07, 0x07, 0x07, 0x08, 0x08, 0x08, 0x09, 0x09, 0x09, 0x0a, 0x0a,
            0x0a, 0x0b, 0x0b, 0x0b, 0x0c, 0x0c, 0x0c, 0x0d, 0x0d, 0x0d, 0x0e, 0x0e, 0x0e, 0x0f, 0x0f, 0x0f,
            0x10, 0x10, 0x10, 0x11, 0x11, 0x11, 0x12, 0x12, 0x12, 0x13, 0x13, 0x13, 0x14, 0x14, 0x14, 0x15,
            0x15, 0x15, 0x16, 0x16, 0x16, 0x17, 0x17, 0x17, 0x18, 0x18, 0x18, 0x19, 0x19, 0x19, 0x1a, 0x1a,
            0x1a, 0x1b, 0x1b, 0x1b, 0x1c, 0x1c, 0x1c, 0x1d, 0x1d, 0x1d, 0x1e, 0x1e, 0x1e, 0x1f, 0x1f, 0x1f,
            0x20, 0x20, 0x20, 0x21, 0x21, 0x21, 0x22, 0x22, 0x22, 0x23, 0x23, 0x23, 0x24, 0x24, 0x24, 0x25,
            0x25, 0x25, 0x26, 0x26, 0x26, 0x27, 0x27, 0x27, 0x28, 0x28, 0x28, 0x29, 0x29, 0x29, 0x2a, 0x2a,
            0x2a, 0x2b, 0x2b, 0x2b, 0x2c, 0x2c, 0x2c, 0x2d, 0x2d, 0x2d, 0x2e, 0x2e, 0x2e, 0x2f, 0x2f, 0x2f,
            0x30, 0x30, 0x30, 0x31, 0x31, 0x31, 0x32, 0x32, 0x32, 0x33, 0x33, 0x33, 0x34, 0x34, 0x34, 0x35,
            0x35, 0x35, 0x36, 0x36, 0x36, 0x37, 0x37, 0x37, 0x38, 0x38, 0x38, 0x39, 0x39, 0x39, 0x3a, 0x3a,
            0x3a, 0x3b, 0x3b, 0x3b, 0x3c, 0x3c, 0x3c, 0x3d, 0x3d, 0x3d, 0x3e, 0x3e, 0x3e, 0x3f, 0x3f, 0x3f,
            0x40, 0x40, 0x40, 0x41, 0x41, 0x41, 0x42, 0x42, 0x42, 0x43, 0x43, 0x43, 0x44, 0x44, 0x44, 0x45,
            0x45, 0x45, 0x46, 0x46, 0x46, 0x47, 0x47, 0x47, 0x48, 0x48, 0x48, 0x49, 0x49, 0x49, 0x4a, 0x4a,
            0x4a, 0x4b, 0x4b, 0x4b, 0x4c, 0x4c, 0x4c, 0x4d, 0x4d, 0x4d, 0x4e, 0x4e, 0x4e, 0x4f, 0x4f, 0x4f,
            0x50, 0x50, 0x50, 0x51, 0x51, 0x51, 0x52, 0x52, 0x52, 0x53, 0x53, 0x53, 0x54, 0x54, 0x54, 0x55,
            0x55, 0x55, 0x56, 0x56, 0x56, 0x57, 0x57, 0x57, 0x58, 0x58, 0x58, 0x59, 0x59, 0x59, 0x5a, 0x5a,
            0x5a, 0x5b, 0x5b, 0x5b, 0x5c, 0x5c, 0x5c, 0x5d, 0x5d, 0x5d, 0x5e, 0x5e, 0x5e, 0x5f, 0x5f, 0x5f,
            0x60, 0x60, 0x60, 0x61, 0x61, 0x61, 0x62, 0x62, 0x62, 0x63, 0x63, 0x63, 0x64, 0x64, 0x64, 0x65,
            0x65, 0x65, 0x66, 0x66, 0x66, 0x67, 0x67, 0x67, 0x68, 0x68, 0x68, 0x69, 0x69, 0x69, 0x6a, 0x6a,
            0x6a, 0x6b, 0x6b, 0x6b, 0x6c, 0x6c, 0x6c, 0x6d, 0x6d, 0x6d, 0x6e, 0x6e, 0x6e, 0x6f, 0x6f, 0x6f,
            0x70, 0x70, 0x70, 0x71, 0x71, 0x71, 0x72, 0x72, 0x72, 0x73, 0x73, 0x73, 0x74, 0x74, 0x74, 0x75,
            0x75, 0x75, 0x76, 0x76, 0x76, 0x77, 0x77, 0x77, 0x78, 0x78, 0x78, 0x79, 0x79, 0x79, 0x7a, 0x7a,
            0x7a, 0x7b, 0x7b, 0x7b, 0x7c, 0x7c, 0x7c, 0x7d, 0x7d, 0x7d, 0x7e, 0x7e, 0x7e, 0x7f, 0x7f, 0x7f,
            0x80, 0x80, 0x80, 0x81, 0x81, 0x81, 0x82, 0x82, 0x82, 0x83, 0x83, 0x83, 0x84, 0x84, 0x84, 0x85,
            0x85, 0x85, 0x86, 0x86, 0x86, 0x87, 0x87, 0x87, 0x88, 0x88, 0x88, 0x89, 0x89, 0x89, 0x8a, 0x8a,
            0x8a, 0x8b, 0x8b, 0x8b, 0x8c, 0x8c, 0x8c, 0x8d, 0x8d, 0x8d, 0x8e, 0x8e, 0x8e, 0x8f, 0x8f, 0x8f,
            0x90, 0x90, 0x90, 0x91, 0x91, 0x91, 0x92, 0x92, 0x92, 0x93, 0x93, 0x93, 0x94, 0x94, 0x94, 0x95,
            0x95, 0x95, 0x96, 0x96, 0x96, 0x97, 0x97, 0x97, 0x98, 0x98, 0x98, 0x99, 0x99, 0x99, 0x9a, 0x9a,
            0x9a, 0x9b, 0x9b, 0x9b, 0x9c, 0x9c, 0x9c, 0x9d, 0x9d, 0x9d, 0x9e, 0x9e, 0x9e, 0x9f, 0x9f, 0x9f,
            0xa0, 0xa0, 0xa0, 0xa1, 0xa1, 0xa1, 0xa2, 0xa2, 0xa2, 0xa3, 0xa3, 0xa3, 0xa4, 0xa4, 0xa4, 0xa5,
            0xa5, 0xa5, 0xa6, 0xa6, 0xa6, 0xa7, 0xa7, 0xa7, 0xa8, 0xa8, 0xa8, 0xa9, 0xa9, 0xa9, 0xaa, 0xaa,
            0xaa, 0xab, 0xab, 0xab, 0xac, 0xac, 0xac, 0xad, 0xad, 0xad, 0xae, 0xae, 0xae, 0xaf, 0xaf, 0xaf,
            0xb0, 0xb0, 0xb0, 0xb1, 0xb1, 0xb1, 0xb2, 0xb2, 0xb2, 0xb3, 0xb3, 0xb3, 0xb4, 0xb4, 0xb4, 0xb5,
            0xb5, 0xb5, 0xb6, 0xb6, 0xb6, 0xb7, 0xb7, 0xb7, 0xb8, 0xb8, 0xb8, 0xb9, 0xb9, 0xb9, 0xba, 0xba,
            0xba, 0xbb, 0xbb, 0xbb, 0xbc, 0xbc, 0xbc, 0xbd, 0xbd, 0xbd, 0xbe, 0xbe, 0xbe, 0xbf, 0xbf, 0xbf,
            0xc0, 0xc0, 0xc0, 0xc1, 0xc1, 0xc1, 0xc2, 0xc2, 0xc2, 0xc3, 0xc3, 0xc3, 0xc4, 0xc4, 0xc4, 0xc5,
            0xc5, 0xc5, 0xc6, 0xc6, 0xc6, 0xc7, 0xc7, 0xc7, 0xc8, 0xc8, 0xc8, 0xc9, 0xc9, 0xc9, 0xca, 0xca,
            0xca, 0xcb, 0xcb, 0xcb, 0xcc, 0xcc, 0xcc, 0xcd, 0xcd, 0xcd, 0xce, 0xce, 0xce, 0xcf, 0xcf, 0xcf,
            0xd0, 0xd0, 0xd0, 0xd1, 0xd1, 0xd1, 0xd2, 0xd2, 0xd2, 0xd3, 0xd3, 0xd3, 0xd4, 0xd4, 0xd4, 0xd5,
            0xd5, 0xd5, 0xd6, 0xd6, 0xd6, 0xd7, 0xd7, 0xd7, 0xd8, 0xd8, 0xd8, 0xd9, 0xd9, 0xd9, 0xda, 0xda,
            0xda, 0xdb, 0xdb, 0xdb, 0xdc, 0xdc, 0xdc, 0xdd, 0xdd, 0xdd, 0xde, 0xde, 0xde, 0xdf, 0xdf, 0xdf,
            0xe0, 0xe0, 0xe0, 0xe1, 0xe1, 0xe1, 0xe2, 0xe2, 0xe2, 0xe3, 0xe3, 0xe3, 0xe4, 0xe4, 0xe4, 0xe5,
            0xe5, 0xe5, 0xe6, 0xe6, 0xe6, 0xe7, 0xe7, 0xe7, 0xe8, 0xe8, 0xe8, 0xe9, 0xe9, 0xe9, 0xea, 0xea,
            0xea, 0xeb, 0xeb, 0xeb, 0xec, 0xec, 0xec, 0xed, 0xed, 0xed, 0xee, 0xee, 0xee, 0xef, 0xef, 0xef,
            0xf0, 0xf0, 0xf0, 0xf1, 0xf1, 0xf1, 0xf2, 0xf2, 0xf2, 0xf3, 0xf3, 0xf3, 0xf4, 0xf4, 0xf4, 0xf5,
            0xf5, 0xf5, 0xf6, 0xf6, 0xf6, 0xf7, 0xf7, 0xf7, 0xf8, 0xf8, 0xf8, 0xf9, 0xf9, 0xf9, 0xfa, 0xfa,
            0xfa, 0xfb, 0xfb, 0xfb, 0xfc, 0xfc, 0xfc, 0xfd, 0xfd, 0xfd, 0xfe, 0xfe, 0xfe, 0xff
        };

        uint8_t Convert:: GreyScale(const uint8_t r, const uint8_t g, const uint8_t b) throw()
        {
            const unsigned i = unsigned(r) + unsigned(g) + unsigned(b);
            assert(i<sizeof(GreyScaleTable)/sizeof(GreyScaleTable[0]));
            return GreyScaleTable[i];
        }


        YUV Convert:: RGB2YUV(const float r, const float g, const float b) throw()
        {
            return YUV( 0.299f * r + 0.587f * g + 0.114f * b,
                       -0.147f * r - 0.289f * g + 0.436f * b,
                       0.615f  * r - 0.515f * g - 0.100f * b);
        }

        float Convert:: YUV2R( const YUV &_ ) throw()
        {
            return clamp<float>(0.0f, _.y + 1.140f * _.v,1.0f);
        }

        float Convert:: YUV2G( const YUV &_ ) throw()
        {
            return clamp<float>(0.0f, _.y - 0.394f * _.u - 0.581f * _.v,1.0f);
        }

        float Convert:: YUV2B( const YUV &_ ) throw()
        {
            return clamp<float>(0.0f, _.y + 2.028f * _.u,1.0f);
        }


        template <>
        YUV Convert:: Get<YUV,rgb>( const rgb &C ) throw()
        {
            return RGB2YUV( C );
        }

        template <>
        YUV Convert:: Get<YUV,rgba>( const rgba &C ) throw()
        {
            return RGB2YUV( C );
        }


        template <>
        rgb Convert:: Get<rgb,YUV>(const YUV &_) throw()
        {
            return YUV2RGB(_);
        }

        template <>
        rgba Convert:: Get<rgba,YUV>(const YUV &_) throw()
        {
            return YUV2RGB(_);
        }


        template <>
        float Convert::Get<float,float>( const float &f ) throw()
        {
            return f;
        }
        
        template <>
        float Convert::Get<float,rgb>(const rgb &C) throw()
        {
            return UnitFloat[ GreyScale(C) ];
        }
        
        template <>
        float Convert::Get<float,rgba>(const rgba &C) throw()
        {
            return UnitFloat[ GreyScale(C) ];
        }

        template <>
        uint8_t Convert::Get<uint8_t,float>( const float &f ) throw()
        {
            return Float2Byte(f);
        }

        template <>
        float Convert::Get<float,uint8_t>( const uint8_t &u) throw()
        {
            return UnitFloat[u];
        }

        template <>
        uint8_t Convert::Get<uint8_t,uint8_t>(const uint8_t &u ) throw()
        {
            return u;
        }


        template <>
        uint8_t Convert::Get<uint8_t,rgb>(const rgb &C) throw()
        {
            return GreyScale(C);
        }

        template<>
        rgb Convert::Get<rgb,uint8_t>(const uint8_t &u) throw()
        {
            return rgb(u,u,u);
        }


        template <>
        float Convert::Get<float,size_t>(const size_t &s ) throw()
        {
            return float(s);
        }


        // RGBA->something

        template <>
        uint8_t Convert::Get<uint8_t,rgba>(const rgba &C) throw()
        {
            return GreyScale(C);
        }


        template <>
        rgb Convert::Get<rgb,rgba>(const rgba &C) throw()
        {
            return C;
        }

        

        template <>
        rgba Convert::Get<rgba,rgba>(const rgba &C) throw() { return C; }

        // something->RGBA
        template<>
        rgba Convert::Get<rgba,uint8_t>(const uint8_t &u) throw()
        {
            return rgba(u,u,u);
        }

        template <>
        rgba Convert:: Get<rgba,float>(const float &f) throw()
        {
            const uint8_t u = Float2Byte(f);
            return rgba(u,u,u);
        }

        template <>
        rgba Convert::Get<rgba,rgb>(const rgb &C) throw() { return C; }

       
        
    }

}

