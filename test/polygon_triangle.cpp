#include "treeface/guts/shapegenerator_guts.h"

class TestFramework
{
public:
    static void run(const char* file_in, const char* file_out)
    {
        FILE* fh_in = fopen(file_in, "rb");
        if (!fh_in)
        {
            fprintf(stderr, "failed to open input file %s: %s\n", file_in, strerror(errno));
            exit(EXIT_FAILURE);
        }

        treeface::ShapeGenerator generator;
        for (;;)
        {
            float x = std::numeric_limits<float>::signaling_NaN();
            float y = std::numeric_limits<float>::signaling_NaN();

            if (fscanf(fh_in, "%f %f", &x, &y) == EOF)
                break;

            generator.line_to(treeface::Vec2f(x, y));
        }

        fclose(fh_in);

        treecore::Array<treeface::Vec2f> result_vertices;
        treecore::Array<treeface::IndexType> result_indices;
        generator.m_guts->triangulate(result_vertices, result_indices);


        FILE* fh_out = fopen(file_out, "wb");
        if (!fh_out)
        {
            fprintf(stderr, "failed to open output file %s: %s\n", file_out, strerror(errno));
            exit(EXIT_FAILURE);
        }

        for (treeface::IndexType idx : result_indices)
        {
            fprintf(fh_out, "%f\t%f\n", result_vertices[idx].x, result_vertices[idx].y);
        }

        fclose(fh_out);
    }
};


int main(int argc, char** argv)
{
    if (argc != 3)
    {
        fprintf(stderr, "usage: %s input_file output_file\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    TestFramework::run(argv[1], argv[2]);
}
