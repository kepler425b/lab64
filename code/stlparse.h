#ifndef __STLPARSE_H__
#define __STLPARSE_H__

#include <istream>
#include <map>
#include <vector>

#include "mesh.h"

namespace meshparse {


typedef struct {
    uint32_t verts[3];
    vec3 normal;
} stlface;

typedef struct {
    uint32_t next_vertid;
    std::map<vec3, uint32_t, vector_comparitor> vert_to_id;
    std::vector<stlface> faces;

} stlmesh;

void stl_to_mesh(stlmesh &stlm, mesh &mesh);
bool is_ascii_stl(std::istream &input);
bool load_ascii_stl(std::istream &input, mesh &mesh);
bool load_binary_stl(std::ifstream &input, mesh &mesh);

}

#endif
