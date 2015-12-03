#include "treeface/event/nativewindow.h"

#include <SDL.h>

#include <treecore/HashMap.h>

namespace treeface
{

typedef treecore::HashMap<SDL_Window*, NativeWindow*> WindowMap;
WindowMap& native_window_map() { static WindowMap map; return map; }

struct NativeWindow::Impl
{
    SDL_Window* window;
    SDL_GLContext context;
};

NativeWindow::NativeWindow(): m_impl(new NativeWindow::Impl())
{
    // create window
    m_impl->window = SDL_CreateWindow("", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1, 1, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if (!m_impl->window)
        die("failed to create SDL window: %s\n", SDL_GetError());

    // create OpenGL context
    m_impl->context = SDL_GL_CreateContext(m_impl->window);
    if (!m_impl->context)
        die("failed to create SDL OpenGL context: %s\n", SDL_GetError());

    // map SDL window and treeface window
    native_window_map()[m_impl->window] = this;
}

NativeWindow::NativeWindow(void* data)
    : m_impl(new NativeWindow::Impl())
{
    // create window
    m_impl->window = SDL_CreateWindowFrom(data);
    if (!m_impl->window)
        die("failed to create SDL window: %s\n", SDL_GetError());

    // it should have OpenGL enabled
    treecore::uint32 flags = SDL_GetWindowFlags(m_impl->window);
    if (!(flags & SDL_WINDOW_OPENGL))
        die("SDL window %p created from native window %p don't have OpenGL support", m_impl->window, data);

    // create OpenGL context
    m_impl->context = SDL_GL_CreateContext(m_impl->window);
    if (!m_impl->context)
        die("failed to create SDL OpenGL context: %s\n", SDL_GetError());

    // map SDL window and treeface window
    native_window_map()[m_impl->window] = this;
}

NativeWindow::NativeWindow(const Vec2i& position, const Vec2i& size, bool resizable)
{
    treecore::uint32 flags = 0;
    flags |= SDL_WINDOW_OPENGL;
    if (resizable)
        flags |= SDL_WINDOW_RESIZABLE;

    // create window
    m_impl->window = SDL_CreateWindow("", position.x, position.y, size.x, size.y, flags);
    if (!m_impl->window)
        die("failed to create SDL window: %s\n", SDL_GetError());

    // create OpenGL context
    m_impl->context = SDL_GL_CreateContext(m_impl->window);
    if (!m_impl->context)
        die("failed to create SDL OpenGL context: %s\n", SDL_GetError());

    // map SDL window and treeface window
    native_window_map()[m_impl->window] = this;
}

NativeWindow::~NativeWindow()
{
    if (m_impl)
    {
        SDL_DestroyWindow(m_impl->window);
        SDL_GL_DeleteContext(m_impl->context);
        delete m_impl;
    }
}

FullScreenMode NativeWindow::get_full_screen_mode() const
{
    Uint32 flags = SDL_GetWindowFlags(m_impl->window);
    if (flags & SDL_WINDOW_FULLSCREEN)
        return FULL_SCREEN_REAL;
    else if (flags & SDL_WINDOW_FULLSCREEN_DESKTOP)
        return FULL_SCREEN_FAKE;
    else
        return FULL_SCREEN_OFF;
}

Vec2i NativeWindow::get_max_size() const
{
    Vec2i result;
    SDL_GetWindowMaximumSize(m_impl->window, &result.x, &result.y);
    return result;
}

Vec2i NativeWindow::get_min_size() const
{
    Vec2i result;
    SDL_GetWindowMinimumSize(m_impl->window, &result.x, &result.y);
    return result;
}

Vec2i NativeWindow::get_position() const
{
    Vec2i result;
    SDL_GetWindowPosition(m_impl->window, &result.x, &result.y);
    return result;
}

Vec2i NativeWindow::get_size() const
{
    Vec2i result;
    SDL_GetWindowSize(m_impl->window, &result.x, &result.y);
    return result;
}

treecore::String NativeWindow::get_title() const
{
    return treecore::String(SDL_GetWindowTitle(m_impl->window));
}

void NativeWindow::grab()
{
    SDL_SetWindowGrab(m_impl->window, SDL_TRUE);
}

bool NativeWindow::has_decoration() const
{
    treecore::uint32 flags = SDL_GetWindowFlags(m_impl->window);
    return !bool(flags & SDL_WINDOW_BORDERLESS);
}

void NativeWindow::hide()
{
    SDL_HideWindow(m_impl->window);
}

bool NativeWindow::is_grabbed() const
{
    treecore::uint32 flags = SDL_GetWindowFlags(m_impl->window);
    return bool(flags & SDL_WINDOW_INPUT_GRABBED);
}

bool NativeWindow::is_hidden() const
{
    treecore::uint32 flags = SDL_GetWindowFlags(m_impl->window);
    return bool(flags & SDL_WINDOW_HIDDEN);
}

bool NativeWindow::is_maximized() const
{
    treecore::uint32 flags = SDL_GetWindowFlags(m_impl->window);
    return bool(flags & SDL_WINDOW_MAXIMIZED);
}

bool NativeWindow::is_minimized() const
{
    treecore::uint32 flags = SDL_GetWindowFlags(m_impl->window);
    return bool(flags & SDL_WINDOW_MINIMIZED);
}

bool NativeWindow::is_resizable() const
{
    treecore::uint32 flags = SDL_GetWindowFlags(m_impl->window);
    return bool(flags & SDL_WINDOW_RESIZABLE);
}

void NativeWindow::maximize()
{
    SDL_MaximizeWindow(m_impl->window);
}

void NativeWindow::minimize()
{
    SDL_MinimizeWindow(m_impl->window);
}

void NativeWindow::raise()
{
    SDL_RaiseWindow(m_impl->window);
}

void NativeWindow::set_decoration(bool deco)
{
    SDL_SetWindowBordered(m_impl->window, SDL_bool(deco));
}

void NativeWindow::set_full_screen_mode(FullScreenMode mode)
{
    SDL_Window* nat_win = (SDL_Window*) m_impl;

    switch(mode)
    {
    case treeface::FULL_SCREEN_OFF:
        SDL_SetWindowFullscreen(nat_win, 0);
        break;
    case treeface::FULL_SCREEN_FAKE:
        SDL_SetWindowFullscreen(nat_win, SDL_WINDOW_FULLSCREEN_DESKTOP);
        break;
    case treeface::FULL_SCREEN_REAL:
        SDL_SetWindowFullscreen(nat_win, SDL_WINDOW_FULLSCREEN);
        break;
    default:;
    }
}

void NativeWindow::make_gl_context_to_current()
{
    SDL_GL_MakeCurrent(m_impl->window, m_impl->context);
}

void NativeWindow::set_max_size(Vec2i value)
{
    SDL_SetWindowMaximumSize(m_impl->window, value.x, value.y);
}

void NativeWindow::set_min_size(Vec2i value)
{
    SDL_SetWindowMinimumSize(m_impl->window, value.x, value.y);
}

void NativeWindow::set_position(Vec2i value)
{
    SDL_SetWindowPosition(m_impl->window, value.x, value.y);
}

void NativeWindow::set_size(Vec2i value)
{
    SDL_SetWindowSize(m_impl->window, value.x, value.y);
}

void NativeWindow::set_title(const treecore::String& title_text)
{
    SDL_SetWindowTitle(m_impl->window, title_text.getCharPointer().getAddress());
}

void NativeWindow::show()
{
    SDL_ShowWindow((SDL_Window*) m_impl);
}

void NativeWindow::swap_gl_buffer()
{
    SDL_GL_SwapWindow(m_impl->window);
}

void NativeWindow::ungrab()
{
    SDL_SetWindowGrab(m_impl->window, SDL_FALSE);
}

} // namespace treeface
