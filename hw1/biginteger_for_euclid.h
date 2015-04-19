#ifndef BIGINTEGER_FOR_EUCLID

#define BIGINTEGER_FOR_EUCLID

class BigInteger{
public:
  int top;
  int digit[512];
  BigInteger();
  BigInteger(const int&);
  BigInteger(const std::string&);
  BigInteger(const BigInteger&);
  ~BigInteger();

  bool operator<(const BigInteger&) const;
  BigInteger operator%(const BigInteger&) const;
  BigInteger& operator=(const BigInteger&);
  bool iszero();
  const BigInteger shift(const int&);
  const BigInteger operator-(const BigInteger& b) const;
  bool operator==(const BigInteger& b) const;

  friend std::ostream& operator<<(std::ostream&, const BigInteger&);

};


#endif
