include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG main
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(DummyTests Dummy.cpp)
target_link_libraries(DummyTests EugenChess gtest gtest_main)

add_test(NAME DummyTests COMMAND DummyTests)
