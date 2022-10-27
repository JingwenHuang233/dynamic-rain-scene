#include "render_world.h"
#include "flat_shader.h"
#include "object.h"
#include "light.h"
#include "ray.h"

extern bool disable_hierarchy;

Render_World::Render_World()
    :background_shader(0),ambient_intensity(0),enable_shadows(true),
    recursion_depth_limit(3)
{}

Render_World::~Render_World()
{
    delete background_shader;
    for(size_t i=0;i<objects.size();i++) delete objects[i];
    for(size_t i=0;i<lights.size();i++) delete lights[i];
}

// Find and return the Hit structure for the closest intersection.  Be careful
// to ensure that hit.dist>=small_t.
Hit Render_World::Closest_Intersection(const Ray& ray)
{
    Hit closest = {0,0,-1};
    Hit temp = {0,0,-1};
    double min_t = std::numeric_limits<double>::max();
    for(size_t i=0;i<objects.size();i++){
        temp = objects[i]->Intersection(ray, -1);  //onjects[i]->number_part
        if (temp.dist < min_t && temp.dist >= small_t && temp.object != nullptr){
            closest = temp;
            min_t = closest.dist;
        }
        
    }
    return closest;
}

// set up the initial view ray and call
void Render_World::Render_Pixel(const ivec2& pixel_index)
{
    vec3 e = camera.position; // set up the initial view ray here
    vec3 u = camera.World_Position(pixel_index)-e;
    Ray ray (e,u);
    vec3 color=Cast_Ray(ray,1);
    camera.Set_Pixel(pixel_index,Pixel_Color(color));
}

void Render_World::Render()
{
    if(!disable_hierarchy)
        Initialize_Hierarchy();

    for(int j=0;j<camera.number_pixels[1];j++)
        for(int i=0;i<camera.number_pixels[0];i++)
            Render_Pixel(ivec2(i,j));
}

// cast ray and return the color of the closest intersected surface point,
// or the background color if there is no object intersection
vec3 Render_World::Cast_Ray(const Ray& ray,int recursion_depth)
{
    if (recursion_depth > recursion_depth_limit){
        return {0,0,0};
    }
    vec3 color;
    Hit closest = Closest_Intersection(ray);
    if( closest.object != nullptr){
        vec3 intersection_point = ray.Point(closest.dist);
        vec3 normal = closest.object->Normal(intersection_point, closest.part);
        color = closest.object->material_shader->Shade_Surface(ray, intersection_point, normal, recursion_depth);
    }
    else{
        /*
        u, v = spheremap_coords(r.direction);
        color =  texture_lookup(scene.env_map, u, v);
        */
        double u = 1, v = 1;
        if(abs(ray.direction.x[2]) > abs(ray.direction.x[0]) && abs(ray.direction.x[2]) > abs(ray.direction.x[1]) 
          && ray.direction.x[2] > 0){ // +z face
            u = 0.5*(1+ray.direction.x[0]/ray.direction.x[2]);
            v = 0.5*(1+(-1)*ray.direction.x[1]/ray.direction.x[2]);
        }
        else if(abs(ray.direction.x[2]) > abs(ray.direction.x[0]) && abs(ray.direction.x[2]) > abs(ray.direction.x[1]) 
          && ray.direction.x[2] < 0){ // -z face
            u = 0.5*(1+ray.direction.x[0]/ray.direction.x[2]);
            v = 0.5*(1+(-1)*ray.direction.x[1]/ray.direction.x[2]);
        }
        else if (abs(ray.direction.x[0]) > abs(ray.direction.x[1]) && abs(ray.direction.x[0]) > abs(ray.direction.x[2]) 
          && ray.direction.x[0] > 0){ // +x face
            u = 0.5*(1+(-1)*ray.direction.x[2]/ray.direction.x[0]);
            v = 0.5*(1+(-1)*ray.direction.x[1]/ray.direction.x[0]);
        }
        else if (abs(ray.direction.x[0]) > abs(ray.direction.x[1]) && abs(ray.direction.x[0]) > abs(ray.direction.x[2]) 
          && ray.direction.x[0] < 0){ // -x face
            u = 0.5*(1+ray.direction.x[2]/ray.direction.x[0]);
            v = 0.5*(1+(-1)*ray.direction.x[1]/ray.direction.x[0]);
        }
        else if (abs(ray.direction.x[1]) > abs(ray.direction.x[0]) && abs(ray.direction.x[1]) > abs(ray.direction.x[2]) 
          && ray.direction.x[1] > 0){ // +y face
            u = 0.5*(1+ray.direction.x[0]/ray.direction.x[1]);
            v = 0.5*(1+ray.direction.x[2]/ray.direction.x[1]);
        }
        else if (abs(ray.direction.x[1]) > abs(ray.direction.x[0]) && abs(ray.direction.x[1]) > abs(ray.direction.x[2]) 
          && ray.direction.x[1] < 0){ // -y face
            u = 0.5*(1+ray.direction.x[0]/ray.direction.x[1]);
            v = 0.5*(1+(-1)*ray.direction.x[2]/ray.direction.x[1]);
        }
        
        color =  texture_lookup(bg_data, u, v);
        
        
        //color = background_shader->Shade_Surface(ray, vec3(0,0,0), vec3(0,0,0), recursion_depth);
    }
    // determine the color here
    
    return color;
}

void Render_World::Initialize_Hierarchy()
{
    TODO; // Fill in hierarchy.entries; there should be one entry for
    // each part of each object.

    hierarchy.Reorder_Entries();
    hierarchy.Build_Tree();
}


vec3 Render_World::texture_lookup(Pixel* t, float u, float v) {
    int i = round(u * w - 0.5);
    int j = round(v * h - 0.5);
    Pixel color = t[j*w+i];
    //std::cout<<color<< std::endl;
    return From_Pixel(color);
}
