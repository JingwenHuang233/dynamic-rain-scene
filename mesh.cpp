#include "mesh.h"
#include <fstream>
#include <string>
#include <limits>
#include <cfloat>
#include <algorithm>
#include "plane.h"


// Consider a triangle to intersect a ray if the ray intersects the plane of the
// triangle with barycentric weights in [-weight_tolerance, 1+weight_tolerance]
static const double weight_tolerance = 1e-4;

// Read in a mesh from an obj file.  Populates the bounding box and registers
// one part per triangle (by setting number_parts).
void Mesh::Read_Obj(const char* file)
{
    std::ifstream fin(file);
    if(!fin)
    {
        exit(EXIT_FAILURE);
    }
    std::string line;
    ivec3 e;
    vec3 v;
    box.Make_Empty();
    while(fin)
    {
        getline(fin,line);

        if(sscanf(line.c_str(), "v %lg %lg %lg", &v[0], &v[1], &v[2]) == 3)
        {
            vertices.push_back(v);
            box.Include_Point(v);
        }

        if(sscanf(line.c_str(), "f %d %d %d", &e[0], &e[1], &e[2]) == 3)
        {
            for(int i=0;i<3;i++) e[i]--;
            triangles.push_back(e);
        }
    }
    number_parts=triangles.size();
    scale();
    move();
}

// Check for an intersection against the ray.  See the base class for details.
Hit Mesh::Intersection(const Ray& ray, int part) const
{
    //TODO;
    Hit temp = {this, std::numeric_limits<double>::max(), part};
	double dist = 0;
	for(unsigned i = 0; i<triangles.size();i++){
		if(Intersect_Triangle(ray, i, dist)){
			if(dist < temp.dist){
				temp.dist = dist;
				temp.part = i;
			}
		}

	}
	return temp;
}

// Compute the normal direction for the triangle with index part.
vec3 Mesh::Normal(const vec3& point, int part) const
{
    
    assert(part>=0);
    //TODO;
    vec3 A = vertices.at(triangles.at(part)[0]);
    vec3 B = vertices.at(triangles.at(part)[1]);
    vec3 C = vertices.at(triangles.at(part)[2]);

    
    return cross((B-A),(C-A)).normalized();
    
}

// This is a helper routine whose purpose is to simplify the implementation
// of the Intersection routine.  It should test for an intersection between
// the ray and the triangle with index tri.  If an intersection exists,
// record the distance and return true.  Otherwise, return false.
// This intersection should be computed by determining the intersection of
// the ray and the plane of the triangle.  From this, determine (1) where
// along the ray the intersection point occurs (dist) and (2) the barycentric
// coordinates within the triangle where the intersection occurs.  The
// triangle intersects the ray if dist>small_t and the barycentric weights are
// larger than -weight_tolerance.  The use of small_t avoid the self-shadowing
// bug, and the use of weight_tolerance prevents rays from passing in between
// two triangles.
bool Mesh::Intersect_Triangle(const Ray& ray, int tri, double& dist) const
{
    //TODO;
    
    vec3 x(0, 0, 0);
    
    ivec3 triangle = triangles[tri];
    vec3 normal = Normal(x, tri);
    
    vec3 a= vertices[triangle[0]];
    vec3 b = vertices[triangle[1]];
    vec3 c = vertices[triangle[2]];
    vec3 ab = b - a;
    vec3 ac = c - a;
	//calculate if ray intersects triangle.
    
    double t = (dot((a - ray.endpoint), normal) / dot(ray.direction, normal));
    
    if(t > small_t){ 
        //ray intersects plane created by the triangle.
        
		vec3 p = ray.endpoint + t * ray.direction;
        //p for barycentric coordinates

		vec3 pa = p - a;
		double bet, gam;
		bet = dot(pa, cross(normal, ac)) / dot(ab, cross(normal, ac));
		gam = dot(pa, cross(normal, ab)) / dot(ac, cross(normal, ab));
		if(bet > -weight_tolerance && gam > -weight_tolerance && (1 - bet - gam) > -weight_tolerance){
			dist = t;
			return true;
		}
	}
    else{
		return false;
	}
    
   return false;
   
}

// Compute the bounding box.  Return the bounding box of only the triangle whose
// index is part.
Box Mesh::Bounding_Box(int part) const
{
    Box b;
    TODO;
    return b;
}

void Mesh::scale(){
    std::cout<<"scale executed"<<std::endl;
    
    for (size_t i = 0; i<vertices.size(); ++i){
        vertices[i] *= 0.1;
    }

    double x_max = vertices[0].x[0], y_max = vertices[0].x[1], z_max = vertices[0].x[2];
    double x_min = vertices[0].x[0], y_min = vertices[0].x[1], z_min = vertices[0].x[2];
    for (size_t i = 0; i<vertices.size(); ++i){
        if (vertices[i].x[0]>x_max) {
            x_max = vertices[i].x[0];
        }
        if (vertices[i].x[1]>y_max) {
            y_max = vertices[i].x[1];
        }
        if (vertices[i].x[2]>z_max) {
            z_max = vertices[i].x[2];
        }
        if (vertices[i].x[0] < x_min) {
            x_min = vertices[i].x[0];
        }
        if (vertices[i].x[1] < y_min) {
            y_min = vertices[i].x[1];
        }
        if (vertices[i].x[2] < z_min) {
            z_min = vertices[i].x[2];
        }
    }
    std::cout<<"after scaling: "<<std::endl;
    std::cout<<"x_max: "<< x_max <<" y_max: "<< y_max <<" z_max: "<< z_max <<std::endl;
    std::cout<<"x_min: "<< x_min <<" y_min: "<< y_min <<" z_min: "<< z_min <<std::endl;
    vec3 center = vec3((x_max+x_min)/2, (y_max+y_min)/2, (z_max+z_min)/2);
    
    std::cout<<center<<std::endl;
    
    for (size_t i = 0; i<vertices.size(); ++i){
        vertices[i] = vertices[i] - center;
    }
    std::cout<<"after translation: "<<std::endl;
    x_max = vertices[0].x[0]; y_max = vertices[0].x[1]; z_max = vertices[0].x[2];
    x_min = vertices[0].x[0]; y_min = vertices[0].x[1]; z_min = vertices[0].x[2];
    for (size_t i = 0; i<vertices.size(); ++i){
        if (vertices[i].x[0]>x_max) {
            x_max = vertices[i].x[0];
        }
        if (vertices[i].x[1]>y_max) {
            y_max = vertices[i].x[1];
        }
        if (vertices[i].x[2]>z_max) {
            z_max = vertices[i].x[2];
        }
        if (vertices[i].x[0] < x_min) {
            x_min = vertices[i].x[0];
        }
        if (vertices[i].x[1] < y_min) {
            y_min = vertices[i].x[1];
        }
        if (vertices[i].x[2] < z_min) {
            z_min = vertices[i].x[2];
        }
    }
    std::cout<<"x_max: "<< x_max <<" y_max: "<< y_max <<" z_max: "<< z_max <<std::endl;
    std::cout<<"x_min: "<< x_min <<" y_min: "<< y_min <<" z_min: "<< z_min <<std::endl;
    center = vec3((x_max+x_min)/2, (y_max+y_min)/2, (z_max+z_min)/2);
    std::cout<<center<<std::endl;
    
}

void Mesh::move(){
    for (size_t i = 0; i<vertices.size(); ++i){
        vertices[i].x[0] += x_move;
        vertices[i].x[1] += y_move;
        vertices[i].x[2] += z_move;
    }
}