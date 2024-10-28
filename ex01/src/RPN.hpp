#ifndef RPN_HPP
#define RPN_HPP

#include <stack>
#include <string>
#include <vector>

struct Token {
  enum Type { KNum, KOperator, KOther };

  Token(const std::string &token, Type type) {
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
  void Tokenize();
  void ParseAndEvaluate();
  void Evaluate(char);
  void ParseNum(const std::string &);
  bool IsOperator(char) const;
  void AddToken(std::string &, Token::Type);
  std::string Error(const std::string & = "");

  std::string line_;
  std::vector<Token> tokens_;
  std::stack<long> rpn_stack_;
  long min_;
  long max_;
  static const long kDefaultMin_ = 0;
  static const long kDefaultMax_ = 9;

public:
  RPN();
  RPN(const std::string &);
  RPN(const RPN &);
  ~RPN();
  RPN &operator=(const RPN &);
  long Calculate(const std::string & = "");
  void Clear();
  long GetMin() const;
  void SetMin(long);
  long GetMax() const;
  void SetMax(long);
};

#endif // !RPN_HPP
