#pragma once
#include <vector>
#include "D3D.h"
#include "Light.h"


class Mesh;

using namespace std;

class Material
{
public:
	Material(void) ;
	Material(D3DXCOLOR &color);
	~Material(void);

	D3DXCOLOR color;
	virtual D3DXCOLOR shade(vector<Light*>* lightList, vector<Mesh*>* objectList, Mesh* object) = 0;



};
