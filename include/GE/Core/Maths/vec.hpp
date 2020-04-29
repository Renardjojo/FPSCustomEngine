#ifndef _VEC_H
#define _VEC_H

#include <iostream>
#include <string>
#include <math.h>
#include "GE/Core/Debug/assert.hpp"

//Engine::Core::Maths::[..]
namespace Engine::Core::Maths
{
	typedef struct Vec2
	{
		float 		dot_product	 	(const Vec2& other) const;
		float 		length		 	() 					const;
		float 		cross_product	(const Vec2& other) const;

		/**
		 * function : rotate
		 *
		 * parameter : 
		 * float angleDeg : in degres
		 *
		 * return : Vec2&
		 *
		 * brief : turn Vec2 in sens of trigono
		 */
		Vec2& 		rotate			(float angleDeg);
		Vec2  		getRotate 		(float angleDeg) const;

		Vec2&  		rotated90 		();
		Vec2  		getRotated90 	() const;

		Vec2&  		rotated180 		(); 
		Vec2  		getRotated180	() const;

		Vec2&  		rotated270 		();
		Vec2  		getRotated270 	() const;


		/**
		 * function : foundAngleType
		 *
		 * parameter : 
		 * const Vec2& other
		 *
		 * return (type int) :
		 * -1 if angle is the angle is obtuse
		 *  0 if angle is the angle valu is 90 degres or pi/2 
		 *  1 if angle is the angle is aigu 
		 *
		 * brief : this function found the angle type and return it. The angle found is the shortess angle between two point
		 */
		int		foundAngleType		(const Vec2& other) const;

		/**
		 * function : foundAngle
		 *
		 * parameter : 
		 * const Vec2& other
		 *
		 * return (type float): angle between two Vec2 in deg
		 *
		 * brief : calcul the angle between two Vec2 of center 0. The angle found is the shortess angle between two point
		 */
		float		angle_between_vectors			(Vec2 other) const;
		float		signedAngle_between_vectors		(Vec2 other) const;

		/**
		 * function : projection
		 *
		 * parameter : 
		 * Vec2 other
		 *
		 * return (type Vec2):
		 *
		 * brief : create Vec2 projection between two Vec2. Vector this is project on vector other
		 * Formule : vect(vp) = ((vect(u)*vect(v))/vect(u)*vect(u)) * vect(u)
		 */
		Vec2		projection		(const Vec2& other) const;


		//creat unit vector
		Vec2& 	normalize		();
		Vec2 	getNormalize	() const;

			/*----------*/
		/* operator */
		/*----------*/

		Vec2& 	operator++	();
		Vec2 	operator++	(int);
		Vec2& 	operator--	();
		Vec2 	operator--	(int);
		Vec2 	operator-	() const;
		Vec2& 	operator=	(const Vec2& vec);
		Vec2& 	operator+=	(const Vec2& vec);
		Vec2& 	operator-=	(const Vec2& vec);
		Vec2& 	operator*=	(const float& scale);
		Vec2& 	operator/=	(const float& scale);
		float& 	operator[]	(int val);

		friend void operator>>(::std::istream& in, Vec2& vec);
		friend ::std::ostream& operator<<(::std::ostream& out, const Vec2& vec);

		union 
		{
			struct {float x, y;};
			float e[2];
		};
	} Vector2D;

	/**
	 * function : Vec2_dotProductNormeAngle
	 *
	 * parameter : 
	 * float normeVec21 : magnitude of vec 2 (can use Vec2_getMagnitude)
	 * float normeVec22 : magnitude of vec 1
	 * float angleRad : angle in deg
	 *
	 * return : float
	 *
	 * brief : calcul dot product between two Vec2
	 */
	float Vec2_dotProductNormeAngle (float normeVec21, float normeVec22, float angleDeg);

	Vec2 		operator-		(Vec2 vec1, const Vec2& vec2);
	Vec2 		operator+		(Vec2 vec1, const Vec2& vec2);
	Vec2		operator*		(Vec2 vec, const float& scale);
	Vec2 		operator*		(const float& scale, Vec2 vec);
	Vec2 		operator/		(Vec2 vec, const float& scale);
	Vec2 		operator/		(const float& scale, Vec2 vec);
	bool 		operator==		(Vec2 const& vec1, Vec2 const& vec2);
	bool 		operator!=		(Vec2 const& vec1, Vec2 const& vec2);
	bool 		operator<		(Vec2 const& vec1, Vec2 const& vec2);
	bool 		operator<=		(Vec2 const& vec1, Vec2 const& vec2);
	bool 		operator>		(Vec2 const& vec1, Vec2 const& vec2);
	bool 		operator>=		(Vec2 const& vec1, Vec2 const& vec2);

	void 		operator>>		(::std::istream& in, Vec2& vec);
	::std::ostream& operator<<	(::std::ostream& out, const Vec2& vec);

	typedef struct Vec3
	{	

		float 		dot_product	 	(const Vec3& other) const;

		//same as get magnitude
		float 		length		 	() 					const;

		//creat unit vector
		Vec3& 	normalize		();
		Vec3 	getNormalize	() const;

		//cross product between 2 vectors
		void 	cross		(const Vec3& other);
		Vec3 	getCross	(const Vec3& other) const;

		//this function return true if vector is colineaire this other vector. 
		//Comput dot product. If dot product is near of zero, vectors is colinear
		bool 	isColinear	(const Vec3& other, const float epsilon = __FLT_EPSILON__);

		//This function computes length of dot product between current vect and other vector. This result correspond to the area between the two vectors
		//this area correspond to the ares of parrallelogram formed by the two vectors 
		float getArea		(const Vec3& other) const;
		//This version calcul aera of triangle in function of 3 points
		float getAreaTriangle		(const Vec3& pos1, const Vec3& pos2, const Vec3& pos3) const;

		//This function rotate vector around another unit vector. This function assert if axis is not unit
		void rotateArroundAxis (const Vec3& unitAxis, float angleRad);
		Vec3 getRotateArroundAxis (const Vec3& unitAxis, float angleRad) const;

		/*----------*/
		/* operator */
		/*----------*/

		Vec3& 	operator++	();
		Vec3 	operator++	(int);
		Vec3& 	operator--	();
		Vec3 	operator--	(int);
		Vec3 	operator-	() const;
		Vec3& 	operator=	(const Vec3& vec);
		Vec3& 	operator+=	(const Vec3& vec);
		Vec3& 	operator+=	(float scale);
		Vec3& 	operator-=	(const Vec3& vec);
		Vec3& 	operator*=	(const float& scale);
		Vec3& 	operator/=	(const float& scale);
		float& 	operator[]	(unsigned int val);

		friend void operator>>(::std::istream& in, Vec3& vec); //TODO:
		friend ::std::ostream& operator<<(::std::ostream& out, const Vec3& vec);

		union 
		{
			struct {float x, y, z;};
			float e[3];
			Vec2 xy;
		};
	} Vector3D;

	Vec3 		operator-		(Vec3 vec1, Vec3 const& vec2);
	Vec3 		operator+		(Vec3 vec1, Vec3 const& vec2);
	Vec3		operator*		(Vec3 vec, const float& scale);
	Vec3 		operator*		(const float& scale, Vec3 vec);
	Vec3 		operator/		(Vec3 vec, const float& scale);
	Vec3 		operator/		(const float& scale, Vec3 vec);
	bool 		operator==		(Vec3 const& vec1, Vec3 const& vec2);
	bool 		operator!=		(Vec3 const& vec1, Vec3 const& vec2);
	bool 		operator<		(Vec3 const& vec1, Vec3 const& vec2);
	bool 		operator<=		(Vec3 const& vec1, Vec3 const& vec2);
	bool 		operator>		(Vec3 const& vec1, Vec3 const& vec2);
	bool 		operator>=		(Vec3 const& vec1, Vec3 const& vec2);

	::std::ostream& operator<<	(::std::ostream& out, const Vec3& vec);


	typedef struct Vec4
	{	
		float 		dot_product	 	(const Vec4& other) const;

		//same as get magnitude
		float 		length		 	() 					const;

		//divise each component by w execpt if w == 0
		void 		homogenize		(); 

		//creat unit vector
		Vec4& 	normalize		();
		Vec4 	getNormalize	() const;

		

		/*----------*/
		/* operator */
		/*----------*/

		Vec4& 	operator++	();
		Vec4 	operator++	(int);
		Vec4& 	operator--	();
		Vec4 	operator--	(int);
		Vec4 	operator-	() const;
		Vec4& 	operator*=	(const Vec4& vec);
		Vec4& 	operator=	(const Vec4& vec);
		Vec4& 	operator+=	(const Vec4& vec);
		Vec4& 	operator-=	(const Vec4& vec);
		Vec4& 	operator*=	(const float& scale);
		Vec4& 	operator/=	(const float& scale);
		float& 	operator[]	(unsigned int val);

		friend void operator>>(::std::istream& in, Vec4& vec); // TODO:
		friend ::std::ostream& operator<<(::std::ostream& out, const Vec4& vec);

		union 
		{
			struct {float x, y, z, w;};
			float e[4];
			Vec3 xyz;
			Vec2 xy;
		};
	} Vector4D;

	Vec4 		operator-		(Vec4 vec1, Vec4 const& vec2);
	Vec4 		operator+		(Vec4 vec1, Vec4 const& vec2);
	Vec4		operator*		(Vec4 vec1, const Vec4& vec2);
	Vec4		operator*		(Vec4 vec, const float& scale);
	Vec4 		operator*		(const float& scale, Vec4 vec);
	Vec4 		operator/		(Vec4 vec, const float& scale);
	Vec4 		operator/		(const float& scale, Vec4 vec);
	bool 		operator==		(Vec4 const& vec1, Vec4 const& vec2);
	bool 		operator!=		(Vec4 const& vec1, Vec4 const& vec2);
	bool 		operator<		(Vec4 const& vec1, Vec4 const& vec2);
	bool 		operator<=		(Vec4 const& vec1, Vec4 const& vec2);
	bool 		operator>		(Vec4 const& vec1, Vec4 const& vec2);
	bool 		operator>=		(Vec4 const& vec1, Vec4 const& vec2);

	::std::ostream& 	operator<<		(::std::ostream& out, const Vec4& vec);

	#include "vec.inl"
} //namespace Engine::Core::Maths

#endif // _VEC_H