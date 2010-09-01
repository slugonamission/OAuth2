set(SOURCE_FILES oauth2.c
)

add_library(OAuth SHARED ${SOURCE_FILES})
target_link_libraries(OAuth curl)
