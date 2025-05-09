#include <iostream>
#include "PolyProcessor.h"

int main()
{
    // Define the vertices of a polygon (e.g., a square)
    TColgp_Array1OfPnt vertices(1, 4);
    vertices(1) = gp_Pnt(0.0, 0.0, 0.0);
    vertices(2) = gp_Pnt(1.0, 0.0, 0.0);
    vertices(3) = gp_Pnt(1.0, 1.0, 0.0);
    vertices(4) = gp_Pnt(0.0, 1.0, 0.0);

    // Define the triangle indices
    TColStd_Array1OfInteger indices(1, 6); // 2 triangles * 3 indices
    indices(1) = 1;
    indices(2) = 2;
    indices(3) = 3; // Triangle 1
    indices(4) = 1;
    indices(5) = 3;
    indices(6) = 4; // Triangle 2

    // Create the Poly_Triangulation object
    Handle(Poly_Triangulation) triangulation = PolyProcessor::CreateTriangulation(vertices, indices);

    // Output the triangulation data
    std::cout << "Number of Nodes: " << triangulation->NbNodes() << std::endl;
    std::cout << "Number of Triangles: " << triangulation->NbTriangles() << std::endl;

    // Print the vertices
    std::cout << "Vertices:" << std::endl;
    for (int i = 1; i <= triangulation->NbNodes(); ++i)
    {
        gp_Pnt p = triangulation->Node(i);
        std::cout << "  Vertex " << i << ": (" << p.X() << ", " << p.Y() << ", " << p.Z() << ")" << std::endl;
    }

    // Print the triangles
    std::cout << "Triangles:" << std::endl;
    for (int i = 1; i <= triangulation->NbTriangles(); ++i)
    {
        Standard_Integer n1, n2, n3;
        triangulation->Triangle(i).Get(n1, n2, n3);
        std::cout << "  Triangle " << i << ": (" << n1 << ", " << n2 << ", " << n3 << ")" << std::endl;
    }

    return 0;
}