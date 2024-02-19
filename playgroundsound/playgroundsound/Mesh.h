#include <vector>
namespace Playground {
	struct Vertex
	{
		float x, y, z;
	};
	struct Mesh
	{
		std::vector<Vertex> vertices;
	};
}