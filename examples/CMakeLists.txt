file(GLOB resource_files RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*")

ZIP_FILES(resource.zip ${resource_files})
