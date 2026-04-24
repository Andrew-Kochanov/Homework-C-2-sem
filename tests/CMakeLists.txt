# Исполняемый файл для тестирования
add_executable(testDFA testDFA.c)

# Линкуем с библиотекой
target_link_libraries(testDFA PRIVATE dfaLib)

# Добавляем тест в CTest
add_test(NAME DFAtest COMMAND testDFA)