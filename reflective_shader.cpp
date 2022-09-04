#include "reflective_shader.h"
#include "ray.h"
#include "render_world.h"

vec3 Reflective_Shader::
Shade_Surface(const Ray& ray,const vec3& intersection_point,
    const vec3& normal,int recursion_depth) const
{
    vec3 color = shader->Shade_Surface(ray, intersection_point, normal, recursion_depth); //phone shader
    
    vec3 u = ray.direction;
    vec3 r = -2*dot(normal.normalized(), u.normalized())*normal.normalized()+u.normalized(); //reflected ray

    Ray reflected_ray(intersection_point, r);

    vec3 reflected_color = {0,0,0};

    if(recursion_depth< world.recursion_depth_limit){
        reflected_color = world.Cast_Ray(reflected_ray, recursion_depth+1);
        
    }
    
    color = (1-reflectivity)*color + reflectivity*reflected_color;

    return color;
}
