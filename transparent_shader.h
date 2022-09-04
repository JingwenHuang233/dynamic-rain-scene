#ifndef __TRANSPARENT_SHADER_H__
#define __TRANSPARENT_SHADER_H__

#include "shader.h"

class Transparent_Shader : public Shader
{
public:
    Shader* shader;
    double refract_n; //refractive index, we assume that all objects are embedded in air with refractive index very close to 1.0

    Transparent_Shader(Render_World& world_input, Shader* shader_input, double n)
        :Shader(world_input),shader(shader_input), refract_n(n)
    {}

    //example call: transparent_shader transA white_shader 1.34

    virtual vec3 Shade_Surface(const Ray& ray,const vec3& intersection_point,
        const vec3& normal,int recursion_depth) const override;

    bool refract(const vec3 d, const vec3 normal, const double n, vec3& t) const;
};
#endif