#pragma once
#include <iostream>
#include <math.h>
#include <stdio.h>

class vec4 {
public:
	float x;
	float y;
	float z;
	float w;

	vec4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
	vec4(const float x, const float y, const float z, const float w) {
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}
	vec4(const vec4& v) {
		this->x = v.x;
		this->y = v.y;
		this->z = v.z;
		this->w = v.w;
	}

	//Assignment Operator: This method assigns the values from the vector rhs (right-hand side) to the current vector. 
	//It returns a reference to the current vector (*this), allowing for chained assignments (e.g., a = b = c).
	inline vec4& operator=(vec4 const& rhs) {
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;
		w = rhs.w;
		return *this;
	}

	//Unary Negation Operator: This method returns a new vector that is the negation of the current vector. 
	//Each component of the vector is negated (e.g., if the vector is (x, y, z), it returns (-x, -y, -z)).
	inline vec4 operator-() {
		vec4 invVector;
		invVector.x = x * -1;
		invVector.y = y * -1;
		invVector.z = z * -1;
		invVector.w = w * -1;
		return invVector;
	}

	//Binary Addition Operator: This method adds the vector rhs to the current vector and returns the resulting vector.
	//It doesn't modify the current vector or rhs (e.g., if the vectors are (x1, y1, z1) and (x2, y2, z2), it returns (x1 + x2, y1 + y2, z1 + z2)).
	inline vec4 operator+(vec4 const& rhs) {
		vec4 addVector;
		addVector.x = rhs.x + x;
		addVector.y = rhs.y + y;
		addVector.z = rhs.z + z;
		addVector.w = rhs.w + w;
		return addVector;
	}

	//Addition Assignment Operator: This method adds the vector rhs to the current vector and updates the current vector with the result. 
	//It returns a reference to the current vector (*this), allowing for chaining (e.g., a += b += c).
	inline vec4& operator+=(vec4 const& rhs) {
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		w += rhs.w;
		return *this;
	}

	//Binary Subtraction Operator: This method subtracts the vector rhs from the current vector and returns the resulting vector. 
	//It doesn’t modify the current vector or rhs (e.g., if the vectors are (x1, y1, z1) and (x2, y2, z2), it returns (x1 - x2, y1 - y2, z1 - z2)).
	inline vec4 operator-(vec4 const& rhs) {
		vec4 subtVector;
		subtVector.x = x - rhs.x;
		subtVector.y = y - rhs.y;
		subtVector.z = z - rhs.z;
		subtVector.w = w - rhs.w;
		return subtVector;
	}

	//Subtraction Assignment Operator: This method subtracts the vector rhs from the current vector and updates the current vector with the result. 
	//It returns a reference to the current vector (*this), allowing for chaining (e.g., a -= b -= c).
	inline vec4& operator-=(vec4 const& rhs) {
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		w -= rhs.w;
		return *this;
	}

	//Scalar Multiplication Assignment Operator: This method multiplies each component of the current vector by the scalar value and updates the current vector with the result. 
	//It returns a reference to the current vector (*this), allowing for chaining (e.g., a *= 2.0f).
	inline vec4& operator*=(float const scalar) {
		x = x * scalar;
		y = y * scalar;
		z = z * scalar;
		w = w * scalar;
		return *this;
	}

	//Scalar Multiplication Operator: This method multiplies each component of the current vector by the scalar value and returns a new vector with the result. 
	//The current vector is not modified (e.g., if the vector is (x, y, z) and the scalar is s, it returns (x * s, y * s, z * s)).
	inline vec4 operator*(float const scalar) {
		vec4 scaVector;
		scaVector.x = x * scalar;
		scaVector.y = y * scalar;
		scaVector.z = z * scalar;
		scaVector.w = w * scalar;
		return scaVector;
	}

	//Equality Comparison Operator: This method compares the current vector with rhs to check if they are equal (i.e., if all their components are the same). 
	//It returns true if they are equal, false otherwise.
	inline bool operator==(vec4 const& rhs) {
		if (x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w) {
			return true;
		}
		return false;
	}

	//Inequality Comparison Operator: This method compares the current vector with rhs to check if they are not equal. 
	//It returns true if they are not equal, false otherwise.
	inline bool operator!=(vec4 const& rhs) {
		if (x != rhs.x || y != rhs.y || z != rhs.z || w != rhs.w) {
			return true;
		}
		return false;
	}

	//Element Access Operator (Non-const): This method allows access to individual components of the vector by index i 
	//(where i is in the range [0, 2]). It returns a reference to the component, 
	//allowing it to be modified (e.g., vec[0] = 1.0f to set the x component).
	inline float& operator[](uint32_t const i) {
		if (i == 0) return x;
		else if (i == 1) return y;
		else if (i == 2) return z;
		else if (i == 3) return w;
		else throw std::out_of_range("index out of range for vec4");
	}

	//Element Access Operator (Const): This method allows read-only access to individual components of the vector by index i (where i is in the range [0, 2]). 
	//It returns a constant reference to the component, preventing modification (e.g., float x = vec[0]; to retrieve the x component).
	inline float const& operator[](uint32_t const i) const {
		if (i == 0) return x;
		else if (i == 1) return y;
		else if (i == 2) return z;
		else if (i == 3) return w;
		else throw std::out_of_range("index out of range for vec4");
	}
};

inline float dot(vec4 const& a, vec4 const& b) {
	float dot = a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
	return dot;
}

inline float length(vec4 const& v) {
	float magnitude = sqrtf(powf(v.x, 2.0f) + powf(v.y, 2.0f) + powf(v.z, 2.0f) + powf(v.w, 2.0f));
	return magnitude;
}

inline vec4 normalize(vec4 const& v) {
	float l = length(v);
	vec4 normVector(v.x / l, v.y / l, v.z / l, v.w / l);
	return normVector;
}