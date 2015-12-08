#include "treeface/guts/shapegenerator_guts.h"

#include <cstdio>

int main(int argc, char** argv)
{
    if (argc != 4)
    {
        fprintf(stderr, "usage: ./subpath_triangulate input_file output_file simple|complex\n");
        exit(EXIT_FAILURE);
    }

    FILE* fh_in = fopen(argv[1], "rb");
    if (fh_in == nullptr)
    {
        fprintf(stderr, "failed to open input file %s: %s\n", argv[1], strerror(errno));
        exit(EXIT_FAILURE);
    }

    treeface::SubPath sub_path;

    for (;;)
    {
        float x = std::numeric_limits<float>::signaling_NaN();
        float y = std::numeric_limits<float>::signaling_NaN();

        if (fscanf(fh_in, "%f %f", &x, &y) == EOF)
            break;

        sub_path.vertices.add(treeface::Vec2f(x, y));
    }

    fclose(fh_in);

    treecore::Array<treeface::Vec2f> vertices;
    treecore::Array<treeface::IndexType> indices;

    if (strcmp(argv[3],"simple") == 0)
        sub_path.triangulate_simple(vertices, indices);
    else if (strcmp(argv[3], "complex") == 0)
        sub_path.triangulate_complex(vertices, indices);
    else
    {
        fprintf(stderr, "invalid triangulation mode: %s\n", argv[3]);
        exit(EXIT_FAILURE);
    }

    FILE* fh_out = fopen(argv[2], "wb");
    if (fh_out == nullptr)
    {
        fprintf(stderr, "failed to open output file %s: %s\n", argv[2], strerror(errno));
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < indices.size(); i++)
    {
        treeface::IndexType idx = indices[i];
        fprintf(fh_out, "%f\t%f\n", vertices[idx].x, vertices[idx].y);
    }
    fclose(fh_out);
}
