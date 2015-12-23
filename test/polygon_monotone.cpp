#include "treeface/graphics/guts/shapegenerator_guts.h"
#include "treeface/graphics/guts/HalfEdgeNetwork.h"

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
    HalfEdgeNetwork network(vertices);

    {
        bool ccw = (clockwise_accum(vertices, 0, vertices.size()) < 0.0);
        Array<IndexType> begin;
        begin.add(0);
        network.build_half_edges(begin, ccw);
    }

    // do split
    HalfEdgeNetwork network_result(vertices);
    network.partition_polygon_monotone(network_result);

    printf("%d input edges generated %d edges\n", network.edges.size(), network_result.edges.size());

    // write result
    FILE* fh_out = fopen(argv[2], "wb");
    if (!fh_out)
    {
        fprintf(stderr, "failed to open output file %s: %s\n", argv[2], strerror(errno));
        exit(EXIT_FAILURE);
    }

    Array<bool> edges_out_written;
    for (int i = 0; i < network_result.edges.size(); i++)
        edges_out_written.add(false);

    for (int i_begin = 0; i_begin < network_result.edges.size(); i_begin++)
    {
        if (edges_out_written[i_begin])
            continue;

        IndexType i_edge = IndexType(i_begin);
        for (;;)
        {
            const HalfEdge& edge = network_result.edges[i_edge];

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
