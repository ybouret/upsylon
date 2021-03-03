
#include "y/color/convert.hpp"

namespace upsylon
{

        template <> float convert_color<float,uint8_t>:: from(const uint8_t &u) throw()
        {
            return core::convert_color::unit_float[u];
        }
        
        template <> rgb convert_color<rgb,uint8_t>:: from(const uint8_t &u) throw()
        {
            return rgb(u,u,u);
        }
        
        template <> rgba convert_color<rgba,uint8_t>:: from(const uint8_t &u) throw()
        {
            return rgba(u,u,u);
        }
        
        
        template <>
        YUV convert_color<YUV,uint8_t>:: from(const uint8_t &u) throw()
        {
            return YUV(core::convert_color::unit_float[u],0,0);
        }
        

    namespace core
    {
#define _F(X) (X##.0f/255.0f)
        
        const float convert_color:: unit_float[256] =
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
        
        
        
    }
    
}

