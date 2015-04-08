#ifndef TREEFACE_GEOMETRY_H
#define TREEFACE_GEOMETRY_H

#include "treeface/common.h"
#include "treeface/gl/vertexindexbuffer.h"

#include "treejuce/Array.h"
#include "treejuce/Holder.h"
#include "treejuce/MathsFunctions.h"
#include "treejuce/Object.h"

TREEFACE_NAMESPACE_BEGIN

class Geometry: public treejuce::Object
{
public:
    /**
     * @brief create empty geometry
     */
    Geometry();

    /**
     * @brief create geometry with data specified
     */
    template<typename VTX_T>
    Geometry(const treejuce::Array<VTX_T>& data_vertex,
             const treejuce::Array<treejuce::uint16>& data_index);

    /**
     * @brief set data
     */
    template<typename VTX_T>
    void set_data(const treejuce::Array<VTX_T>& data_vertex,
                  const treejuce::Array<treejuce::uint16>& data_index);

    /**
     * @brief check if data is modified and is out of sync with server side
     */
    inline bool is_dirty() const NOEXCEPT { return m_dirty; }

    /**
     * @brief send data to GL server side
     */
    void send_to_gl_server() NOEXCEPT;

protected:
    /**
     * @brief vertex buffer on host side, in raw binaries
     */
    treejuce::Array<treejuce::uint8> m_data_vertex;

    /**
     * @brief index buffer on host side. It seems ten thousands of points for
     * one shape is enough, so we use 16bit uint.
     */
    treejuce::Array<treejuce::uint16> m_data_index;

    /**
     * @brief the buffer on GL server side
     */
    VertexIndexBuffer m_gl_buffer;

    /**
     * @brief whether we need to send data to server side
     */
    bool m_dirty = true;
};

TREEFACE_NAMESPACE_END

#endif // TREEFACE_GEOMETRY_H
