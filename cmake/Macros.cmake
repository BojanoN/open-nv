macro(subdirlist result curdir)


  file(GLOB children RELATIVE ${curdir} ${curdir}/*)
  set(dirlist "")

  message(STATUS "${children}")

  foreach(child ${children})
    if(is_directory ${curdir}/${child})
      list(append dirlist ${child})
    endif()
  endforeach()

  set(${result} ${dirlist})

endmacro()


macro(add_subdirectories curdir)

  subdirlist(subdirs ${curdir})

  foreach(subdir ${subdirs})
    add_subdirectory(${subdir})
  endforeach()

endmacro()
