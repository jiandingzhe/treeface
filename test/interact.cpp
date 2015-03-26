#include <SDL.h>

#define GLEW_STATIC
#include <GL/glew.h>

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <set>
#include <map>

#include "treeface/config.h"
#include "treejuce/BasicNativeHeaders.h"

using namespace std;

struct BBox
{
    float x = 0;
    float y = 0;
    float width = 0;
    float height = 0;
};

struct Vector4F
{
    Vector4F()
    {
        data[0] = 0;
        data[1] = 0;
        data[2] = 0;
        data[3] = 1;
    }
    float data[4];
};

struct Matrix4F
{
    Matrix4F()
    {
        data[0] = 1;
        data[1] = 0;
        data[2] = 0;
        data[3] = 0;

        data[4] = 0;
        data[5] = 1;
        data[6] = 0;
        data[7] = 0;

        data[8] = 0;
        data[9] = 0;
        data[10] = 1;
        data[11] = 0;

        data[12] = 0;
        data[13] = 0;
        data[14] = 0;
        data[15] = 1;
    }

    float data[16];
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

struct ShaderProgram
{
    ShaderProgram()
    {
        shader_vertex = glCreateShader(GL_VERTEX_SHADER);
        shader_frag = glCreateShader(GL_FRAGMENT_SHADER);
        program = glCreateProgram();
    }

    const char* src_vertex()
    {
        return "#version 130\n"
               "in vec4 in_position;\n"
               "in vec4 in_color;\n"
               "out vec4 frag_color;\n"
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
    }

    const char* src_fragment()
    {
        return "#version 130\n"
               "in vec4 frag_color;\n"
               ""
               "void main()\n"
               "{\n"
               "  gl_FragColor = frag_color;\n"
               "}\n";
    }

    void init()
    {
        _do_compile(shader_vertex, src_vertex());
        _do_compile(shader_frag, src_fragment());

        glAttachShader(program, shader_vertex);
        glAttachShader(program, shader_frag);
        glLinkProgram(program);
        validate_program_state(GL_LINK_STATUS);

        attr_position = glGetAttribLocation(program, "in_position");
        attr_color = glGetAttribLocation(program, "in_color");
        uni_matrix = glGetUniformLocation(program, "matrix");
        uni_active = glGetUniformLocation(program, "is_active");
    }

    void _do_compile(GLuint shader, const char* src)
    {
        const char* code_array[] = {src};
        glShaderSource(shader, 1, code_array, nullptr);
        glCompileShader(shader);

        validate_shader_state(shader, GL_COMPILE_STATUS);
    }

    void validate_shader_state(GLuint shader, GLuint enum_name)
    {
        GLint result;
        glGetShaderiv(shader, enum_name, &result);

        if (result == GL_FALSE)
        {
            GLint log_len = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_len);
            if (log_len)
            {
                char* log = (char*) malloc(log_len);
                GLsizei written = 0;
                glGetShaderInfoLog(shader, log_len, &written, log);
                fprintf(stderr, "shader error: %s\n", log);
                free(log);
            }
            exit(1);
        }
    }

    void validate_program_state(GLuint enum_name)
    {
        GLint result = 0;
        glGetProgramiv(program, enum_name, &result);
        if (result == GL_FALSE)
        {
            GLint log_len = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_len);
            if (log_len)
            {
                char* log = (char*) malloc(log_len);
                GLsizei written = 0;
                glGetProgramInfoLog(program, log_len, &written, log);
                fprintf(stderr, "program error: %s\n", log);
                free(log);
            }
            exit(1);
        }
    }

    GLuint shader_vertex = 0;
    GLuint shader_frag = 0;
    GLuint program = 0;

    GLint attr_position = -1;
    GLint attr_color = -1;
    GLint uni_matrix = -1;
    GLint uni_active = -1;
};

struct Geometry
{
    Geometry()
    {
        glGenBuffers(1, &buffer_vertex);
        glGenBuffers(1, &buffer_index);
    }

    void set_data(const vector<ColoredPoint>& vertices, const vector<unsigned short>& indices)
    {
        n_vertex = vertices.size();
        n_index = indices.size();

        glBindBuffer(GL_ARRAY_BUFFER, buffer_vertex);
        glBufferData(GL_ARRAY_BUFFER, sizeof(ColoredPoint)*vertices.size(), vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_index);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short)*indices.size(), indices.data(), GL_STATIC_DRAW);
    }

    int n_vertex = 0;
    int n_index = 0;

    GLuint buffer_vertex = 0;
    GLuint buffer_index = 0;
};

struct Widget
{
    Widget(float x, float y, float width, float height)
    {
        bound.x = x;
        bound.y = y;
        bound.width = width;
        bound.height = height;

        trans.data[0] = width;
        trans.data[5] = height;
        trans.data[12] = x;
        trans.data[13] = y;

        glGenVertexArrays(1, &array);
    }

    void set_position(float x, float y)
    {
        if (x != bound.x && y != bound.y)
        {
            bound.x = x;
            bound.y = y;

            trans.data[12] = x;
            trans.data[13] = y;
        }
    }

    void move(float dx, float dy)
    {
        bound.x += dx;
        bound.y += dy;
        trans.data[12] += dx;
        trans.data[13] += dy;
    }

    void bound_geometry_with_program()
    {
        glBindVertexArray(array);
        glEnableVertexAttribArray(program->attr_position);
        glEnableVertexAttribArray(program->attr_color);

        glBindBuffer(GL_ARRAY_BUFFER, geometry->buffer_vertex);
        glVertexAttribPointer(program->attr_position, 4, GL_FLOAT, GL_FALSE, sizeof(ColoredPoint), 0);
        glVertexAttribPointer(program->attr_color, 4, GL_FLOAT, GL_FALSE, sizeof(ColoredPoint), (void*) (4 * sizeof(float)));
    }

    BBox bound;

    Matrix4F trans;

    bool is_active = false;
    bool is_pressed = false;
    ShaderProgram* program = nullptr;
    Geometry* geometry = nullptr;

    GLuint array = 0;
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

struct WidgetRenderer
{
    void add_widget(Widget* widget)
    {
        widgets_by_program.insert({widget->program, widget});
    }

    void render()
    {
        ShaderProgram* prev_program = nullptr;

        for (const pair<ShaderProgram*, Widget*>& value : widgets_by_program)
        {
            ShaderProgram* program = value.first;
            Widget* widget = value.second;

            if (prev_program != program)
            {
                prev_program = program;
                glUseProgram(program->program);
            }

            glBindBuffer(GL_ARRAY_BUFFER, widget->geometry->buffer_vertex);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, widget->geometry->buffer_index);
            glBindVertexArray(widget->array);

            glUniformMatrix4fv(program->uni_matrix, 1, GL_FALSE, widget->trans.data);
            glUniform1i(program->uni_active, widget->is_active);

            glDrawElements(GL_TRIANGLES, widget->geometry->n_index, GL_UNSIGNED_SHORT, 0);
        }
    }

    multimap<ShaderProgram*, Widget*> widgets_by_program;
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

vector<ColoredPoint> vertex_hex = {
    {0.5, 1, 0, 1, 1, 0, 0, 1},
    {1, 0, 0, 1, 1, 0, 0, 1},
    {0.5, -1, 0, 1, 1, 0, 0, 1},
    {-0.5, -1, 0, 1, 1, 0, 0, 1},
    {-1, 0, 0, 1, 1, 0, 0, 1},
    {-0.5, 1, 0, 1, 1, 0, 0, 1}
};

vector<unsigned short> index_hex = {
    0, 2, 1,
    0, 3, 2,
    0, 5, 3,
    5, 4, 3
};

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

    for (const pair<ShaderProgram*, Widget*>& value : renderer.widgets_by_program)
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

        for (const pair<ShaderProgram*, Widget*>& value : renderer.widgets_by_program)
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

#ifdef TREEFACE_OS_WINDOWS
int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#else
int main(int argc, char** argv)
#endif
{
    SDL_Window* window = nullptr;
    SDL_GLContext context = nullptr;
    build_up_sdl(&window, &context);

    ShaderProgram program;
    program.init();

    printf("position: %d, color: %d, matrix: %d, is_active: %d\n", program.attr_position, program.attr_color, program.uni_matrix, program.uni_active);

    Geometry geom;
    geom.set_data(vertex_rect, index_rect);
    printf("geometry vertex: %u, index: %u\n", geom.buffer_vertex, geom.buffer_index);

    vector<Widget*> widgets;

    for (int i = 0; i < 10; i++)
    {
        float x = -1 + 2 * float(i)/10;
        for (int j = 0; j < 10; j++)
        {
            float y = -1 + 2 * float(j) / 10;
            Widget* widget = new Widget(x, y, 0.15f, 0.1f);
            widget->program = &program;
            widget->geometry = &geom;
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
        SDL_Delay(10);
    }

    SDL_GL_DeleteContext(context);
    SDL_Quit();

    return 0;
}
