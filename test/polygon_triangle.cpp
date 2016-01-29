#include "treeface/graphics/guts/ShapeGenerator_guts.h"
#include "treeface/scene/Geometry.h"
#include "treeface/math/Vec2.h"

class TestFramework
{
public:
    static void run( const char* file_in, const char* file_out )
    {
        FILE* fh_in = fopen( file_in, "rb" );
        if (!fh_in)
        {
            fprintf( stderr, "failed to open input file %s: %s\n", file_in, strerror( errno ) );
            exit( EXIT_FAILURE );
        }

        treeface::ShapeGenerator generator;
        for (;; )
        {
            float x = std::numeric_limits<float>::signaling_NaN();
            float y = std::numeric_limits<float>::signaling_NaN();

            if (fscanf( fh_in, "%f %f", &x, &y ) == EOF)
                break;

            generator.line_to( treeface::Vec2f( x, y ) );
        }

        fclose( fh_in );

        treeface::Geometry::HostVertexCache  result_vertices( sizeof(treeface::Vec2f) );
        treecore::Array<treeface::IndexType> result_indices;
        treeface::Vec2f skeleton_min( std::numeric_limits<float>::max(), std::numeric_limits<float>::max() );
        treeface::Vec2f skeleton_max( std::numeric_limits<float>::min(), std::numeric_limits<float>::min() );
        generator.m_guts->triangulate( result_vertices, result_indices, skeleton_min, skeleton_max );

        FILE* fh_out = fopen( file_out, "wb" );
        if (!fh_out)
        {
            fprintf( stderr, "failed to open output file %s: %s\n", file_out, strerror( errno ) );
            exit( EXIT_FAILURE );
        }

        for (treeface::IndexType idx : result_indices)
        {
            const treeface::Vec2f& vtx = result_vertices.get<treeface::Vec2f>( idx );
            fprintf( fh_out, "%f\t%f\n", vtx.x, vtx.y );
        }

        fclose( fh_out );
    }
};

int main( int argc, char** argv )
{
    if (argc != 3)
    {
        fprintf( stderr, "usage: %s input_file output_file\n", argv[0] );
        exit( EXIT_FAILURE );
    }

    TestFramework::run( argv[1], argv[2] );
}
