#ifndef TREEFACE_WINDOW_H
#define TREEFACE_WINDOW_H

#include "treeface/enums.h"
#include "treeface/math/vec2.h"

#include <treecore/IntTypes.h>
#include <treecore/RefCountObject.h>

namespace treeface
{

/**
 * @brief platform-independent window wrapper
 *
 * Note: this class is not a widget. Instead, widgets are run inside Window objects.
 */
class NativeWindow: public treecore::RefCountObject
{
public:
    ///
    /// \brief create a resizable window with default parameters and 1x1 size
    ///
    NativeWindow();

    ///
    /// \brief create window object from previously created native window
    /// \param data   native window pointer or handle
    ///
    NativeWindow(void* data);

    ///
    /// \brief create window at specified position with specified size and properties
    /// \param position   window position
    /// \param size       window size
    /// \param resizable  whether window can be resized
    ///
    NativeWindow(const Vec2i& position, const Vec2i& size, bool resizable = true);

    virtual ~NativeWindow();

    TREECORE_DECLARE_NON_COPYABLE(NativeWindow);
    TREECORE_DECLARE_NON_MOVABLE(NativeWindow);

    //static Window* get_active_window();

    ///
    /// \brief get window's full screen mode
    /// \return full screen mode
    /// \see FullScreenMode
    ///
    FullScreenMode get_full_screen_mode() const;

    Vec2i get_max_size() const;

    Vec2i get_min_size() const;

    Vec2i get_position() const;

    Vec2i get_size() const;

    treecore::String get_title() const;

    void grab();

    bool has_decoration() const;

    void hide();

    ///
    /// \brief whether the window has grabbed input devices
    /// \return true if grabbed, otherwise false
    /// \see grab() ungrab()
    ///
    bool is_grabbed() const;

    bool is_hidden() const;

    bool is_maximized() const;

    bool is_minimized() const;

    bool is_resizable() const;

    ///
    /// \brief make this window's OpenGL context to be current context
    ///
    void make_gl_context_to_current();

    void maximize();

    void minimize();

    void raise();

    void set_decoration(bool value);

    void set_full_screen_mode(FullScreenMode mode);

    void set_gl_swap_vsync(bool value);

    void set_max_size(Vec2i value);

    void set_min_size(Vec2i value);

    void set_position(Vec2i value);

    void set_size(Vec2i value);

    void set_title(const treecore::String& title_text);

    void show();

    ///
    /// \brief swap OpenGL front and back buffer of this window
    ///
    void swap_gl_buffer();

    void ungrab();

private:
    class Impl;
    Impl* m_impl;
};

} // namespace treeface


#endif // TREEFACE_WINDOW_H
