cmake_minimum_required(VERSION 3.3 FATAL_ERROR)

project(CreateMolecule)

find_package(VTK COMPONENTS 
  vtkCommonColor
  vtkCommonCore
  vtkFiltersCore
  vtkFiltersSources
  vtkCommonDataModel
  vtkDomainsChemistry
  vtkDomainsChemistryOpenGL2
  vtkIOGeometry
  vtkInteractionStyle
  vtkRenderingContextOpenGL2
  vtkRenderingCore
  vtkRenderingFreeType
  vtkRenderingGL2PSOpenGL2
  vtkRenderingLOD
  vtkRenderingOpenGL2 QUIET)
if (NOT VTK_FOUND)
  message("Skipping CreateMolecule: ${VTK_NOT_FOUND_MESSAGE}")
  return ()
endif()
message (STATUS "VTK_VERSION: ${VTK_VERSION}")
if (VTK_VERSION VERSION_LESS "8.90.0")
  # old system
  include(${VTK_USE_FILE})
  add_executable(CreateMolecule MACOSX_BUNDLE CreateMolecule.cxx )
  target_link_libraries(CreateMolecule PRIVATE ${VTK_LIBRARIES})
else ()
  # include all components
  add_executable(CreateMolecule MACOSX_BUNDLE CreateMolecule.cxx )
  target_link_libraries(CreateMolecule PRIVATE ${VTK_LIBRARIES})
  # vtk_module_autoinit is needed
  vtk_module_autoinit(
    TARGETS CreateMolecule
    MODULES ${VTK_LIBRARIES}
    )
endif () 
