#pragma once

#include<cmath>
#include<iterator>

// Последовательность чисел (Например: 0,0.1,0.2,...180 )
template<class T>
class num_sequence {
public:
    typedef T value_type;
    typedef size_t size_type;

    num_sequence() : x0(value_type()), h(value_type()),  n(size_type()) {}
    num_sequence(value_type Begin, value_type Inc, value_type End) :
        x0(Begin), h(Inc), n( static_cast<size_type>((End-Begin)/Inc) + 1u ) {}

    value_type operator [](const size_type& i)const { return value(i); } // Значение последовательности в точке с номером i

    value_type at(const size_type& i, const value_type& defVal = value_type() )const { // Значение последовательности в точке с номером index c проверкой
        return ( i < n ) ? value(i) : defVal ;
    }

    value_type value(const size_type& i)const { return x0 + i * h; }

    size_type index(const value_type& val)const { // Возращяет номер точки для заданного значения
        const size_type val_idx = static_cast<size_type>( ::round( (val-x0) / h ) );
        return ( val_idx >= n ) ? n-1u : val_idx;
    }

    value_type round(const value_type& val)const { return value(index(val)); }

    value_type front()const       { return x0; }
    value_type increment()const   { return h; }
    value_type back()const        { return x0 + ( n-1u ) * h; }
    size_type size()const         { return n; }
    bool empty()const             { return !n; }
    num_sequence& operator += (const value_type& val) { x0 += val; return *this; }
    num_sequence& operator -= (const value_type& val) { x0 -= val; return *this; }
    num_sequence& operator *= (const value_type& val) { x0 *= val; return *this; }
    num_sequence& operator /= (const value_type& val) { x0 /= val; return *this; }

public:
    class const_iterator : public std::iterator<std::random_access_iterator_tag,value_type,int> {
    public:
        typedef int difference_type;

        explicit const_iterator(value_type x0_ = value_type(), value_type h_ = value_type(), difference_type i_ = difference_type())
            : x0(x0_), h(h_), i(i_) {}

        value_type operator*() const { return x0 + i * h; }
        const_iterator& operator++()   { ++i; return *this; }
        const_iterator operator++(int) { const_iterator tmp(*this); ++*this; return tmp; }
        const_iterator& operator--()   { --i; return (*this); }
        const_iterator operator--(int) { const_iterator tmp(*this); --*this; return tmp; }
        const_iterator& operator+=(difference_type n) {	i += n; return (*this); }
        const_iterator& operator-=(difference_type n) {	i -= n; return (*this); }
        const_iterator operator+(difference_type n)const{ return const_iterator(*this) += n; }
        const_iterator operator-(difference_type n)const{ return const_iterator(*this) -= n; }
        difference_type operator-(const const_iterator& r) const { return i - r.i; }
        value_type operator[](difference_type n)const { return x0 + (i + n) * h; }
        bool operator==(const const_iterator& r)const { return i == r.i; }
        bool operator!=(const const_iterator& r)const { return i != r.i; }
        bool operator< (const const_iterator& r)const { return i <  r.i; }
        bool operator> (const const_iterator& r)const { return i >  r.i; }
        bool operator<=(const const_iterator& r)const { return i <= r.i; }
        bool operator>=(const const_iterator& r)const { return i >= r.i; }

    private:
        value_type x0, h;
        difference_type i;
    };

public:
    const_iterator begin()const   { return const_iterator(x0,h); }
    const_iterator end()const     { return const_iterator(x0,h,n); }

private:
    value_type x0, h; // Начальное значение, Шаг
    size_type n;	  // Число точек
};

template<class T> inline num_sequence<T> operator + (num_sequence<T> x, const T& val) { return x += val; }
template<class T> inline num_sequence<T> operator - (num_sequence<T> x, const T& val) { return x -= val; }
template<class T> inline num_sequence<T> operator * (num_sequence<T> x, const T& val) { return x *= val; }
template<class T> inline num_sequence<T> operator / (num_sequence<T> x, const T& val) { return x /= val; }
