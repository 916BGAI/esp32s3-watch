file(GLOB_RECURSE SRC_SOURCES components/*.c images/*.c screens/*.c app/*.c)

idf_component_register(SRCS "ui_helpers.c" "ui.c" ${SRC_SOURCES}
                       INCLUDE_DIRS "." "app" "screens"
                       REQUIRES "lvgl" "main")
