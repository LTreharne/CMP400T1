#pragma once
#include "PlaneMesh.h"
#include <vector>
#include "DirectXMath.h"
#include "Lithosphere.h"
using std::vector;
struct Vector2f
{
	float x;
	float y;
};

struct Line
{
	Vector2f start;
	Vector2f end;
};
class TerrainMesh :
	public PlaneMesh {
public:
	TerrainMesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int resolution = 512);
	~TerrainMesh();

	void Resize(int newResolution);
	void Regenerate(ID3D11Device* device, ID3D11DeviceContext* deviceContext, vector<vector<float>> h);
	void smooth();
	void fault();
	void Lower();
	void Average();
	Lithosphere lith;

	Vector2f CrossProduct(Line a, Line b);
	float length(Line);
	const inline int GetResolution() { return resolution; }
private:
	void CreateBuffers(ID3D11Device* device, VertexType* vertices, unsigned long* indices);
	void BuildHeightMap(vector<vector<float>> h);

	const float m_UVscale = 10.0f;			//Tile the UV map 10 times across the plane
	const float terrainSize = 100.0f;		//What is the width and height of our terrain
	vector<vector<float>> heightMap;
};
