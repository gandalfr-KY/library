#ifndef FRACTION
#define FRACTION
#include <iostream>
#include <boost/multiprecision/cpp_int.hpp>
#include "gandalfr/standard/power.hpp"
#include "gandalfr/standard/gcdlcm.hpp"

class fraction{
  private:
    boost::multiprecision::cpp_int num, den;

    void simplify(){
        boost::multiprecision::cpp_int gcd_tmp = _gcd((num >= 0 ? num : -num), (den >= 0 ? den : -den));
        num /= gcd_tmp;
        den /= gcd_tmp;
        if(den < 0){
            num *= -1;
            den *= -1;
        }
    }

    friend const fraction operator+(const fraction &a){
        return a;
    }

    friend const fraction operator-(const fraction &a){
        return {-a.num, a.den};
    }

    friend const fraction operator+(const fraction &a, const fraction &b){
        boost::multiprecision::cpp_int lcm_tmp = _lcm((a.den >= 0 ? a.den : -a.den), (b.den >= 0 ? b.den : -b.den));
        return {lcm_tmp / a.den * a.num + lcm_tmp / b.den * b.num, lcm_tmp};
    }

    friend const fraction operator-(const fraction &a, const fraction &b){
        boost::multiprecision::cpp_int lcm_tmp = _lcm((a.den >= 0 ? a.den : -a.den), (b.den >= 0 ? b.den : -b.den));
        return {lcm_tmp / a.den * a.num - lcm_tmp / b.den * b.num, lcm_tmp};
    }

    friend const fraction operator*(const fraction &a, const fraction &b){
        boost::multiprecision::cpp_int gcd_tmp1 = _gcd((a.num >= 0 ? a.num : -a.num), (b.den >= 0 ? b.den : -b.den)),
                                       gcd_tmp2 = _gcd((b.num >= 0 ? b.num : -b.num), (a.den >= 0 ? a.den : -a.den));
        return {(a.num / gcd_tmp1) * (b.num / gcd_tmp2), (a.den / gcd_tmp2) * (b.den / gcd_tmp1)};
    }

    friend const fraction operator/(const fraction &a, const fraction &b){
        assert(b.num != 0);
        boost::multiprecision::cpp_int gcd_tmp1 = _gcd((a.num >= 0 ? a.num : -a.num), (b.num >= 0 ? b.num : -b.num)),
                                       gcd_tmp2 = _gcd((b.den >= 0 ? b.den : -b.den), (a.den >= 0 ? a.den : -a.den));
        return {(a.num / gcd_tmp1) * (b.den / gcd_tmp2), (a.den / gcd_tmp2) * (b.num / gcd_tmp1)};
    }

    friend bool operator==(const fraction &a, const fraction &b){
        return a.num == b.num && a.den == b.den;
    }

    friend bool operator!=(const fraction &a, const fraction &b){
        return a.num != b.num || a.den != b.den;
    }   

    friend bool operator>(const fraction &a, const fraction &b){
        return a.num * b.den > b.num * a.den;
    }

    friend bool operator>=(const fraction &a, const fraction &b){
        return a.num * b.den >= b.num * a.den;
    }

    friend bool operator<(const fraction &a, const fraction &b){
        return a.num * b.den < b.num * a.den;
    }

    friend bool operator<=(const fraction &a, const fraction &b){
        return a.num * b.den <= b.num * a.den;
    }

  public:
    template<class INT> fraction(const INT &n) : num(n), den(1) {}
    fraction(const fraction &a) : num(a.num), den(a.den) {}
    template<class INT1, class INT2> fraction(const INT1 &numerator, const INT2 &denominator) : num(numerator), den(denominator) {
        assert(den != 0);
        simplify();
    }
    fraction() : num(0), den(1) {}

    boost::multiprecision::cpp_int numerator(){ return num; }
    boost::multiprecision::cpp_int denomitnator(){ return den; }
    double value(){ return num.convert_to<double>() / den.convert_to<double>(); }
    const fraction abs(){ return {boost::multiprecision::abs(num), den}; } 
    const fraction inverse(){ return {den, num}; } 

    fraction &operator=(const fraction &a){
        num = a.num;
        den = a.den;
        return *this;
    }

    void operator+=(const fraction &a){
        boost::multiprecision::cpp_int lcm_tmp = _lcm((den >= 0 ? den : -den), (a.den >= 0 ? a.den : -a.den));
        num = lcm_tmp / den * num + lcm_tmp / a.den * a.num;
        den = lcm_tmp;
        simplify();
    }

    void operator-=(const fraction &a){
        boost::multiprecision::cpp_int lcm_tmp = _lcm((den >= 0 ? den : -den), (a.den >= 0 ? a.den : -a.den));
        num = lcm_tmp / den * num - lcm_tmp / a.den * a.num;
        den = lcm_tmp;
        simplify();
    }

    void operator*=(const fraction &a){
        boost::multiprecision::cpp_int gcd_tmp1 = _gcd((num >= 0 ? num : -num), (a.den >= 0 ? a.den : -a.den)),
                                       gcd_tmp2 = _gcd((a.num >= 0 ? a.num : -a.num), (den >= 0 ? den : -den));
        num = (num / gcd_tmp1) * (a.num / gcd_tmp2);
        den = (den / gcd_tmp2) * (a.den / gcd_tmp1);
    }

    void operator/=(const fraction &a){
        assert(a.num != 0);
        boost::multiprecision::cpp_int gcd_tmp1 = _gcd((num >= 0 ? num : -num), (a.num >= 0 ? a.num : -a.num)),
                                       gcd_tmp2 = _gcd((a.den >= 0 ? a.den : -a.den), (den >= 0 ? den : -den));
        num = (num / gcd_tmp1) * (a.den / gcd_tmp2);
        den = (den / gcd_tmp2) * (a.num / gcd_tmp1);
    }

    friend std::istream &operator>>(std::istream &is, fraction &a){
        is >> a.num >> a.den;
        return is;
    }

    friend std::ostream &operator<<(std::ostream &os, const fraction &a) {
        os << a.num;
        if(a.den != 1) os << '/' << a.den;
        return os;
    }
};

#endif