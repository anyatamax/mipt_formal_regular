add_executable(Test main_test.cpp test_regular.cpp test_regular.h)
target_link_libraries(Test ${GTEST_LIBRARIES} ${GMOCK_BOTH_LIBRARIES} gcov Threads::Threads Library)