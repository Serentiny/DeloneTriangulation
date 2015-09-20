#ifndef STRUCT_H
#define STRUCT_H
#include <vector>
#include <fstream>

struct vertex
{
	double x, y, natural;	//координаты и знак, абстрактная ли эта точка
	int link;				//ссылка на точно такую же вершину в предыдущей триангуляции
	std::vector<int> triangles_id;

	vertex() :
		x(0),
		y(0),
		natural(0),
		link(0),
		triangles_id()
	{};
	vertex(double &temp_x, double &temp_y) :
		x(temp_x),
		y(temp_y),
		natural(1),
		link(-1),
		triangles_id()
	{};
	vertex(const vertex &v) :
		x(v.x),
		y(v.y),
		natural(v.natural),
		link(v.link),
		triangles_id(v.triangles_id)
	{};

	bool operator== (const vertex &v)
	{
		return (natural == v.natural) && ((x == v.x) && (y == v.y));
	};
	bool operator!= (const vertex &v)
	{
		return !(*this == v);
	};

	friend std::istream& operator >>(std::istream& input, vertex &v)
	{
		input >> v.x;
		input >> v.y;
		v.natural = 1;
		v.link = -1;
		return input;
	};
	friend std::ostream& operator <<(std::ostream& output, const vertex &v)
	{
		output << v.x << " " << v.y;
		return output;
	};
};

struct triangle
{
	//номер вершины в массиве
	//стараемся поддерживать инвариант: точки лежат по ходу часовой стрелки
	int p1, p2, p3;

	//номер треугольника в массиве(который лежит напротив одноименной точки)
	int ot1, ot2, ot3;

	triangle() :
		p1(-1),
		p2(-1),
		p3(-1),
		ot1(-1),
		ot2(-1),
		ot3(-1)
	{};
	triangle(const int point1, const int point2, const int point3) :
		p1(point1),
		p2(point2),
		p3(point3),
		ot1(-1),
		ot2(-1),
		ot3(-1)
	{};
	triangle(const int point1, const int point2, const int point3, const int v1, const int v2, const int v3) :
		p1(point1),
		p2(point2),
		p3(point3),
		ot1(v1),
		ot2(v2),
		ot3(v3)
	{};

	bool operator== (const triangle &t)
	{
		return (p1 == t.p1) && (p2 == t.p2) && (p3 == t.p3)
			|| (p1 == t.p2) && (p2 == t.p3) && (p3 == t.p1)
			|| (p1 == t.p3) && (p2 == t.p1) && (p3 == t.p2);
	}
	bool operator!= (const triangle &t)
	{
		return !(*this == t);
	}
};

struct DTriangle
{
	std::vector<vertex>   set_points;		//набор всех точек
	std::vector<triangle> set_triangles;	//набор всех треугольников

	DTriangle()
	{
		set_points.push_back(vertex());
	};
};

#endif