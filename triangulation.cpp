#include "triangulation.h"
#include "functions.h"

void delaunay_test(DTriangle &DT, std::vector<std::pair<int, int>> &queue)
{
	for (size_t i = 0; i < queue.size(); i++)
	{
		triangle &t = DT.set_triangles[queue[i].first];
		int v1, v2, ov, ot;
		if (t.p1 == queue[i].second)		{ov = take_last(DT.set_triangles[t.ot1], t.p2, t.p3); ot = t.ot1; v1 = t.p2; v2 = t.p3;}
		else if (t.p1 == queue[i].second)	{ov = take_last(DT.set_triangles[t.ot2], t.p3, t.p1); ot = t.ot2; v1 = t.p3; v2 = t.p1;}
		else								{ov = take_last(DT.set_triangles[t.ot3], t.p1, t.p2); ot = t.ot3; v1 = t.p1; v2 = t.p2;}
		//������, �����, ���� �� ������ ���������� - ��������, � ���� ���� - ��������
		if (!v1 || !v2)
		{
			if (!v2)
			{
				if (rightTurn(DT.set_points[queue[i].second], DT.set_points[v1], DT.set_points[ov]) > 0)
				{
					flip(DT, queue[i].first, ot, queue[i].second, v1, ov, v2);
					queue.push_back(std::pair<int, int> (queue[i].first, queue[i].second));
					queue.push_back(std::pair<int, int> (ot, queue[i].second));
				}
				continue;
			}
			if (rightTurn(DT.set_points[ov], DT.set_points[v2], DT.set_points[queue[i].second]) > 0)
			{
				flip(DT, queue[i].first, ot, queue[i].second, v1, ov, v2);
				queue.push_back(std::pair<int, int> (queue[i].first, queue[i].second));
				queue.push_back(std::pair<int, int> (ot, queue[i].second));
			}
			continue;
		}
		//���� ��������� ����� ����� � ��������� ����������, �� ������ ����, � ������� ��� ����� ����� � ������� �� ��������
		if (delaunay_factor(DT.set_points[ov], DT.set_points[t.p1], DT.set_points[t.p2], DT.set_points[t.p3]) < 0)
		{
			flip(DT, queue[i].first, ot, queue[i].second, v1, ov, v2);
			queue.push_back(std::pair<int, int> (queue[i].first, queue[i].second));
			queue.push_back(std::pair<int, int> (ot, queue[i].second));
		}
	}
}

void add_point_to_edge(DTriangle &DT, const vertex &q, int t1, int t2, int vr, const int &link)
{
	int vu, vd, vl;
	auto tot = take_other_two(DT.set_triangles[t1], vr);
	vd = tot.first; vu = tot.second; vl = take_last(DT.set_triangles[t2], vu, vd);

	//����������, ����� ������, �� ������� �������, � ����� ������������� ����
	int ot_1, ot_2, ot_3, ot_4;
	if (DT.set_triangles[t1].ot1 == t2)			{ot_4 = DT.set_triangles[t1].ot2; ot_1 = DT.set_triangles[t1].ot3;}
	else if (DT.set_triangles[t1].ot2 == t2)	{ot_4 = DT.set_triangles[t1].ot3; ot_1 = DT.set_triangles[t1].ot1;}
	else										{ot_4 = DT.set_triangles[t1].ot1; ot_1 = DT.set_triangles[t1].ot2;}
	if (DT.set_triangles[t2].ot1 == t1)			{ot_2 = DT.set_triangles[t2].ot2; ot_3 = DT.set_triangles[t2].ot3;}
	else if (DT.set_triangles[t2].ot2 == t1)	{ot_2 = DT.set_triangles[t2].ot3; ot_3 = DT.set_triangles[t2].ot1;}
	else										{ot_2 = DT.set_triangles[t2].ot1; ot_3 = DT.set_triangles[t2].ot2;}
	//����������� ���������� �������������, ����� ���� ���� ����� � �����������
	int DTst_size = DT.set_triangles.size();
	//������/��������� ������������, ������������ ���������� �������
	DT.set_triangles[t2] = triangle(DT.set_points.size(), vl, vu, ot_3, t1, DTst_size + 1);
	DT.set_triangles[t1] = triangle(DT.set_points.size(), vu, vr, ot_4, DTst_size, t2);
	DT.set_triangles.push_back(triangle(DT.set_points.size(), vr, vd, ot_1, DTst_size + 1, t1));
	DT.set_triangles.push_back(triangle(DT.set_points.size(), vd, vl, ot_2, t2, DTst_size));
	//��� ��� ����� �� ��������� �� ����������� ������� - �������� ��
	DT.set_points.push_back(vertex(q));
	//������ ������� ������� ����� ������� ��������, � ���������, ������� ������������
	DT.set_points.back().triangles_id.push_back(t1);
	DT.set_points.back().triangles_id.push_back(t2);
	DT.set_points.back().triangles_id.push_back(DTst_size);
	DT.set_points.back().triangles_id.push_back(DTst_size + 1);
	DT.set_points.back().link = link;
	DT.set_points[vr].triangles_id.push_back(DTst_size);
	DT.set_points[vl].triangles_id.push_back(DTst_size + 1);
	for (size_t i = 0; i < DT.set_points[vd].triangles_id.size(); i++)
	{
		if (DT.set_points[vd].triangles_id[i] == t1)
			DT.set_points[vd].triangles_id[i] = DTst_size;
		if (DT.set_points[vd].triangles_id[i] == t2)
			DT.set_points[vd].triangles_id[i] = DTst_size + 1;
	}
	//�� ������ ����������� ��������� ��� ������ ������. ���� ������� � �� ������ �� ��������
	if (DT.set_triangles[ot_1].ot1 == t1)		DT.set_triangles[ot_1].ot1 = DTst_size;
	else if (DT.set_triangles[ot_1].ot2 == t1)	DT.set_triangles[ot_1].ot2 = DTst_size;
	else										DT.set_triangles[ot_1].ot3 = DTst_size;
	if (DT.set_triangles[ot_2].ot1 == t2)		DT.set_triangles[ot_2].ot1 = DTst_size + 1;
	else if (DT.set_triangles[ot_2].ot2 == t2)	DT.set_triangles[ot_2].ot2 = DTst_size + 1;
	else										DT.set_triangles[ot_2].ot3 = DTst_size + 1;
	//�������� ������� �� ���: ����� ������������ �� ��������, � ����� �������, ����� �������� ������� �� ��������
	std::vector<std::pair<int, int>> queue;
	queue.push_back(std::pair<int, int> (t1, DT.set_points.size() - 1));
	queue.push_back(std::pair<int, int> (t2, DT.set_points.size() - 1));
	queue.push_back(std::pair<int, int> (DTst_size, DT.set_points.size() - 1));
	queue.push_back(std::pair<int, int> (DTst_size + 1, DT.set_points.size() - 1));
	//������ ������ ������������� ��� ����� �� ������� ������
	delaunay_test(DT, queue);
};

//���������� ����� � ����������� �� ������
void add_point_to_face(DTriangle &DT, const vertex &q, int t_ind, const int &link)
{
	//�������� �� ��������� � ������, ������� � add_point_to_edge
	if (!DT.set_triangles[t_ind].p1 || !DT.set_triangles[t_ind].p2 || !DT.set_triangles[t_ind].p3)
	{
		int v1, v2, v3 = 0, ot;
		auto tot = take_other_two_and_ot(DT.set_triangles[t_ind], v3);
		v1 = tot.first.first; v2 = tot.first.first; ot = tot.second;
		if (is_on_segment(DT.set_points[v1], DT.set_points[v2], q))
			return add_point_to_edge(DT, q, t_ind, ot, v3, link);
	}
	if (is_on_segment(DT.set_points[DT.set_triangles[t_ind].p1], DT.set_points[DT.set_triangles[t_ind].p2], q))
		return add_point_to_edge(DT, q, t_ind, DT.set_triangles[t_ind].ot3, DT.set_triangles[t_ind].p3, link);
	if (is_on_segment(DT.set_points[DT.set_triangles[t_ind].p2], DT.set_points[DT.set_triangles[t_ind].p3], q))
		return add_point_to_edge(DT, q, t_ind, DT.set_triangles[t_ind].ot1, DT.set_triangles[t_ind].p1, link);
	if (is_on_segment(DT.set_points[DT.set_triangles[t_ind].p3], DT.set_points[DT.set_triangles[t_ind].p1], q))
		return add_point_to_edge(DT, q, t_ind, DT.set_triangles[t_ind].ot2, DT.set_triangles[t_ind].p2, link);
	//���, �� ��������� ������ � �����������
	//����������� ���������� �������������, ����� ���� ���� ����� � �����������
	int DTst_size = DT.set_triangles.size();
	//������ ����� ������������, ��� �������
	triangle t = DT.set_triangles[t_ind];
	//�� ������ ����� ������� ����� �����������
	DT.set_triangles[t_ind] = triangle(t.p1, t.p2, DT.set_points.size(), DTst_size, DTst_size + 1, t.ot3);
	//����� ���� ��������� ���������� ���
	DT.set_triangles.push_back(triangle(t.p2, t.p3, DT.set_points.size(), DTst_size + 1, t_ind, t.ot1));
	DT.set_triangles.push_back(triangle(t.p3, t.p1, DT.set_points.size(), t_ind, DTst_size, t.ot2));
	//��� ��� ����� �� ��������� �� ����������� ������� - �������� ��
	DT.set_points.push_back(q);
	//������� ������� ����� �����
	DT.set_points.back().triangles_id.push_back(t_ind);
	DT.set_points.back().triangles_id.push_back(DTst_size);
	DT.set_points.back().triangles_id.push_back(DTst_size + 1);
	DT.set_points.back().link = link;
	//��� �� ���� ����� ������� �������� ������ ������
	DT.set_points[t.p1].triangles_id.push_back(DTst_size + 1);
	DT.set_points[t.p2].triangles_id.push_back(DTst_size);
	//� ��� ������ �������� ������� � ��������� ���� �����
	DT.set_points[t.p3].triangles_id.push_back(DTst_size + 1);
	for (size_t i = 0; i < DT.set_points[t.p3].triangles_id.size() - 1; i++)
		if (DT.set_points[t.p3].triangles_id[i] == t_ind)
		{
			DT.set_points[t.p3].triangles_id[i] = DTst_size;
			break;
		}
	//�� ������ ����������� ��������� ��� ������ ������. ������ ������� � ��� ������ �� ��������
	if (DT.set_triangles[t.ot1].p1 == t.p2)			DT.set_triangles[t.ot1].ot2 = DTst_size;
	else if (DT.set_triangles[t.ot1].p2 == t.p2)	DT.set_triangles[t.ot1].ot3 = DTst_size;
	else											DT.set_triangles[t.ot1].ot1 = DTst_size;
	if (DT.set_triangles[t.ot2].p1 == t.p3)			DT.set_triangles[t.ot2].ot2 = DTst_size + 1;
	else if (DT.set_triangles[t.ot2].p2 == t.p3)	DT.set_triangles[t.ot2].ot3 = DTst_size + 1;
	else											DT.set_triangles[t.ot2].ot1 = DTst_size + 1;
	//�������� ������� �� ���: ����� ������������ �� ��������, � ����� �������, ����� �������� ������� �� ��������
	std::vector<std::pair<int, int>> queue;
	queue.push_back(std::pair<int, int> (t_ind, DT.set_points.size() - 1));
	queue.push_back(std::pair<int, int> (DTst_size, DT.set_points.size() - 1));
	queue.push_back(std::pair<int, int> (DTst_size + 1, DT.set_points.size() - 1));
	//������ ������ ������������� ��� ����� �� ������� ������
	delaunay_test(DT, queue);
};
//==============================================================================================================================================
std::pair<int, int> local_phase_1(DTriangle&, int, const vertex&);
std::pair<int, int> local_phase_2(DTriangle&, const vertex&, const vertex&, triangle&, int);
std::pair<int, int> local_phase_3(DTriangle&, const vertex&, int&);

std::pair<int, int> local_phase_1(DTriangle &DT, int v, const vertex &q)
{
	//�������� �������, �� ������� ������� ����������� � ������, ������������ � Q, ���� ������ ����������� � ����
	for (int i = 0; i < DT.set_points[v].triangles_id.size(); i++)
	{
		//������� ���������� ������ �������� ������������
		triangle &t = DT.set_triangles[DT.set_points[v].triangles_id[i]];
		int v1, v2;
		if (is_non_natural(t, v1, v2))
		{
			if (rightTurn(DT.set_points[v1], DT.set_points[v2], q) >= 0)
				return local_phase_3(DT, q, DT.set_points[v].triangles_id[i]);//����� ����� �� ������� ������������
			continue;//����� �� ����� �� ������� ������������
		}
		//� ������ ����� �����������
		if (line_pass_triangle(DT, q, v, t))
		{
			if (check_vertexes_on_line(DT, q, v, t))
			{
				std::pair<int, int> cvol = compare_vertex_on_line(DT, q, v, t);
				if (cvol.first == 0)
					return std::pair<int, int> (-1, -1);//����� ���������
				if (range(DT.set_points[cvol.second], q) < range(DT.set_points[v], q))
					return local_phase_1(DT, cvol.second, q);//������ �� ����� - phase #1
				continue;//������ �� �����, �� ��� ��� ������� - ��������� �����
			}
			else if (point_on_opposite_segment(DT, q, v, t))
				return local_phase_3(DT, q, i);//����� ����� - phase #3
			return local_phase_2(DT, DT.set_points[v], q, t, v);//����� ����������� - phase #2
		}
//������� vq �� ���������� ������� v1v2
		int lit = line_in_triangle(DT, q, v, t);
		auto tot = take_other_two(t, v);
		v1 = tot.first; v2 = tot.second;
		if (lit == -1)
		{
			if (vertex_in_triangle(DT, q, t))
				return local_phase_3(DT, q, i);//����� ����������� - phase #3
			continue;//������� ����� �� �������� ����� ���� �����������, ���� ������
		}
		if (lit == 0)
			if (rightTurn(DT.set_points[v], q, DT.set_points[v1]) == 0 && range(DT.set_points[v], q) > range(DT.set_points[v1], q)
			 || rightTurn(DT.set_points[v], q, DT.set_points[v2]) == 0 && range(DT.set_points[v], q) > range(DT.set_points[v2], q))
				return local_phase_3(DT, q, i);//����� �����, ���������, ��� ��� �� � ��������������� ������� - phase #3
		continue;//��� ����� ������ ������
	}
	return std::pair<int, int> (-1, -1);//�������� ��� Warning-�
};

std::pair<int, int> local_phase_2(DTriangle &DT, const vertex &from, const vertex &q, triangle &old_triangle, int v)
{
/*��� �������� ������ �����������, � �������, ����� �������� ������� �� ����������, �������� � ��������� �����������
  � ������� �������� 5 ��������: �� �������� � ������������, �� �������� �� �����, �� �������� ����� �����, 
  �� �������� �� �����, � �� �������� ����� �����*/
	int v1, v2, ov, tri_ind = 0;
	std::pair<std::pair<int, int>, int> v1v2ot = take_other_two_and_ot(old_triangle, v);
	v1 = v1v2ot.first.first;
	v2 = v1v2ot.first.second;
	triangle &new_triangle = DT.set_triangles[v1v2ot.second];
	ov = take_last(new_triangle, v1, v2);
	//�������� �� ������������� ������������, � ������� �� ������ �������, ���� ���, �� �� ��� � ��������
	if (!ov)
		local_phase_3(DT, q, v1v2ot.second);
	int inter_1, inter_2;
	inter_1 = intersection(from, q, DT.set_points[v1], DT.set_points[ov]);
	inter_2 = intersection(from, q, DT.set_points[v2], DT.set_points[ov]);

	if (!(inter_1 || inter_2))
		return local_phase_3(DT, q, v1v2ot.second);//�� �������� � ������������ - phase #3
	if (inter_1 && inter_2)
	{
		if (DT.set_points[ov] == q)
			return std::pair<int, int> (-1, -1);//�� �������� �� �����
		return local_phase_1(DT, ov, q);//�� �������� ����� �����
	}
	if (rightTurn(DT.set_points[ov], DT.set_points[v1], q) == 0
	 && range(DT.set_points[v1], q) < range(DT.set_points[v1], DT.set_points[ov])
	 && range(DT.set_points[ov], q) < range(DT.set_points[v1], DT.set_points[ov])
	 || rightTurn(DT.set_points[ov], DT.set_points[v2], q) == 0
	 && range(DT.set_points[v2], q) < range(DT.set_points[v2], DT.set_points[ov])
	 && range(DT.set_points[ov], q) < range(DT.set_points[v2], DT.set_points[ov]))
		return local_phase_3(DT, q, v1v2ot.second);//�� �������� �� ����� - phase #3
	if (inter_1)
		return local_phase_2(DT, from, q, new_triangle, v2);//� ���
	return local_phase_2(DT, from, q, new_triangle, v1);	//� ��� �� �������� ����� ����� - phase #2
}

std::pair<int, int> local_phase_3(DTriangle &DT, const vertex &q, int &index)
{
/*��� �������� �����������, � ������� (���� ����� � ���) ����� ������������ ����� � ����� q
  �������� �� ������ �����. ������� ��� ������ ������ ��������� ����� � ���� ������������,
  �������� �� ����� ���������� ���� ������ � ���� ������ �� ��� ���������� �����, � ��� ���
  NB ���� ������ ���� ���������� ����� �� ����������, �� �� ������� ������ ����*/
	triangle &t = DT.set_triangles[index];
	std::vector<std::pair<triangle&, int>> queue_ind;

	double r1 = range(q, DT.set_points[t.p1]);
	double r2 = range(q, DT.set_points[t.p2]);
	double r3 = range(q, DT.set_points[t.p3]);
	double min_range = minimal_range(r1, r2, r3);
	std::pair<int, int> cn = magic_4_case_in_phase_3(t, r1 ,r2, r3);
	int nearest_ind = cn.second;	//����� ��������� �������
	//���������� �����, �� �������� �������, ����� ������� �������� � ������� � ����� ������
	int counter = cn.first;
	switch (counter)
	{
		case 3:
		{
			queue_ind.push_back(std::pair<triangle&, int> (t, t.p1));
			queue_ind.push_back(std::pair<triangle&, int> (t, t.p2));
			break;
		}
		case 4:
		{
			queue_ind.push_back(std::pair<triangle&, int> (t, t.p1));
			queue_ind.push_back(std::pair<triangle&, int> (t, t.p3));
			break;
		}
		case 5:
		{
			queue_ind.push_back(std::pair<triangle&, int> (t, t.p2));
			queue_ind.push_back(std::pair<triangle&, int> (t, t.p3));
			break;
		}
		default:
		{
			queue_ind.push_back(std::pair<triangle&, int> (t, t.p1));
			queue_ind.push_back(std::pair<triangle&, int> (t, t.p2));
			queue_ind.push_back(std::pair<triangle&, int> (t, t.p3));
		}
	}
	//� ����� �� �������� ������� �� ��������� �������, ����� ������� ����� �� ������ ������, � ���������� �� ���
	for (size_t i = 0; i < queue_ind.size(); i++)
	{
		int v1, v2, ov, tri_ind = 0;
		std::pair<std::pair<int, int>, int> v1v2ot = take_other_two_and_ot(queue_ind[i].first, queue_ind[i].second);
		v1 = v1v2ot.first.first;
		v2 = v1v2ot.first.second;
		triangle &new_triangle = DT.set_triangles[v1v2ot.second];
		ov = take_last(new_triangle, v1, v2);
		if (range(q, DT.set_points[ov]) <= min_range)
		{
			min_range = range(q, DT.set_points[ov]);
			nearest_ind = ov;
			queue_ind.push_back(std::pair<triangle&, int> (new_triangle, v1));
			queue_ind.push_back(std::pair<triangle&, int> (new_triangle, v2));
		}
	}
	return std::pair<int, int> (nearest_ind, index);
}

//���������� ������������ �����, � �������������� �����������
std::pair<int, int> localization(const vertex &q, DTriangle &DT, int v)
{
	if (DT.set_points[v] == q)
		return std::pair<int, int> (-1, -1);
	return local_phase_1(DT, v, q);
};
//==============================================================================================================================================
void DT_initialization(DTriangle &DT, const vertex &vert, const int &link)
{
	DT.set_points.push_back(vert);
	DT.set_points[DT.set_points.size() - 1].link = link;
};

void DT_add_second_vertex(DTriangle &DT, const vertex &vert, int link)
{
	if (DT.set_points[DT.set_points.size() - 1] == vert)
		return;
	DT.set_points.push_back(vert);
	DT.set_points[DT.set_points.size() - 1].link = link;

	DT.set_triangles.push_back(triangle(0, 1, 2, 1, 1, 1));//������� ������ ����������� �� ���� �����, � ����� �����������
	DT.set_triangles.push_back(triangle(0, 2, 1, 0, 0, 0));//������� ������ ������� ����������� �� ��� �� ���� �����, � ����� �����������
	DT.set_points[0].triangles_id.push_back(0);
	DT.set_points[0].triangles_id.push_back(1);
	DT.set_points[1].triangles_id.push_back(0);
	DT.set_points[1].triangles_id.push_back(1);
	DT.set_points[2].triangles_id.push_back(0);
	DT.set_points[2].triangles_id.push_back(1);
};
//==============================================================================================================================================
void add_point_to_DT(const vertex &vert, std::vector<DTriangle> &DT)
{
	//��������� ������ �����
	if (DT[0].set_points.size() == 1)
	{
		size_t count = 0;
		DT_initialization(DT[count], vert, -1);
		while (bernoulli_chance())
		{
			count++;
			if (DT.size() <= count)
				DT.push_back(DTriangle());
			DT_initialization(DT[count], vert, DT[count - 1].set_points.size() - 1);
		}
		return;
	}
	//��������� ������ �����
	if (DT[0].set_points.size() == 2)
	{
		size_t count = 0;
		DT_add_second_vertex(DT[count], vert, -1);
		while (bernoulli_chance())
		{
			count++;
			if (DT.size() <= count)
			{
				DT.push_back(DTriangle());
				DT_initialization(DT[count], vert, DT[count - 1].set_points.size() - 1);
				continue;
			}
			DT_add_second_vertex(DT[count], vert, DT[count - 1].set_points.size() - 1);
		}
		return;
	}
	//������ ��������� ����� � �������������, � ������� �� ��������������, �� ������ ���� ������������
	std::vector<std::pair<int, int>> last_nearest(DT.size());
	last_nearest[DT.size() - 1].first = 1;
	for (size_t i = DT.size(); i > 0; i--)
	{
	//� ������ ������������ ���� ��������� ����� � �����������, ������� ���������� � ��������� ������������ ��� ���������
	//� ����� ����������� ������������(�� ����� ���� �����) ��� ��������� ����� ���������� ������ ����������
		std::pair<int, int> nearest;
		if (DT[i - 1].set_points.size() == 2)
		{
			last_nearest[i - 2].first = DT[i - 1].set_points[last_nearest[i - 1].first].link;
			continue;
		}
		nearest = localization(vert, DT[i - 1], last_nearest[i - 1].first);
		if (nearest.first == -1)
			return;
		if (i != 1)
			last_nearest[i - 2].first = DT[i - 1].set_points[nearest.first].link;
		last_nearest[i - 1].second = nearest.second;
	}
	//������� �� �������-�� ���� �����
	size_t count = 0;
	add_point_to_face(DT[count], vert, last_nearest[count].second, -1);
	while (bernoulli_chance())
	{
		count++;
		if (DT.size() <= count)
		{
			DT.push_back(DTriangle());
			DT_initialization(DT[count], vert, DT[count - 1].set_points.size() - 1);
			continue;
		}
		if (DT[count].set_points.size() == 2)
		{
			DT_add_second_vertex(DT[count], vert, DT[count - 1].set_points.size() - 1);
			continue;
		}
		add_point_to_face(DT[count], vert, last_nearest[count].second, DT[count - 1].set_points.size() - 1);
	}
	return;
};