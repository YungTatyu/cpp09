#ifndef RPN_HPP
#define RPN_HPP

#include <stack>
#include <string>
#include <vector>

struct Token {
  enum Type { KNum, KOperator, KOther };

  Token(const std::string token, Type type) {
    token_ = token;
    type_ = type;
  }
  Token(const Token &other) { *this = other; }
  ~Token() {}
  Token &operator=(const Token &other) {
    if (this != &other) {
      token_ = other.token_;
      type_ = other.type_;
    }
    return *this;
  }

  std::string token_;
  Type type_;

private:
  Token();
};

class RPN {
private:
  RPN();
  RPN(const RPN &);
  RPN &operator=(const RPN &);
  void tokenize();
  void parseAndEvaluate();

  std::string line_;
  std::vector<Token> tokens_;
  std::stack<long> rpn_stack_;

public:
  RPN(const std::string &);
  ~RPN();
  long calculate();
};

#endif // !RPN_HPP
