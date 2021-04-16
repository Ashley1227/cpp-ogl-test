#pragma once

#include <vector>
#include "chunk/Chunk.h"
#include "../rendering/camera/Camera.h"

class World {
public:
	std::vector<Chunk*> chunks;
	Camera* camera;

	World(Camera* camera);

	void switchCamera(Camera* camera);

	void loadChunk(Chunk* chunk);
	void loadChunks(unsigned char count, Chunk** chunks);

	static Chunk* generateChunk(int x,int y,int z);
	static Chunk** generateChunks(int x,int y,int z, int x2,int y2,int z2);
};