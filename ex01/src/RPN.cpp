#include "RPN.hpp"
#include <cctype>
#include <cstddef>
#include <cstdio>
#include <iostream>
#include <limits>
#include <sstream>

RPN::RPN() {
  min_ = kDefaultMin_;
  max_ = kDefaultMax_;
}

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

void RPN::AddToken(std::string &token, Token::Type type) {
  if (token.empty()) {
    return;
  }
  tokens_.push_back(Token(token, type));
  token.clear();
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
      /*std::cerr << "sw_start" << "\n";*/
      if (std::isspace(static_cast<int>(ch))) {
        state = sw_space;
      } else if (std::isdigit(ch)) {
        state = sw_num;
      } else if (ch == '+' || ch == '-') {
        state = sw_sign;
        cur_val += ch;
        ++i;
        break;
      } else if (IsOperator(ch)) {
        cur_val += ch;
        state = sw_operator;
      } else {
        state = sw_str;
      }
      break;
    case sw_space:
      /*std::cerr << "sw_space" << "\n";*/
      if (!std::isspace(static_cast<int>(ch))) {
        state = sw_start;
        break;
      }
      ++i;
      break;
    case sw_num:
      /*std::cerr << "sw_num" << "\n";*/
      if (!std::isdigit(ch)) {
        AddToken(cur_val, Token::KNum);
        state = sw_start;
        break;
      }
      cur_val += ch;
      ++i;
      break;
    case sw_sign:
      /*std::cerr << "sw_sign" << "\n";*/
      if (std::isdigit(ch)) {
        state = sw_num;
        cur_val += ch;
        ++i;
        break;
      }
      state = sw_operator;
      --i; // operatorのindexに戻る
      break;
    case sw_operator:
      /*std::cerr << "sw_ope: cur_val: ";*/
      /*std::cerr << cur_val << "\n";*/
      AddToken(cur_val, Token::KOperator);
      ++i;
      state = sw_start;
      break;
    case sw_str:
      /*std::cerr << "sw_str" << "\n";*/
      if (std::isspace(static_cast<int>(ch)) || IsOperator(ch)) {
        AddToken(cur_val, Token::KOther);
        state = sw_start;
        break;
      }
      cur_val += ch;
      ++i;
      break;
    }
  }
  /*std::cerr << "cur val: " << cur_val << "\n";*/
  if (!cur_val.empty()) {
    switch (state) {
    case sw_sign:
      /*std::cerr << "ope token" << "\n";*/
      AddToken(cur_val, Token::KOperator);
      break;
    case sw_num:
      AddToken(cur_val, Token::KNum);
      break;
    case sw_str:
      AddToken(cur_val, Token::KOther);
      break;
    default:
      /*std::cerr << "def" << "\n";*/
      break;
    }
  }
  /*for (std::vector<Token>::iterator it = tokens_.begin(); it !=
   * tokens_.end();*/
  /*     ++it) {*/
  /*  std::cerr << "token=" << it->token_ << "\n";*/
  /*}*/
}

std::string RPN::Error(const std::string &msg) {
  return msg.empty() ? "error" : "error: " + msg;
}

void RPN::ParseNum(const std::string &str) {
  std::istringstream iss(str);
  long result;
  iss >> result;
  if (iss.fail() || iss.peek() != EOF) {
    throw std::runtime_error(Error());
  }
  if (result > max_ || result < min_) {
    throw std::runtime_error(Error("value out of range"));
  }
  rpn_stack_.push(result);
}

void RPN::Evaluate(char ope) {
  long rh = rpn_stack_.top();
  rpn_stack_.pop();
  long lh = rpn_stack_.top();
  rpn_stack_.pop();
  long max = std::numeric_limits<long>::max();
  long min = std::numeric_limits<long>::min();

  switch (ope) {
  case '+':
    // check overflow
    if ((lh > 0 && rh > 0 && lh > max - rh) ||
        (lh < 0 && rh < 0 && lh < min - rh)) {
      throw std::runtime_error(
          Error("result of the calculation is out of range"));
    }
    rpn_stack_.push(lh + rh);
    break;
  case '-':
    if ((lh > 0 && rh < 0 && lh > max + rh) ||
        (lh < 0 && rh > 0 && lh < min + rh)) {
      throw std::runtime_error(
          Error("result of the calculation is out of range"));
    }
    rpn_stack_.push(lh - rh);
    break;
  case '*': {
    long result = lh * rh;
    std::cerr << "result/ " << result << "\n";
    /**
     * check overflow
     * overflowしたときは、resultが0になるときがある
     *
     */
    if ((lh != 0 && rh != 0 && result == 0) ||
        (lh != 0 && rh != 0 && lh != (rh / result))) {
      throw std::runtime_error(
          Error("result of the calculation is out of range"));
    }
    rpn_stack_.push(result);
    break;
  }
  case '/':
    if (rh == 0) {
      throw std::runtime_error(Error("divide by zero"));
    }
    if (lh == min && rh == -1) {
      throw std::runtime_error(
          Error("result of the calculation is out of range"));
    }
    rpn_stack_.push(lh / rh);
    break;
  default:
    break;
  }
}

void RPN::ParseAndEvaluate() {
  if (tokens_.size() == 0) {
    throw std::runtime_error(Error("stack empty"));
  }
  for (std::vector<Token>::iterator it = tokens_.begin(); it != tokens_.end();
       ++it) {
    switch (it->type_) {
    case Token::KNum:
      ParseNum(it->token_);
      break;
    case Token::KOperator:
      if (rpn_stack_.size() <= 1) {
        throw std::runtime_error(Error("stack empty"));
      }
      Evaluate(it->token_[0]);
      break;
    case Token::KOther:
      throw std::runtime_error(Error("unexpected token: `") + it->token_ + "'");
      break;
    }
  }
  if (rpn_stack_.size() != 1) {
    /*while (!rpn_stack_.empty()) {*/
    /*  std::cout << rpn_stack_.top() << "\n";*/
    /*  rpn_stack_.pop();*/
    /*}*/
    throw std::runtime_error(Error("stack has multiple results"));
  }
}

long RPN::Calculate(const std::string &line) {
  // 既に計算済み
  if (rpn_stack_.size() != 0) {
    return rpn_stack_.top();
  }
  if (!line.empty()) {
    line_ = line;
  }
  Tokenize();
  ParseAndEvaluate();
  return rpn_stack_.top();
}

void RPN::Clear() {
  line_.clear();
  tokens_.clear();
  rpn_stack_ = std::stack<long>();
}
