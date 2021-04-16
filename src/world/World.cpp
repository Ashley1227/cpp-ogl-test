#include "World.h"

World::World(Camera* camera): camera(camera) {}

void World::loadChunk(Chunk* chunk) {
	chunks.push_back(chunk);
}
void World::loadChunks(unsigned char count, Chunk** chunks) {
	this->chunks.reserve(count);
	for(unsigned char i = 0; i < count; i++)
		this->chunks.push_back(chunks[i]);
}

Chunk* World::generateChunk(int x, int y, int z) {
	return new Chunk(x,y,z);
}
Chunk** World::generateChunks(int x,int y,int z, int x2,int y2,int z2) {
	Chunk* ret[(x2-x) * (y2-y) * (z2-z)];
	unsigned int i;
	for(int x3 = x; x3 < x2; x3 += CHUNK_SIZE) {
		for(int y3 = y; y3 < y2; y3 += CHUNK_SIZE) {
			for(int z3 = z; z3 < z2; z3 += CHUNK_SIZE, i++) {
				ret[i] = generateChunk(x,y,z);
			}
		}
	}
	return ret;
}