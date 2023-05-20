cmake_minimum_required(VERSION 3.16)
project(code)

set(CMAKE_CXX_STANDARD 14)

add_executable(code
        database_copy.hpp
        exceptions.hpp
        main.cpp
        map.hpp
        ticket_left.hpp
        ticket.hpp
        Tokenscanner.hpp
        trains.hpp
        users.hpp
        utility.hpp
        vector.hpp
        )
