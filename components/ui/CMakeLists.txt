file(GLOB_RECURSE SRC_SOURCES components/*.c fonts/*.c images/*.c screens/*.c)

idf_component_register(SRCS "ui_events.c" "ui_helpers.c" "ui.c" ${SRC_SOURCES}
                       INCLUDE_DIRS "." "components"
                       REQUIRES "lvgl")
