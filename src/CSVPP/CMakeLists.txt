# Создаём библиотеку из файла в поддиректории
add_library(CSVPPlib STATIC CSVPP.c)

# Указание заголовочного файла
target_include_directories(CSVPPlib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Создаём исполняемый файл из файла в другой поддиректории
add_executable(main main.c)

# Линкуем библиотеку
target_link_libraries(main PRIVATE CSVPPlib)
