#include "struct.h"
#include <limits>

//Is used to anser the question: Have we to "escort" point into next triangulation?
bool bernoulli_chance()
{
	if (std::rand() % 10)
		return false;
	return false;//true
}

//Return two points of triangle in pair and "ot", if third is set
std::pair<std::pair<int, int>, int> take_other_two_and_ot(triangle &t, int &v)
{
	if (v == t.p1) return std::pair<std::pair<int, int>, int> (std::pair<int, int> (t.p2, t.p3), t.ot1);
	if (v == t.p2) return std::pair<std::pair<int, int>, int> (std::pair<int, int> (t.p3, t.p1), t.ot2);
	return std::pair<std::pair<int, int>, int> (std::pair<int, int> (t.p1, t.p2), t.ot3);
}

//Return two points of triangle in pair if third is set
std::pair<int, int> take_other_two(triangle &t, int &v)
{
	if (v == t.p1) return std::pair<int, int> (t.p2, t.p3);
	if (v == t.p2) return std::pair<int, int> (t.p3, t.p1);
	return std::pair<int, int> (t.p1, t.p2);
}

//Return third point of triangle by other two
int take_last(triangle &t, int &v1, int &v2)
{
	if (v1 == t.p1)
		if (v2 == t.p2)	return t.p3;
		if (v2 == t.p3)	return t.p2;
	if (v1 == t.p2)
		if (v2 == t.p1)	return t.p3;
		if (v2 == t.p3)	return t.p1;
	if (v2 == t.p1)	return t.p2;
	return t.p1;
}

//Checks on the abstract one of points. If it is true - set other two by clockwise
bool is_non_natural(triangle &t, int &v1, int &v2)
{
	if (!t.p1 || !t.p2 || !t.p3)
	{
		if (!t.p1) {v1 = t.p2; v2 = t.p3;}
		if (!t.p2) {v1 = t.p3; v2 = t.p1;}
		if (!t.p3) {v1 = t.p1; v2 = t.p2;}
		return true;
	}
	return false;
}

//Checks on intersection by segment and opposite side of triangle
bool line_pass_triangle(DTriangle &DT, const vertex &q, int v, triangle &t)
{
	std::pair<int, int> opposite = take_other_two(t, v);
	return intersection(DT.set_points[v], q, DT.set_points[opposite.first], DT.set_points[opposite.second]);
}

//Checks on overlapping of segment and a side of triangle
bool check_vertexes_on_line(DTriangle &DT, const vertex &q, int v, triangle &t)
{
	std::pair<int, int> opposite = take_other_two(t, v);
	return rightTurn(DT.set_points[v], q, DT.set_points[opposite.first])  == 0
		|| rightTurn(DT.set_points[v], q, DT.set_points[opposite.second]) == 0;
}

//Return distance between two points
double range(const vertex &from, const vertex &to)
{
	if (!to.natural || !from.natural)
		return std::numeric_limits<double>::max();
	return std::sqrt((from.x - to.x)*(from.x - to.x) + (from.y - to.y)*(from.y - to.y));
}

//Used to compare: which point ("q" è "to") is farther away from "from"?
	//1  - "q" farther
	//0  - "to" == "q"
	//-1 - "q" closer
int how_far(const vertex &from, const vertex &q, const vertex &to)
{
	double dist = range(from, q) - range(from, to);
	return (int) (dist / std::abs(dist));
}

//Used to check: Is point "v" lying on segment by other two points?
bool is_on_segment(const vertex &v1, const vertex &v2, const vertex &v)
{
	if (rightTurn(v1, v2, v) == 0 && range(v1, v) < range(v1, v2) && range(v2, v) < range(v1, v2))
		return true;
	return false;
}

//Selection and comparing overlapping segment and side of triangle
std::pair<int, int> compare_vertex_on_line(DTriangle &DT, const vertex &q, int v, triangle &t)
{
	std::pair<int, int> opposite = take_other_two(t, v);
	if (rightTurn(DT.set_points[v], q, DT.set_points[opposite.first]) == 0)
		return std::pair<int, int> (how_far(DT.set_points[v], q, DT.set_points[opposite.first]), opposite.first);
	return std::pair<int, int> (how_far(DT.set_points[v], q, DT.set_points[opposite.second]), opposite.second);
}

//Checks on if point "q" is lying on opposite side of triangle
bool point_on_opposite_segment(DTriangle &DT, const vertex &q, int v, triangle &t)
{
	std::pair<int, int> opposite = take_other_two(t, v);
	return rightTurn(DT.set_points[opposite.first], DT.set_points[opposite.second], q) == 0;
}

//Checks on that segment is between two sides of triangle
int line_in_triangle(DTriangle &DT, const vertex &q, int v, triangle &t)
{
	std::pair<int, int> opposite = take_other_two(t, v);
	return rightTurn(DT.set_points[v], q, DT.set_points[opposite.first])
		 * rightTurn(DT.set_points[v], q, DT.set_points[opposite.second]);
};

//Checs on if point belong to triangle
bool vertex_in_triangle(DTriangle &DT, const vertex &q, triangle &t)
{
	return rightTurn(q, DT.set_points[t.p1], DT.set_points[t.p2]) == rightTurn(q, DT.set_points[t.p2], DT.set_points[t.p3]) 
		&& rightTurn(q, DT.set_points[t.p2], DT.set_points[t.p3]) == rightTurn(q, DT.set_points[t.p3], DT.set_points[t.p1]);
};

//return min of three variables
double minimal_range(double &d1, double &d2, double &d3)
{
	if (d1 == -1)		d1 = d2 + d3;
	else if (d2 == -1)	d2 = d1 + d3;
	else if (d3 == -1)	d3 = d1 + d2;
	double d = std::min(std::min(d1, d2), d3);
	return d;
}

//Checks on what neighbours we have to include in test by distance from "q" to every point in triangle
std::pair<int, int> magic_4_case_in_phase_3(triangle &t, const double &r1, const double &r2, const double &r3)
{
	int counter = 0, nearest_ind;
	if (r1 < r2)
	{
		counter += 2;
		nearest_ind = t.p1;
		if (r1 < r3)
			counter += 3;
		else if (r3 < r1)
		{
			counter += 1;
			nearest_ind = t.p3;
		}
		else
			counter += 4;
	}
	else if (r1 == r2)
	{
		nearest_ind = t.p3;
		counter += 3;
		if (r3 >= r1)
		{
			counter += 3;
			nearest_ind = t.p1;
		}
	}
	else
	{
		nearest_ind = t.p2;
		counter += 1;
		if (r2 < r3)
			counter += 3;
		else if (r3 < r2)
		{
			counter += 2;
			nearest_ind = t.p3;
		}
		else
			counter += 5;
	}
	return std::pair<int, int> (counter, nearest_ind);
}

//Flip edge return much information to simplification working with coordinates
void flip(DTriangle &DT, int t1, int t2, int v0, int v1, int ov, int v2)
{
	//Keep in mind, what neghbours in clockwise our triangles have
	int ot_1, ot_2, ot_3, ot_4;
	if (DT.set_triangles[t1].ot1 == t2)			{ot_4 = DT.set_triangles[t1].ot2; ot_1 = DT.set_triangles[t1].ot3;}
	else if (DT.set_triangles[t1].ot2 == t2)	{ot_4 = DT.set_triangles[t1].ot3; ot_1 = DT.set_triangles[t1].ot1;}
	else										{ot_4 = DT.set_triangles[t1].ot1; ot_1 = DT.set_triangles[t1].ot2;}
	if (DT.set_triangles[t2].ot1 == t1)			{ot_2 = DT.set_triangles[t2].ot2; ot_3 = DT.set_triangles[t2].ot3;}
	else if (DT.set_triangles[t2].ot2 == t1)	{ot_2 = DT.set_triangles[t2].ot3; ot_3 = DT.set_triangles[t2].ot1;}
	else										{ot_2 = DT.set_triangles[t2].ot1; ot_3 = DT.set_triangles[t2].ot2;}
	//Swap triangles, doing "right" neighbours
	DT.set_triangles[t1] = triangle(v0, v1, ov, ot_2, t2, ot_1);
	DT.set_triangles[t2] = triangle(ov, v2, v0, ot_4, t1, ot_3);
	//Refresh neighbours link on opposite triangle
	if (DT.set_triangles[ot_2].ot1 == t2)		DT.set_triangles[ot_2].ot1 = t1;
	else if (DT.set_triangles[ot_2].ot2 == t2)	DT.set_triangles[ot_2].ot2 = t1;
	else										DT.set_triangles[ot_2].ot3 = t1;
	if (DT.set_triangles[ot_4].ot1 == t1)		DT.set_triangles[ot_4].ot1 = t2;
	else if (DT.set_triangles[ot_4].ot2 == t1)	DT.set_triangles[ot_4].ot2 = t2;
	else										DT.set_triangles[ot_4].ot3 = t2;
	//Now add a neighbour to two linked points
	DT.set_points[v0].triangles_id.push_back(t2);
	DT.set_points[ov].triangles_id.push_back(t1);
	//And delete from other two
	for (size_t i = 0; i < DT.set_points[v1].triangles_id.size(); i++)
		if (DT.set_points[v1].triangles_id[i] == t2)
		{
			if (i != DT.set_points[v1].triangles_id.size() - 1)
				DT.set_points[v1].triangles_id[i] = DT.set_points[v1].triangles_id[DT.set_points[v1].triangles_id.size() - 1];
			DT.set_points[v1].triangles_id.pop_back();
			break;
		}
	for (size_t i = 0; i < DT.set_points[v2].triangles_id.size(); i++)
		if (DT.set_points[v2].triangles_id[i] == t1)
		{
			if (i != DT.set_points[v2].triangles_id.size() - 1)
				DT.set_points[v2].triangles_id[i] = DT.set_points[v2].triangles_id[DT.set_points[v2].triangles_id.size() - 1];
			DT.set_points[v2].triangles_id.pop_back();
			break;
		}
	//return our changed struct
	return;
}

//Count a matrix for answering the question: is point belong to circumcircle?
double delaunay_factor(const vertex &v, const vertex &v1, const vertex &v2, const vertex &v3)
{
	if (!v.natural || !v1.natural || !v2.natural || !v3.natural )
		return 0;
	double a = v1.x*(v2.y - v3.y) + v2.x*(v3.y - v1.y) + v3.x*(v1.y - v2.y);
	double sign_a = a / std::abs(a);
	double v_1 = v1.x * v1.x + v1.y * v1.y;
	double v_2 = v2.x * v2.x + v2.y * v2.y;
	double v_3 = v3.x * v3.x + v3.y * v3.y;
	double b = v_1*(v2.y - v3.y) + v_2*(v3.y - v1.y) + v_3*(v1.y - v2.y);
	double c = v_1*(v2.x - v3.x) + v_2*(v3.x - v1.x) + v_3*(v1.x - v2.x);
	double d = v_1*(v2.x*v3.y - v3.x*v2.y) + v_2*(v3.x*v1.y - v1.x*v3.y) + v_3*(v1.x*v2.y - v2.x*v1.y);

	return (a*(v.x*v.x + v.y*v.y) - b*v.x + c*v.y - d)*sign_a;
//	    |x1, y1, n1|	    |x1^2+y1^2, y1, n1|	    |x1^2+y1^2, x1, n1|	    |x1^2+y1^2, x1, y1|
//	a =	|x2, y2, n2|	b = |x2^2+y2^2, y2, n2|	c =	|x2^2+y2^2, x2, n2|	d = |x2^2+y2^2, x2, y2|
//	    |x3, y3, n3|	    |x3^2+y3^2, y3, n3|	    |x3^2+y3^2, x3, n3|	    |x3^2+y3^2, x3, y3|
//	point with coordinates (õ, ó) considered in circle is equal below is right
//			a*(x^2 + y^2) - b*x + c*y - d  <  0
}