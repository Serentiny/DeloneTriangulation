#include <cmath>
#include <limits>
#include "predicates.h"
#include <gmp.h>

//*
void valueRightTurn(const vertex& a, const vertex& b, const vertex& c, mpq_t& result)
{
	mpq_t bx;
	mpq_init(bx);
	mpq_set_d(bx, b.x);

	mpq_t ax;
	mpq_init(ax);
	mpq_set_d(ax, a.x);

	mpq_t cy;
	mpq_init(cy);
	mpq_set_d(cy, c.y);

	mpq_t ay;
	mpq_init(ay);
	mpq_set_d(ay, a.y);

	mpq_t by;
	mpq_init(by);
	mpq_set_d(by, b.y);

	mpq_t cx;
	mpq_init(cx);
	mpq_set_d(cx, c.x);

	mpq_t tmp1;
	mpq_init(tmp1);
	mpq_sub(tmp1, bx, ax);

	mpq_t tmp2;
	mpq_init(tmp2);
	mpq_sub(tmp2, cy, ay);

	mpq_mul(result, tmp1, tmp2);

	mpq_sub(tmp1, by, ay);
	mpq_sub(tmp2, cx, ax);
	mpq_mul(tmp1, tmp1, tmp2);
	mpq_sub(result, result, tmp1);

	mpq_clear(ax); mpq_clear(bx); mpq_clear(cx); mpq_clear(ay); mpq_clear(by); mpq_clear(cy); mpq_clear(tmp1); mpq_clear(tmp2);
};

int exactRightTurn(const vertex& a, const vertex& b, const vertex& c)
{
	mpq_t value;
	mpq_init(value);
	valueRightTurn(a, b, c, value);

	int sign = mpq_sgn(value); mpq_clear(value);
	return sign;
};	//*/

const double eps = 4 * std::numeric_limits<double>::epsilon();

int rightTurn(const vertex& a, const vertex& b, const vertex& c)
{
	if (!a.natural || !b.natural || !c.natural)
		return 0;
	double left = a.x * b.y + b.x * c.y + c.x * a.y;
	double right = a.x * c.y + b.x * a.y + c.x * b.y;
	double right_turn = right - left;

	if (eps * (std::abs(left) + std::abs(right)) < right_turn)
		return 1;
	if (-(eps * (std::abs(left) + std::abs(right))) > right_turn)
		return -1;

	return exactRightTurn(a, b, c);
};

bool intersection(const vertex& v11, const vertex& v12, const vertex& v21, const vertex& v22)
{
	if (rightTurn(v11, v12, v21) * rightTurn(v11, v12, v22) <= 0 && rightTurn(v21, v22, v11) * rightTurn(v21, v22, v12) <= 0)
		return true;
	return false;
};