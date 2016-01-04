#include "treeface/graphics/guts/SubPath.h"

#include <treecore/FileInputStream.h>
#include <treecore/FileOutputStream.h>
#include <treecore/OptionParser.h>
#include <treecore/StringArray.h>

using namespace treecore;
using namespace treeface;

namespace treecore
{

template<>
bool fromString<LineCap>( const String& str, LineCap& result )
{
    String value_lc = str.toLowerCase();

    bool re = true;
    if ( value_lc.contains( "butt" ) ) result = LINE_CAP_BUTT;
    else if ( value_lc.contains( "round" ) ) result = LINE_CAP_ROUND;
    else if ( value_lc.contains( "square" ) ) result = LINE_CAP_SQUARE;
    else re = false;

    return re;
}

template<>
bool fromString<LineJoin>( const String& str, LineJoin& result )
{
    String value_lc = str.toLowerCase();

    bool re = true;
    if ( value_lc.contains( "bevel" ) ) result = LINE_JOIN_BEVEL;
    else if ( value_lc.contains( "miter" ) ) result = LINE_JOIN_MITER;
    else if ( value_lc.contains( "round" ) ) result = LINE_JOIN_ROUND;
    else re = false;

    return re;
}

template<>
treecore::String toString<LineCap>( LineCap cap )
{
    switch (cap)
    {
    case LINE_CAP_BUTT: return "butt";
    case LINE_CAP_ROUND: return "round";
    case LINE_CAP_SQUARE: return "square";
    }
}

template<>
treecore::String toString<LineJoin>( LineJoin join )
{
    switch (join)
    {
    case LINE_JOIN_BEVEL: return "bevel";
    case LINE_JOIN_MITER: return "miter";
    case LINE_JOIN_ROUND: return "round";
    }
}

} // namespace treecore

LineJoin line_join = LINE_JOIN_BEVEL;
LineCap line_cap   = LINE_CAP_BUTT;
float line_width   = 1.0f;
float miter_cutoff = 120;
File file_in;
File file_out;
bool close_path = false;
bool show_help  = false;

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
    Array<StrokeVertex> vertices;
    Array<IndexType> indices;
    path.stroke_complex( vertices, indices, StrokeStyle{line_cap, line_join, miter_cutoff, line_width} );

    // write result
    file_out.deleteFile();
    FileOutputStream fh_out(file_out);
    for (IndexType idx : indices)
    {
        const Vec2f& pos = vertices[idx].position;
        fh_out << pos.x << "\t" << pos.y << "\n";
    }
}
