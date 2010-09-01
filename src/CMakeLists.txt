set(SOURCE_FILES oauth2.c
)

add_library(oauth SHARED ${SOURCE_FILES})
target_link_libraries(oauth curl)
