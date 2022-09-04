#include "light.h"
#include "transparent_shader.h"
#include "ray.h"
#include "render_world.h"
#include "object.h"
#include <algorithm>

vec3 Transparent_Shader::
Shade_Surface(const Ray& ray,const vec3& intersection_point,
    const vec3& normal,int recursion_depth) const
{
    if(recursion_depth >= world.recursion_depth_limit){
        return {0,0,0};
    }
    vec3 u = ray.direction;
    vec3 r = (-2)*dot(normal.normalized(), u.normalized())*normal.normalized()+u.normalized(); 
    Ray reflected_ray(intersection_point, r);   //reflected ray
    vec3 t = {0,0,0};
    double c = 0;
    if(dot(u,normal) < 0){ //entering
        refract(u, normal, refract_n, t);
        c = (-1)*dot(u, normal);
    }
    else{ //exiting
        if(refract(u, normal*(-1), 1/refract_n, t) == true){
            c = dot(t, normal);
        }
        else{
            return world.Cast_Ray(reflected_ray, recursion_depth+1);
        }
    }
    Ray refraction_ray(intersection_point, t);
    double r0 = pow(refract_n-1, 2)/pow(refract_n+1, 2);
    double reflectivity = r0 + (1-r0)*pow(1-c, 5);
    return reflectivity*world.Cast_Ray(reflected_ray, recursion_depth+1) + (1-reflectivity)*world.Cast_Ray(refraction_ray, recursion_depth+1);
}

bool Transparent_Shader::refract(const vec3 d, const vec3 normal, const double n, vec3& t) const{
    if((1 - (1- pow(dot(normal.normalized(), d.normalized()), 2))/pow(n, 2)) < 0){ //total internal reflection
        return false;
    }
    t = d-normal*dot(normal.normalized(), d.normalized())/n - normal*sqrt(1 - (1- pow(dot(normal.normalized(), d.normalized()), 2))/pow(n, 2));
    return true;
}