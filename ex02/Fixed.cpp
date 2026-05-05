#include "Fixed.hpp"

Fixed::Fixed()
{
	std::cout << "Default constructor called" << std::endl;
	_value = 0;
}

Fixed::Fixed(const int n)
{
	std::cout << "Int constructor called" << std::endl;
	_value = n << _bits;
}

Fixed::Fixed(const float f)
{
	std::cout << "Float constructor called" << std::endl;
	_value = roundf(f * (1 << _bits));
}

Fixed::Fixed(const Fixed& other)
{
	std::cout << "Copy constructor called" << std::endl;
	*this = other;
}

Fixed& Fixed::operator=(const Fixed& other)
{
	std::cout << "Copy assignment operator called" << std::endl;

	if (this != &other)
		_value = other.getRawBits();

	return *this;
}

Fixed::~Fixed()
{
	std::cout << "Destructor called" << std::endl;
}

int Fixed::getRawBits(void) const
{
	return _value;
}

float Fixed::toFloat(void) const
{
	return static_cast<float>(_value) / (1 << _bits);
}

int Fixed::toInt(void) const
{
	return _value / (1 << _bits);
}

void Fixed::setRawBits(int const raw)
{
	_value = raw;
}

bool Fixed::operator>(const Fixed& other) const
{
	return _value > other._value;
}

bool Fixed::operator<(const Fixed& other) const
{
	return _value < other._value;
}

bool Fixed::operator>=(const Fixed& other) const
{
	return _value >= other._value;
}

bool Fixed::operator<=(const Fixed& other) const
{
	return _value <= other._value;
}

bool Fixed::operator==(const Fixed& other) const
{
	return _value == other._value;
}

bool Fixed::operator!=(const Fixed& other) const
{
	return _value != other._value;
}

Fixed Fixed::operator+(const Fixed& other) const
{
	Fixed result;

	result.setRawBits(_value + other._value);
	return result;
}

Fixed Fixed::operator-(const Fixed& other) const
{
	Fixed result;

	result.setRawBits(_value - other._value);
	return result;
}

Fixed Fixed::operator*(const Fixed& other) const
{
	Fixed result;

	result.setRawBits(static_cast<int>((static_cast<double>(_value) * other._value) / (1 << _bits)));
	return result;
}

Fixed Fixed::operator/(const Fixed& other) const
{
	Fixed result;

	result.setRawBits(static_cast<int>((static_cast<double>(_value) * (1 << _bits)) / other._value));
	return result;
}

Fixed& Fixed::operator++(void)
{
	++_value;
	return *this;
}

Fixed Fixed::operator++(int)
{
	Fixed copy(*this);

	++_value;
	return copy;
}

Fixed& Fixed::operator--(void)
{
	--_value;
	return *this;
}

Fixed Fixed::operator--(int)
{
	Fixed copy(*this);

	--_value;
	return copy;
}

Fixed& Fixed::min(Fixed& first, Fixed& second)
{
	return (first < second) ? first : second;
}

const Fixed& Fixed::min(const Fixed& first, const Fixed& second)
{
	return (first < second) ? first : second;
}

Fixed& Fixed::max(Fixed& first, Fixed& second)
{
	return (first > second) ? first : second;
}

const Fixed& Fixed::max(const Fixed& first, const Fixed& second)
{
	return (first > second) ? first : second;
}

std::ostream& operator<<(std::ostream& out, const Fixed& fixed)
{
	out << fixed.toFloat();
	return out;
}