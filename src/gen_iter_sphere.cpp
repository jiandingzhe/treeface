#include "treeface/math/vec4.h"

#include <treejuce/Array.h>
#include <cstdio>
#include <cmath>

using namespace treeface;
using namespace treejuce;
using std::printf;
using std::abs;

int id_seed = 0;

void show_usage_and_exit()
{
    printf("usage: gen_iter_sphere num_iteration\n");
    exit(1);
}


void write_one(const Vec4f& p)
{
    printf("        [ %f,%f,%f,1.0,  0.5,0.5,0.5,1.0,  %f,%f,%f,0.0 ],\n",
           p.get_x(), p.get_y(), p.get_z(),
           p.get_x(), p.get_y(), p.get_z() // position use as normal vector
           );
}

void write_result(const Array<Vec4f>& result)
{
    printf("{\n");
    printf("    \"primitive\": \"triangles\",\n");
    printf("    \"attributes\":\n");
    printf("    [\n");
    printf("        {\n");
    printf("            \"name\": \"position\",\n");
    printf("            \"type\": \"float\",\n");
    printf("            \"n_elem\": 4\n");
    printf("        },\n");
    printf("        {\n");
    printf("            \"name\": \"color\",\n");
    printf("            \"type\": \"float\",\n");
    printf("            \"n_elem\": 4\n");
    printf("        },\n");
    printf("        {\n");
    printf("            \"name\": \"normal\",\n");
    printf("            \"type\": \"float\",\n");
    printf("            \"n_elem\": 4\n");
    printf("        },\n");
    printf("    ],\n");
    printf("    \"vertices\":\n");
    printf("    [\n");
    for (int i = 0; i < result.size(); i += 3)
    {
        write_one(result[i]);
        write_one(result[i+1]);
        write_one(result[i+2]);
    }
    printf("    ],\n");
    printf("    \"indices\":\n");
    printf("    [\n");
    for (int i = 0; i < result.size(); i+=3)
    {
        printf("        %d, %d, %d,\n", i, i+1, i+2);
    }
    printf("    ]\n");
    printf("}\n");
}


void iter(const Vec4f& p1, const Vec4f& p2, const Vec4f& p3, Array<Vec4f>& result, int n_iter)
{
    if (n_iter == 0)
    {
        result.add(p1);
        result.add(p2);
        result.add(p3);
        return;
    }

    Vec4f pa = (p1 + p2) / 2.0f;
    Vec4f pb = (p2 + p3) / 2.0f;
    Vec4f pc = (p3 + p1) / 2.0f;
    pa.normalize();
    pb.normalize();
    pc.normalize();

    iter(p1, pa, pc, result, n_iter-1);
    iter(pa, p2, pb, result, n_iter-1);
    iter(pc, pb, p3, result, n_iter-1);
    iter(pa, pb, pc, result, n_iter-1);
}

int main(int argc, char** argv)
{
    if (argc != 2)
        show_usage_and_exit();

    int n_iter = -1;
    {
        int re = std::sscanf(argv[1], "%d", &n_iter);
        if (n_iter < 0 or re != 1)
            show_usage_and_exit();
    }

    Vec4f p_top(0, 0, 1, 0);
    Vec4f p_bottom(0, 0, -1, 0);

    Vec4f p1(1, 0, 0, 0);
    Vec4f p2(0, 1, 0, 0);
    Vec4f p3(-1, 0, 0, 0);
    Vec4f p4(0, -1, 0, 0);

    Array<Vec4f> result;
    iter(p_top, p1, p2, result, n_iter);
    iter(p_top, p2, p3, result, n_iter);
    iter(p_top, p3, p4, result, n_iter);
    iter(p_top, p4, p1, result, n_iter);
    iter(p1, p_bottom, p2, result, n_iter);
    iter(p2, p_bottom, p3, result, n_iter);
    iter(p3, p_bottom, p4, result, n_iter);
    iter(p4, p_bottom, p1, result, n_iter);

    write_result(result);
}


