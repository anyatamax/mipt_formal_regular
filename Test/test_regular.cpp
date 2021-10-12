#include "test_regular.h"
#include <sstream>

TEST_F(TestRegular, ConstructorsAssigments) {
    std::string str = "abc..*";
    MockRegular str_first(str, 'b', 2);
    moc_reg.SetRegular(str);
    moc_reg.SetLetterAndCount('b', 2);
    EXPECT_EQ(str_first.GetCount(), moc_reg.GetCount());
    EXPECT_EQ(str_first.GetLetter(), moc_reg.GetLetter());
    EXPECT_EQ(str_first.GetReg(), moc_reg.GetReg());
    MockRegular str_second(moc_reg);
    EXPECT_EQ(str_second.GetCount(), moc_reg.GetCount());
    EXPECT_EQ(str_second.GetLetter(), moc_reg.GetLetter());
    EXPECT_EQ(str_second.GetReg(), moc_reg.GetReg());
    MockRegular str_third(std::move(moc_reg));
    EXPECT_EQ(str_third.GetCount(), 2);
    EXPECT_EQ(str_third.GetLetter(), 'b');
    EXPECT_EQ(str_third.GetReg(), "abc..*");
}

TEST_F(TestRegular, Letter) {
    moc_reg.SetLetterAndCount('b', 2);
    std::unordered_map<int, int> map;
    map[1] = 1;
    EXPECT_EQ(moc_reg.TestLetterCase('b'), map);
    try {
        moc_reg.TestLetterCase('k');
        FAIL();
    }
    catch(const RegularException& expected) {
        ASSERT_STREQ("Invalid syntax", expected.what());
    }
}

TEST_F(TestRegular, LetterOne) {
    std::unordered_map<int, int> map;
    map[0] = 0;
    EXPECT_EQ(moc_reg.TestEmptyLetterCase(), map);
}

TEST_F(TestRegular, Concatenation) {
    std::unordered_map<int, int> str_first;
    std::unordered_map<int, int> str_second;
    str_first[0] = 1;
    str_first[1] = 3;
    str_first[2] = 5;
    str_second[0] = 2;
    str_second[1] = 1;
    std::stack<std::unordered_map<int, int>> buffer_reg;
    buffer_reg.push(str_first);
    buffer_reg.push(str_second);
    moc_reg.SetBuffer(buffer_reg);
    moc_reg.SetLetterAndCount('b', 2);
    std::unordered_map<int, int> result;
    result[0] = 3;
    result[1] = 2;
    result[2] = 4;
    EXPECT_EQ(moc_reg.TestConcatenation(), result);
}

TEST_F(TestRegular, Addition) {
    std::unordered_map<int, int> str_first;
    std::unordered_map<int, int> str_second;
    str_first[0] = 3;
    str_first[1] = 2;
    str_first[2] = 4;
    str_second[0] = 2;
    str_second[1] = 2;
    std::stack<std::unordered_map<int, int>> buffer_reg;
    buffer_reg.push(str_first);
    buffer_reg.push(str_second);
    moc_reg.SetBuffer(buffer_reg);
    moc_reg.SetLetterAndCount('b', 2);
    std::unordered_map<int, int> result;
    result[0] = 2;
    result[1] = 2;
    result[2] = 4;
    EXPECT_EQ(moc_reg.TestAddition(), result);
    std::stack<std::unordered_map<int, int>> buffer_reg_other;
    buffer_reg.push(str_second);
    buffer_reg.push(str_first);
    moc_reg.SetBuffer(buffer_reg);
    EXPECT_EQ(moc_reg.TestAddition(), result);
}

TEST_F(TestRegular, CliniClosure) {
    std::unordered_map<int, int> str;
    str[1] = 3;
    str[2] = 3;
    str[4] = 9;
    str[5] = 9;
    str[7] = 15;
    str[8] = 15;
    std::stack<std::unordered_map<int, int>> buffer_reg;
    buffer_reg.push(str);
    moc_reg.SetBuffer(buffer_reg);
    moc_reg.SetLetterAndCount('b', 8);
    std::unordered_map<int, int> result;
    result[0] = 0;
    result[1] = 3;
    result[2] = 3;
    result[3] = 6;
    result[4] = 6;
    result[5] = 9;
    result[6] = 9;
    result[7] = 12;
    result[8] = 12;
    EXPECT_EQ(moc_reg.TestCliniClosure(), result);
}

TEST_F(TestRegular, GetAnswer) {
    std::unordered_map<int, int> str;
    str[1] = 3;
    str[2] = 3;
    std::stack<std::unordered_map<int, int>> buffer_reg;
    buffer_reg.push(str);
    moc_reg.SetBuffer(buffer_reg);
    moc_reg.SetLetterAndCount('b', 2);
    EXPECT_EQ(moc_reg.TestGetAnswer(), 3);
    moc_reg.SetLetterAndCount('b', 3);
    EXPECT_EQ(moc_reg.TestGetAnswer(), -1);
    std::unordered_map<int, int> str_second;
    str[1] = 1;
    buffer_reg.push(str_second);
    moc_reg.SetBuffer(buffer_reg);
    try {
        moc_reg.TestGetAnswer();
        FAIL();
    }
    catch(const RegularException& expected) {
        ASSERT_STREQ("Invalid polish notation of regular expression", expected.what());
    }
}

TEST_F(TestRegular, ParsingProcess) {
    std::string regular = "b1ab++.*";
    moc_reg.SetRegular(regular);
    moc_reg.SetLetterAndCount('b', 2);
    std::unordered_map<int, int> map_result;
    map_result[0] = 0;
    map_result[1] = 1;
    map_result[2] = 2;
    EXPECT_EQ(moc_reg.TestParsingProcess(), map_result);
    regular = "b1ab++.*k";
    moc_reg.SetRegular(regular);
    std::stack<std::unordered_map<int, int>> buffer_empty;
    moc_reg.SetBuffer(buffer_empty);
    try {
        map_result = moc_reg.TestParsingProcess();
        FAIL();
    }
    catch(const RegularException& expected) {
        ASSERT_STREQ("Invalid syntax", expected.what());
    }
}

TEST_F(TestRegular, TestParsingBased) {
    std::string regular = "ab+c.aba.*.bac.+.+*";
    moc_reg.SetLetterAndCount('b', 2);
    moc_reg.SetRegular(regular);
    ASSERT_EQ(moc_reg.ParsingRegular(), "4");
    std::stack<std::unordered_map<int, int>> buffer_empty;
    moc_reg.SetBuffer(buffer_empty);
    regular = "abb..*";
    moc_reg.SetLetterAndCount('b', 3);
    moc_reg.SetRegular(regular);
    EXPECT_EQ(moc_reg.ParsingRegular(), "INF");
    moc_reg.SetLetterAndCount('d', 1);
    moc_reg.SetBuffer(buffer_empty);
    try {
        moc_reg.ParsingRegular();
        FAIL();
    }
    catch(const RegularException& expected) {
        ASSERT_STREQ("Invalid syntax", expected.what());
    }
}

TEST_F(TestRegular, Stream) {
    std::stringstream str;
    str << "abb..* b 2";
    str >> moc_reg;
    std::stringstream str_1;
    str_1 << moc_reg;
    int x;
    str_1 >> x;
    EXPECT_EQ(x, 3);
}