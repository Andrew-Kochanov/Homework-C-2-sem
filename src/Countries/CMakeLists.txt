# Создание статической библиотеки
add_library(countriesLib STATIC countries.c)

# Указание заголовочного файла
target_include_directories(countriesLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Создание исполняемого файла из main-файла
add_executable(main main.c)

# Связка исполняемого файла с библиотекой
target_link_libraries(main PRIVATE countriesLib)