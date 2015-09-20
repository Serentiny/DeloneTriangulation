#include "struct.h"
#include <limits>

//Используется для ответа на вопрос: надо ли "сопровождать" точку в следующую триангуляцию?
bool bernoulli_chance()
{
	if (std::rand() % 10)
		return false;
	return false;//true
}

//Возвращает две вершины треугольника в паре и "ot", при заданной третьей
std::pair<std::pair<int, int>, int> take_other_two_and_ot(triangle &t, int &v)
{
	if (v == t.p1) return std::pair<std::pair<int, int>, int> (std::pair<int, int> (t.p2, t.p3), t.ot1);
	if (v == t.p2) return std::pair<std::pair<int, int>, int> (std::pair<int, int> (t.p3, t.p1), t.ot2);
	return std::pair<std::pair<int, int>, int> (std::pair<int, int> (t.p1, t.p2), t.ot3);
}

//Возвращает две вершины треугольника в паре, при заданной третьей
std::pair<int, int> take_other_two(triangle &t, int &v)
{
	if (v == t.p1) return std::pair<int, int> (t.p2, t.p3);
	if (v == t.p2) return std::pair<int, int> (t.p3, t.p1);
	return std::pair<int, int> (t.p1, t.p2);
}

//Возвращает третью вершину треугольника по заданным остальным двум
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

//Проверяет на абстрактность одной из точек. Если верно, то выдаем остальные две вершины в обходе часовой стрелки
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

//Проверяет на пересечение отрезком противоположной стороны треугольника
bool line_pass_triangle(DTriangle &DT, const vertex &q, int v, triangle &t)
{
	std::pair<int, int> opposite = take_other_two(t, v);
	return intersection(DT.set_points[v], q, DT.set_points[opposite.first], DT.set_points[opposite.second]);
}

//Проверяет совпадения отрезка и стороны треугольника
bool check_vertexes_on_line(DTriangle &DT, const vertex &q, int v, triangle &t)
{
	std::pair<int, int> opposite = take_other_two(t, v);
	return rightTurn(DT.set_points[v], q, DT.set_points[opposite.first])  == 0
		|| rightTurn(DT.set_points[v], q, DT.set_points[opposite.second]) == 0;
}

//Возвращает расстояние между двумя точками
double range(const vertex &from, const vertex &to)
{
	if (!to.natural || !from.natural)
		return std::numeric_limits<double>::max();
	return std::sqrt((from.x - to.x)*(from.x - to.x) + (from.y - to.y)*(from.y - to.y));
}

//Используется для сравнения: какая точка из "q" и "to" дальше от "from"?
	//1  - "q" дальше
	//0  - "to" == "q"
	//-1 - "q" ближе
int how_far(const vertex &from, const vertex &q, const vertex &to)
{
	double dist = range(from, q) - range(from, to);
	return (int) (dist / std::abs(dist));
}

//Используется для проверки: лежит ли точка "v" на отрезке из двух других точек?
bool is_on_segment(const vertex &v1, const vertex &v2, const vertex &v)
{
	if (rightTurn(v1, v2, v) == 0 && range(v1, v) < range(v1, v2) && range(v2, v) < range(v1, v2))
		return true;
	return false;
}

//выбор и сравнение совпадающих отрезка и стороны треугольника
std::pair<int, int> compare_vertex_on_line(DTriangle &DT, const vertex &q, int v, triangle &t)
{
	std::pair<int, int> opposite = take_other_two(t, v);
	if (rightTurn(DT.set_points[v], q, DT.set_points[opposite.first]) == 0)
		return std::pair<int, int> (how_far(DT.set_points[v], q, DT.set_points[opposite.first]), opposite.first);
	return std::pair<int, int> (how_far(DT.set_points[v], q, DT.set_points[opposite.second]), opposite.second);
}

//проверка на присутствие точки q на противоположной стороне треугольника
bool point_on_opposite_segment(DTriangle &DT, const vertex &q, int v, triangle &t)
{
	std::pair<int, int> opposite = take_other_two(t, v);
	return rightTurn(DT.set_points[opposite.first], DT.set_points[opposite.second], q) == 0;
}

//проверка нахождения отрезка между сторонами треугольника
int line_in_triangle(DTriangle &DT, const vertex &q, int v, triangle &t)
{
	std::pair<int, int> opposite = take_other_two(t, v);
	return rightTurn(DT.set_points[v], q, DT.set_points[opposite.first])
		 * rightTurn(DT.set_points[v], q, DT.set_points[opposite.second]);
};

//проверка на принадлежание точки треугольнику
bool vertex_in_triangle(DTriangle &DT, const vertex &q, triangle &t)
{
	return rightTurn(q, DT.set_points[t.p1], DT.set_points[t.p2]) == rightTurn(q, DT.set_points[t.p2], DT.set_points[t.p3]) 
		&& rightTurn(q, DT.set_points[t.p2], DT.set_points[t.p3]) == rightTurn(q, DT.set_points[t.p3], DT.set_points[t.p1]);
};

//из трех расстояний взять минимум
double minimal_range(double &d1, double &d2, double &d3)
{
	if (d1 == -1)		d1 = d2 + d3;
	else if (d2 == -1)	d2 = d1 + d3;
	else if (d3 == -1)	d3 = d1 + d2;
	double d = std::min(std::min(d1, d2), d3);
	return d;
}

//по расстояниям от точки "q" до каждой точки в треугольнике узнаем, каких соседей включать в проверку
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

//функция разворота ребра, получаем очень много информации, для упрощения игр с координатами
void flip(DTriangle &DT, int t1, int t2, int v0, int v1, int ov, int v2)
{
	//запоминаем, какие соседи, по часовой стрелке, у наших треугольников есть
	int ot_1, ot_2, ot_3, ot_4;
	if (DT.set_triangles[t1].ot1 == t2)			{ot_4 = DT.set_triangles[t1].ot2; ot_1 = DT.set_triangles[t1].ot3;}
	else if (DT.set_triangles[t1].ot2 == t2)	{ot_4 = DT.set_triangles[t1].ot3; ot_1 = DT.set_triangles[t1].ot1;}
	else										{ot_4 = DT.set_triangles[t1].ot1; ot_1 = DT.set_triangles[t1].ot2;}
	if (DT.set_triangles[t2].ot1 == t1)			{ot_2 = DT.set_triangles[t2].ot2; ot_3 = DT.set_triangles[t2].ot3;}
	else if (DT.set_triangles[t2].ot2 == t1)	{ot_2 = DT.set_triangles[t2].ot3; ot_3 = DT.set_triangles[t2].ot1;}
	else										{ot_2 = DT.set_triangles[t2].ot1; ot_3 = DT.set_triangles[t2].ot2;}
	//меняем треугольники местами, прикручиваем правильных соседей
	DT.set_triangles[t1] = triangle(v0, v1, ov, ot_2, t2, ot_1);
	DT.set_triangles[t2] = triangle(ov, v2, v0, ot_4, t1, ot_3);
	//у двух соседей обновляем ссылки на противоположный треугольник
	if (DT.set_triangles[ot_2].ot1 == t2)		DT.set_triangles[ot_2].ot1 = t1;
	else if (DT.set_triangles[ot_2].ot2 == t2)	DT.set_triangles[ot_2].ot2 = t1;
	else										DT.set_triangles[ot_2].ot3 = t1;
	if (DT.set_triangles[ot_4].ot1 == t1)		DT.set_triangles[ot_4].ot1 = t2;
	else if (DT.set_triangles[ot_4].ot2 == t1)	DT.set_triangles[ot_4].ot2 = t2;
	else										DT.set_triangles[ot_4].ot3 = t2;
	//теперь у двух смежных вершин добавляем соседа
	DT.set_points[v0].triangles_id.push_back(t2);
	DT.set_points[ov].triangles_id.push_back(t1);
	//а у двух отнимаем
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
	//наконец полностью выполненный флип, возвращаем измененную структуру
	return;
}

//подсчет матрицы, для ответа на вопрос: входит ли точка в описанную окружность?
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
//		|x1, y1, n1|		|x1^2+y1^2, y1, n1|		|x1^2+y1^2, x1, n1|		|x1^2+y1^2, x1, y1|
//	a =	|x2, y2, n2|	b = |x2^2+y2^2, y2, n2|	c =	|x2^2+y2^2, x2, n2|	d = |x2^2+y2^2, x2, y2|
//		|x3, y3, n3|		|x3^2+y3^2, y3, n3|		|x3^2+y3^2, x3, n3|		|x3^2+y3^2, x3, y3|
//	точка с координатами (х, у) считается внутри окружности, если выполняется неравенство ниже
//			a*(x^2 + y^2) - b*x + c*y - d  <  0
}