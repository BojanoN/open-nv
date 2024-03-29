include(Macros)

set(sources
  ./cellblock.cpp
  ./esm/spel.cpp
  ./esm/lscr.cpp
  ./esm/pack.cpp
  ./esm/chip.cpp
  ./esm/navm.cpp
  ./esm/item.cpp
  ./esm/watr.cpp
  ./esm/weap.cpp
  ./esm/cams.cpp
  ./esm/wrld.cpp
  ./esm/tree.cpp
  ./esm/stat.cpp
  ./esm/anio.cpp
  ./esm/wthr.cpp
  ./esm/mset.cpp
  ./esm/ench.cpp
  ./esm/ccrd.cpp
  ./esm/idlm.cpp
  ./esm/sound.cpp
  ./esm/furn.cpp
  ./esm/land.cpp
  ./esm/cmny.cpp
  ./esm/aspc.cpp
  ./esm/term.cpp
  ./esm/cell.cpp
  ./esm/pgre.cpp
  ./esm/acti.cpp
  ./esm/ligh.cpp
  ./esm/cdck.cpp
  ./esm/achr.cpp
  ./esm/alch.cpp
  ./esm/cont.cpp
  ./esm/aloc.cpp
  ./esm/qust.cpp
  ./esm/mesg.cpp
  ./esm/chal.cpp
  ./esm/lvli.cpp
  ./esm/tes4.cpp
  ./esm/txst.cpp
  ./esm/pwat.cpp
  ./esm/ipct.cpp
  ./esm/bptd.cpp
  ./esm/utils.cpp
  ./esm/book.cpp
  ./esm/mgef.cpp
  ./esm/repu.cpp
  ./esm/dehy.cpp
  ./esm/armo.cpp
  ./esm/scol.cpp
  ./esm/hair.cpp
  ./esm/vtyp.cpp
  ./esm/addn.cpp
  ./esm/npc.cpp
  ./esm/micn.cpp
  ./esm/imad.cpp
  ./esm/crea.cpp
  ./esm/hdpt.cpp
  ./esm/imod.cpp
  ./esm/imgs.cpp
  ./esm/idle.cpp
  ./esm/amef.cpp
  ./esm/eczn.cpp
  ./esm/door.cpp
  ./esm/dobj.cpp
  ./esm/ingr.cpp
  ./esm/rcpe.cpp
  ./esm/ltex.cpp
  ./esm/clas.cpp
  ./esm/csno.cpp
  ./esm/acre.cpp
  ./esm/avif.cpp
  ./esm/rgdl.cpp
  ./esm/perk.cpp
  ./esm/cpth.cpp
  ./esm/ipds.cpp
  ./esm/debr.cpp
  ./esm/race.cpp
  ./esm/csty.cpp
  ./esm/clmt.cpp
  ./esm/slpd.cpp
  ./esm/record.cpp
  ./esm/eyes.cpp
  ./esm/tact.cpp
  ./esm/gras.cpp
  ./esm/refr.cpp
  ./esm/expl.cpp
  ./esm/ammo.cpp
  ./esm/rcct.cpp
  ./esm/lvlc.cpp
  ./esm/rads.cpp
  ./esm/musc.cpp
  ./esm/lgtm.cpp
  ./esm/glob.cpp
  ./esm/regn.cpp
  ./esm/fact.cpp
  ./esm/arma.cpp
  ./esm/note.cpp
  ./esm/proj.cpp
  ./esm/flst.cpp
  ./esm/lsct.cpp
  ./esm/efsh.cpp
  ./esm/scpt.cpp
  ./esm/mstt.cpp
  ./esm/subrecords/npc_common.cpp
  ./esm/subrecords/objectbounds.cpp
  ./esm/subrecords/scriptdata.cpp
  ./esm/subrecords/effect.cpp
  ./esm/subrecords/patroldata.cpp
  ./esm/subrecords/modeldata.cpp
  ./esm/subrecords/destructiondata.cpp
  ./esm/subrecords/armor_common.cpp
  ./esm/subrecords/condition.cpp
  ./esm/subrecords/instance_record_common.cpp
  ./esm/hung.cpp
  ./esm/gmst.cpp
  ./esm/reader.cpp
  ./esm/dial.cpp
  ./esm/headers.cpp
  ./engine.cpp
  ./bsa/bsa.cpp
  ./bsa/hash.cpp
  ./providers/fileprovider.cpp
  ./file/configuration.cpp
  ./file/configuration_manager.cpp
  ./file/reader.cpp
  ./util/strmanip.cpp
  ./util/stringutils.cpp
  ./util/compress.cpp
  ./gamedata.cpp
  ./resources/texture2D.cpp
  ./resources/resource.cpp
  ./resources/shader.cpp
  ./gameworld.cpp
  ./game/fnv.cpp
  ./main.cpp
  ./nif/nifdata.cpp
  ./nif/nifreader.cpp
  ./nif/nifpointer.cpp
  ./nif/nifactory.cpp
  ./nif/main/BSShaderPPLightingProperty.cpp
  ./nif/main/NiProperty.cpp
  ./nif/main/NiCollisionObject.cpp
  ./nif/main/BSDismemberSkinInstance.cpp
  ./nif/main/NiObject.cpp
  ./nif/main/NiSkinInstance.cpp
  ./nif/main/BSShaderTextureSet.cpp
  ./nif/main/enums.cpp
  ./nif/main/BSBound.cpp
  ./nif/main/NiTriBasedGeomData.cpp
  ./nif/main/NiTriStripsData.cpp
  ./nif/main/structs.cpp
  ./nif/main/NiIntegerExtraData.cpp
  ./nif/main/NiStringsExtraData.cpp
  ./nif/main/NiGeometryData.cpp
  ./nif/main/NiTriShapeData.cpp
  ./nif/main/NiSkinPartition.cpp
  ./nif/main/BSShaderLightingProperty.cpp
  ./nif/main/NiDynamicEffect.cpp
  ./nif/main/BSShaderProperty.cpp
  ./nif/main/NiMaterialProperty.cpp
  ./nif/main/NiAlphaProperty.cpp
  ./nif/main/NiExtraData.cpp
  ./nif/main/NiStringExtraData.cpp
  ./nif/main/NiAVObject.cpp
  ./nif/main/NiShadeProperty.cpp
  ./nif/main/NiFloatsExtraData.cpp
  ./nif/main/NiSkinData.cpp
  ./nif/main/NiFloatExtraData.cpp
  ./nif/main/AbstractAdditionalGeometryData.cpp
  ./nif/main/NiObjectNET.cpp
  ./nif/main/NiGeometry.cpp
  ./nif/main/NiTriBasedGeom.cpp
  ./nif/main/NiTriStrips.cpp
  ./nif/main/NiTriShape.cpp
  ./nif/main/bitfields.cpp
  ./nif/main/NiNode.cpp
  ./nif/main/BSXFlags.cpp
  ./nif/main/BSFadeNode.cpp
  ./nif/main/NiTimeController.cpp
  ./engine/video.cpp
  ./engine/audio.cpp
  ./openNV.cpp
  ./logc/log.cpp
  ./script/parser.cpp
  ./script/functions.cpp
  ./script/compile.cpp
  ./script/vm.cpp
  ./script/tokenizer.cpp
  ./script/types/base.cpp
  ./decoder/video.cpp
  ./decoder/video_audio.cpp
  ./decoder/av_utils.cpp
  ./decoder/audio.cpp
  )

add_executable(${PROJECT_NAME}
  "${sources}"
)

target_include_directories(${PROJECT_NAME} PRIVATE
  ${IncludeDirectories}
)

target_compile_options(${PROJECT_NAME} PUBLIC $<$<CONFIG:DEBUG>: -DDEBUG>)

if(NOT LINK_STATIC)
  message(STATUS "Using dynamic linking for libraries...")
  set(LibrarySuffix ".so")
else()
  message(STATUS "Using static linking for libraries...")
  set(LibrarySuffix ".a")
endif()

# Required libraries, linked statically or dynamically
list(APPEND RequiredLibraries
  z

  GLEW
  SDL2


  )

list(APPEND RequiredDynamicLibraries
  avutil
  swresample
  swscale
  avcodec
  avformat
  )


foreach(library IN LISTS RequiredLibraries)
  find_library(${library}  lib${library}${LibrarySuffix} PATHS "${LibraryDirectory}" REQUIRED)
endforeach()


list(APPEND LibraryPaths)

foreach(library IN LISTS RequiredLibraries)
  list(APPEND LibraryPaths "${LibraryDirectory}/lib${library}${LibrarySuffix}")
endforeach()


foreach(library IN LISTS RequiredDynamicLibraries)
  list(APPEND LibraryPaths "${LibraryDirectory}/lib${library}.so")
endforeach()


target_link_libraries(${PROJECT_NAME} ${LibraryPaths} pthread GL GLU openal dl)

add_subdirectories(".")

