#include "treeface/guts/shapegenerator_guts.h"

int main(int argc, char** argv)
{
    if (argc != 3)
    {
        fprintf(stderr, "usage: %s input_file output_file\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    FILE* fh_in = fopen(argv[1], "rb");
    if (!fh_in)
    {
        fprintf(stderr, "failed to open input file %s: %s\n", argv[1], strerror(errno));
        exit(EXIT_FAILURE);
    }

    treeface::SubPath obj;
    for (;;)
    {
        float x = std::numeric_limits<float>::signaling_NaN();
        float y = std::numeric_limits<float>::signaling_NaN();

        if (fscanf(fh_in, "%f %f", &x, &y) == EOF)
            break;

        obj.vertices.add(treeface::Vec2f(x, y));
    }

    fclose(fh_in);

    treecore::Array<treeface::Vec2f> result_vertices;
    treecore::Array<treeface::IndexType> result_indices;
    obj.triangulate_simple(result_vertices, result_indices);


    FILE* fh_out = fopen(argv[2], "wb");
    if (!fh_out)
    {
        fprintf(stderr, "failed to open output file %s: %s\n", argv[2], strerror(errno));
        exit(EXIT_FAILURE);
    }

    for (treeface::IndexType idx : result_indices)
    {
        fprintf(fh_out, "%f\t%f\n", result_vertices[idx].x, result_vertices[idx].y);
    }

    fclose(fh_out);
}
