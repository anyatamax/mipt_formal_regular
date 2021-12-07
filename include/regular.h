#include <iostream>
#include <string>
#include <stack>
#include <unordered_map>

class RegularParse;

class RegularException : public  std::exception {
public:
    RegularException(std::string err) : error_(err) {};
    const char* what() const noexcept override;

private:
    std::string error_;
};

class BinaryOperationRegular {
public:
    explicit BinaryOperationRegular(std::unordered_map<int, int>& first, std::unordered_map<int, int>& second, int count):
    expression_first_(first), expression_second_(second), count_letter_(count) {};
    friend RegularParse;

protected:
    std::unordered_map<int, int> expression_first_;
    std::unordered_map<int, int> expression_second_;
    std::unordered_map<int, int> expression_result_;
    int count_letter_;
    std::unordered_map<int, int> GetConcatination();
    std::unordered_map<int, int> GetAddition();
};

class UnaryOperationRegular {
public:
    explicit UnaryOperationRegular(std::unordered_map<int, int>& expression, int count) : expression_old_(expression), count_letter_(count) {};
    friend RegularParse;

protected:
    std::unordered_map<int, int> expression_old_;
    std::unordered_map<int, int> expression_result_;
    int count_letter_;
    const int MAXVALUE = 100000;
    int FindMin(int index, bool& flag_find);
    std::unordered_map<int, int> GetOperation();
};

class RegularParse {
public:
    RegularParse() = default;
    RegularParse(const RegularParse& other) = default;;
    RegularParse(RegularParse&& other) noexcept = default;
    explicit RegularParse(std::string& regular, char letter, int count) :
    regular_expression_(regular), letter_for_find_(letter), count_letter_(count) {};
    friend std::istream& operator>>(std::istream& in, RegularParse& regular_expression);
    friend std::ostream& operator<<(std::ostream& out, const RegularParse& regular_expression);
    std::string ParsingRegular();
    ~RegularParse() = default;

protected:
    std::string regular_expression_;
    char letter_for_find_ = ' ';
    int count_letter_ = 0;
    std::stack<std::unordered_map<int, int>> buffer_regular_;
    void ParsingProcess();
    void LetterCase(char letter);
    void EmptyLetterCase();
    void Concatenation();
    void Addition();
    void CliniClosure();
    int GetAnswer() const;
};
