#include <SDL.h>

#define GLEW_STATIC
#include <GL/glew.h>

#include <treejuce/Holder.h>
#include <treejuce/Logger.h>
#include <treejuce/MathsFunctions.h>
#include <treejuce/Result.h>
#include <treejuce/StringRef.h>

#include "treeface/gl/imageref.h"
#include "treeface/gl/program.h"
#include "treeface/gl/sampler.h"
#include "treeface/gl/texture.h"
#include "treeface/gl/vertexarray.h"
#include "treeface/gl/vertexattrib.h"
#include "treeface/gl/vertexindexbuffer.h"
#include "treeface/gl/vertextemplate.h"

using namespace treejuce;
using namespace treeface;

const float data_vertices[] = {
    -0.5, -0.5, 0, 1, 0, 0,
     0.5, -0.5, 0, 1, 1, 0,
    -0.5,  0.5, 0, 1, 0, 1,
     0.5,  0.5, 0, 1, 1, 1
};

const uint16 data_indices[6] = {0, 1, 2, 2, 1, 3};

Holder<treeface::VertexIndexBuffer> buffer;
Holder<treeface::VertexArray> vertex_array_tex;
Holder<treeface::VertexArray> vertex_array_simple;
Holder<treeface::Texture> texture;
Holder<treeface::Program> program_tex;
Holder<treeface::Program> program_simple;
int program_simple_uni = -1;

int window_w = 400;
int window_h = 400;

uint8 data_texture1[] = {
    0xff, 0, 0,    0xff,    0,    0xff, 0,    0xff,    0,    0,    0xff, 0xff,
    0xff, 0, 0xff, 0xff,    0xff, 0xff, 0,    0xff,    0,    0xff, 0xff, 0xff,
    0,    0, 0,    0xff,    0x7f, 0x7f, 0x7f, 0xff,    0xff, 0xff, 0xff, 0xff,
};

ImageRef img_texture1{GL_RGBA, GL_UNSIGNED_BYTE, 3, 3, data_texture1};

Mat4f offset(0.5, 0, 0, 0,
             0, 0.25, 0, 0,
             0, 0, 1, 0,
             0.4, 0.6, 0, 1);

//float data_texture[] = {
//    1,0,0,1,  0,1,0,1,  0,0,1,1,
//    1,0,1,1,  1,1,0,1,  0,1,1,1,
//    0,0,0,1,  0.5,0.5,0.5,1, 1,1,1,1
//};

const char* src_vertex_texture =
        "#version 130\n"
        "in vec4 position;\n"
        "in vec2 tex_position;\n"
        "out vec2 frag_tex_position;\n"
        "void main()\n"
        "{\n"
        "    gl_Position = position;\n"
        "    frag_tex_position = tex_position;\n"
        "}\n"
        ;

const char* src_fragment_texture =
        "#version 130\n"
        "in vec2 frag_tex_position;\n"
        "uniform sampler2D tex_sampler;\n"
        "void main()\n"
        "{\n"
//        "    gl_FragColor = vec4(frag_tex_position, 0, 1);"
        "    gl_FragColor = texture(tex_sampler, frag_tex_position);"
        "}\n"
        ;

const char* src_vertex_simple =
        "#version 130\n"
        "in vec4 position;\n"
        "uniform mat4 matrix;\n"
        ""
        "void main()\n"
        "{\n"
        "  gl_Position = matrix * position;\n"
        "}\n";

const char* src_fragment_simple =
        "#version 130\n"
        ""
        "void main()\n"
        "{\n"
        "  gl_FragColor = vec4(1, 0.5, 0, 1);\n"
        "}\n";

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

void build_up_gl()
{
    program_tex = new Program();
    {
        Result program_re = program_tex->build(src_vertex_texture, src_fragment_texture);
        if (!program_re)
            die("%s", program_re.getErrorMessage().toRawUTF8());
    }

    program_simple = new Program();
    {
        Result re = program_simple->build(src_vertex_simple, src_fragment_simple);
        if (!re)
            die("%s", re.getErrorMessage().toRawUTF8());
    }
    program_simple_uni = program_simple->get_uniform_index("matrix");
    if (program_simple_uni == -1)
        die("failed to get uniform");

    buffer = new VertexIndexBuffer();
    buffer->set_host_data(ArrayRef<const float>(data_vertices, 24), ArrayRef<const uint16>(data_indices, 6));

    glActiveTexture(GL_TEXTURE1);
    texture = new Texture();
    Result texture_re = texture->set_image_data(img_texture1, GL_RGBA, false);
    if (!texture_re)
    {
        die("texture image failed: "+texture_re.getErrorMessage());
    }
    texture->set_min_filter(GL_NEAREST);
    texture->set_mag_filter(GL_NEAREST);

    VertexAttrib attr1 = {"position",     4, GL_FLOAT};
    VertexAttrib attr2 = {"tex_position", 2, GL_FLOAT};
    VertexTemplate vtx_temp;
    vtx_temp.add_attrib(attr1, false);
    vtx_temp.add_attrib(attr2, false);

    vertex_array_tex = new VertexArray();
    {
        Result re = vertex_array_tex->build(buffer, vtx_temp, program_tex);
        if (!re)
            die("vertex array building failed: %s", re.getErrorMessage().toRawUTF8());
    }

    vertex_array_simple = new VertexArray();
    {
        Result re = vertex_array_simple->build(buffer, vtx_temp, program_simple);
        if (!re)
            die("vertex array building failed: %s", re.getErrorMessage().toRawUTF8());
    }
}

void main_loop(SDL_Window* window)
{
    int uni_tex = program_tex->get_uniform_index("tex_sampler");
    Logger::outputDebugString("set to texture uniform: " + String(uni_tex));

    glActiveTexture(GL_TEXTURE0);

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
            }

            if (event.type == SDL_QUIT)
            {
                should_exit = true;
            }
        }

        if (should_exit) break;

        // do render
        printf("clear color\n");
        glClear(GL_COLOR_BUFFER_BIT);

        printf("  use vertex array\n");
        vertex_array_tex->use();
        printf("  use buffer\n");
        buffer->use();

        glActiveTexture(GL_TEXTURE0);
        printf("  use texture\n");
        texture->use();

        printf("  use program\n");
        program_tex->use();
        printf("  set sampler %u to uniform %d\n", 0 , uni_tex);
        program_tex->instant_set_uniform(uni_tex, int(0));

        printf("  draw using index buffer\n");
        buffer->draw(GL_TRIANGLES);

        program_tex->unuse();
        texture->unuse();

        glBindSampler(0, 0);
        vertex_array_tex->unuse();
        buffer->unuse();

        vertex_array_simple->use();
        buffer->use();
        printf("  use program 2\n");
        program_simple->use();
        printf("  set program2 matrix to uniform %d\n", program_simple_uni);
        program_simple->instant_set_uniform(program_simple_uni, offset);

        printf("  draw using index buffer\n");
        buffer->draw(GL_TRIANGLES);

        vertex_array_simple->unuse();
        buffer->unuse();

        SDL_GL_SwapWindow(window);
        SDL_Delay(200);
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
    build_up_gl();

    main_loop(window);

    SDL_GL_DeleteContext(context);
    SDL_Quit();

    return 0;
}
