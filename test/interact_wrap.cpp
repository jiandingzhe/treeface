// should produce same result with interact.cpp
// use wrapped API in src/treeface/gl
#include <SDL.h>

#define GLEW_STATIC
#include <GL/glew.h>

#include <stdio.h>
#include <stdlib.h>

#include <set>

#include "treeface/gl/GLBuffer.h"
#include "treeface/gl/Program.h"
#include "treeface/gl/VertexArray.h"
#include "treeface/gl/VertexTemplate.h"

#include <treecore/AlignedMalloc.h>

#include <vector>
#include <set>

#include <treecore/ArrayRef.h>
#include <treecore/PlatformDefs.h>
#include <treecore/RefCountHolder.h>
#include <treecore/Logger.h>
#include <treecore/HashMultiMap.h>

using namespace std;
using namespace treeface;
using namespace treecore;

struct BBox
{
    float x      = 0;
    float y      = 0;
    float width  = 0;
    float height = 0;
};

struct ColoredPoint
{
    float x;
    float y;
    float z;
    float w;
    float r;
    float g;
    float b;
    float a;
};

TypedTemplate attr_desc_position{"in_position", 4, treeface::TFGL_TYPE_FLOAT};
TypedTemplate attr_desc_color{"in_color", 4, treeface::TFGL_TYPE_FLOAT};

const char* src_vertex =
    "#version 130\n"
    ""
    "in vec4 in_position;\n"
    "in vec4 in_color;\n"
    ""
    "out vec4 frag_color;\n"
    ""
    "uniform bool is_active;\n"
    "uniform mat4 matrix;\n"
    ""
    "void main()\n"
    "{\n"
    "  frag_color = in_color;\n"
    "  if (!is_active)\n"
    "  {\n"
    "    frag_color = vec4(0.2, 0.2, 0.2, 1);\n"
    "  }\n"
    "  gl_Position = matrix * in_position;\n"
    "}\n";

const char* src_fragment =
    "#version 130\n"
    "in vec4 frag_color;\n"
    ""
    "void main()\n"
    "{\n"
    "  gl_FragColor = frag_color;\n"
    "}\n";

void show_matrix( Mat4f& mat )
{
    printf( "%f %f %f %f\n"
            "%f %f %f %f\n"
            "%f %f %f %f\n"
            "%f %f %f %f\n",
            mat.get<0, 0>(), mat.get<0, 1>(), mat.get<0, 2>(), mat.get<0, 3>(),
            mat.get<1, 0>(), mat.get<1, 1>(), mat.get<1, 2>(), mat.get<1, 3>(),
            mat.get<2, 0>(), mat.get<2, 1>(), mat.get<2, 2>(), mat.get<2, 3>(),
            mat.get<3, 0>(), mat.get<3, 1>(), mat.get<3, 2>(), mat.get<3, 3>()
    );
}

TREECORE_ALN_BEGIN( 16 )
struct Widget
{
    TREECORE_ALIGNED_ALLOCATOR( Widget );

    Widget( float x, float y, float width, float height )
    {
        bound.x      = x;
        bound.y      = y;
        bound.width  = width;
        bound.height = height;

        trans.data[0].set<0>( width );
        trans.data[1].set<1>( height );
        trans.data[3].set<0>( x );
        trans.data[3].set<1>( y );

        printf( "after construction\n" );
        show_matrix( trans );
    }

    void set_position( float x, float y )
    {
        if (x != bound.x && y != bound.y)
        {
            bound.x = x;
            bound.y = y;

            trans.data[3].set<0>( x );
            trans.data[3].set<1>( y );
        }
    }

    void move( float dx, float dy )
    {
        bound.x += dx;
        bound.y += dy;

        trans.data[3] += treecore::SimdObject<float, 4>( dx, dy, 0.0f, 0.0f );
    }

    void bound_geometry_with_program()
    {
        VertexTemplate vtx_temp;
        vtx_temp.add_attrib( attr_desc_position, false, 0 );
        vtx_temp.add_attrib( attr_desc_color,    false, 0 );
        array = new VertexArray( vtx_buffer, idx_buffer, vtx_temp, program );
    }

    BBox bound;

    Mat4f trans;

    bool is_active  = false;
    bool is_pressed = false;
    treecore::RefCountHolder<Program>     program;
    treecore::RefCountHolder<GLBuffer>    vtx_buffer;
    treecore::RefCountHolder<GLBuffer>    idx_buffer;
    treecore::RefCountHolder<VertexArray> array;
    size_t n_vertex = 0;
    size_t n_index  = 0;

} TREECORE_ALN_END( 16 );

typedef bool (* is_inside_func)( float x, float y, const BBox& bound );

bool is_inside_rect( float x, float y, const BBox& bound )
{
    return !(x < bound.x || x > bound.x + bound.width || y < bound.y || y > bound.y + bound.height);
}

int window_w = 400;
int window_h = 400;

void build_up_sdl( SDL_Window** window, SDL_GLContext* context )
{
    SDL_Init( SDL_INIT_VIDEO & SDL_INIT_TIMER & SDL_INIT_EVENTS );

    printf( "create window\n" );
    *window = SDL_CreateWindow( "sdl_setup test", 50, 50, window_w, window_h, SDL_WINDOW_OPENGL );
    if (!*window)
    {
        fprintf( stderr, "error: failed to create window: %s\n", SDL_GetError() );
        exit( 1 );
    }

    printf( "create opengl context\n" );
    *context = SDL_GL_CreateContext( *window );
    if (!context)
    {
        fprintf( stderr, "error: failed to create GL context: %s\n", SDL_GetError() );
        exit( 1 );
    }

    SDL_GL_MakeCurrent( *window, *context );

    printf( "init glew\n" );
    {
        GLenum glew_err = glewInit();
        if (glew_err != GLEW_OK)
        {
            fprintf( stderr, "error: failed to init glew: %s\n", glewGetErrorString( glew_err ) );
            exit( 1 );
        }
    }
}

void show_buffer_info( GLenum target )
{
    int mapped     = -1;
    int size       = -1;
    int usage      = -1;
    int map_length = -1;
    int map_offset = -1;
    glGetBufferParameteriv( target, GL_BUFFER_MAPPED,     &mapped );
    glGetBufferParameteriv( target, GL_BUFFER_SIZE,       &size );
    glGetBufferParameteriv( target, GL_BUFFER_USAGE,      &usage );
    glGetBufferParameteriv( target, GL_BUFFER_MAP_LENGTH, &map_length );
    glGetBufferParameteriv( target, GL_BUFFER_MAP_OFFSET, &map_offset );

    printf( "mapped: %d at %d + %d, size %d, usage %d\n", mapped, map_offset, map_length, size, usage );
}

struct WidgetRenderer
{
    void add_widget( Widget* widget )
    {
        widgets_by_program.store( widget->program, widget );
    }

    void render()
    {
        HashMultiMap<Program*, Widget*>::Iterator it_program_widgets( widgets_by_program );
        while ( it_program_widgets.nextKey() )
        {
            Program* program        = it_program_widgets.key();
            Array<Widget*>& widgets = it_program_widgets.values();

            program->bind();

            GLint loc_matrix    = program->get_uniform_location( "matrix" );
            GLint loc_is_active = program->get_uniform_location( "is_active" );

            for (int i = 0; i < widgets.size(); i++)
            {
                Widget* widget = widgets[i];
                program->set_uniform( loc_is_active, widget->is_active );
                program->set_uniform( loc_matrix,    widget->trans );

                widget->array->bind();

                glDrawElements( GL_TRIANGLES, 6, GLTypeEnumHelper<IndexType>::value, nullptr );

                widget->array->unbind();
            }

            program->bind();
        }
    }

    HashMultiMap<Program*, Widget*> widgets_by_program;
};

vector<ColoredPoint> vertex_rect = {
    {1, 1, 0, 1,   1,   0,   0, 1},
    {1, 0, 0, 1,   0,   1,   0, 1},
    {0, 1, 0, 1,   0,   0,   1, 1},
    {0, 0, 0, 1, 0.5, 0.5, 0.5, 1},
};

vector<unsigned short> index_rect = {
    0, 2, 1,
    2, 3, 1
};

//vector<ColoredPoint> vertex_hex = {
//    {0.5, 1, 0, 1, 1, 0, 0, 1},
//    {1, 0, 0, 1, 1, 0, 0, 1},
//    {0.5, -1, 0, 1, 1, 0, 0, 1},
//    {-0.5, -1, 0, 1, 1, 0, 0, 1},
//    {-1, 0, 0, 1, 1, 0, 0, 1},
//    {-0.5, 1, 0, 1, 1, 0, 0, 1}
//};

//vector<unsigned short> index_hex = {
//    0, 2, 1,
//    0, 3, 2,
//    0, 5, 3,
//    5, 4, 3
//};

float iden_mat[16] = {1, 0, 0, 0,
                      0, 1, 0, 0,
                      0, 0, 1, 0,
                      0, 0, 0, 1};

WidgetRenderer renderer;

set<Widget*> pressed_widgets;

void on_mouse_motion( SDL_MouseMotionEvent& e )
{
    float x = float(e.x) / float(window_w) * 2 - 1;
    float y = -(float(e.y) / float(window_h) * 2 - 1);

    HashMultiMap<Program*, Widget*>::Iterator it_program_widgets( renderer.widgets_by_program );
    while ( it_program_widgets.next() )
    {
        Array<Widget*>& widgets = it_program_widgets.values();
        for (int i = 0; i < widgets.size(); i++)
        {
            if ( is_inside_rect( x, y, widgets[i]->bound ) )
            {
                widgets[i]->is_active = true;
            }
            else
            {
                widgets[i]->is_active = false;
            }
        }
    }

    float xrel = float(e.xrel) / float(window_w) * 2;
    float yrel = -float(e.yrel) / float(window_h) * 2;

    if ( pressed_widgets.size() )
    {
        for (Widget* widget : pressed_widgets)
        {
            widget->move( xrel, yrel );
        }
    }
}

void on_mouse_down( SDL_MouseButtonEvent& e )
{
    if (e.button == SDL_BUTTON_LEFT)
    {
        float x = float(e.x) / float(window_w) * 2 - 1;
        float y = -(float(e.y) / float(window_h) * 2 - 1);
        printf( "press at %f %f: %d\n", x, y, e.state );

        HashMultiMap<Program*, Widget*>::Iterator it_program_widgets( renderer.widgets_by_program );
        while ( it_program_widgets.next() )
        {
            Array<Widget*>& widgets = it_program_widgets.values();
            for (int i = 0; i < widgets.size(); i++)
            {
                Widget* widget = widgets[i];
                if ( is_inside_rect( x, y, widget->bound ) )
                {
                    printf( "  widget %p\n", widget );
                    widget->is_pressed = true;
                    pressed_widgets.insert( widget );
                }
            }
        }
        printf( "%lu widgets pressed\n", pressed_widgets.size() );
    }
}

void on_mouse_up( SDL_MouseButtonEvent& e )
{
    if (e.button == SDL_BUTTON_LEFT)
    {
        printf( "left released\n" );
        for (Widget* widget : pressed_widgets)
        {
            widget->is_pressed = false;
        }
        pressed_widgets.clear();
    }
}

//#ifdef TREEFACE_OS_WINDOWS
//int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
//#else
#undef main
int main( int argc, char** argv )
//#endif
{
    SDL_Window*   window  = nullptr;
    SDL_GLContext context = nullptr;
    build_up_sdl( &window, &context );

    RefCountHolder<Program> program = new Program( src_vertex, src_fragment );

    RefCountHolder<GLBuffer> buf_vtx = new GLBuffer( TFGL_BUFFER_VERTEX, TFGL_BUFFER_STATIC_DRAW );
    RefCountHolder<GLBuffer> buf_idx = new GLBuffer( TFGL_BUFFER_INDEX, TFGL_BUFFER_STATIC_DRAW );

    buf_vtx->bind();
    buf_vtx->upload_data( vertex_rect.data(), vertex_rect.size() * sizeof(ColoredPoint) );
    buf_vtx->unbind();

    buf_idx->bind();
    buf_idx->upload_data( index_rect.data(), index_rect.size() * sizeof(short) );
    buf_idx->unbind();

    Logger::writeToLog( "geometry vertex: " + String( buf_vtx->get_gl_handle() ) + ", index: " + String( buf_idx->get_gl_handle() ) );

    vector<Widget*> widgets;

    for (int i = 0; i < 1; i++)
    {
        float x = -1 + 2 * float(i) / 10;
        for (int j = 0; j < 1; j++)
        {
            float   y      = -1 + 2 * float(j) / 10;
            Widget* widget = new Widget( x, y, 0.15f, 0.1f );
            widget->program     = program;
            widget->vtx_buffer = buf_vtx;
            widget->idx_buffer = buf_idx;
            widget->n_vertex    = vertex_rect.size();
            widget->n_index     = index_rect.size();
            widget->bound_geometry_with_program();
            renderer.add_widget( widget );
        }
    }

    while (1)
    {
        // process events
        SDL_Event event{};
        bool      should_exit = false;

        while ( SDL_PollEvent( &event ) )
        {
            switch (event.type)
            {
            case SDL_QUIT:
                should_exit = true;
                break;
            case SDL_MOUSEMOTION:
                on_mouse_motion( event.motion );
                break;
            case SDL_MOUSEBUTTONDOWN:
                on_mouse_down( event.button );
                break;
            case SDL_MOUSEBUTTONUP:
                on_mouse_up( event.button );
                break;
            }

            if (event.type == SDL_QUIT)
            {
                should_exit = true;
            }
        }

        if (should_exit) break;

        // do render
        glClear( GL_COLOR_BUFFER_BIT );

        renderer.render();

        SDL_GL_SwapWindow( window );
        SDL_Delay( 20 );
    }

    SDL_GL_DeleteContext( context );
    SDL_Quit();

    return 0;
}
