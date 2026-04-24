# Создание статической библиотеки
add_library(dfaLib STATIC dfa.c)

# Указание заголовочного файла
target_include_directories(dfaLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Создание исполняемого файла из main-файла
add_executable(main main.c)

# Связка исполняемого файла с библиотекой
target_link_libraries(main PRIVATE dfaLib)