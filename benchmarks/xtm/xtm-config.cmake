get_filename_component(SELF_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)
include(${SELF_DIR}/xtm.cmake)
get_filename_component(xtm_INCLUDE_DIRS "${SELF_DIR}/../../include" ABSOLUTE)
set(xtm_LIBRARIES xtm)
