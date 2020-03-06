#include "sort42.hpp"
namespace upsylon {
	const unsigned network_sort<42>::I[375] = {
 0, 3, 5, 8, 10, 13, 16, 19, 21, 24, 26, 29, 31, 34, 37, 40, 2, 7, 12, 15, 18, 23, 28, 33, 36, 39, 2, 1, 7, 6, 12, 11, 15, 18, 17, 23, 22, 28, 27, 33, 32, 36, 39, 38, 0, 5, 4, 10, 15, 16, 14, 21, 26, 25, 31, 36, 37, 35, 0, 1, 5, 6, 10, 11, 17, 16, 9, 21, 22, 26, 27, 31, 32, 38, 37, 30, 1, 6, 0, 3, 11, 17, 10, 13, 22, 27, 21, 24, 32, 38, 31, 34, 20, 1, 2, 4, 10, 11, 12, 14, 22, 23, 25, 31, 32, 33, 35, 1, 3, 11, 13, 8, 22, 24, 32, 34, 29, 4, 2, 3, 11, 14, 13, 9, 25, 23, 24, 32, 35, 34, 30, 4, 3, 12, 14, 0, 7, 25, 24, 33, 35, 21, 28, 19, 4, 14, 13, 0, 1, 6, 8, 25, 35, 34, 21, 22, 27, 29, 20, 14, 1, 2, 5, 9, 8, 35, 22, 23, 26, 30, 29, 0, 1, 3, 4, 6, 9, 22, 24, 25, 27, 30, 18, 4, 3, 2, 7, 25, 24, 23, 28, 1, 17, 19, 4, 3, 8, 25, 24, 29, 1, 2, 20, 4, 3, 9, 7, 8, 25, 24, 30, 28, 29, 2, 20, 4, 6, 9, 8, 25, 27, 30, 29, 3, 16, 4, 5, 9, 25, 26, 30, 3, 15, 17, 5, 6, 9, 26, 27, 30, 4, 3, 14, 16, 19, 6, 8, 9, 27, 29, 30, 4, 3, 5, 13, 17, 20, 7, 9, 28, 30, 4, 6, 12, 14, 18, 8, 29, 4, 6, 7, 11, 13, 19, 18, 9, 30, 4, 8, 7, 6, 14, 13, 20, 19, 9, 8, 5, 7, 10, 14, 17, 20, 19, 9, 8, 6, 11, 16, 20, 9, 8, 7, 12, 17, 20, 9, 8, 13, 19, 9, 8, 14, 20, 9, 8, 15, 9, 8, 16, 14, 15, 9, 17, 13, 16, 9, 18, 12, 14, 17, 16, 9, 19, 11, 13, 17, 18, 20, 10, 14, 13, 17, 19, 11, 14, 20, 19, 18, 17, 12, 20, 19, 16, 18, 13, 20, 19, 17, 14, 20, 19, 20, 15, 19, 16, 20, 19, 17, 20, 18, 20, 20, 19, 20};

	const unsigned network_sort<42>::J[375] = {
 1, 4, 6, 9, 11, 14, 17, 20, 22, 25, 27, 30, 32, 35, 38, 41, 4, 9, 14, 17, 20, 25, 30, 35, 38, 41, 3, 4, 8, 9, 13, 14, 16, 19, 20, 24, 25, 29, 30, 34, 35, 37, 40, 41, 3, 8, 9, 13, 18, 19, 20, 24, 29, 30, 34, 39, 40, 41, 2, 3, 7, 8, 12, 13, 19, 18, 20, 23, 24, 28, 29, 33, 34, 40, 39, 41, 2, 7, 5, 8, 12, 18, 16, 19, 23, 28, 26, 29, 33, 39, 37, 40, 41, 6, 7, 8, 15, 17, 18, 19, 27, 28, 29, 36, 38, 39, 40, 5, 7, 16, 18, 19, 26, 28, 37, 39, 40, 7, 5, 6, 15, 18, 16, 19, 28, 26, 27, 36, 39, 37, 40, 6, 5, 15, 17, 11, 18, 27, 26, 36, 38, 32, 39, 40, 5, 16, 15, 10, 12, 17, 18, 26, 37, 36, 31, 33, 38, 39, 40, 15, 11, 13, 16, 18, 17, 36, 32, 34, 37, 39, 38, 21, 10, 14, 15, 16, 17, 31, 35, 36, 37, 38, 39, 14, 13, 10, 16, 35, 34, 31, 37, 22, 38, 39, 13, 11, 16, 34, 32, 37, 21, 23, 39, 12, 10, 16, 13, 14, 33, 31, 37, 34, 35, 21, 38, 11, 12, 15, 13, 32, 33, 36, 34, 24, 37, 10, 11, 14, 31, 32, 35, 23, 36, 37, 10, 11, 13, 31, 32, 34, 25, 22, 35, 36, 37, 10, 11, 12, 31, 32, 33, 24, 21, 26, 34, 36, 37, 10, 11, 31, 32, 23, 27, 33, 34, 36, 10, 31, 22, 26, 28, 32, 33, 36, 34, 10, 31, 21, 29, 26, 22, 33, 32, 36, 35, 30, 28, 21, 23, 31, 32, 33, 35, 34, 29, 27, 21, 31, 32, 34, 28, 26, 21, 31, 32, 33, 27, 24, 31, 32, 26, 23, 31, 32, 25, 22, 31, 24, 21, 31, 25, 26, 23, 31, 24, 27, 22, 31, 23, 24, 28, 26, 21, 31, 22, 23, 27, 29, 31, 21, 23, 22, 26, 30, 21, 22, 30, 29, 26, 23, 21, 29, 27, 22, 24, 21, 28, 26, 22, 21, 27, 25, 26, 21, 24, 21, 25, 22, 21, 24, 21, 23, 22, 21, 21};

}
