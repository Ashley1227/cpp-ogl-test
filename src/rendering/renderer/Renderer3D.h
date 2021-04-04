#pragma once
#include "../includes.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Renderer.h"

class Renderer3D : public Renderer {
protected:
	glm::mat4 model, view, projection;
public:
	Renderer3D();
	Renderer3D(glm::mat4& m, glm::mat4& v, glm::mat4& p);

	inline glm::mat4 getMVP() const {
		return projection*view*model;
	}
	inline glm::mat4& getModel() {
		return model;
	}
	inline glm::mat4& getView() {
		return view;
	}
	inline glm::mat4& getProjection() {
		return projection;
	}
	inline void setModel(glm::mat4& m) {
		model = m;
	}
	inline void setView(glm::mat4& v) {
		view = v;
	}
	inline void setProjection(glm::mat4& p) {
		projection = p;
	}
};