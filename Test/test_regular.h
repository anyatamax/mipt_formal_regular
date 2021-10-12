#pragma once

#include "../include/regular.h"
#include <gtest/gtest.h>
#include <gmock/gmock.h>



class MockRegular : public RegularParse {
public:
    MockRegular() : RegularParse() {};
    MockRegular(const MockRegular& other) : RegularParse(other) {};
    MockRegular(MockRegular&& other) noexcept : RegularParse(other) {};
    explicit MockRegular(std::string& regular, char letter, int count) : RegularParse(regular, letter, count) {};
    std::stack<std::unordered_map<int, int>> GetBuffer() {
        return buffer_regular_;
    }
    void SetBuffer(std::stack<std::unordered_map<int, int>>& buff) {
        buffer_regular_ = buff;
    }
    void SetLetterAndCount(char letter, int count) {
        letter_for_find_ = letter;
        count_letter_ = count;
    }
    void SetRegular(std::string& str) {
        regular_expression_ = str;
    }
    std::unordered_map<int, int> TestLetterCase(char letter) {
        LetterCase(letter);
        return buffer_regular_.top();
    }
    std::unordered_map<int, int> TestEmptyLetterCase() {
        EmptyLetterCase();
        return buffer_regular_.top();
    }
    std::unordered_map<int, int> TestConcatenation() {
        Concatenation();
        return buffer_regular_.top();
    }
    std::unordered_map<int, int> TestAddition() {
        Addition();
        return buffer_regular_.top();
    }
    std::unordered_map<int, int> TestCliniClosure() {
        CliniClosure();
        return buffer_regular_.top();
    }
    std::unordered_map<int, int> TestParsingProcess() {
        ParsingProcess();
        return buffer_regular_.top();
    }
    int TestGetAnswer() {
        return GetAnswer();
    }

private:

};

class TestRegular : public ::testing::Test {
public:
    MockRegular moc_reg;
    std::string regular = "ab.*";
private:

};

