# The recommended way to collect sources in variable 
# PERLIN_SOURCES by explicitly specifying the source files
set  (PERLIN_SOURCES
      "Perlin.h"
      "Perlin.cpp"
      "main.cpp")

# Create named folders for the sources within the .vcproj
# Empty name lists them directly under the .vcproj
source_group("" FILES ${PERLIN_SOURCES})

# Set Properties->General->Configuration Type to Application(.exe)
# Creates Huffman.exe with the listed sources collected in the variables
# Adds sources to the Solution Explorer
add_executable (Perlin ${PERLIN_SOURCES})
 
# Creates a folder "executables" and adds target 
# project (Perlin.vcproj) under it
set_property(TARGET Perlin PROPERTY FOLDER "executables")
 
# Adds logic to INSTALL.vcproj to copy Perlin.exe to destination directory
install (TARGETS Perlin
         RUNTIME DESTINATION ${PROJECT_BINARY_DIR}/bin)