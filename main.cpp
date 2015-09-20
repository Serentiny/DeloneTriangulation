#include <ctime>
#include "struct.h"
#include "triangulation.h"

int main()
{
	std::ifstream in ("triangle.in");
	std::ofstream out("triangle.txt");

	//use real time as a seed for random generator
	std::srand((size_t) std::time(0));
	std::vector<DTriangle> DT;
	DT.push_back(DTriangle());

	int count;
	in >> count;
	std::vector<vertex> set(count);
	for (int i = 0; i < count; i++)
		in >> set[i];
	for (size_t i = 0; i < set.size(); i++)
		add_point_to_DT(set[i], DT);
	count = 0;
	for (size_t i = 0; i < DT[0].set_triangles.size(); i++)
		if (DT[0].set_triangles[i].p1 && DT[0].set_triangles[i].p2 && DT[0].set_triangles[i].p3)
			count++;
	out << count << '\n';
	for (size_t i = 0; i < DT[0].set_triangles.size(); i++)
		if (DT[0].set_triangles[i].p1 && DT[0].set_triangles[i].p2 && DT[0].set_triangles[i].p3)
			out << DT[0].set_points[DT[0].set_triangles[i].p1] << " 	"
				<< DT[0].set_points[DT[0].set_triangles[i].p2] << " 	"
				<< DT[0].set_points[DT[0].set_triangles[i].p3] << '\n';
}