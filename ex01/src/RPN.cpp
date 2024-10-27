#include "RPN.hpp"
#include <algorithm>
#include <cctype>
#include <cstddef>

RPN::RPN(const std::string &line) {
  line_ = line;
  min_ = kDefaultMin_;
  max_ = kDefaultMax_;
}

RPN::RPN(const RPN &other) { *this = other; }

RPN::~RPN() {}

RPN &RPN::operator=(const RPN &other) {
  if (this != &other) {
    line_ = other.line_;
    min_ = other.min_;
    max_ = other.max_;
  }
  return *this;
}

long RPN::GetMin() const { return min_; }

void RPN::SetMin(long min) { min_ = min; }

long RPN::GetMax() const { return max_; }

void RPN::SetMax(long max) { max_ = max; }

bool RPN::IsOperator(char ch) const {
  return ch == '+' || ch == '-' || ch == '*' || ch == '/';
}

void RPN::AddToken(const std::string &token, Token::Type type) {
  if (token.empty()) {
    return;
  }
  tokens_.push_back(Token(token, type));
}

void RPN::Tokenize() {
  enum {
    sw_start = 0,
    sw_num,
    sw_sign,
    sw_str,
    sw_space,
    sw_operator
  } state = sw_start;
  const std::string &line = line_;
  size_t i = 0;
  std::string cur_val;
  while (i < line.size()) {
    char ch = line[i];

    switch (state) {
    case sw_start:
      if (std::isspace(static_cast<int>(ch))) {
        state = sw_space;
      } else if (std::isdigit(ch)) {
        state = sw_num;
      } else if (ch == '+' || ch == '-') {
        state = sw_sign;
        cur_val += ch;
        i++;
        break;
      } else if (IsOperator(ch)) {
        state = sw_operator;
      } else {
        state = sw_str;
      }
      break;
    case sw_space:
      if (!std::isspace(static_cast<int>(ch))) {
        state = sw_start;
        break;
      }
      ++i;
      break;
    case sw_num:
      if (!std::isdigit(ch)) {
        AddToken(cur_val, Token::KNum);
        state = sw_start;
        break;
      }
      cur_val += ch;
      ++i;
      break;
    case sw_sign:
      if (std::isdigit(ch)) {
        state = sw_num;
        cur_val += ch;
        ++i;
        break;
      }
      state = sw_operator;
      break;
    case sw_operator:
      AddToken(cur_val, Token::KOperator);
      ++i;
      state = sw_start;
      break;
    case sw_str:
      if (std::isspace(static_cast<int>(ch)) || IsOperator(ch)) {
        AddToken(cur_val, Token::KOther);
        state = sw_start;
        break;
      }
      cur_val += ch;
      break;
    }
  }
  if (!cur_val.empty()) {
    switch (state) {
    case sw_sign:
      AddToken(cur_val, Token::KOperator);
      break;
    case sw_num:
      AddToken(cur_val, Token::KNum);
      break;
    case sw_str:
      AddToken(cur_val, Token::KOther);
      break;
    default:
      break;
    }
  }
}
