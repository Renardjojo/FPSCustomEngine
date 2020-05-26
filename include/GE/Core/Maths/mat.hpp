//Project : Engine
//Editing by Six Jonathan
//Date : 07/01/2020 - 18 h 11

/**
*	By default, matrix use raw convention like openGL.
*   If you want to use line convention. Define LINE_CONVENTION like DirectX
*	Example : #DEFINE LINE_CONVENTION
*
*/

#ifndef _GE_MAT_H
#define _GE_MAT_H

#include <iostream>
#include <utility>
#include <array>
#include <cmath>
#include "GE/Core/Maths/vec.hpp"

namespace Engine::Core::Maths
{

	struct Mat4;
	typedef struct Mat3
	{
			/**
			 * function : Mat3 (constructor)
			 *
			 * parameter : 
			 * unsigned int size : height and width of squart Mat3
			 *
			 * brief : construct identity Mat3. This Mat3 is squart 
			 */
			Mat3();
			Mat3(	float, float, float, 
					float, float, float,
					float, float, float);

			Mat3(const Vec3& v1, const Vec3& v2, const Vec3& v3);
			Mat3(const Mat4& m);

			/**
			 * function : Mat3 (constructor)
			 *
			 * parameter : 
			 * unsigned int line : number of line
			 * unsigned int column : number of column
			 * float value : value to set in Mat3 
			 *
			 * brief : create null Mat3 with x line en y column 
			 */
			Mat3(float value);

			/**
			 * function : Mat3 (copy constructor)
			 *
			 * parameter : 
			 * const Mat3& other : other Mat3 to copy
			 *
			 * brief : construct Mat3 in function of over Mat3. The new Mat3 is a copy 
			 */
			Mat3(const Mat3& other);

				/*----------*/
			/* accessor */
			/*----------*/

			unsigned int 	line	()	const		{	return 3;	}
			unsigned int 	column  ()	const		{	return 3;	}
			unsigned int 	size	()	const		{   return 9;	}


			//static function that return scale Mat3 with scale in parameter
			static Mat3 createScaleMatrix		(const Vec3& tVec);

			//few static function that return rotation in x, y or z axis arround origin. Rotation is in degres
			static Mat3 createXRotationMatrix		(float rotRadx); //rot of axis Y to axis Z arround X
			static Mat3 createYRotationMatrix		(float rotRady); //rot of axis Z to axis X arround Y
			static Mat3 createZRotationMatrix		(float rotRadz); //rot of axis X to axis Y arround Z

			//static function that return rotation matrix arround unit axis give in parameter.
			//This function use the Rodrigues' rotation formula
			static Mat3 createRotationArroundAxisMatrix	(const Vec3& unitAxis, float angleRad);


			//static function that return euler rotation arround axis x, y and z give in parameter
			static Mat3 createFixedAngleEulerRotationMatrix	(const Vec3& rVec);

			//return true of false if matrix is orthogonal. If M*M.transpose() = I. Assert if matrix is not square
			bool		isOrtho		() const;

				//return (type float): return the determinant usfull to found the inverse matrix. If determinant is egal to 0, matrix can be reseable
				// brief : function to calcul the derteminant of square matrix 3*3. Create assert if matrix too big.
			float	foundDeterminant	() const;

			/**
			 * function : transpose
			 *
			 * return (type Mat3&	): return ref of current matrice after transposition
			 *
			 * brief : transpose matrix. The line become columne
			 */
			Mat3&		transpose		();
			Mat3		getTranspose	() const;

			/**
			 * function : getMinor
			 *
			 * parameter : 
			 *  unsigned int i : start to 0 just to n - 1 
			 *	unsigned int j : start to 0 just to n - 1
				* 
				*
				* return (type float):
				*
				* brief : this function return the determinant of minor element in function of M aij. Assert if matrix is not square.
				*/
			float		getMinor		(unsigned int i, unsigned int j) const;


			/**
			 * function : getCofactor
			 *
			 * parameter : 
			 *  unsigned int i : start to 0 just to n - 1 
			 *	unsigned int j : start to 0 just to n - 1
				*
				* return (type float):
				*
				* brief : return the result of coeficient multipliate by his cofactor multipliate by the signe : (-1)^(i+j) * minor(i, j)
				*/
			float		getCofactor		(unsigned int i, unsigned int j) const;
				

			/**
			 * function : getCoMatrix
			 *
			 * parameter : 
			 *
			 * return (type Matrix):
			 *
			 * brief : return the cofactor matrix of the current matrix. Assert if is square
			 */
			Mat3		getCoMatrix		() const;
		
			/**
			 * function : getCoMatrix
			 *
			 * parameter : 
			 *
			 * return (type Matrix):
			 *
			 * brief : transpose the cofactor matrix. Assert if is square
			 */
			void		tranformCoMatToAdjointMat		();

			/**
			 * function : getCoMatrix
			 *
			 * parameter : 
			 *
			 * return (type bool): return if matrx can be invert (if zero was found in matrix)
			 *
			 * brief : retrun if the adjoint matrix can be inverse. Assert if is square
			 */
			bool		adjointMatrixIsReversible		() const;

			/**
			 * function : getinverse
			 *
			 * parameter : 
			 *
			 * return (type bool): return false if inverse is not possible
			 *
			 * brief : reserse matrix if it's possible, else return false. 
			 * If function if orthogonal, this function return the transposate of matrix.
			 * Assert if is square or empty
			 */
			bool	inverse		(Mat3& reversMat) const;

			/**
			 * @brief Get the Vector Up object
			 * 
			 * @return Vec3 
			 */
			Vec3 	getVectorUp() 		const;

			/**
			 * @brief Get the Vector Right object. For indirect referential
			 * 
			 * @return Vec3 
			 */
			Vec3 	getVectorRight() 	const;

			/**
			 * @brief Get the Vector Forward object
			 * 
			 * @return Vec3 
			 */
			Vec3 	getVectorForward() 	const;

			/**
			 * @brief Create a Look At View object. Don't intagrat translation and scale. Only look at in direction of
			 * 
			 * @param eye
			 * @param center
			 * @param up
			 * @return Mat3
			 */
			static
			Mat3 createLookAtView (Vec3 const & eye, Vec3 const & center, Vec3 const & up);

			/*----------*/
			/* operator */
			/*----------*/

			/**
			 * function : operator[]
			 *
			 * parameter : 
			 * unsigned int indexLine : index of line in Mat3
			 *
			 * return (type float*):return tab of float corresponding to line of Mat3.
			 *
			 * brief : this function return tab of float corresponding to line of Mat3.
			 * This form allow this call Mat3[n][m]. Thirst element start to 0 : [0][0]. Max = [line-1][colomn-1]
			 */
			const float*		operator[]		(unsigned int indexLine) const;

			/**
			 * function : operator[]
			 *
			 * parameter : 
			 * unsigned int indexLine : index of line in Mat3
			 *
			 * return (type float*):return tab of float corresponding to line of Mat3.
			 *
			 * brief : this function return tab of float corresponding to line of Mat3.
			 * This form allow this call Mat3[n][m]. Thirst element start to 0 : [0][0]. Max = [line-1][colomn-1]
			 */
			float*		operator[]		(unsigned int indexLine);


			/**
			 * function : operator==
			 *
			 * parameter : 
			 * const Mat3& other: 
			 *
			 * return (type bool):
			 *
			 * brief : return true check if Mat3 A egal other Mat3.
			 */
			bool	operator==		(const Mat3& other) const;

			/**
			 * function : operator=
			 *
			 * parameter : 
			 * const Mat3& other: 
			 *
			 * return (type Mat3&):
			 *
			 * brief : copy content of other Mat3 in Mat3.
			 */
			const Mat3&		operator=		(const Mat3& other);
		

			/**
			 * function : operator+=
			 *
			 * parameter : 
			 * const Mat3& other
			 * 
			 * return (type Mat3&):
			 *
			 * brief : add Mat3 en return result.
			 * Assert is other Mat3 size is not egal to Mat3
			 */
			Mat3&		operator+=		(const Mat3& other);

			/**
			 * function : operator-=
			 *
			 * parameter : 
			 * const Mat3& other
			 * 
			 * return (type Mat3&):
			 *
			 * brief : sub Mat3 en return result.
			 * Assert is other Mat3 size is not egal to Mat3
			 */
			Mat3&		operator-=		(const Mat3& other);

			/**
			 * function : operator*=
			 *
			 * parameter : 
			 * const Mat3& other
			 * 
			 * return (type Mat3&):
			 *
			 * brief : multiplie Mat3 en return result.
			 * Assert if other Mat3 line is not egal to matrx's column.
			 */
			Mat3&		operator*=		(const Mat3& other);
		
			/**
			 * function : operator+
			 *
			 * parameter : 
			 * Mat3 other : copy of other Mat3. Will be return after operation
			 *
			 * return (type Mat3):
			 *
			 * brief : create Mat3 of adition between *this and other.
			 * Assert is other Mat3 size is not egal to Mat3
			 */
			Mat3		operator+		(Mat3 other) const;

			/**
			 * function : operator-
			 *
			 * parameter : 
			 * Mat3 other : copy of other Mat3. Will be return after operation
			 *
			 * return (type Mat3):
			 *
			 * brief : create Mat3 of substraction between *this and other.
			 * Assert is other Mat3 size is not egal to Mat3
			 */
			Mat3		operator-		(Mat3 other) const;
		

			/**
			 * function : operator*
			 *
			 * parameter : 
			 * const Mat3& other :
			 *
			 * return (type Mat3):
			 *
			 * brief : create Mat3 of multiplication between *this and other according to Mat3 multiplication formul.
			 * New Mat3 (M) size with multiplication of A and B is egal to M[An][Am]
			 * Assert if other Mat3 line is not egal to matrx's column.
			 */
			Mat3		operator*		(const Mat3& other) const;

			/**
			 * function : operator*
			 *
			 * parameter : 
			 * const Mat3& other :
			 *
			 * return (type Vec4):
			 *
			 * brief : create Mat3 of multiplication between *this and other according to Mat3 multiplication formul.
			 * New Mat3 (M) size with multiplication of A and B is egal to M[An][Am]
			 * Assert if other Mat3 line is not egal to matrx's column.
			 */
			Vec3		operator*		(const Vec3& other) const;

			::std::array <float, 9> mat;

	} Matix3D;

	//like display function. Display float this prescision of 3 number befor coma.
	::std::ostream& 	operator<<		(::std::ostream& out, const Mat3& mat);

	typedef struct Mat4
	{
			/**
			 * function : Mat4 (constructor)
			 *
			 * parameter : 
			 * unsigned int size : height and width of squart Mat4
			 *
			 * brief : construct identity Mat4. This Mat4 is squart 
			 */
			Mat4();
			Mat4(	float, float, float, float,
					float, float, float, float,
					float, float, float, float,
					float, float, float, float);
			
			Mat4(	const Vec4& v1, const Vec4& v2, const Vec4& v3, const Vec4& v4);
			Mat4(	const Mat3& m);


			/**
			 * function : Mat4 (constructor)
			 *
			 * parameter : 
			 * unsigned int line : number of line
			 * unsigned int column : number of column
			 * float value : value to set in Mat4 
			 *
			 * brief : create null Mat4 with x line en y column 
			 */
			Mat4(float value);

			/**
			 * function : Mat4 (copy constructor)
			 *
			 * parameter : 
			 * const Mat4& other : other Mat4 to copy
			 *
			 * brief : construct Mat4 in function of over Mat4. The new Mat4 is a copy 
			 */
			Mat4(const Mat4& other);

			/*----------*/
			/* accessor */
			/*----------*/

			unsigned int 	line	()	const		{	return 4;	}
			unsigned int 	column  ()	const		{	return 4;	}
			unsigned int 	size	()	const		{   return 16;	}

				/*----------*/
			/* Methods  */
			/*----------*/

			//static function that return scale Mat4 with scale in parameter
			static Mat4 createScaleMatrix			(const Vec3& tVec);

			//static function that return translation Mat4 with translation in parameter
			static Mat4 createTranslationMatrix		(const Vec3& tVec);

			//few static function that return rotation in x, y or z axis arround origin. Rotation is in degres
			static Mat4 createXRotationMatrix		(float rotRadx); //rot of axis Y to axis Z arround X
			static Mat4 createYRotationMatrix		(float rotRady); //rot of axis Z to axis X arround Y
			static Mat4 createZRotationMatrix		(float rotRadz); //rot of axis X to axis Y arround Z

			//static function that return rotation matrix arround unit axis give in parameter.
			//This function use the Rodrigues' rotation formula
			static Mat4 createRotationArroundAxisMatrix	(const Vec3& unitAxis, float angleRad);

			//static function that return euler rotation arround axis x, y and z give in parameter
			static Mat4 createFixedAngleEulerRotationMatrix	(const Vec3& rVec);

			//static function that create projection in orthographinc Mat4
			static Mat4 createOrthoMatrix	(float left, float right, float bottom, float top, float nearVal, float farVal);

			//static function that create projection in perspective matrix
			static Mat4 createPerspectiveMatrix  (float aspect, float near, float far, float fov);

			//static function that return projection Mat4 to pass from 3D to 2D.
			//Distance in parameter is the distance between pin hole and user.
			static Mat4 createProjectionMatrix (float distance);

			//Build a look at view matrix
			//eyeX, eyeY, eyeZ : Specifies the position of the eye point
			//centerX, centerY, centerZ : Specifies the position of the reference point
			//upX, upY, upZ : Specifies the direction of the up vector
			static Mat4 createLookAtView (Vec3 const & eye, Vec3 const & center, Vec3 const & up);

			//static function that return TRS Mat4 (Mat4 to passing to local to global wit scale, rotation and translation). 
			//Same as creat transform Mat4
			static Mat4 createTRSMatrix(const Vec3& scaleVec, const Vec3& rotVec, const Vec3& translVec);

			//return true of false if matrix is orthogonal. If M*M.transpose() = I. Assert if matrix is not square
			bool		isOrtho		() const;

				//return (type float): return the determinant usfull to found the inverse matrix. If determinant is egal to 0, matrix can be reseable
				// brief : function to calcul the derteminant of square matrix 4*4. Create assert if matrix too big.
			float	foundDeterminant	() const;

			/**
			 * function : transpose
			 *
			 * return (type Mat4&	): return ref of current matrice after transposition
			 *
			 * brief : transpose matrix. The line become columne
			 */
			Mat4&		transpose		();
			Mat4		getTranspose	() const;

			/**
			 * function : getMinor
			 *
			 * parameter : 
			 *  unsigned int i : start to 0 just to n - 1 
			 *	unsigned int j : start to 0 just to n - 1
				* 
				*
				* return (type float):
				*
				* brief : this function return the determinant of minor element in function of M aij. Assert if matrix is not square.
				*/
			float		getMinor		(unsigned int i, unsigned int j) const;


			/**
			 * function : getCofactor
			 *
			 * parameter : 
			 *  unsigned int i : start to 0 just to n - 1 
			 *	unsigned int j : start to 0 just to n - 1
				*
				* return (type float):
				*
				* brief : return the result of coeficient multipliate by his cofactor multipliate by the signe : (-1)^(i+j) * minor(i, j)
				*/
			float		getCofactor		(unsigned int i, unsigned int j) const;
				

			/**
			 * function : getCoMatrix
			 *
			 * parameter : 
			 *
			 * return (type Matrix):
			 *
			 * brief : return the cofactor matrix of the current matrix. Assert if is square
			 */
			Mat4		getCoMatrix		() const;

			/**
			 * @brief Get the Vector Up object
			 * 
			 * @return Vec3 
			 */
			Vec3 	getVectorUp() 		const;
			void 	setVectorUp(const Vec3& newUp);

			/**
			 * @brief Get the Vector Right object. For indirect referential
			 * 
			 * @return Vec3 
			 */
			Vec3 	getVectorRight() 	const;
			void 	setVectorRight(const Vec3& newRight);

			/**
			 * @brief Get the Vector Forward object
			 * 
			 * @return Vec3 
			 */
			Vec3 	getVectorForward() 	const;
			void 	setVectorForward(const Vec3& newForward);
		
			/**
			 * function : getCoMatrix
			 *
			 * parameter : 
			 *
			 * return (type Matrix):
			 *
			 * brief : transpose the cofactor matrix. Assert if is square
			 */
			void		tranformCoMatToAdjointMat		();

			/**
			 * function : getCoMatrix
			 *
			 * parameter : 
			 *
			 * return (type bool): return if matrx can be invert (if zero was found in matrix)
			 *
			 * brief : retrun if the adjoint matrix can be inverse. Assert if is square
			 */
			bool		adjointMatrixIsReversible		() const;

			/**
			 * function : getinverse
			 *
			 * parameter : 
			 *
			 * return (type bool): return false if inverse is not possible
			 *
			 * brief : reserse matrix if it's possible, else return false. 
			 * If function if orthogonal, this function return the transposate of matrix.
			 * Assert if is square or empty
			 */
			bool	inverse		(Mat4& reversMat) const;


			/*----------*/
			/* operator */
			/*----------*/

			/**
			 * function : operator[]
			 *
			 * parameter : 
			 * unsigned int indexLine : index of line in Mat4
			 *
			 * return (type float*):return tab of float corresponding to line of Mat4.
			 *
			 * brief : this function return tab of float corresponding to line of Mat4.
			 * This form allow this call Mat4[n][m]. Thirst element start to 0 : [0][0]. Max = [line-1][colomn-1]
			 */
			const float*		operator[]		(unsigned int indexLine) const;

			/**
			 * function : operator[]
			 *
			 * parameter : 
			 * unsigned int indexLine : index of line in Mat4
			 *
			 * return (type float*):return tab of float corresponding to line of Mat4.
			 *
			 * brief : this function return tab of float corresponding to line of Mat4.
			 * This form allow this call Mat4[n][m]. Thirst element start to 0 : [0][0]. Max = [line-1][colomn-1]
			 */
			float*		operator[]		(unsigned int indexLine);


			/**
			 * function : operator==
			 *
			 * parameter : 
			 * const Mat4& other: 
			 *
			 * return (type bool):
			 *
			 * brief : return true check if Mat4 A egal other Mat4.
			 */
			bool	operator==		(const Mat4& other) const;

			/**
			 * function : operator=
			 *
			 * parameter : 
			 * const Mat4& other: 
			 *
			 * return (type Mat4&):
			 *
			 * brief : copy content of other Mat4 in Mat4.
			 */
			const Mat4&		operator=		(const Mat4& other);
		

			/**
			 * function : operator+=
			 *
			 * parameter : 
			 * const Mat4& other
			 * 
			 * return (type Mat4&):
			 *
			 * brief : add Mat4 en return result.
			 * Assert is other Mat4 size is not egal to Mat4
			 */
			Mat4&		operator+=		(const Mat4& other);

			/**
			 * function : operator-=
			 *
			 * parameter : 
			 * const Mat4& other
			 * 
			 * return (type Mat4&):
			 *
			 * brief : sub Mat4 en return result.
			 * Assert is other Mat4 size is not egal to Mat4
			 */
			Mat4&		operator-=		(const Mat4& other);

			/**
			 * function : operator*=
			 *
			 * parameter : 
			 * const Mat4& other
			 * 
			 * return (type Mat4&):
			 *
			 * brief : multiplie Mat4 en return result.
			 * Assert if other Mat4 line is not egal to matrx's column.
			 */
			Mat4&		operator*=		(const Mat4& other);
		
			/**
			 * function : operator+
			 *
			 * parameter : 
			 * Mat4 other : copy of other Mat4. Will be return after operation
			 *
			 * return (type Mat4):
			 *
			 * brief : create Mat4 of adition between *this and other.
			 * Assert is other Mat4 size is not egal to Mat4
			 */
			Mat4		operator+		(Mat4 other) const;

			/**
			 * function : operator-
			 *
			 * parameter : 
			 * Mat4 other : copy of other Mat4. Will be return after operation
			 *
			 * return (type Mat4):
			 *
			 * brief : create Mat4 of substraction between *this and other.
			 * Assert is other Mat4 size is not egal to Mat4
			 */
			Mat4		operator-		(Mat4 other) const;
		

			/**
			 * function : operator*
			 *
			 * parameter : 
			 * const Mat4& other :
			 *
			 * return (type Mat4):
			 *
			 * brief : create Mat4 of multiplication between *this and other according to Mat4 multiplication formul.
			 * New Mat4 (M) size with multiplication of A and B is egal to M[An][Am]
			 * Assert if other Mat4 line is not egal to matrx's column.
			 */
			Mat4		operator*		(const Mat4& other) const;
			Mat4		operator*		(const float scale) const;

			/**
			 * function : operator*
			 *
			 * parameter : 
			 * const Mat4& other :
			 *
			 * return (type Vec4):
			 *
			 * brief : create Mat4 of multiplication between *this and other according to Mat4 multiplication formul.
			 * New Mat4 (M) size with multiplication of A and B is egal to M[An][Am]
			 * Assert if other Mat4 line is not egal to matrx's column.
			 */
			Vec4		operator*		(const Vec4& other) const;
			//considere that the vector is normalize. W = 1
			Vec4		operator*		(const Vec3& other) const;

			::std::array <float, 16> mat;

	} Matix4D;

	//like display function. Display float this prescision of 3 number befor coma.
	::std::ostream& 	operator<<		(::std::ostream& out, const Mat4& mat);

	#include "mat.inl"

} //namespace Engine::Core::Maths

#endif // _GE_MAT_H
