#pragma once 

#include<float.h>
#include<algorithm>
#include<limits>

#undef min
#undef max

#ifdef _MSC_VER
	#define constexpr const
#endif

inline constexpr long double max_valid_limit() { return FLT_MAX / 100; }
inline constexpr long double zero_valid_limit() { return 100 * FLT_EPSILON; }

inline bool valid_limits(long double min, long double max) {
    return (max - min) > zero_valid_limit() &&
        (max - min) < max_valid_limit() &&
        max < max_valid_limit() &&
        min > -max_valid_limit();
}

struct limits {
    typedef long double real_t;
    static constexpr real_t default_min() { return DBL_MAX; }
    static constexpr real_t default_max() { return -DBL_MAX; }

    real_t min_, max_;

	limits() : min_(default_min()), max_(default_max()) {}

    limits(real_t min__, real_t max__) : min_(default_min()), max_(default_max()) { set(min__, max__); }

    bool set(real_t min__, real_t max__) {
        return valid_limits(min__, max__) && (min_ = min__, max_ = max__, true);
	}

    real_t min()const { return min_; }
    real_t max()const { return max_; }
    real_t length()const { return max_ - min_; }
    real_t mean()const { return ( max_ + min_ ) / 2; }
	limits inv()const { limits lim; lim.max_ = min_; lim.min_ = max_; return lim; }
    real_t valid()const { return valid_limits(min_, max_); }
    bool in(real_t value)const { return value > min_ && value < max_; }

    real_t kx(const limits& target)const { return target.length() / length(); }
    real_t dx(const limits& target)const { return ( target.min_ * max_ - target.max_ * min_ ) / length(); }
    real_t to(const limits& target, double x)const { return dx(target) + kx(target) * x; }
    real_t from(const limits& target, double x)const { return (x-dx(target)) / kx(target); }
};

inline limits opt(const limits& a, const limits& b) {
	return limits(std::min(a.min(), b.min()), std::max(a.max(), b.max()));
}
