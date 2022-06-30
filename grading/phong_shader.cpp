#include "light.h"
#include "phong_shader.h"
#include "ray.h"
#include "render_world.h"
#include "object.h"
#include <algorithm>

vec3 Phong_Shader::
Shade_Surface(const Ray& ray,const vec3& intersection_point,
    const vec3& normal,int recursion_depth) const
{
    vec3 color = {0,0,0};
    vec3 ambient = world.ambient_color*color_ambient*world.ambient_intensity;
    vec3 diffuse = {0,0,0};
    vec3 specular = {0,0,0};
    

    for(size_t i=0;i<world.lights.size();i++){

        Ray shadowRay(intersection_point, world.lights[i]->position - intersection_point);
        Hit shadowHit = world.Closest_Intersection(shadowRay);
        if(world.enable_shadows == true && shadowHit.object != nullptr && shadowHit.dist < (world.lights[i]->position - intersection_point).magnitude()){
            
        }
        else{
            vec3 l = world.lights[i]->position - intersection_point;
            diffuse += world.lights[i]->Emitted_Light(l)*color_diffuse*std::max(dot(normal.normalized(),l.normalized()), 0.0);
            vec3 r = 2*(dot(normal.normalized(),l.normalized()))*normal.normalized() - l.normalized(); 
            vec3 c = world.camera.position-intersection_point;
            specular += color_specular*world.lights[i]->Emitted_Light(l)*pow(std::max(dot(r.normalized(), c.normalized()), 0.0), specular_power);
        }
        
    }
    color += ambient + diffuse + specular;

    return color;
}
