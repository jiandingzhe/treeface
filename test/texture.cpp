#include <SDL.h>

#define GLEW_STATIC
#include <GL/glew.h>

#include <treejuce/Logger.h>
#include <treejuce/MathsFunctions.h>
#include <treejuce/Result.h>
#include <treejuce/StringRef.h>

#include "treeface/gl/imageref.h"
#include "treeface/gl/program.h"
#include "treeface/gl/sampler.h"
#include "treeface/gl/texture.h"
#include "treeface/gl/vertexindexbuffer.h"
#include "treeface/gl/vertexarray.h"

using namespace treejuce;
using namespace treeface;

const float data_vertices[] = {
    -0.5, -0.5, 0, 1, 0, 0,
     0.5, -0.5, 0, 1, 1, 0,
    -0.5,  0.5, 0, 1, 0, 1,
     0.5,  0.5, 0, 1, 1, 1
};

const uint16 data_indices[6] = {0, 1, 2, 2, 1, 3};

treeface::VertexIndexBuffer buffer;
treeface::VertexArray vertex_array;
treeface::Texture texture;
treeface::Texture texture_fake;
treeface::Program program;

int window_w = 400;
int window_h = 400;

uint8 data_texture1[] = {
    0xff, 0, 0,    0xff,    0,    0xff, 0,    0xff,    0,    0,    0xff, 0xff,
    0xff, 0, 0xff, 0xff,    0xff, 0xff, 0,    0xff,    0,    0xff, 0xff, 0xff,
    0,    0, 0,    0xff,    0x7f, 0x7f, 0x7f, 0xff,    0xff, 0xff, 0xff, 0xff,
};

ImageRef img_texture1{GL_RGBA, GL_UNSIGNED_BYTE, 3, 3, data_texture1};


//float data_texture[] = {
//    1,0,0,1,  0,1,0,1,  0,0,1,1,
//    1,0,1,1,  1,1,0,1,  0,1,1,1,
//    0,0,0,1,  0.5,0.5,0.5,1, 1,1,1,1
//};

const char* src_vertex =
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

const char* src_fragment =
        "#version 130\n"
        "in vec2 frag_tex_position;\n"
        "uniform sampler2D tex_sampler;\n"
        "void main()\n"
        "{\n"
//        "    gl_FragColor = vec4(frag_tex_position, 0, 1);"
        "    gl_FragColor = texture(tex_sampler, frag_tex_position);"
        "}\n"
        ;

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
    Result program_re = program.init(src_vertex, src_fragment);
    if (!program_re)
    {
        Logger::writeToLog(program_re.getErrorMessage());
        abort();
    }

    buffer.init();
    buffer.load_data(data_vertices, 24, data_indices, 6);

    glActiveTexture(GL_TEXTURE1);
    texture.init();
    texture_fake.init();
    texture.set_image_data(img_texture1, GL_RGBA, false);
    texture.set_min_filter(GL_NEAREST);
    texture.set_mag_filter(GL_NEAREST);

    VertexArray::AttrDesc attr1 = {"position",     0,               4, GL_FLOAT, false};
    VertexArray::AttrDesc attr2 = {"tex_position", sizeof(float)*4, 2, GL_FLOAT, false};
    Array<VertexArray::AttrDesc> attributes;
    attributes.add(attr1);
    attributes.add(attr2);
    vertex_array.init(buffer, sizeof(float)*6, attributes, program);
}

void main_loop(SDL_Window* window)
{
    int uni_tex = program.get_uniform_index_by_name("tex_sampler");
    Logger::outputDebugString("set to texture uniform: " + String(uni_tex));

    glActiveTexture(GL_TEXTURE0);
    Sampler sampler(GL_TEXTURE0);

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
        vertex_array.use();
        printf("  use buffer\n");
        buffer.use();

        glActiveTexture(GL_TEXTURE0);
        printf("  use texture\n");
        texture.use();

        printf("  use program\n");
        program.use();
        printf("  set sampler %u to uniform %d\n", 0, uni_tex);
        program.instant_set_uniform(uni_tex, int(0));

        printf("  draw using index buffer\n");
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);

        program.unuse();
        vertex_array.unuse();
        buffer.unuse();
        texture.unuse();
        glBindSampler(0, 0);

        SDL_GL_SwapWindow(window);
        SDL_Delay(500);
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
