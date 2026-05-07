cmake_minimum_required(VERSION 4.2)
project(word_frequency_counter C)

set(CMAKE_C_STANDARD 99)

add_executable(word_frequency_counter main.c
        hashtable.c
        hashtable.h
        string_utils.c
        string_utils.h
)
