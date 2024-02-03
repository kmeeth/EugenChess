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

add_executable(EngineTests Engine.cpp)
target_link_libraries(EngineTests EugenChess gtest gtest_main)
add_test(NAME EngineTests COMMAND EngineTests)

add_executable(OptionsListingTests
        uci/OptionsListing.cpp)
target_link_libraries(OptionsListingTests EugenChess gtest gtest_main)
add_test(NAME OptionsListingTests COMMAND OptionsListingTests)

add_executable(SetOptionTests
        uci/SetOption.cpp)
target_link_libraries(SetOptionTests EugenChess gtest gtest_main)
add_test(NAME SetOptionTests COMMAND SetOptionTests)

add_executable(RegisterTests
        uci/Register.cpp)
target_link_libraries(RegisterTests EugenChess gtest gtest_main)
add_test(NAME RegisterTests COMMAND RegisterTests)

add_executable(PositionTests
        uci/PositionTests.cpp)
target_link_libraries(PositionTests EugenChess gtest gtest_main)
add_test(NAME PositionTests COMMAND PositionTests)