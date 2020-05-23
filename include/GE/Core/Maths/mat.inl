//////////////////
//// Mat3
//////////////////

inline
Mat3::Mat3()
	: mat {	1.f, 0.f, 0.f,
       		0.f, 1.f, 0.f,
       		0.f, 0.f, 1.f}
{}

inline
Mat3::Mat3(	float a11, float a12, float a13, float a21, float a22, float a23, float a31, float a32, float a33)
#ifdef LINE_CONVENTION
	: mat {	a11, a21, a31,
       		a12, a22, a33,
       		a13, a23, a33}
#else
	: mat {	a11, a12, a13,
       		a21, a22, a23,
       		a31, a32, a33}
#endif
{}

inline
Mat3::Mat3(	const Vec3& v1, const Vec3& v2, const Vec3& v3)
#ifdef LINE_CONVENTION
	: mat {	v1.x, v1.y, v1.z
       		v2.x, v2.y, v2.z
			v3.x, v3.y, v3.z}
#else
	: mat {	v1.x, v2.x, v3.x,
       		v1.y, v2.y, v3.y,
			v1.z, v2.z, v3.z}
#endif
{}

inline
Mat3::Mat3(	const Mat4& m)
	: mat {	m[0][0], m[1][0], m[2][0],
       		m[0][1], m[1][1], m[2][1],
       		m[0][2], m[1][2], m[2][2]}
{}


inline
Mat3::Mat3(float value)
{
    mat.fill(value);
}

inline
Mat3::Mat3(const Mat3& other)
	: mat (other.mat)
{}

inline
Mat3 Mat3::createScaleMatrix(const Vec3& sVec)
{
    //will be optimisate by compilator
    float kx = sVec.x;
    float ky = sVec.y;
    float kz = sVec.z;

	return {  kx , 0.f, 0.f,
              0.f, ky , 0.f,
              0.f, 0.f, kz };
}

inline
Mat3 Mat3::createXRotationMatrix		(float rotRadx)
{
	float cosT, sinT;
	sincosf(rotRadx, &sinT, &cosT);

	return {  1.f, 0.f , 0.f ,
              0.f, cosT,-sinT,
              0.f, sinT, cosT};
}

inline
Mat3 Mat3::createYRotationMatrix		(float rotRady)
{
	float cosT, sinT;
	sincosf(rotRady, &sinT, &cosT);

	return {  cosT , 0.f, sinT,
              0.f  , 1.f, 0.f ,
              -sinT, 0.f, cosT};
}

inline
Mat3 Mat3::createZRotationMatrix		(float rotRadz)
{
	float cosT, sinT;
	sincosf(rotRadz, &sinT, &cosT);

	return {  cosT, -sinT, 0.f,
              sinT,  cosT, 0.f,
              0.f ,  0.f , 1.f};
}

inline
Mat3 Mat3::createRotationArroundAxisMatrix	(const Vec3& unitAxis, float angleRad)
{
	float s, c;
	sincosf(angleRad, &s, &c); //comput sin and cos in same time
	float t = (1 - c);
	float txy = t * unitAxis.x * unitAxis.y;
	float txz = t * unitAxis.x * unitAxis.z;
	float tyz = t * unitAxis.y * unitAxis.z;
	float txx = t * unitAxis.x * unitAxis.x;
	float tyy = t * unitAxis.y * unitAxis.y;
	float tzz = t * unitAxis.z * unitAxis.z;

	float sx = s * unitAxis.x;
	float sy = s * unitAxis.y;
	float sz = s * unitAxis.z;

	return { txx + c , txy - sz, txz + sy,
             txy + sz, tyy + c , tyz - sx,
             txz - sy, tyz + sx, tzz + c};
}

inline
Mat3 Mat3::createFixedAngleEulerRotationMatrix	(const Vec3& rVec)
{

#ifdef LINE_CONVENTION
	return 	createYRotationMatrix (rVec.y) * 
			createXRotationMatrix (rVec.x) *
			createZRotationMatrix (rVec.z);
#else
	return 	createYRotationMatrix (rVec.z) * 
			createXRotationMatrix (rVec.x) *
			createZRotationMatrix (rVec.y);
#endif
}

inline
bool	Mat3::isOrtho		() const
{
	return getTranspose() * (*this) == Mat3{}; //if egal identity matrix
}

inline
Mat3&		Mat3::transpose		()
{
	//Swap first line with first column
	::std::swap(mat[1], mat[3]);
	::std::swap(mat[2], mat[6]);

	//Swap second line with second column
	::std::swap(mat[5], mat[7]);

	return *this;
}

inline
Mat3		Mat3::getTranspose	() const
{
	Mat3 rMat (*this);
	return rMat.transpose();
}

inline
float Mat3::foundDeterminant		() const
{
	float determinantMat1 = (*this)[1][1] * (*this)[2][2] - (*this)[2][1] * (*this)[1][2];
	float determinantMat2 = (*this)[0][1] * (*this)[2][2] - (*this)[2][1] * (*this)[0][2];
	float determinantMat3 = (*this)[0][1] * (*this)[1][2] - (*this)[1][1] * (*this)[0][2];

	return 	  (*this)[0][0] * determinantMat1 
			- (*this)[1][0] * determinantMat2 
			+ (*this)[2][0] * determinantMat3;
}

inline
float		Mat3::getMinor		(unsigned int i, unsigned int j) const
{
	::std::array<float, 4> subMatrix;

	//fill submatrix
	bool coefLineFound = false;
	bool coefRowFound = false;

	for (unsigned int iSubMatrix = 0; iSubMatrix < 2; iSubMatrix++)
	{
		if (iSubMatrix == i)
			coefLineFound = true;

		for (unsigned int jSubMatrix = 0; jSubMatrix < 2; jSubMatrix++)
		{
			if (jSubMatrix == j)
				coefRowFound = true;

			subMatrix[iSubMatrix * 2 + jSubMatrix] = (*this)[iSubMatrix + coefLineFound]
															[jSubMatrix + coefRowFound];
		}
		coefRowFound = false;
	}

	return subMatrix[0] * subMatrix[3] - subMatrix[2] * subMatrix[1];
}

inline
float	Mat3::getCofactor		(unsigned int i, unsigned int j) const
{
	return pow(-1, i+j) * getMinor(i, j);
}

inline
Mat3		Mat3::getCoMatrix		() const
{
	Mat3 coMatrix;

    for ( unsigned int i = 0; i < line() ; i++ )
	{
        for ( unsigned int j = 0; j < column(); j++ )
		{
			coMatrix[i][j] = getCofactor(i, j);
		}
	}
	
	return coMatrix;
}

inline
void		Mat3::tranformCoMatToAdjointMat		()
{
	transpose();
}

inline
bool		Mat3::adjointMatrixIsReversible		() const
{
    for ( unsigned int i = 0; i < line(); i++ )
	{
        for ( unsigned int j = 0; j < column(); j++ )
		{
			if ((*this)[i][j] == 0.f)
				return false;
		}
	}

	return true;
}


inline
bool	Mat3::inverse		(Mat3& reversMat) const
{
	if (isOrtho() == true)
	{
		reversMat = getTranspose();
		return true;
	}

	float determinant = foundDeterminant();
	
	if (determinant == 0.f) //in two step for more perform
		return false;

	reversMat = getCoMatrix();
	reversMat.tranformCoMatToAdjointMat();

	if (!reversMat.adjointMatrixIsReversible()) //in two step for more perform
		return false;

    for ( size_t i = 0; i < line(); i++ )
	{
        for ( size_t j = 0; j < column(); j++ )
		{
			 reversMat[i][j] =  reversMat[i][j] / determinant;
		}
	}

	return true;
}

inline
Vec3 	Mat3::getVectorUp() 		const
{
	return Vec3{mat[1], mat[4], mat[7]};
}

inline 
Vec3 	Mat3::getVectorRight() 	const
{
	return Vec3{mat[0], mat[3], mat[6]};
}

inline
Vec3 	Mat3::getVectorForward() 	const
{
	return Vec3{mat[2], mat[5], mat[8]};
}

inline
Mat3 Mat3::createLookAtView (Vec3 const & eye, Vec3 const & center, Vec3 const & up)
{
	Vec3 const f((center - eye).getNormalize());
	Vec3 const s(f.getCross(up).getNormalize());
	Vec3 const u(s.getCross(f));

	return {  s.x, u.x, -f.x,
              s.y, u.y, -f.y,
              s.z, u.z, -f.z};
}

inline
const float*		Mat3::operator[]		(unsigned int indexLine) const
{
#ifdef LINE_CONVENTION
	return &mat[indexLine * line()];
#else
	return &mat[indexLine * column()];
#endif
}

inline
float*		Mat3::operator[]		(unsigned int indexLine)
{
#ifdef LINE_CONVENTION
	return &mat[indexLine * line()];
#else
	return &mat[indexLine * column()];
#endif
}

inline
bool		Mat3::operator==		(const Mat3& other) const
{
    return 	other.mat == mat;
}

inline
const Mat3&		Mat3::operator=		(const Mat3& other)
{
    mat = other.mat;
	return *this;
}

inline
Mat3&		Mat3::operator+=		(const Mat3& other)
{
    for (size_t i = 0; i < size(); i++ )
    {
        mat[i] += other.mat[i];
    }
    
	return *this;
}


inline
Mat3&		Mat3::operator-=		(const Mat3& other)
{
    for (size_t i = 0; i < size(); i++ )
    {
        mat[i] -= other.mat[i];
    }

	return *this;
}


inline
Mat3&		Mat3::operator*=		(const Mat3& other)
{
    for ( size_t i = 0; i < column() ; i++ )
	{
        for ( size_t j = 0; j < line(); j++ )
		{
			float result = 0.f;

			for ( size_t index = 0; index < 3; index++)
			{
				result += ((*this)[i][index] * other[index][j]);
			}	
		}
	}
		
	return *this;
}


inline
Mat3		Mat3::operator+		(Mat3 other) const
{
    for (size_t i = 0; i < size(); i++ )
    {
        other.mat[i] += mat[i];
    }

	return other;
}


inline
Mat3		Mat3::operator-		(Mat3 other) const
{
    for (size_t i = 0; i < size(); i++ )
    {
        other.mat[i] -= mat[i];
    }

	return other;
}

inline
Mat3		Mat3::operator*		(const Mat3& other) const
{
	Mat3 mResult {};

#ifdef LINE_CONVENTION
    for ( size_t i = 0; i < column() ; i++ )
	{
        for ( size_t j = 0; j < line(); j++ )
		{
			float result = 0.f;

			for ( size_t index = 0; index < 3; index++)
			{
				result += ((*this)[i][index] * other[index][j]);
			}
			mResult[i][j] = result;
		}
	}
#else
    for ( size_t i = 0; i < line() ; i++ )
	{
        for ( size_t j = 0; j < column(); j++ )
		{
			float result = 0.f;

			for ( size_t index = 0; index < 3; index++)
			{
				result += ((*this)[index][i] * other[j][index]);
			}
			mResult[j][i] = result;
		}
	}
#endif

	return mResult;
}

inline
Vec3	Mat3::operator*		(const Vec3& other) const
{
	Vec3 vResult {};

    for ( size_t j = 0; j < line(); j++ )
	{
		for ( size_t i = 0; i < column() ; i++ )
		{
			vResult[j] += mat[j * column() + i] * other.e[i];
		}
	}

	return vResult;
}


inline
::std::ostream& 	operator<<		(::std::ostream& out, const Mat3& mat)
{
	::std::cout.precision(3);
    for ( size_t i = 0; i < mat.column() ; i++ )
	{
        for ( size_t j = 0; j < mat.line(); j++ )
        {
            out << mat[i][j] << "	";
        }
	    out << ::std::endl;
    }
	return out;
}

//////////////////
//// Mat4
//////////////////

inline
Mat4::Mat4()
	:mat  {1.f, 0.f, 0.f, 0.f,
           0.f, 1.f, 0.f, 0.f,
           0.f, 0.f, 1.f, 0.f,
           0.f, 0.f, 0.f, 1.f}
{}

inline
Mat4::Mat4(	float a11, float a12, float a13, float a14, float a21, float a22, float a23, float a24, float a31, float a32, float a33, float a34, float a41, float a42, float a43, float a44)
#ifdef LINE_CONVENTION
	: mat {	a11, a12, a13, a14,
       		a21, a22, a23, a24,
       		a31, a32, a33, a34,
       		a41, a42, a43, a44}
#else
	: mat {	a11, a21, a31, a41,
       		a12, a22, a32, a42,
       		a13, a23, a33, a43,
       		a14, a24, a34, a44}
#endif
{}

inline
Mat4::Mat4(	const Vec4& v1, const Vec4& v2, const Vec4& v3, const Vec4& v4)
#ifdef LINE_CONVENTION
	: mat {	v1.x, v1.y, v1.z, v1.w,
       		v2.x, v2.y, v2.z, v2.w,
			v3.x, v3.y, v3.z, v3.w,
       		v4.x, v4.y, v4.z, v4.w,}
#else
	: mat {	v1.x, v2.x, v3.x, v4.x,
       		v1.y, v2.y, v3.y, v4.y,
			v1.z, v2.z, v3.z, v4.z,
       		v1.w, v2.w, v3.w, v4.w,}
#endif
{}

inline
Mat4::Mat4(	const Mat3& m)
	: mat {	m[0][0], m[1][0], m[2][0], 0.f,
       		m[0][1], m[1][1], m[2][1], 0.f,
       		m[0][2], m[1][2], m[2][2], 0.f,
       		0.f    , 0.f    , 0.f    , 1.f}
{}

inline
Mat4::Mat4(float value)
{
    mat.fill(value);
}

inline
Mat4::Mat4(const Mat4& other)
	:  mat (other.mat)
{}

inline
Mat4 Mat4::createScaleMatrix(const Vec3& sVec)
{
    //will be optimisate by compilator
    float kx = sVec.x;
    float ky = sVec.y;
    float kz = sVec.z;

	return {  kx , 0.f, 0.f, 0.f,
              0.f, ky , 0.f, 0.f,
              0.f, 0.f, kz , 0.f,
              0.f, 0.f, 0.f, 1.f};
}

inline
Mat4 Mat4::createTranslationMatrix(const Vec3& tVec)
{
    //will be optimisate by compilator
    float Ux = tVec.x;
    float Uy = tVec.y;
    float Uz = tVec.z;

	return {  1.f, 0.f, 0.f, Ux,
              0.f, 1.f, 0.f, Uy,
              0.f, 0.f, 1.f, Uz,
              0.f, 0.f, 0.f, 1.f};
}

inline
Mat4 Mat4::createXRotationMatrix		(float rotRadx)
{
	float cosT = cosf(rotRadx);
	float sinT = sinf(rotRadx);

	return {  1.f, 0.f , 0.f , 0.f,
              0.f, cosT,-sinT, 0.f,
              0.f, sinT, cosT, 0.f,
              0.f, 0.f , 0.f , 1.f};
}

inline
Mat4 Mat4::createYRotationMatrix		(float rotRady)
{
	float cosT = cosf(rotRady);
	float sinT = sinf(rotRady);

	return {  cosT , 0.f, sinT, 0.f,
              0.f  , 1.f, 0.f , 0.f,
              -sinT, 0.f, cosT, 0.f,
              0.f  , 0.f, 0.f , 1.f};
}

inline
Mat4 Mat4::createZRotationMatrix		(float rotRadz)
{
	float cosT = cosf(rotRadz);
	float sinT = sinf(rotRadz);

	return {  cosT, -sinT, 0.f, 0.f,
              sinT,  cosT, 0.f, 0.f,
              0.f ,  0.f , 1.f, 0.f,
              0.f ,  0.f , 0.f, 1.f};
}

inline
Mat4 Mat4::createRotationArroundAxisMatrix	(const Vec3& unitAxis, float angleRad)
{
	float s, c;
	sincosf(angleRad, &s, &c); //comput sin and cos in same time
	float t = (1 - c);
	float txy = t * unitAxis.x * unitAxis.y;
	float txz = t * unitAxis.x * unitAxis.z;
	float tyz = t * unitAxis.y * unitAxis.z;
	float txx = t * unitAxis.x * unitAxis.x;
	float tyy = t * unitAxis.y * unitAxis.y;
	float tzz = t * unitAxis.z * unitAxis.z;

	float sx = s * unitAxis.x;
	float sy = s * unitAxis.y;
	float sz = s * unitAxis.z;

	return {txx + c , txy - sz, txz + sy, 0.f,
            txy + sz, tyy + c , tyz - sx, 0.f,
            txz - sy, tyz + sx, tzz + c , 0.f,
			0.f     , 0.f     , 0.f     , 1.f};
}

inline
Mat4 Mat4::createFixedAngleEulerRotationMatrix	(const Vec3& rVec)
{
#ifdef LINE_CONVENTION
	return 	createYRotationMatrix (rVec.z) * 
			createXRotationMatrix (rVec.x) *
			createZRotationMatrix (rVec.y);
#else
	return 	createYRotationMatrix (rVec.y) * 
			createXRotationMatrix (rVec.x) * 
			createZRotationMatrix (rVec.z);
#endif
}

inline
Mat4 Mat4::createTRSMatrix(const Vec3& scaleVec, const Vec3& rotVec, const Vec3& translVec)
{
#ifdef LINE_CONVENTION
	return 	createScaleMatrix (scaleVec) * 
			createFixedAngleEulerRotationMatrix (rotVec) *
			createTranslationMatrix (translVec);
#else
	return 	createTranslationMatrix (translVec) * 
			createFixedAngleEulerRotationMatrix (rotVec) *
			createScaleMatrix (scaleVec);
#endif
}

inline
Mat4 Mat4::createOrthoMatrix	(float left, float right, float bottom, float top, float nearVal, float farVal)
{
    //will be optimisate by compilator
    float a11 = 2.f / (right - left);
    float a22 = 2.f / (top - bottom);
    float a33 = -2.f / (farVal - nearVal);

    float tx = (right + left) / (right - left);
    float ty = (top + bottom) / (top - bottom);  
    float tz = (farVal + nearVal)/ (farVal - nearVal);

	return {  a11, 0.f, 0.f, tx,
              0.f, a22, 0.f, ty,
              0.f, 0.f, a33, tz,
              0.f, 0.f, 0.f, 1.f};
}


inline
Mat4 Mat4::createPerspectiveMatrix 	(float aspect, float near, float far, float fov)
{
    float scale = tanf(fov * 0.5f * M_PI / 180.f) * near;
    float rigth = aspect * scale;

    //will be optimisate by compilator
    float left   = -rigth;
    float top    = scale;
    float bottom = -scale;

    float a11 = 2 * near / (rigth - left);
    float a22 = 2 * near / (top - bottom);
    float a31 = (rigth + left) / (rigth - left);
    float a32 = (top + bottom) / (top - bottom);
    float a33 = -(far + near) / (far - near);
    float a43 = -2 * far * near / (far - near);
    float zDir = -1.f;

	return {  a11, 0.f, 0.f , 0.f,
              0.f, a22, 0.f , 0.f,
              a31, a32, a33 , a43,
              0.f, 0.f, zDir, 0.f};
}

inline
Mat4 Mat4::createProjectionMatrix		(float distance)
{
    //will be optimisate by compilator
    float p {1.f / distance};

	return {  1.f, 0.f, 0.f, 0.f,
              0.f, 1.f, 0.f, 0.f,
              0.f, 0.f, 1.f, 0.f,
              0.f, 0.f, p  , 0.f};
}

inline
Mat4 Mat4::createLookAtView (Vec3 const & eye, Vec3 const & center, Vec3 const & upp)
{
	Vec3 const forward((center - eye).getNormalize());
	Vec3 const right(forward.getCross(upp).getNormalize());
	Vec3 const up(right.getCross(forward));

	return {  right.x	, right.y	, right.z		, -right.dotProduct(eye),
              up.x		, up.y		, up.z			, -up.dotProduct(eye),
              -forward.x, -forward.y, -forward.z	, forward.dotProduct(eye),
              0.f		, 0.f		,  0.f			, 1.f};
}

inline
bool	Mat4::isOrtho		() const
{
	return getTranspose() * (*this) == Mat4{}; //if egal identity matrix
}

inline
Mat4&		Mat4::transpose		()
{
	//Swap first line with first column
	::std::swap(mat[1], mat[4]);
	::std::swap(mat[2], mat[8]);
	::std::swap(mat[3], mat[12]);

	//Swap second line with second column
	::std::swap(mat[6], mat[9]);
	::std::swap(mat[7], mat[13]);

	//Swap third line with third column
	::std::swap(mat[11], mat[14]);

	return *this;
}

inline
Mat4		Mat4::getTranspose	() const
{
	Mat4 rMat (*this);
	return rMat.transpose();
}

inline
float Mat4::foundDeterminant		() const
{
	float determinantMat1 = (*this)[2][2] * (*this)[3][3] - (*this)[3][2] * (*this)[2][3]; 
	float determinantMat2 = (*this)[1][2] * (*this)[3][3] - (*this)[3][2] * (*this)[1][3]; 
	float determinantMat3 = (*this)[1][2] * (*this)[2][3] - (*this)[2][2] * (*this)[1][3];
	float determinantMat4 = (*this)[0][2] * (*this)[3][3] - (*this)[3][2] * (*this)[0][3];
	float determinantMat5 = (*this)[0][2] * (*this)[2][3] - (*this)[2][2] * (*this)[0][3];
	float determinantMat6 = (*this)[0][2] * (*this)[1][3] - (*this)[1][2] * (*this)[0][3];

	float determinantMat2_1 =  	  (*this)[1][1] * determinantMat1 
								- (*this)[2][1] * determinantMat2 
								+ (*this)[3][1] * determinantMat3; 

	float determinantMat2_2 =  	  (*this)[0][1] * determinantMat1 
								- (*this)[2][1] * determinantMat4 
								+ (*this)[3][1] * determinantMat5; 

	float determinantMat2_3 =  	  (*this)[0][1] * determinantMat2 
								- (*this)[1][1] * determinantMat4 
								+ (*this)[3][1] * determinantMat6;

	float determinantMat2_4 =  	  (*this)[0][1] * determinantMat3 
								- (*this)[1][1] * determinantMat5 
								+ (*this)[2][1] * determinantMat6;

	return 	  (*this)[0][0] * determinantMat2_1
			- (*this)[1][0] * determinantMat2_2
			+ (*this)[2][0] * determinantMat2_3
			- (*this)[3][0] * determinantMat2_4;
}

inline
float		Mat4::getMinor		(unsigned int i, unsigned int j) const
{
	Mat3 subMatrix;

	//fill submatrix
	bool coefLineFound = false;
	bool coefRowFound = false;

	for (unsigned int iSubMatrix = 0; iSubMatrix < subMatrix.line(); iSubMatrix++)
	{
		if (iSubMatrix == i)
			coefLineFound = true;

		for (unsigned int jSubMatrix = 0; jSubMatrix < subMatrix.column(); jSubMatrix++)
		{
			if (jSubMatrix == j)
				coefRowFound = true;

			subMatrix[iSubMatrix][jSubMatrix] = (*this)	[iSubMatrix + coefLineFound]
														[jSubMatrix + coefRowFound];
		}
		coefRowFound = false;
	}

	return subMatrix.foundDeterminant();
}

inline
float	Mat4::getCofactor		(unsigned int i, unsigned int j) const
{
	return pow(-1, i+j) * getMinor(i, j);
}

inline
Mat4		Mat4::getCoMatrix		() const
{
	Mat4 coMatrix;

    for ( unsigned int i = 0; i < line() ; i++ )
	{
        for ( unsigned int j = 0; j < column(); j++ )
		{
			coMatrix[i][j] = getCofactor(i, j);
		}
	}

	return coMatrix;
}

inline
Vec3 	Mat4::getVectorUp() 		const
{
	return Vec3{mat[9], mat[5], mat[1]};
}

inline 
Vec3 	Mat4::getVectorRight() 	const
{
	return Vec3{mat[10], mat[6], mat[2]};
}

inline
Vec3 	Mat4::getVectorForward() 	const
{
	return Vec3{mat[8], mat[4], mat[0]};

}

inline
void 	Mat4::setVectorUp(const Vec3& newUp)
{
	mat[9] = newUp.x;
	mat[5] = newUp.y;
	mat[1] = newUp.z;
}

inline 
void 	Mat4::setVectorRight(const Vec3& newRight) 
{
	mat[10] = newRight.x;
	mat[6] = newRight.y;
	mat[2] = newRight.z;
}

inline
void 	Mat4::setVectorForward(const Vec3& newForward)
{
	mat[8] = newForward.x;
	mat[4] = newForward.y;
	mat[0] = newForward.z;
}

inline
void		Mat4::tranformCoMatToAdjointMat		()
{
	transpose();
}

inline
bool		Mat4::adjointMatrixIsReversible		() const
{
    for ( unsigned int i = 0; i < line(); i++ )
	{
        for ( unsigned int j = 0; j < column(); j++ )
		{
			if ((*this)[i][j] == 0.f)
				return false;
		}
	}

	return true;
}


inline
bool	Mat4::inverse		(Mat4& m) const
{
	if (isOrtho() == true)
	{
		m = getTranspose();
		return true;
	}

	float determinant = foundDeterminant();
	
	if (determinant == 0.f) //in two step for more perform
		return false;

	m = getCoMatrix();
	m.tranformCoMatToAdjointMat();

	if (!m.adjointMatrixIsReversible()) //in two step for more perform
		return false;

	m = getCoMatrix();
	m.tranformCoMatToAdjointMat();

	if (!m.adjointMatrixIsReversible()) //in two step for more perform
		return false;

    for ( size_t i = 0; i < line(); i++ )
	{
        for ( size_t j = 0; j < column(); j++ )
		{
			 m[i][j] =  m[i][j] / determinant;
		}
	}

	return true;
}

inline
const float*		Mat4::operator[]		(unsigned int indexLine) const
{
#ifdef LINE_CONVENTION
	return &mat[indexLine * line()];
#else
	return &mat[indexLine * column()];
#endif
}

inline
float*		Mat4::operator[]		(unsigned int indexLine)
{
#ifdef LINE_CONVENTION
	return &mat[indexLine * line()];
#else
	return &mat[indexLine * column()];
#endif
}

inline
bool		Mat4::operator==		(const Mat4& other) const
{
    return 	other.mat == mat;
}

inline
const Mat4&		Mat4::operator=		(const Mat4& other)
{
    mat = other.mat;
	return *this;
}

inline
Mat4&		Mat4::operator+=		(const Mat4& other)
{
    for (size_t i = 0; i < size(); i++ )
    {
        mat[i] += other.mat[i];
    }

	return *this;
}


inline
Mat4&		Mat4::operator-=		(const Mat4& other)
{
    for (size_t i = 0; i < size(); i++ )
    {
        mat[i] -= other.mat[i];
    }

	return *this;
}


inline
Mat4&		Mat4::operator*=		(const Mat4& other)
{
#ifdef LINE_CONVENTION
    for ( size_t i = 0; i < column() ; i++ )
	{
        for ( size_t j = 0; j < line(); j++ )
		{
			float result = 0.f;

			for ( size_t index = 0; index < 4; index++)
			{
				result += ((*this)[i][index] * other[index][j]);
			}	
		}
	}

#else
    for ( size_t i = 0; i < line() ; i++ )
	{
        for ( size_t j = 0; j < column(); j++ )
		{
			float result = 0.f;

			for ( size_t index = 0; index < 4; index++)
			{
				result += ((*this)[index][i] * other[j][index]);
			}
		}
	}
#endif

	return *this;
}


inline
Mat4		Mat4::operator+		(Mat4 other) const
{
    for (size_t i = 0; i < size(); i++ )
    {
        other.mat[i] += mat[i];
    }

	return other;
}


inline
Mat4		Mat4::operator-		(Mat4 other) const
{
    for (size_t i = 0; i < size(); i++ )
    {
        other.mat[i] -= mat[i];
    }

	return other;
}

inline
Mat4		Mat4::operator*		(const Mat4& other) const
{
	Mat4 mResult {};

#ifdef LINE_CONVENTION
    for ( size_t i = 0; i < column() ; i++ )
	{
        for ( size_t j = 0; j < line(); j++ )
		{
			float result = 0.f;

			for ( size_t index = 0; index < 4; index++)
			{
				result += ((*this)[i][index] * other[index][j]);
			}
			mResult[i][j] = result;
		}
	}
#else
    for ( size_t i = 0; i < line() ; i++ )
	{
        for ( size_t j = 0; j < column(); j++ )
		{
			float result = 0.f;

			for ( size_t index = 0; index < 4; index++)
			{
				result += ((*this)[index][i] * other[j][index]);
			}
			mResult[j][i] = result;
		}
	}
#endif


	return mResult;
}

inline
Mat4		Mat4::operator*		(const float scale) const
{
	Mat4 mResult = (*this);

    for ( size_t i = 0; i < line() ; i++ )
	{
        for ( size_t j = 0; j < column(); j++ )
		{
			mResult[j][i] *= scale;
		}
	}
	return mResult;
}

inline
Vec4	Mat4::operator*		(const Vec4& other) const
{
	Vec4 vResult {};

    for ( size_t j = 0; j < line(); j++ )
	{
		for ( size_t i = 0; i < column() ; i++ )
		{
			vResult[j] += mat[j * column() + i] * other.e[i];
		}
	}

	return vResult;
}

inline
Vec4		Mat4::operator*		(const Vec3& other) const
{
	Vec4 vResult {};

    for ( size_t j = 0; j < line(); j++ )
	{
		for ( size_t i = 0; i < column() - 1; i++ )
		{
			vResult[j] += mat[j * column() + i] * other.e[i];
		}
		vResult[j] += mat[j * column() + 3]; //* 1 for w
	}

	return vResult;
}



inline
::std::ostream& 	operator<<		(::std::ostream& out, const Mat4& mat)
{
	::std::cout.precision(2);
    for ( size_t i = 0; i < mat.column() ; i++ )
	{
        for ( size_t j = 0; j < mat.line(); j++ )
        {
            out << mat[i][j] << "	";
        }
	    out << ::std::endl;
    }
	return out;
}
