set(SOURCE_FILES oauth2.c
                 curl_request.c
)

add_library(oauth SHARED ${SOURCE_FILES})
target_link_libraries(oauth curl)
