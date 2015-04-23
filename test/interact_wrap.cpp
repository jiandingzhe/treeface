// should produce same result with interact.cpp
// use wrapped API in src/treeface/gl
#include <SDL.h>

#define GLEW_STATIC
#include <GL/glew.h>

#include <stdio.h>
#include <stdlib.h>

#include <set>

#include "treeface/gl/program.h"
#include "treeface/gl/vertexindexbuffer.h"
#include "treeface/gl/vertexarray.h"
#include "treeface/gl/vertexattrib.h"

#include <vector>
#include <set>
#include <map>

#include <treejuce/BasicNativeHeaders.h>
#include <treejuce/Holder.h>
#include <treejuce/Logger.h>

using namespace std;
using namespace treeface;
using namespace treejuce;

struct BBox
{
    float x = 0;
    float y = 0;
    float width = 0;
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

HostVertexAttrib attr_desc_position{"in_position", 4, GL_FLOAT, 0, false};
HostVertexAttrib attr_desc_color{"in_color", 4, GL_FLOAT, sizeof(float)*4, false};

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

struct Widget
{
    Widget(float x, float y, float width, float height)
    {
        bound.x = x;
        bound.y = y;
        bound.width = width;
        bound.height = height;

        simd_set_one<0, float>(trans.data[0], width);
        simd_set_one<1, float>(trans.data[1], height);
        simd_set_one<0, float>(trans.data[3], x);
        simd_set_one<1, float>(trans.data[3], y);
    }

    void set_position(float x, float y)
    {
        if (x != bound.x && y != bound.y)
        {
            bound.x = x;
            bound.y = y;

            simd_set_one<0, float>(trans.data[3], x);
            simd_set_one<1, float>(trans.data[3], y);
        }
    }

    void move(float dx, float dy)
    {
        bound.x += dx;
        bound.y += dy;

        trans.data[3] = simd_add<float>(trans.data[3], simd_set<float, 16>(dx, dy, 0, 0));
    }

    void bound_geometry_with_program()
    {
        Array<HostVertexAttrib> attr_desc_list;
        attr_desc_list.add(attr_desc_position);
        attr_desc_list.add(attr_desc_color);
        array.build(geom_buffer, attr_desc_list, program);
    }

    BBox bound;

    Mat4f trans;

    bool is_active = false;
    bool is_pressed = false;
    Holder<Program> program = nullptr;
    Holder<VertexIndexBuffer> geom_buffer = nullptr;
    size_t n_vertex = 0;
    size_t n_index = 0;

    VertexArray array;
};

typedef bool (*is_inside_func)(float x, float y, const BBox& bound);

bool is_inside_rect(float x, float y, const BBox& bound)
{
    return !(x < bound.x || x > bound.x + bound.width || y < bound.y || y > bound.y + bound.height);
}

int window_w = 400;
int window_h = 400;

void build_up_sdl(SDL_Window** window, SDL_GLContext* context)
{
    SDL_Init(SDL_INIT_VIDEO & SDL_INIT_TIMER & SDL_INIT_EVENTS);

    printf("create window\n");
    *window = SDL_CreateWindow("sdl_setup test", 50, 50, window_w, window_h, SDL_WINDOW_OPENGL);
    if (!*window)
    {
        fprintf(stderr, "error: failed to create window: %s\n", SDL_GetError());
        exit(1);
    }

    printf("create opengl context\n");
    *context = SDL_GL_CreateContext(*window);
    if (!context)
    {
        fprintf(stderr, "error: failed to create GL context: %s\n", SDL_GetError());
        exit(1);
    }

    SDL_GL_MakeCurrent(*window, *context);

    printf("init glew\n");
    {
        GLenum glew_err = glewInit();
        if (glew_err != GLEW_OK)
        {
            fprintf(stderr, "error: failed to init glew: %s\n", glewGetErrorString(glew_err));
            exit(1);
        }
    }
}

void show_buffer_info(GLenum target)
{
    int mapped = -1;
    int size = -1;
    int usage = -1;
    int map_length = -1;
    int map_offset = -1;
    glGetBufferParameteriv(target, GL_BUFFER_MAPPED, &mapped);
    glGetBufferParameteriv(target, GL_BUFFER_SIZE, &size);
    glGetBufferParameteriv(target, GL_BUFFER_USAGE, &usage);
    glGetBufferParameteriv(target, GL_BUFFER_MAP_LENGTH, &map_length);
    glGetBufferParameteriv(target, GL_BUFFER_MAP_OFFSET, &map_offset);

    printf("mapped: %d at %d + %d, size %d, usage %d\n", mapped, map_offset, map_length, size, usage);
}

struct WidgetRenderer
{
    void add_widget(Widget* widget)
    {
        widgets_by_program.insert({widget->program, widget});
    }

    void render()
    {
        Program* prev_program = nullptr;
        GLint i_matrix = -1;
        GLint i_is_active = -1;
        for (pair<Program*, Widget*> kv : widgets_by_program)
        {
            Program* program = kv.first;
            Widget* widget = kv.second;

            if (program != prev_program)
            {
                if (prev_program)
                    prev_program->unuse();

                prev_program = program;

//                program->use();

                i_matrix    = program->get_uniform_index("matrix");
                i_is_active = program->get_uniform_index("is_active");
            }

//            program->instant_set_uniform(i_is_active, widget->is_active);
//            program->instant_set_uniform(i_matrix, widget->trans);
            program->set_uniform(i_matrix, widget->trans);
            program->set_uniform(i_is_active, widget->is_active);
            program->use();

            widget->array.use();
            widget->geom_buffer->use();

            widget->geom_buffer->draw(GL_TRIANGLES);

            widget->array.unuse();
            widget->geom_buffer->unuse();
        }

        prev_program->unuse();
    }

    multimap<Program*, Widget*> widgets_by_program;
};

vector<ColoredPoint> vertex_rect = {
    {1, 1, 0, 1,  1, 0, 0, 1},
    {1, 0, 0, 1, 0, 1, 0, 1},
    {0, 1, 0, 1, 0, 0, 1, 1},
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

void on_mouse_motion(SDL_MouseMotionEvent& e)
{
    float x = float(e.x) / float(window_w) * 2 - 1;
    float y = -(float(e.y) / float(window_h) * 2 - 1);

    for (const pair<Program*, Widget*>& value : renderer.widgets_by_program)
    {
        if (is_inside_rect(x, y, value.second->bound))
        {
            value.second->is_active = true;
        }
        else
        {
            value.second->is_active = false;
        }
    }

    float xrel = float(e.xrel) / float(window_w) * 2;
    float yrel = -float(e.yrel) / float(window_h) * 2;

    if (pressed_widgets.size())
    {
        printf("move %lu widgets\n", pressed_widgets.size());
        for (Widget* widget : pressed_widgets)
        {
            printf("  move widget %p: %f, %f\n", widget, xrel, yrel);
            widget->move(xrel, yrel);
        }
    }
}

void on_mouse_down(SDL_MouseButtonEvent& e)
{
    if (e.button == SDL_BUTTON_LEFT)
    {
        float x = float(e.x) / float(window_w) * 2 - 1;
        float y = -(float(e.y) / float(window_h) * 2 - 1);
        printf("press at %f %f: %d\n", x, y, e.state);

        for (const pair<Program*, Widget*>& value : renderer.widgets_by_program)
        {
            if (is_inside_rect(x, y, value.second->bound))
            {
                printf("  widget %p\n", value.second);
                value.second->is_pressed = true;
                pressed_widgets.insert(value.second);
            }
        }
        printf("%lu widgets pressed\n", pressed_widgets.size());
    }
}

void on_mouse_up(SDL_MouseButtonEvent& e)
{
    if (e.button == SDL_BUTTON_LEFT)
    {
        printf("left released\n");
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
int main(int argc, char** argv)
//#endif
{
    SDL_Window* window = nullptr;
    SDL_GLContext context = nullptr;
    build_up_sdl(&window, &context);

    Holder<Program> program = new Program();
    program->build(src_vertex, src_fragment);

    Holder<VertexIndexBuffer> geom_buffer_rect = new VertexIndexBuffer();
    geom_buffer_rect->set_host_data(ArrayRef<const ColoredPoint>(vertex_rect.data(), vertex_rect.size()),
                                    ArrayRef<const uint16>(index_rect.data(), index_rect.size())
                                    );
    Logger::writeToLog("geometry vertex: "+String(geom_buffer_rect->get_vertex_buffer_id())+", index: "+String(geom_buffer_rect->get_index_buffer_id()));

    vector<Widget*> widgets;

    for (int i = 0; i < 10; i++)
    {
        float x = -1 + 2 * float(i)/10;
        for (int j = 0; j < 10; j++)
        {
            float y = -1 + 2 * float(j) / 10;
            Widget* widget = new Widget(x, y, 0.15f, 0.1f);
            widget->program = program;
            widget->geom_buffer = geom_buffer_rect;
            widget->n_vertex = vertex_rect.size();
            widget->n_index = index_rect.size();
            widget->bound_geometry_with_program();
            renderer.add_widget(widget);
        }
    }

    while (1)
    {
        // process events
        SDL_Event event{};
        bool should_exit = false;

        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                should_exit = true;
                break;
            case SDL_MOUSEMOTION:
                on_mouse_motion(event.motion);
                break;
            case SDL_MOUSEBUTTONDOWN:
                on_mouse_down(event.button);
                break;
            case SDL_MOUSEBUTTONUP:
                on_mouse_up(event.button);
                break;
            }

            if (event.type == SDL_QUIT)
            {
                should_exit = true;
            }
        }

        if (should_exit) break;

        // do render
        glClear(GL_COLOR_BUFFER_BIT);

        renderer.render();

        SDL_GL_SwapWindow(window);
        SDL_Delay(20);
    }

    SDL_GL_DeleteContext(context);
    SDL_Quit();

    return 0;
}
