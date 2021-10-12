#include "../include/regular.h"

const char* RegularException::what() const noexcept {
    return error_.c_str();
}

std::unordered_map<int, int> BinaryOperationRegular::GetConcatination() {
    for (int j = 0; j <= count_letter_; ++j) {
        if (expression_first_.find(j) != expression_first_.end()) {
            for (int p = 0; p <= count_letter_; ++p) {
                if (expression_second_.find(p) != expression_second_.end()) {
                    int key = j + p;
                    if (key > count_letter_) {
                        break;
                    }
                    if (expression_result_.find(key) == expression_result_.end()) {
                        expression_result_[key] = expression_first_[j] + expression_second_[p];
                    } else {
                        expression_result_[key] = std::min(expression_result_[key], expression_first_[j] + expression_second_[p]);
                    }
                }
            }
        }
    }
    return expression_result_;
}

std::unordered_map<int, int> BinaryOperationRegular::GetAddition() {
    for (int j = 0; j <= count_letter_; ++j) {
        if (expression_first_.find(j) != expression_first_.end() && expression_second_.find(j) != expression_second_.end()) {
            expression_result_[j] = std::min(expression_first_[j], expression_second_[j]);
        } else if (expression_first_.find(j) != expression_second_.end()) {
            expression_result_[j] = expression_first_[j];
        } else if (expression_second_.find(j) != expression_second_.end()) {
            expression_result_[j] = expression_second_[j];
        }
    }
    return expression_result_;
}

std::unordered_map<int, int> UnaryOperationRegular::GetOperation() {
    expression_result_[0] = 0;
    for (int j = 1; j <= count_letter_; ++j) {
        bool flag_find_another_result = false;
        int min_j = FindMin(j, flag_find_another_result);
        if (expression_old_.find(j) != expression_old_.end() && flag_find_another_result) {
            expression_result_[j] = std::min(expression_old_[j], min_j);
        } else if (expression_old_.find(j) != expression_old_.end()) {
            expression_result_[j] = expression_old_[j];
        } else if (flag_find_another_result) {
            expression_result_[j] = min_j;
        }
    }
    return expression_result_;
}

int UnaryOperationRegular::FindMin(int index, bool& flag_find) {
    int min_res = MAXVALUE;
    for (int p = 1; p <= index / 2; ++p) {
        if (expression_result_.find(p) != expression_result_.end() && expression_result_.find(index - p) != expression_result_.end()) {
            min_res = std::min(min_res, expression_result_[p] + expression_result_[index - p]);
            flag_find = true;
        }
    }
    return min_res;
}

RegularParse& RegularParse::operator=(RegularParse&& other) noexcept {
    if (this != &other) {
        regular_expression_ = std::move(other.regular_expression_);
        letter_for_find_ = other.letter_for_find_;
        count_letter_ = other.count_letter_;
        buffer_regular_ = std::move(other.buffer_regular_);
        other.letter_for_find_ = ' ';
        other.count_letter_ = 0;
    }
    return *this;
}

RegularParse& RegularParse::operator=(const RegularParse& other) {
    if (this != &other) {
        regular_expression_ = other.regular_expression_;
        letter_for_find_ = other.letter_for_find_;
        count_letter_ = other.count_letter_;
        buffer_regular_ = other.buffer_regular_;
    }
    return *this;
}

std::istream& operator>>(std::istream& in, RegularParse& regular_expression) {
    in >> regular_expression.regular_expression_;
    in >> regular_expression.letter_for_find_;
    in >> regular_expression.count_letter_;
    if (!(regular_expression.letter_for_find_ == 'a' || regular_expression.letter_for_find_ == 'b' || regular_expression.letter_for_find_ == 'c')) {
        throw RegularException("Invalid syntax");
    }
    regular_expression.ParsingProcess();
    return in;
}

std::ostream& operator<<(std::ostream& out, const RegularParse& regular_expression) {
    int answer = regular_expression.GetAnswer();
    if (answer == -1) {
        out << "INF\n";
    } else {
        out << answer << "\n";
    }
    return out;
}

std::string RegularParse::ParsingRegular() {
    if (!(letter_for_find_ == 'a' || letter_for_find_ == 'b' || letter_for_find_ == 'c')) {
        throw RegularException("Invalid syntax");
    }
    ParsingProcess();
    int result = GetAnswer();
    if (result == -1) {
        return "INF";
    } else {
        std::string  result_str = std::to_string(result);
        return result_str;

    }
}

void RegularParse::ParsingProcess() {
    for (char i : regular_expression_) {
        if (isalpha(i)) {
            LetterCase(i);
        } else if (i == '1') {
            EmptyLetterCase();
        } else if (i == '.') {
            Concatenation();
        } else if (i == '+') {
            Addition();
        } else if (i == '*') {
            CliniClosure();
        } else {
            throw RegularException("Invalid syntax");
        }
    }
}

void RegularParse::LetterCase(char letter) {
    if (letter == letter_for_find_) {
        if (count_letter_ >= 1) {
            std::unordered_map<int, int> new_value;
            new_value[1] = 1;
            buffer_regular_.push(new_value);
        }
    } else if (letter == 'a' || letter == 'b' || letter == 'c') {
        std::unordered_map<int, int> new_value;
        new_value[0] = 1;
        buffer_regular_.push(new_value);
    } else {
        throw RegularException("Invalid syntax");
    }
}

void RegularParse::EmptyLetterCase() {
    std::unordered_map<int, int> new_value;
    new_value[0] = 0;
    buffer_regular_.push(new_value);
}

void RegularParse::Concatenation() {
    std::unordered_map<int, int> expression_second = buffer_regular_.top();
    buffer_regular_.pop();
    std::unordered_map<int, int> expression_first = buffer_regular_.top();
    buffer_regular_.pop();
    BinaryOperationRegular conc_reg(expression_first, expression_second, count_letter_);
    buffer_regular_.push(conc_reg.GetConcatination());
}

void RegularParse::Addition() {
    std::unordered_map<int, int> expression_second = buffer_regular_.top();
    buffer_regular_.pop();
    std::unordered_map<int, int> expression_first = buffer_regular_.top();
    buffer_regular_.pop();
    BinaryOperationRegular add_reg(expression_first, expression_second, count_letter_);
    buffer_regular_.push(add_reg.GetAddition());
}

void RegularParse::CliniClosure() {
    std::unordered_map<int, int> expression_old = buffer_regular_.top();
    buffer_regular_.pop();
    UnaryOperationRegular clini_reg(expression_old, count_letter_);
    buffer_regular_.push(clini_reg.GetOperation());
}

int RegularParse::GetAnswer() const {
    if (buffer_regular_.size() != 1) {
        throw RegularException("Invalid polish notation of regular expression");
    }
    if (buffer_regular_.top().find(count_letter_) == buffer_regular_.top().end()) {
        return -1;
    }
    return buffer_regular_.top().at(count_letter_);
}
