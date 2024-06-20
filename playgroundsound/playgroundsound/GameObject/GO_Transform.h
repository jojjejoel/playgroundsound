#pragma once
#include "GO_Vector3.h"

struct GO_Matrix {
	float m0, m4, m8, m12;  // Matrix first row (4 components)
	float m1, m5, m9, m13;  // Matrix second row (4 components)
	float m2, m6, m10, m14; // Matrix third row (4 components)
	float m3, m7, m11, m15; // Matrix fourth row (4 components)
};

struct GO_Transform {
	GO_Vector3 position{ 0,0,0 };
	GO_Vector3 right{ 1,0,0 };
	GO_Vector3 up{ 0,1,0 };
	GO_Vector3 forward{ 0,0,1 };
	GO_Vector3 scale{ 1,1,1 };
	GO_Matrix matrix;
};
