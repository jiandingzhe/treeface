#ifndef TREEFACE_PACKAGE_MANAGER_H
#define TREEFACE_PACKAGE_MANAGER_H

#include <treecore/MathsFunctions.h>
#include <treecore/Singleton.h>
#include <treecore/String.h>
#include <treecore/Time.h>

#include "treeface/common.h"

class TestFramework;

namespace treecore {
class File;
class InputStream;
class Result;
class ZipFile;
class MemoryBlock;
} // namespace treecore

namespace treeface {

class PackageManager
{
    friend class ::TestFramework;
public:
    typedef enum
    {
        KEEP_EXISTING,
        OVERWRITE,
        USE_OLDER,
        USE_NEWER
    } PackageItemConflictPolicy;

    juce_DeclareSingleton(PackageManager, false);

    JUCE_DECLARE_NON_COPYABLE(PackageManager);
    JUCE_DECLARE_NON_MOVABLE(PackageManager);

    /**
     * @brief add a zip package into resource manager
     * @param pkg: the zip package object. It will be owned by resource manager,
     *        and when ResourceManager object is destructed, this ZipFile object
     *        will also be deleted. If the package object already added to the
     *        manager, the function will return immediately.
     * @param pol: how to act when the name of an item in the newly added
     *        package already exists in the resource manager.
     */
    void add_package(treecore::ZipFile* pkg, PackageItemConflictPolicy pol);

    /**
     * @brief add a zip package by specifying zip data in memory.
     * @param zip_data: the pointer to the raw data of a zip archive.
     * @param zip_data_size: size of the raw data.
     * @param pol: how to act when the name of an item in the newly added
     *        package already exists in the resource manager.
     */
    void add_package(const void* zip_data, size_t zip_data_size, PackageItemConflictPolicy pol);

    /**
     * @brief add a zip package by specifying zip file.
     * @param zip_file_name: the zip file.
     * @param pol: how to act when the name of an item in the newly added
     *        package already exists in the resource manager.
     */
    void add_package(const treecore::File& zip_file, PackageItemConflictPolicy pol);

    /**
     * @brief get a stream which can used for reading an item
     * @param name: item name
     * @return a stream from which data can be accessed. User is resposible for
     *         delete the stream object when it is no longer needed.
     */
    treecore::InputStream* get_item_stream(const treecore::String& name);

    /**
     * @brief get the raw data of an item
     * @param name: item name
     * @param data: the place to store data. Existing data in this memory block
     *              object will be erased. A trailing '0' will be appended after
     *              the end of data, so that it can be directly used as a C
     *              string. As a result, the size of memory block will be set to
     *              data size + 1, be careful with that.
     * @return ok if success, otherwise fail.
     */
    treecore::Result get_item_data(const treecore::String& name,
                                   treecore::MemoryBlock& data);

    bool has_resource(const treecore::String& name) const NOEXCEPT;

protected:
    PackageManager();
    ~PackageManager();

    struct Impl;

    Impl* m_impl = nullptr;
}; // class ResourceManage

} // namespace treeface

#endif // TREEFACE_PACKAGE_MANAGER_H
