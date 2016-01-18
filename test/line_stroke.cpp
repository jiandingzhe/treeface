#include "treeface/graphics/guts/SubPath.h"
#include "treeface/math/constants.h"

#include <treecore/FileInputStream.h>
#include <treecore/FileOutputStream.h>
#include <treecore/OptionParser.h>
#include <treecore/StringArray.h>

using namespace treecore;
using namespace treeface;

LineJoin line_join    = LINE_JOIN_BEVEL;
LineCap  line_cap     = LINE_CAP_BUTT;
float    line_width   = 1.0f;
float    miter_cutoff = 150;
File     file_in;
File     file_out;
bool     close_path = false;
bool     show_help  = false;

Option opt_file_in( "in", 'i', "Input/Output",
                    &file_in, 0,
                    "Input file name", "FILE" );

Option opt_file_out( "out", 'o', "Input/Output",
                     &file_out, 0,
                     "Output file name", "FILE" );

Option opt_line_join( "join", 'j', "Line Style",
                      &line_join, 0,
                      "Line join, bevel|miter|round", "VALUE" );

Option opt_line_cap( "cap", 'c', "Line Style",
                     &line_cap, 0,
                     "Line cap, butt|round|square", "VALUE" );

Option opt_line_width( "width", 'w', "Line Style",
                       &line_width, 0,
                       "Line width", "NUMBER" );

Option opt_miter_cut( "miter-cutoff", 0, "Line Style",
                      &miter_cutoff, 0,
                      "Miter join cutoff in degree", "NUMBER" );

Option opt_close_path( "close", 0, "Line Style",
                       &close_path, 0,
                       "Close path after all glyphs" );

Option opt_help( "help", 'h', "Misc",
                 &show_help, 0,
                 "Show help message" );

OptionParser opt_parser;

void show_help_and_exit()
{
    fprintf( stderr, "usage: \n%s\n", opt_parser.format_document().toRawUTF8() );
    exit( EXIT_FAILURE );
}

int main( int argc, const char* argv[] )
{

    opt_parser.add_option( opt_file_in );
    opt_parser.add_option( opt_file_out );
    opt_parser.add_option( opt_line_join );
    opt_parser.add_option( opt_line_cap );
    opt_parser.add_option( opt_line_width );
    opt_parser.add_option( opt_miter_cut );
    opt_parser.add_option( opt_close_path );
    opt_parser.add_option( opt_help );
    opt_parser.parse_options( argc, argv );

    if (show_help) show_help_and_exit();

    SubPath path;

    // read input
    FileInputStream fh_in( file_in );

    while ( !fh_in.isExhausted() )
    {
        String line = fh_in.readNextLine();

        StringArray tokens;
        tokens.addTokens( line, " \t", "\"'" );
        if (tokens.size() == 0) continue;

        if (tokens[0] == "line")
        {
            if (tokens.size() != 3)
                die( "invalid number of tokens for line glyph: %d", tokens.size() );

            float x = tokens[1].getFloatValue();
            float y = tokens[2].getFloatValue();
            path.glyphs.add( PathGlyph( Vec2f( x, y ) ) );
        }
        else if (tokens[0] == "bessel3")
        {
            if (tokens.size() != 5)
                die( "invalid number of tokens for bessel3 glyph: %d", tokens.size() );
            float x1 = tokens[1].getFloatValue();
            float y1 = tokens[2].getFloatValue();
            float x2 = tokens[3].getFloatValue();
            float y2 = tokens[4].getFloatValue();
            path.glyphs.add( PathGlyph( Vec2f( x1, y1 ), Vec2f( x2, y2 ) ) );
        }
        else if (tokens[0] == "bessel4")
        {
            if (tokens.size() != 7)
                die( "invalid number of tokens for bessel4 glyph: %d", tokens.size() );
            float x1 = tokens[1].getFloatValue();
            float y1 = tokens[2].getFloatValue();
            float x2 = tokens[3].getFloatValue();
            float y2 = tokens[4].getFloatValue();
            float x3 = tokens[5].getFloatValue();
            float y3 = tokens[6].getFloatValue();
            path.glyphs.add( PathGlyph( Vec2f( x1, y1 ), Vec2f( x2, y2 ), Vec2f( x3, y3 ) ) );
        }
        else if (tokens[0] == "arc")
        {
            if (tokens.size() != 6)
                die( "invalid number of tokens for bessel4 glyph: %d", tokens.size() );
            float center_x = tokens[1].getFloatValue();
            float center_y = tokens[2].getFloatValue();
            float end_x    = tokens[3].getFloatValue();
            float end_y    = tokens[4].getFloatValue();
            float angle    = tokens[5].getFloatValue();
            path.glyphs.add( PathGlyph( Vec2f( center_x, center_y ), Vec2f( end_x, end_y ), angle, true ) );
        }
        else if (tokens[0] == "arcn")
        {
            if (tokens.size() != 6)
                die( "invalid number of tokens for bessel4 glyph: %d", tokens.size() );
            float center_x = tokens[1].getFloatValue();
            float center_y = tokens[2].getFloatValue();
            float end_x    = tokens[3].getFloatValue();
            float end_y    = tokens[4].getFloatValue();
            float angle    = tokens[5].getFloatValue();
            path.glyphs.add( PathGlyph( Vec2f( center_x, center_y ), Vec2f( end_x, end_y ), angle, false ) );
        }
        else
        {
            die( "invalid glyph type: %s", tokens[0].toRawUTF8() );
        }
    }

    if (close_path) path.closed = true;

    // do line stroke
    Geometry::HostVertexCache vertices( sizeof(StrokeVertex) );
    Array<IndexType> indices;
    path.stroke_complex( vertices, indices, StrokeStyle{ line_cap, line_join, miter_cutoff * treeface::PI / 180, line_width } );

    // write result
    file_out.deleteFile();
    FileOutputStream fh_out( file_out );
    for (IndexType idx : indices)
    {
        const Vec2f& pos = vertices.get<StrokeVertex>( idx ).position;
        fh_out << pos.x << "\t" << pos.y << "\n";
    }
}
