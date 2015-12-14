#include "treeface/guts/shapegenerator_guts.h"

using namespace treecore;
using namespace treeface;

int main(int argc, char** argv)
{
    if (argc != 3)
    {
        fprintf(stderr, "usage: %s input_file\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // read input
    FILE* fh_in = fopen(argv[1], "rb");
    if (fh_in == nullptr)
    {
        fprintf(stderr, "failed to open input file %s: %s\n", argv[1], strerror(errno));
        exit(EXIT_FAILURE);
    }

    Array<Vec2f> vertices;
    for (;;)
    {
        float x = std::numeric_limits<float>::signaling_NaN();
        float y = std::numeric_limits<float>::signaling_NaN();

        if (fscanf(fh_in, "%f %f", &x, &y) == EOF)
            break;

        vertices.add(treeface::Vec2f(x, y));
    }

    fclose(fh_in);

    // generate edges
    Array<HalfEdge> edges_in;
    IndexType total_num_edge = vertices.size();
    bool ccw = is_counter_clockwise(vertices);

    if (ccw)
        printf("polygon is counter-clockwise\n");
    else
        printf("polygon is clockwise\n");

    for (int i_vtx = 0; i_vtx < vertices.size(); i_vtx++)
    {
        IndexType i_edge_curr = IndexType(edges_in.size());

        IndexType i_prev = (i_vtx == 0
                            ? IndexType(total_num_edge - 1)
                            : i_edge_curr - 1);

        IndexType i_next = (i_vtx == vertices.size() - 1
                            ? 0
                            : i_edge_curr + 1);

        if (ccw)
            edges_in.add(HalfEdge{IndexType(i_vtx), i_prev, i_next, std::numeric_limits<IndexType>::max()});
        else
            edges_in.add(HalfEdge{IndexType(i_vtx), i_next, i_prev, std::numeric_limits<IndexType>::max()});
    }

    jassert(edges_in.size() == vertices.size());

    // do split
    Array<HalfEdge> edges_out;
    partition_polygon_monotone(vertices, edges_in, edges_out);

    printf("%d input edges generated %d edges\n", edges_in.size(), edges_out.size());

    // write result
    FILE* fh_out = fopen(argv[2], "wb");
    if (!fh_out)
    {
        fprintf(stderr, "failed to open output file %s: %s\n", argv[2], strerror(errno));
        exit(EXIT_FAILURE);
    }

    Array<bool> edges_out_written;
    for (int i = 0; i < edges_out.size(); i++)
        edges_out_written.add(false);

    for (int i_begin = 0; i_begin < edges_out.size(); i_begin++)
    {
        if (edges_out_written[i_begin])
            continue;

        IndexType i_edge = IndexType(i_begin);
        for (;;)
        {
            const HalfEdge& edge = edges_out[i_edge];

            const Vec2f& vtx = vertices[edge.idx_vertex];

            fprintf(fh_out, "%f\t%f\n", vtx.x, vtx.y);
            edges_out_written[i_edge] = true;

            i_edge = edge.idx_next_edge;
            if (i_edge == i_begin) break;
        }

        fprintf(fh_out, "\n");
    }

    fclose(fh_out);
}
