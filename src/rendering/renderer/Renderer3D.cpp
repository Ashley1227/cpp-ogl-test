#include "Renderer3D.h"

Renderer3D::Renderer3D(): model(glm::mat4(1.0f)), view(glm::mat4(1.0f)), projection(glm::mat4(1.0f)) {}
Renderer3D::Renderer3D(glm::mat4& m, glm::mat4& v, glm::mat4& p): model(m), view(v), projection(p) {}