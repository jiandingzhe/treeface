#ifndef Vec4f_h__
#define Vec4f_h__

#include <cmath>
#include <emmintrin.h>
#include "Vec4.h"

namespace treeface
{

typedef __m128 m128;
typedef __m128i m128i;
typedef __m128d m128d;

template<>
class Vec4<float> 
{
public:
	Vec4()=default;

	Vec4(const Vec4& other)=default;

	Vec4& operator=(const Vec4& other)=default;

	~Vec4()=default;

	Vec4(const m128& value) throw() { m_value.f=value; }

	Vec4(const m128d& value) throw() { m_value.d=value; }

	Vec4(const m128i& value) throw() { m_value.i=value; }

	Vec4(float value4) throw() { m_value.f=_mm_set1_ps(value4); }

	Vec4(float value1,float value2,float value3,float value4) throw() { m_value.f=_mm_set_ps(value4,value3,value2,value1); };

	Vec4(float* ptr4) throw() { m_value.f=_mm_load_ps(ptr4); }

	operator m128&() throw(){ return m_value.f; }

	operator m128i&() throw(){ return m_value.i; }

	operator m128d&() throw(){ return m_value.d; }	

	operator const m128&() const throw(){ return m_value.f; }

	operator const m128i&() const throw(){ return m_value.i; }

	operator const m128d&() const throw(){ return m_value.d; }

	static Vec4<float> load(const float*const ptr) throw() { return _mm_load_ps(ptr); }

	static void store(float*const ptr,const Vec4<float>& value) throw() { _mm_store_ps(ptr,value); }

	static Vec4<float> loadu(const float*const ptr) throw(){ return _mm_loadu_ps(ptr); }

	static void storeu(float*const ptr,const Vec4<float>& value) throw(){ _mm_storeu_ps(ptr,value); }

	Vec4<float>& operator=(const float value) throw() { m_value.f=_mm_set1_ps(value); return *this; }

	float& operator[](const int i) throw() { return m_value.raw[i]; }

	Vec4<float> operator+(const Vec4<float>& other) const throw() { return _mm_add_ps(m_value.f,other.m_value.f);}
	Vec4<float> operator-(const Vec4<float>& other) const throw() { return _mm_sub_ps(m_value.f,other.m_value.f);}
	Vec4<float> operator*(const Vec4<float>& other) const throw() { return _mm_mul_ps(m_value.f,other.m_value.f);}
	Vec4<float> operator/(const Vec4<float>& other) const throw() { return _mm_div_ps(m_value.f,other.m_value.f);}

	Vec4<float> operator+( const float value ) const throw(){ return _mm_add_ps(m_value.f,_mm_set1_ps(value)); }
	Vec4<float> operator-( const float value ) const throw(){ return _mm_sub_ps(m_value.f,_mm_set1_ps(value)); }
	Vec4<float> operator*( const float value ) const throw(){ return _mm_mul_ps(m_value.f,_mm_set1_ps(value)); }
	Vec4<float> operator/( const float value ) const throw(){ return _mm_div_ps(m_value.f,_mm_set1_ps(value)); }

	Vec4<float>& operator+=( const Vec4<float>& other ) throw(){ m_value.f=_mm_add_ps(m_value.f,other.m_value.f); return *this;}
	Vec4<float>& operator-=( const Vec4<float>& other ) throw(){ m_value.f=_mm_sub_ps(m_value.f,other.m_value.f); return *this;}
	Vec4<float>& operator*=( const Vec4<float>& other ) throw(){ m_value.f=_mm_mul_ps(m_value.f,other.m_value.f); return *this;}
	Vec4<float>& operator/=( const Vec4<float>& other ) throw(){ m_value.f=_mm_div_ps(m_value.f,other.m_value.f); return *this;}

	Vec4<float>& operator+=( const float value ) throw(){ m_value.f = _mm_add_ps(m_value.f,_mm_set1_ps(value)); return *this; }
	Vec4<float>& operator-=( const float value ) throw(){ m_value.f = _mm_sub_ps(m_value.f,_mm_set1_ps(value)); return *this; }
	Vec4<float>& operator*=( const float value ) throw(){ m_value.f = _mm_mul_ps(m_value.f,_mm_set1_ps(value)); return *this; }
	Vec4<float>& operator/=( const float value ) throw(){ m_value.f=_mm_div_ps(m_value.f,_mm_set1_ps(value)); return *this;}

	void set(float x, float y, float z, float w)
    {
    	m_value.f=_mm_set_ps(w,z,y,x);
    }

    void set(const float* values)
    {
		loadu(values);
    }

    void set(const Vec4& other)
    {
    	(*this)=other;
    }

    float normalize()
    {
        float len = length();
        (*this)/=len;
        return len;
    }

    float length()
    {
        return std::sqrt(length2());
    }

    float length2()
    {
        Vec4<float> temp = _mm_mul_ps(m_value.f,m_value.f);
        return temp.m_value.raw[0]+temp.m_value.raw[1]+temp.m_value.raw[2]+temp.m_value.raw[3];
    }

	union {
		m128 f;
		m128d d;
		m128i i;
		float raw[4];
	} m_value;
	
};

Vec4<float> operator+( const float value,const Vec4<float>& a ) throw(){ return _mm_add_ps(_mm_set1_ps(value),a); }

Vec4<float> operator-( const float value,const Vec4<float>& a ) throw(){ return _mm_sub_ps(_mm_set1_ps(value),a); }

Vec4<float> operator*( const float value,const Vec4<float>& a ) throw(){ return _mm_mul_ps(_mm_set1_ps(value),a); }

Vec4<float> operator/( const float value,const Vec4<float>& a ) throw(){ return _mm_div_ps(_mm_set1_ps(value),a); }

}

#endif // Vec4<float>_h__
