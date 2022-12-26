#pragma once

#include "Model.h"
#include "CGmath.h"
#include <vector>
#include <assert.h>
#include "Graphics.h"
#include "ResourceManager.h"

struct Vertex {
	Vector3f position;
	Vector2f uv;
};

class ModelLoader {
private:
	ResourceManager& mng;
	Graphcis& graphics;

public:
	ModelLoader(ResourceManager& mng, Graphcis& graphics)
		:graphics(graphics), mng(mng)
	{}

	void model_from_memory(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const unsigned int model_id);


};
