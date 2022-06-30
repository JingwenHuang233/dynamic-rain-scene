#include "sphere.h"
#include "ray.h"

// Determine if the ray intersects with the sphere
Hit Sphere::Intersection(const Ray& ray, int part) const
{
    double t = (double)(-(dot((ray.direction)*2,(ray.endpoint-center)))-sqrt(pow((dot((ray.direction)*2,(ray.endpoint-center))),2)-4*(dot(ray.endpoint-center, ray.endpoint-center)-pow(radius,2))))/2.0;
    if(t>0){
        return{this, t, part};
    }
    t = (double)(-(dot((ray.direction)*2,(ray.endpoint-center)))+sqrt(pow((dot((ray.direction)*2,(ray.endpoint-center))),2)-4*(dot(ray.endpoint-center, ray.endpoint-center)-pow(radius,2))))/2.0;
    if(t>0){
        return{this, t, part};
    }
    return {0,0, part};
}

vec3 Sphere::Normal(const vec3& point, int part) const
{
    vec3 normal = {0,0,0};
    normal = (point-center)/(point-center).magnitude();
    return normal;
}

Box Sphere::Bounding_Box(int part) const
{
    Box box;
    TODO; // calculate bounding box
    return box;
}
