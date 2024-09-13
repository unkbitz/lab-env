#pragma once
#include <iostream>
#include <math.h>
#include <math/vec3.h>
#include <math/vec4.h>
using namespace std;

class mat4 {
public:
	vec4 m[4];

	mat4() {
		m[0] = vec4(1.0f, 0.0f, 0.0f, 0.0f);
		m[1] = vec4(0.0f, 1.0f, 0.0f, 0.0f);
		m[2] = vec4(0.0f, 0.0f, 1.0f, 0.0f);
		m[3] = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	}

	mat4(vec4 const& r0, vec4 const& r1, vec4 const& r2, vec4 const& r3) {
		m[0] = r0;
		m[1] = r1;
		m[2] = r2;
		m[3] = r3;
	}
	mat4(mat4 const& m) {
		for (int i = 0; i < 4; ++i) {
			this->m[i] = m.m[i];
		}
	}

	mat4& operator=(mat4 const& rhs);	
	mat4 operator*(mat4 const& rhs) const;
	vec4 operator*(vec4 const& rhs) const;
	bool operator==(mat4 const& rhs) const;
	bool operator!=(mat4 const& rhs) const;
	vec4& operator[](uint32_t const i);
	vec4 const& operator[](uint32_t const i) const;
	mat4 perspective(float const fovy, float const aspect, float const near, float const far);
	mat4 lookat(vec3 const& eye, vec3 const& at, vec3 const& up);
};

inline mat4 transpose(mat4 const& m) {
	mat4 result;
	result[0][0] = m[0][0];
	result[0][1] = m[1][0];
	result[0][2] = m[2][0];
	result[0][3] = m[3][0];

	result[1][0] = m[0][1];
	result[1][1] = m[1][1];
	result[1][2] = m[2][1];
	result[1][3] = m[3][1];

	result[2][0] = m[0][2];
	result[2][1] = m[1][2];
	result[2][2] = m[2][2];
	result[2][3] = m[3][2];

	result[3][0] = m[0][3];
	result[3][1] = m[1][3];
	result[3][2] = m[2][3];
	result[3][3] = m[3][3];
	return result;
}


inline mat4& mat4::operator=(mat4 const& rhs) {
	for (int i = 0; i < 4; ++i) {
		this->m[i] = rhs.m[i];
	}
	return *this;
}

inline mat4 mat4::operator*(mat4 const& rhs) const {
	vec4 zero(0, 0, 0, 0);
	mat4 result(zero, zero, zero, zero);
	mat4 given = *this;

	for (int r = 0; r < 4; r++) {
		for (int k = 0; k < 4; k++) {
			for (int i = 0; i < 4; i++) {
				result.m[r][k] += given.m[i][k] * rhs.m[r][i];
			}
		}
	}
	return result;
}

inline vec4 mat4::operator*(vec4 const& rhs) const {
	vec4 result(0, 0, 0, 0);
	mat4 given = *this;
	given = transpose(given);
	for (int r = 0; r < 4; r++) {
		for (int k = 0; k < 4; k++) {
			result[r] += given.m[r][k] * rhs[k];
		}
	}
	return result;
}

inline bool mat4::operator==(mat4 const& rhs) const {
	for (int r = 0; r < 4; r++) {
		for (int k = 0; k < 4; k++) {
			if (this->m[r][k] != rhs.m[r][k]) {
				return false;
			}
		}
	}
	return true;
}

inline bool mat4::operator!=(mat4 const& rhs) const {
	for (int r = 0; r < 4; r++) {
		for (int k = 0; k < 4; k++) {
			if (this->m[r][k] != rhs.m[r][k]) {
				return true;
			}
		}
	}
	return false;
}

inline vec4& mat4::operator[](uint32_t const i) {
	if (i >= 4) {
		throw std::out_of_range("Index out of range");
	}
	return m[i];
}

inline vec4 const& mat4::operator[](uint32_t const i) const {
	if (i >= 4) {
		throw std::out_of_range("Index out of range");
	}
	return m[i];
}

inline float determinant(mat4 const m) {
	float det = 1.0f;
	mat4 a = m;
	for (int i = 0; i < 4; i++) {
		int pivot = i;
		// find the pivot row
		for (int j = i + 1; j < 4; j++) {
			if (abs(a[j][i]) > abs(a[pivot][i])) {
				pivot = j;
			}
		}
		// if the pivot row is not the current row, swap them
		if (pivot != i) {
			swap(a[i], a[pivot]);
			det *= -1; //flip the sign of the determinant
		}
		// if the diagonal element is zero, determinant is zero
		if (a[i][i] == 0) {
			return 0;
		}
		det *= a[i][i]; // multiply by the diagonal element
		// eliminate elements below the diagonal
		for (int j = i + 1; j < 4; j++) {
			double factor = a[j][i] / a[i][i];
			for (int k = i + 1; k < 4; k++) {
				a[j][k] -= factor * a[i][k];
			}
		}
	}
	return det;
}

inline mat4 cofactor(mat4 const& a) {
	mat4 result(
		vec4(+(a[1][1] * a[2][2] * a[3][3] + a[1][2] * a[2][3] * a[3][1] + a[1][3] * a[2][1] * a[3][2] - a[1][3] * a[2][2] * a[3][1] - a[1][2] * a[2][1] * a[3][3] - a[1][1] * a[2][3] * a[3][2]),
			-(a[1][0] * a[2][2] * a[3][3] + a[1][2] * a[2][3] * a[3][0] + a[1][3] * a[2][0] * a[3][2] - a[1][3] * a[2][2] * a[3][0] - a[1][2] * a[2][0] * a[3][3] - a[1][0] * a[2][3] * a[3][2]),
			+(a[1][0] * a[2][1] * a[3][3] + a[1][1] * a[2][3] * a[3][0] + a[1][3] * a[2][0] * a[3][1] - a[1][3] * a[2][1] * a[3][0] - a[1][1] * a[2][0] * a[3][3] - a[1][1] * a[2][3] * a[3][2]),
			-(a[1][0] * a[2][1] * a[3][2] + a[1][1] * a[2][2] * a[3][0] + a[1][2] * a[2][0] * a[3][1] - a[1][2] * a[2][1] * a[3][0] - a[1][1] * a[2][0] * a[3][2] - a[1][0] * a[2][2] * a[3][1])),

		vec4(-(a[0][1] * a[2][2] * a[3][3] + a[0][2] * a[2][3] * a[3][1] + a[0][3] * a[2][1] * a[3][2] - a[0][3] * a[2][2] * a[3][1] - a[0][2] * a[2][1] * a[3][3] - a[0][1] * a[2][3] * a[3][2]),
			+(a[0][0] * a[2][2] * a[3][3] + a[0][3] * a[2][3] * a[3][0] + a[0][3] * a[2][0] * a[3][2] - a[0][3] * a[2][2] * a[3][0] - a[0][2] * a[2][0] * a[3][3] - a[0][0] * a[2][3] * a[3][2]),
			-(a[0][0] * a[2][1] * a[3][3] + a[0][2] * a[2][3] * a[3][0] + a[0][3] * a[2][0] * a[3][1] - a[0][3] * a[2][1] * a[3][0] - a[0][2] * a[2][0] * a[3][3] - a[0][0] * a[2][3] * a[3][1]),
			+(a[0][0] * a[2][1] * a[3][2] + a[0][2] * a[2][2] * a[3][0] + a[0][2] * a[2][0] * a[3][1] - a[0][2] * a[2][1] * a[3][0] - a[0][1] * a[2][0] * a[3][2] - a[0][0] * a[2][2] * a[3][1])),

		vec4(+(a[0][1] * a[1][2] * a[3][3] + a[0][2] * a[1][3] * a[3][1] + a[0][3] * a[1][1] * a[3][2] - a[0][3] * a[1][2] * a[3][1] - a[0][2] * a[1][1] * a[3][3] - a[0][1] * a[1][3] * a[3][2]),
			-(a[0][0] * a[1][2] * a[3][3] + a[0][2] * a[1][3] * a[3][0] + a[0][3] * a[1][0] * a[3][2] - a[0][3] * a[1][2] * a[3][0] - a[0][2] * a[1][0] * a[3][3] - a[0][0] * a[1][3] * a[3][2]),
			+(a[0][0] * a[1][1] * a[3][3] + a[0][2] * a[1][3] * a[3][0] + a[0][3] * a[1][0] * a[3][1] - a[0][3] * a[1][1] * a[3][0] - a[0][2] * a[1][0] * a[3][3] - a[0][0] * a[1][3] * a[3][1]),
			-(a[0][0] * a[1][1] * a[3][2] + a[0][2] * a[1][0] * a[3][0] + a[0][2] * a[1][0] * a[3][1] - a[0][2] * a[1][1] * a[3][0] - a[0][1] * a[1][0] * a[3][2] - a[0][0] * a[1][2] * a[3][1])),

		vec4(-(a[0][1] * a[1][2] * a[2][3] + a[0][2] * a[1][3] * a[2][1] + a[0][3] * a[1][1] * a[2][2] - a[0][3] * a[1][2] * a[2][1] - a[0][2] * a[1][1] * a[2][3] - a[0][2] * a[1][3] * a[2][2]),
			+(a[0][0] * a[1][2] * a[2][3] + a[0][2] * a[1][3] * a[2][0] + a[0][3] * a[1][0] * a[2][2] - a[0][3] * a[1][2] * a[2][0] - a[0][2] * a[1][0] * a[2][3] - a[0][0] * a[1][3] * a[2][2]),
			-(a[0][0] * a[1][1] * a[2][3] + a[0][1] * a[1][3] * a[2][0] + a[0][3] * a[1][0] * a[2][1] - a[0][3] * a[1][1] * a[2][0] - a[0][1] * a[1][0] * a[2][3] - a[0][0] * a[1][3] * a[2][1]),
			+(a[0][0] * a[1][1] * a[2][2] + a[0][1] * a[1][2] * a[2][0] + a[0][2] * a[1][0] * a[2][1] - a[0][2] * a[1][1] * a[2][0] - a[0][1] * a[1][0] * a[2][2] - a[0][0] * a[1][2] * a[2][1]))
	);
	return result;
}

inline mat4 adjoint(mat4 const& a) {
	return transpose(cofactor(a));
}

inline mat4 inverse(mat4 const& m) {
	mat4 result;
	mat4 a = m;
	mat4 adj = adjoint(a);
	float det = determinant(a);
	if (det == 0) {
		return mat4();
	}
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result[i][j] = (1 / det * adj[i][j]);
		}
	}
	return result;
}

inline mat4 rotationx(float const rad) {
	const mat4 rotx(vec4(1.0f, 0.0f, 0.0f, 0.0f),
					vec4(0.0f, cos(rad), sin(rad), 0.0f),
					vec4(0.0f, -sin(rad), cos(rad), 0.0f),
					vec4(0.0f, 0.0f, 0.0f, 1.0f));
	return rotx;
}

inline mat4 rotationy(float const rad) {
	const mat4 roty(vec4(cosf(rad), 0.0f, -sinf(rad), 0.0f),
					vec4(0.0f, 1.0f, 0.0f, 0.0f),
					vec4(sinf(rad), 0.0f, cosf(rad), 0.0f),
					vec4(0.0f, 0.0f, 0.0f, 1.0f));
	return roty;
}

inline mat4 rotationz(float const rad) {
	const mat4 rotz(vec4(cos(rad), sin(rad), 0.0f, 0.0f),
					vec4(-sin(rad), cos(rad), 0.0f, 0.0f),
					vec4(0.0f, 0.0f, 1.0f, 0.0f),
					vec4(0.0f, 0.0f, 0.0f, 1.0f));
	return rotz;
}

//rotationaxis(v : vec3 const&, rad : f32 const) : mat4
inline mat4 rotationaxis(vec3 const& v, float const rad) {
	mat4 result(
		vec4(v.x * v.x * (1 - cosf(rad)) + cosf(rad), v.x * v.y * (1 - cosf(rad)) - v.z * sinf(rad), v.x * v.z * (1 - cosf(rad)) + v.y * sinf(rad), 0),
		vec4(v.x * v.y * (1 - cosf(rad)) + v.z * sinf(rad), v.y * v.y * (1 - cosf(rad)) + cosf(rad), v.y * v.z * (1 - cosf(rad)) - v.x * sinf(rad), 0),
		vec4(v.x * v.z * (1 - cosf(rad)) - v.y * sinf(rad), v.y * v.z * (1 - cosf(rad)) + v.x * sinf(rad), v.z * v.z * (1 - cosf(rad)) + cosf(rad), 0),
		vec4(0, 0, 0, 1));
	result = transpose(result);
	return result;
}

inline mat4 perspective(float const fovy, float const aspect, float const near, float const far) {
	float c = 1.0/tanf(fovy/2);
	mat4 matperspective(
		vec4(c / aspect,		0,		0,									0),
		vec4(0,					c,		0,									0),
		vec4(0,					0,		-(far + near) / (far - near),		-1),
		vec4(0,					0,		-(2 * far * near) / (far - near),	0));
	return matperspective;
}

inline mat4 lookat(vec3 const& eye, vec3 const& at, vec3 const& up) {
	vec3 a = at;
	vec3 v = normalize(a - eye);
	vec3 r = -normalize(cross(v, up));
	vec3 u = cross(r, v);

	mat4 matlookat(
		vec4(r.x,			u.x,			v.x,			0),
		vec4(r.y,			u.y,			v.y,			0),
		vec4(r.z,			u.z,			v.z,			0),
		vec4(dot(r, eye),	dot(u, eye),	dot(v, eye),	1));
	return matlookat;
}
