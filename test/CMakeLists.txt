set(SOURCE_FILES main.c
)

add_executable(oauth_test ${SOURCE_FILES})
target_link_libraries(oauth_test oauth)
