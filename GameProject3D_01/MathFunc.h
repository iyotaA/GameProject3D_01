#ifndef MATH_FUNC_H_
#define MATH_FUNC_H_

float Lerp(float start, float end, float t);
Vector3 Lerp(Vector3 start, Vector3 end, float t);
XMFLOAT2 GetScreenPos(Vector3 World_Pos);
float RotateDegree_Normalize(float deg);
float RotateRadian_Normalize(float rad);


#endif // !MATH_FUNC_H_

