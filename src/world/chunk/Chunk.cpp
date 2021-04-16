#include "Chunk.h"
#include "../../global.h"

#include <chrono>

#include "../block/blocks.h"
#include "../../math/noise.h"

#define v0 1024
#define v1 1025
#define v2 1
#define v3 0
#define v4 1056
#define v5 1057
#define v6 33
#define v7 32

Chunk::Chunk(): Chunk(0,0,0, new unsigned char[CHUNK_SIZE3]) {}
Chunk::Chunk(int x, int y, int z): Chunk(x,y,z, new unsigned char[CHUNK_SIZE3]) {}
Chunk::Chunk(unsigned char* blocks): Chunk(0, 0, 0, blocks) {}

Chunk::Chunk(int x, int y, int z, unsigned char* blocks): mesh(), x(x), y(y), z(z), blocks(blocks) {}

Chunk::~Chunk() {
	delete[] blocks;
}

void Chunk::generate() {
	unsigned int i = 0;
	float threshold = 32.0f;

	for (float z = 0; z < CHUNK_SIZE; z++, i += CHUNK_SIZE2) {
		for (float y = 0; y < CHUNK_SIZE; y++, i += CHUNK_SIZE) {
			for (float x = 0; x < CHUNK_SIZE; x++, i++) {
//				setBlock(i, noise((x + this->x)/30.0f, (y + this->y)/30.0f, (z + this->z)/30.0f) > threshold ? STONE : AIR);
				setBlock(i, y + this->y > noise((x + this->x)/60.0f, (z + this->z)/60.0f) * 10.0f + 10.0f ? AIR : STONE);
			}
			i -= CHUNK_SIZE;
		}
		i -= CHUNK_SIZE2;
	}
}
void Chunk::regenMesh() {
	genMesh();
}
void Chunk::genMesh() {
	const auto time = std::chrono::system_clock::now();

    float* va = new float[CHUNK_SIZE3 * 15];

	unsigned int i = 0;
	unsigned int j = 0;

	for (unsigned char z = 0; z < CHUNK_SIZE; z++, i += CHUNK_SIZE2) {
		for (unsigned char y = 0; y < CHUNK_SIZE; y++, i += CHUNK_SIZE) {
			for (unsigned char x = 0; x < CHUNK_SIZE; x++, i++) {
				int cubeIndex = 0;
				if (!outOfBounds(i + v0) && getBlock(i + v0)) cubeIndex |= 1;
				if (!outOfBounds(i + v1) && getBlock(i + v1)) cubeIndex |= 2;
				if (!outOfBounds(i + v2) && getBlock(i + v2)) cubeIndex |= 4;
				if (!outOfBounds(i + v3) && getBlock(i + v3)) cubeIndex |= 8;
				if (!outOfBounds(i + v4) && getBlock(i + v4)) cubeIndex |= 16;
				if (!outOfBounds(i + v5) && getBlock(i + v5)) cubeIndex |= 32;
				if (!outOfBounds(i + v6) && getBlock(i + v6)) cubeIndex |= 64;
				if (!outOfBounds(i + v7) && getBlock(i + v7)) cubeIndex |= 128;

				/* Cube is entirely in/out of the surface */
				if (edgeTable[cubeIndex] == 0)
					continue;

				for (unsigned char k = 0; k < triTable[cubeIndex][0]; k += 3, j += 15) {
					va[j]   = this->x + x + vertlist[triTable[cubeIndex][k + 1]];
					va[j+1] = this->y + y + vertlist[triTable[cubeIndex][k + 1] + 1];
					va[j+2] = this->z + z + vertlist[triTable[cubeIndex][k + 1] + 2];
					va[j+3] = 0;
					va[j+4] = 0;

					va[j+5] = this->x + x + vertlist[triTable[cubeIndex][k + 2]];
					va[j+6] = this->y + y + vertlist[triTable[cubeIndex][k + 2] + 1];
					va[j+7] = this->z + z + vertlist[triTable[cubeIndex][k + 2] + 2];
					va[j+8] = 0;
					va[j+9] = 0;

					va[j+10] = this->x + x + vertlist[triTable[cubeIndex][k + 3]];
					va[j+11] = this->y + y + vertlist[triTable[cubeIndex][k + 3] + 1];
					va[j+12] = this->z + z + vertlist[triTable[cubeIndex][k + 3] + 2];
					va[j+13] = 0;
					va[j+14] = 0;
				}
			}
			i -= CHUNK_SIZE;
		}
		i -= CHUNK_SIZE2;
	}
	mesh.vertexArray.size = j*sizeof(float);
	mesh.vertexArray.length = j;
	mesh.vertexArray.data = va;

	printf("%.1fmb chunk mesh generated in %dms\n", j*sizeof(float) / 1'000'000.0f, std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - time));
}

void Chunk::setBlock(unsigned char x, unsigned char y, unsigned char z, unsigned char block) {
	blocks[getIndex(x, y, z)] = block;
}

void Chunk::setBlock(unsigned int index, unsigned char block) {
	blocks[index] = block;
}

unsigned char Chunk::getBlock(unsigned char x, unsigned char y, unsigned char z) {
	return blocks[getIndex(x, y, z)];
}

unsigned char Chunk::getBlock(unsigned int index) {
	return blocks[index];
}

unsigned int Chunk::getIndex(unsigned char x, unsigned char y, unsigned char z) {
	return (z * CHUNK_SIZE2) + (y * CHUNK_SIZE) + x;
}

unsigned int Chunk::up(unsigned int i) {
	return i + CHUNK_SIZE;
}
unsigned int Chunk::up(unsigned int i, unsigned char n) {
	return i + n * CHUNK_SIZE;
}

unsigned int Chunk::down(unsigned int i) {
	return i - CHUNK_SIZE;
}
unsigned int Chunk::down(unsigned int i, unsigned char n) {
	return i - n * CHUNK_SIZE;
}

unsigned int Chunk::east(unsigned int i) {
	return i + 1;
}
unsigned int Chunk::east(unsigned int i, unsigned char n) {
	return i + n;
}

unsigned int Chunk::west(unsigned int i) {
	return i - 1;
}
unsigned int Chunk::west(unsigned int i, unsigned char n) {
	return i - n;
}

unsigned int Chunk::north(unsigned int i) {
	return i + CHUNK_SIZE2;
}
unsigned int Chunk::north(unsigned int i, unsigned char n) {
	return i + n * CHUNK_SIZE2;
}

unsigned int Chunk::south(unsigned int i) {
	return i - CHUNK_SIZE2;
}
unsigned int Chunk::south(unsigned int i, unsigned char n) {
	return i - n * CHUNK_SIZE2;
}

bool Chunk::outOfBounds(unsigned int i) {
    return i > CHUNK_BOUND;
}

float Chunk::vertlist[36] = {
	0.5f, 0.0f, 1.0f, // 0
	1.0f, 0.0f, 0.5f, // 1
	0.5f, 0.0f, 0.0f, // 2
	0.0f, 0.0f, 0.5f, // 3

	0.5f, 1.0f, 1.0f, // 4
	1.0f, 1.0f, 0.5f, // 5
	0.5f, 1.0f, 0.0f, // 6
	0.0f, 1.0f, 0.5f, // 7

	0.0f, 0.5f, 1.0f, // 8
	1.0f, 0.5f, 1.0f, // 9
	1.0f, 0.5f, 0.0f, // 10
	0.0f, 0.5f, 0.0f  // 11
};

void Chunk::printy() {
	printf("#define v0 %i\n", getIndex(0, 0, 1));
	printf("#define v1 %i\n", getIndex(1, 0, 1));
	printf("#define v2 %i\n", getIndex(1, 0, 0));
	printf("#define v3 %i\n", getIndex(0, 0, 0));
	printf("#define v4 %i\n", getIndex(0, 1, 1));
	printf("#define v5 %i\n", getIndex(1, 1, 1));
	printf("#define v6 %i\n", getIndex(1, 1, 0));
	printf("#define v7 %i\n", getIndex(0, 1, 0));
}

unsigned short** Chunk::triTable = new unsigned short* [256]{
		new unsigned short[]{0},
		new unsigned short[]{3, 0, 24, 9},
		new unsigned short[]{3, 0, 3, 27},
		new unsigned short[]{6, 3, 24, 9, 27, 24, 3},
		new unsigned short[]{3, 3, 6, 30},
		new unsigned short[]{6, 0, 24, 9, 3, 6, 30},
		new unsigned short[]{6, 27, 6, 30, 0, 6, 27},
		new unsigned short[]{9, 6, 24, 9, 6, 30, 24, 30, 27, 24},
		new unsigned short[]{3, 9, 33, 6},
		new unsigned short[]{6, 0, 33, 6, 24, 33, 0},
		new unsigned short[]{6, 3, 27, 0, 6, 9, 33},
		new unsigned short[]{9, 3, 33, 6, 3, 27, 33, 27, 24, 33},
		new unsigned short[]{6, 9, 30, 3, 33, 30, 9},
		new unsigned short[]{9, 0, 30, 3, 0, 24, 30, 24, 33, 30},
		new unsigned short[]{9, 9, 27, 0, 9, 33, 27, 33, 30, 27},
		new unsigned short[]{6, 27, 24, 30, 30, 24, 33},
		new unsigned short[]{3, 12, 21, 24},
		new unsigned short[]{6, 12, 9, 0, 21, 9, 12},
		new unsigned short[]{6, 0, 3, 27, 24, 12, 21},
		new unsigned short[]{9, 12, 3, 27, 12, 21, 3, 21, 9, 3},
		new unsigned short[]{6, 3, 6, 30, 24, 12, 21},
		new unsigned short[]{9, 9, 12, 21, 9, 0, 12, 3, 6, 30},
		new unsigned short[]{9, 27, 6, 30, 27, 0, 6, 24, 12, 21},
		new unsigned short[]{12, 6, 30, 27, 6, 27, 21, 6, 21, 9, 21, 27, 12},
		new unsigned short[]{6, 24, 12, 21, 9, 33, 6},
		new unsigned short[]{9, 33, 12, 21, 33, 6, 12, 6, 0, 12},
		new unsigned short[]{9, 27, 0, 3, 24, 12, 21, 6, 9, 33},
		new unsigned short[]{12, 12, 21, 33, 27, 12, 33, 27, 33, 6, 27, 6, 3},
		new unsigned short[]{9, 9, 30, 3, 9, 33, 30, 21, 24, 12},
		new unsigned short[]{12, 3, 33, 30, 3, 12, 33, 3, 0, 12, 21, 33, 12},
		new unsigned short[]{12, 12, 21, 24, 27, 0, 33, 27, 33, 30, 33, 0, 9},
		new unsigned short[]{9, 12, 21, 33, 12, 33, 27, 27, 33, 30},
		new unsigned short[]{3, 27, 15, 12},
		new unsigned short[]{6, 27, 15, 12, 0, 24, 9},
		new unsigned short[]{6, 0, 15, 12, 3, 15, 0},
		new unsigned short[]{9, 24, 15, 12, 24, 9, 15, 9, 3, 15},
		new unsigned short[]{6, 3, 6, 30, 27, 15, 12},
		new unsigned short[]{9, 9, 0, 24, 3, 6, 30, 12, 27, 15},
		new unsigned short[]{9, 15, 6, 30, 15, 12, 6, 12, 0, 6},
		new unsigned short[]{12, 6, 30, 15, 9, 6, 15, 9, 15, 12, 9, 12, 24},
		new unsigned short[]{6, 27, 15, 12, 6, 9, 33},
		new unsigned short[]{9, 0, 33, 6, 0, 24, 33, 12, 27, 15},
		new unsigned short[]{9, 0, 15, 12, 0, 3, 15, 6, 9, 33},
		new unsigned short[]{12, 6, 3, 15, 6, 15, 24, 6, 24, 33, 12, 24, 15},
		new unsigned short[]{9, 30, 9, 33, 30, 3, 9, 27, 15, 12},
		new unsigned short[]{12, 12, 27, 15, 0, 24, 3, 24, 30, 3, 24, 33, 30},
		new unsigned short[]{12, 15, 12, 0, 15, 0, 33, 15, 33, 30, 33, 0, 9},
		new unsigned short[]{9, 15, 12, 24, 15, 24, 30, 30, 24, 33},
		new unsigned short[]{6, 27, 21, 24, 15, 21, 27},
		new unsigned short[]{9, 27, 9, 0, 27, 15, 9, 15, 21, 9},
		new unsigned short[]{9, 0, 21, 24, 0, 3, 21, 3, 15, 21},
		new unsigned short[]{6, 3, 15, 9, 9, 15, 21},
		new unsigned short[]{9, 27, 21, 24, 27, 15, 21, 30, 3, 6},
		new unsigned short[]{12, 30, 3, 6, 27, 15, 0, 15, 9, 0, 15, 21, 9},
		new unsigned short[]{12, 24, 0, 6, 24, 6, 15, 24, 15, 21, 30, 15, 6},
		new unsigned short[]{9, 6, 30, 15, 6, 15, 9, 9, 15, 21},
		new unsigned short[]{9, 21, 27, 15, 21, 24, 27, 9, 33, 6},
		new unsigned short[]{12, 27, 15, 21, 27, 21, 6, 27, 6, 0, 6, 21, 33},
		new unsigned short[]{12, 6, 9, 33, 0, 3, 24, 3, 21, 24, 3, 15, 21},
		new unsigned short[]{9, 33, 6, 3, 33, 3, 21, 21, 3, 15},
		new unsigned short[]{12, 27, 15, 24, 24, 15, 21, 30, 3, 9, 30, 9, 33},
		new unsigned short[]{15, 15, 21, 0, 15, 0, 27, 21, 33, 0, 3, 0, 30, 33, 30, 0},
		new unsigned short[]{15, 33, 30, 0, 33, 0, 9, 30, 15, 0, 24, 0, 21, 15, 21, 0},
		new unsigned short[]{6, 33, 30, 15, 21, 33, 15},
		new unsigned short[]{3, 30, 18, 15},
		new unsigned short[]{6, 0, 24, 9, 15, 30, 18},
		new unsigned short[]{6, 27, 0, 3, 15, 30, 18},
		new unsigned short[]{9, 3, 24, 9, 3, 27, 24, 15, 30, 18},
		new unsigned short[]{6, 3, 18, 15, 6, 18, 3},
		new unsigned short[]{9, 3, 18, 15, 3, 6, 18, 9, 0, 24},
		new unsigned short[]{9, 27, 18, 15, 27, 0, 18, 0, 6, 18},
		new unsigned short[]{12, 15, 27, 24, 15, 24, 6, 15, 6, 18, 9, 6, 24},
		new unsigned short[]{6, 6, 9, 33, 30, 18, 15},
		new unsigned short[]{9, 33, 0, 24, 33, 6, 0, 30, 18, 15},
		new unsigned short[]{9, 0, 3, 27, 6, 9, 33, 15, 30, 18},
		new unsigned short[]{12, 15, 30, 18, 3, 27, 6, 27, 33, 6, 27, 24, 33},
		new unsigned short[]{9, 18, 9, 33, 18, 15, 9, 15, 3, 9},
		new unsigned short[]{12, 0, 24, 33, 0, 33, 15, 0, 15, 3, 15, 33, 18},
		new unsigned short[]{12, 9, 33, 18, 0, 9, 18, 0, 18, 15, 0, 15, 27},
		new unsigned short[]{9, 18, 15, 27, 18, 27, 33, 33, 27, 24},
		new unsigned short[]{6, 15, 30, 18, 12, 21, 24},
		new unsigned short[]{9, 12, 9, 0, 12, 21, 9, 18, 15, 30},
		new unsigned short[]{9, 3, 27, 0, 15, 30, 18, 24, 12, 21},
		new unsigned short[]{12, 30, 18, 15, 3, 27, 21, 3, 21, 9, 21, 27, 12},
		new unsigned short[]{9, 18, 3, 6, 18, 15, 3, 12, 21, 24},
		new unsigned short[]{12, 3, 6, 15, 15, 6, 18, 9, 0, 12, 9, 12, 21},
		new unsigned short[]{12, 24, 12, 21, 27, 0, 15, 0, 18, 15, 0, 6, 18},
		new unsigned short[]{15, 21, 9, 27, 21, 27, 12, 9, 6, 27, 15, 27, 18, 6, 18, 27},
		new unsigned short[]{9, 9, 33, 6, 21, 24, 12, 30, 18, 15},
		new unsigned short[]{12, 15, 30, 18, 12, 21, 6, 12, 6, 0, 6, 21, 33},
		new unsigned short[]{12, 0, 3, 27, 12, 21, 24, 6, 9, 33, 15, 30, 18},
		new unsigned short[]{15, 27, 6, 3, 27, 33, 6, 27, 12, 33, 21, 33, 12, 15, 30, 18},
		new unsigned short[]{12, 24, 12, 21, 9, 33, 15, 9, 15, 3, 15, 33, 18},
		new unsigned short[]{15, 15, 3, 33, 15, 33, 18, 3, 0, 33, 21, 33, 12, 0, 12, 33},
		new unsigned short[]{15, 0, 15, 27, 0, 18, 15, 0, 9, 18, 33, 18, 9, 24, 12, 21},
		new unsigned short[]{12, 18, 15, 27, 18, 27, 33, 12, 21, 27, 21, 33, 27},
		new unsigned short[]{6, 30, 12, 27, 18, 12, 30},
		new unsigned short[]{9, 12, 30, 18, 12, 27, 30, 0, 24, 9},
		new unsigned short[]{9, 30, 0, 3, 30, 18, 0, 18, 12, 0},
		new unsigned short[]{12, 24, 9, 3, 24, 3, 18, 24, 18, 12, 18, 3, 30},
		new unsigned short[]{9, 3, 12, 27, 3, 6, 12, 6, 18, 12},
		new unsigned short[]{12, 9, 0, 24, 3, 6, 27, 6, 12, 27, 6, 18, 12},
		new unsigned short[]{6, 0, 6, 12, 12, 6, 18},
		new unsigned short[]{9, 24, 9, 6, 24, 6, 12, 12, 6, 18},
		new unsigned short[]{9, 30, 12, 27, 30, 18, 12, 33, 6, 9},
		new unsigned short[]{12, 0, 24, 6, 6, 24, 33, 12, 27, 30, 12, 30, 18},
		new unsigned short[]{12, 9, 33, 6, 0, 3, 18, 0, 18, 12, 18, 3, 30},
		new unsigned short[]{15, 18, 12, 3, 18, 3, 30, 12, 24, 3, 6, 3, 33, 24, 33, 3},
		new unsigned short[]{12, 27, 18, 12, 27, 9, 18, 27, 3, 9, 33, 18, 9},
		new unsigned short[]{15, 24, 33, 3, 24, 3, 0, 33, 18, 3, 27, 3, 12, 18, 12, 3},
		new unsigned short[]{9, 9, 33, 18, 9, 18, 0, 0, 18, 12},
		new unsigned short[]{6, 18, 12, 24, 33, 18, 24},
		new unsigned short[]{9, 21, 30, 18, 21, 24, 30, 24, 27, 30},
		new unsigned short[]{12, 0, 21, 9, 0, 30, 21, 0, 27, 30, 18, 21, 30},
		new unsigned short[]{12, 30, 18, 21, 3, 30, 21, 3, 21, 24, 3, 24, 0},
		new unsigned short[]{9, 30, 18, 21, 30, 21, 3, 3, 21, 9},
		new unsigned short[]{12, 3, 6, 18, 3, 18, 24, 3, 24, 27, 24, 18, 21},
		new unsigned short[]{15, 6, 18, 27, 6, 27, 3, 18, 21, 27, 0, 27, 9, 21, 9, 27},
		new unsigned short[]{9, 21, 24, 0, 21, 0, 18, 18, 0, 6},
		new unsigned short[]{6, 21, 9, 6, 18, 21, 6},
		new unsigned short[]{12, 6, 9, 33, 30, 18, 24, 30, 24, 27, 24, 18, 21},
		new unsigned short[]{15, 6, 0, 21, 6, 21, 33, 0, 27, 21, 18, 21, 30, 27, 30, 21},
		new unsigned short[]{15, 3, 24, 0, 3, 21, 24, 3, 30, 21, 18, 21, 30, 6, 9, 33},
		new unsigned short[]{12, 33, 6, 3, 33, 3, 21, 30, 18, 3, 18, 21, 3},
		new unsigned short[]{15, 24, 27, 18, 24, 18, 21, 27, 3, 18, 33, 18, 9, 3, 9, 18},
		new unsigned short[]{6, 0, 27, 3, 33, 18, 21},
		new unsigned short[]{12, 21, 24, 0, 21, 0, 18, 9, 33, 0, 33, 18, 0},
		new unsigned short[]{3, 21, 33, 18},
		new unsigned short[]{3, 21, 18, 33},
		new unsigned short[]{6, 9, 0, 24, 33, 21, 18},
		new unsigned short[]{6, 0, 3, 27, 33, 21, 18},
		new unsigned short[]{9, 24, 3, 27, 24, 9, 3, 33, 21, 18},
		new unsigned short[]{6, 30, 3, 6, 18, 33, 21},
		new unsigned short[]{9, 3, 6, 30, 9, 0, 24, 18, 33, 21},
		new unsigned short[]{9, 6, 27, 0, 6, 30, 27, 18, 33, 21},
		new unsigned short[]{12, 18, 33, 21, 6, 30, 9, 30, 24, 9, 30, 27, 24},
		new unsigned short[]{6, 21, 6, 9, 18, 6, 21},
		new unsigned short[]{9, 21, 0, 24, 21, 18, 0, 18, 6, 0},
		new unsigned short[]{9, 6, 21, 18, 6, 9, 21, 0, 3, 27},
		new unsigned short[]{12, 3, 18, 6, 3, 24, 18, 3, 27, 24, 24, 21, 18},
		new unsigned short[]{9, 30, 21, 18, 30, 3, 21, 3, 9, 21},
		new unsigned short[]{12, 30, 21, 18, 3, 21, 30, 3, 24, 21, 3, 0, 24},
		new unsigned short[]{12, 0, 9, 21, 0, 21, 30, 0, 30, 27, 18, 30, 21},
		new unsigned short[]{9, 21, 18, 30, 21, 30, 24, 24, 30, 27},
		new unsigned short[]{6, 18, 24, 12, 33, 24, 18},
		new unsigned short[]{9, 9, 18, 33, 9, 0, 18, 0, 12, 18},
		new unsigned short[]{9, 24, 18, 33, 24, 12, 18, 27, 0, 3},
		new unsigned short[]{12, 27, 12, 18, 27, 18, 9, 27, 9, 3, 33, 9, 18},
		new unsigned short[]{9, 18, 24, 12, 18, 33, 24, 6, 30, 3},
		new unsigned short[]{12, 3, 6, 30, 9, 0, 33, 0, 18, 33, 0, 12, 18},
		new unsigned short[]{12, 12, 33, 24, 12, 18, 33, 0, 6, 27, 6, 30, 27},
		new unsigned short[]{15, 30, 27, 9, 30, 9, 6, 27, 12, 9, 33, 9, 18, 12, 18, 9},
		new unsigned short[]{9, 24, 6, 9, 24, 12, 6, 12, 18, 6},
		new unsigned short[]{6, 0, 12, 6, 12, 18, 6},
		new unsigned short[]{12, 3, 27, 0, 6, 9, 12, 6, 12, 18, 12, 9, 24},
		new unsigned short[]{9, 3, 27, 12, 3, 12, 6, 6, 12, 18},
		new unsigned short[]{12, 24, 3, 9, 24, 18, 3, 24, 12, 18, 18, 30, 3},
		new unsigned short[]{9, 30, 3, 0, 30, 0, 18, 18, 0, 12},
		new unsigned short[]{15, 12, 18, 9, 12, 9, 24, 18, 30, 9, 0, 9, 27, 30, 27, 9},
		new unsigned short[]{6, 30, 27, 12, 18, 30, 12},
		new unsigned short[]{6, 12, 27, 15, 21, 18, 33},
		new unsigned short[]{9, 0, 24, 9, 12, 27, 15, 33, 21, 18},
		new unsigned short[]{9, 15, 0, 3, 15, 12, 0, 21, 18, 33},
		new unsigned short[]{12, 33, 21, 18, 24, 9, 12, 9, 15, 12, 9, 3, 15},
		new unsigned short[]{9, 27, 15, 12, 30, 3, 6, 21, 18, 33},
		new unsigned short[]{12, 18, 33, 21, 3, 6, 30, 0, 24, 9, 12, 27, 15},
		new unsigned short[]{12, 21, 18, 33, 15, 12, 30, 12, 6, 30, 12, 0, 6},
		new unsigned short[]{15, 9, 12, 24, 9, 15, 12, 9, 6, 15, 30, 15, 6, 33, 21, 18},
		new unsigned short[]{9, 21, 6, 9, 21, 18, 6, 15, 12, 27},
		new unsigned short[]{12, 27, 15, 12, 0, 24, 18, 0, 18, 6, 18, 24, 21},
		new unsigned short[]{12, 9, 18, 6, 9, 21, 18, 3, 15, 0, 15, 12, 0},
		new unsigned short[]{15, 18, 6, 24, 18, 24, 21, 6, 3, 24, 12, 24, 15, 3, 15, 24},
		new unsigned short[]{12, 27, 15, 12, 30, 3, 18, 3, 21, 18, 3, 9, 21},
		new unsigned short[]{15, 3, 18, 30, 3, 21, 18, 3, 0, 21, 24, 21, 0, 27, 15, 12},
		new unsigned short[]{15, 12, 0, 30, 12, 30, 15, 0, 9, 30, 18, 30, 21, 9, 21, 30},
		new unsigned short[]{12, 21, 18, 30, 21, 30, 24, 15, 12, 30, 12, 24, 30},
		new unsigned short[]{9, 18, 27, 15, 18, 33, 27, 33, 24, 27},
		new unsigned short[]{12, 9, 18, 33, 0, 18, 9, 0, 15, 18, 0, 27, 15},
		new unsigned short[]{12, 0, 33, 24, 0, 15, 33, 0, 3, 15, 15, 18, 33},
		new unsigned short[]{9, 18, 33, 9, 18, 9, 15, 15, 9, 3},
		new unsigned short[]{12, 3, 6, 30, 27, 15, 33, 27, 33, 24, 33, 15, 18},
		new unsigned short[]{15, 0, 33, 9, 0, 18, 33, 0, 27, 18, 15, 18, 27, 3, 6, 30},
		new unsigned short[]{15, 33, 24, 15, 33, 15, 18, 24, 0, 15, 30, 15, 6, 0, 6, 15},
		new unsigned short[]{12, 18, 33, 9, 18, 9, 15, 6, 30, 9, 30, 15, 9},
		new unsigned short[]{12, 15, 24, 27, 15, 6, 24, 15, 18, 6, 9, 24, 6},
		new unsigned short[]{9, 27, 15, 18, 27, 18, 0, 0, 18, 6},
		new unsigned short[]{15, 3, 15, 24, 3, 24, 0, 15, 18, 24, 9, 24, 6, 18, 6, 24},
		new unsigned short[]{6, 3, 15, 18, 6, 3, 18},
		new unsigned short[]{15, 3, 9, 18, 3, 18, 30, 9, 24, 18, 15, 18, 27, 24, 27, 18},
		new unsigned short[]{12, 30, 3, 0, 30, 0, 18, 27, 15, 0, 15, 18, 0},
		new unsigned short[]{6, 0, 9, 24, 15, 18, 30},
		new unsigned short[]{3, 30, 15, 18},
		new unsigned short[]{6, 33, 15, 30, 21, 15, 33},
		new unsigned short[]{9, 33, 15, 30, 33, 21, 15, 24, 9, 0},
		new unsigned short[]{9, 15, 33, 21, 15, 30, 33, 3, 27, 0},
		new unsigned short[]{12, 30, 21, 15, 30, 33, 21, 27, 24, 3, 24, 9, 3},
		new unsigned short[]{9, 33, 3, 6, 33, 21, 3, 21, 15, 3},
		new unsigned short[]{12, 0, 24, 9, 3, 6, 21, 3, 21, 15, 21, 6, 33},
		new unsigned short[]{12, 27, 21, 15, 27, 6, 21, 27, 0, 6, 6, 33, 21},
		new unsigned short[]{15, 21, 15, 6, 21, 6, 33, 15, 27, 6, 9, 6, 24, 27, 24, 6},
		new unsigned short[]{9, 6, 15, 30, 6, 9, 15, 9, 21, 15},
		new unsigned short[]{12, 24, 6, 0, 24, 15, 6, 24, 21, 15, 30, 6, 15},
		new unsigned short[]{12, 27, 0, 3, 15, 30, 9, 15, 9, 21, 9, 30, 6},
		new unsigned short[]{15, 27, 24, 6, 27, 6, 3, 24, 21, 6, 30, 6, 15, 21, 15, 6},
		new unsigned short[]{6, 3, 9, 15, 9, 21, 15},
		new unsigned short[]{9, 0, 24, 21, 0, 21, 3, 3, 21, 15},
		new unsigned short[]{9, 27, 0, 9, 27, 9, 15, 15, 9, 21},
		new unsigned short[]{6, 27, 24, 21, 15, 27, 21},
		new unsigned short[]{9, 15, 24, 12, 15, 30, 24, 30, 33, 24},
		new unsigned short[]{12, 15, 0, 12, 15, 33, 0, 15, 30, 33, 33, 9, 0},
		new unsigned short[]{12, 0, 3, 27, 24, 12, 30, 24, 30, 33, 30, 12, 15},
		new unsigned short[]{15, 30, 33, 12, 30, 12, 15, 33, 9, 12, 27, 12, 3, 9, 3, 12},
		new unsigned short[]{12, 6, 15, 3, 6, 24, 15, 6, 33, 24, 12, 15, 24},
		new unsigned short[]{15, 0, 12, 33, 0, 33, 9, 12, 15, 33, 6, 33, 3, 15, 3, 33},
		new unsigned short[]{15, 0, 6, 15, 0, 15, 27, 6, 33, 15, 12, 15, 24, 33, 24, 15},
		new unsigned short[]{6, 27, 12, 15, 6, 33, 9},
		new unsigned short[]{12, 6, 15, 30, 9, 15, 6, 9, 12, 15, 9, 24, 12},
		new unsigned short[]{9, 15, 30, 6, 15, 6, 12, 12, 6, 0},
		new unsigned short[]{15, 9, 30, 6, 9, 15, 30, 9, 24, 15, 12, 15, 24, 0, 3, 27},
		new unsigned short[]{12, 15, 30, 6, 15, 6, 12, 3, 27, 6, 27, 12, 6},
		new unsigned short[]{9, 24, 12, 15, 24, 15, 9, 9, 15, 3},
		new unsigned short[]{6, 0, 12, 15, 3, 0, 15},
		new unsigned short[]{12, 24, 12, 15, 24, 15, 9, 27, 0, 15, 0, 9, 15},
		new unsigned short[]{3, 27, 12, 15},
		new unsigned short[]{9, 12, 33, 21, 12, 27, 33, 27, 30, 33},
		new unsigned short[]{12, 0, 24, 9, 12, 27, 21, 27, 33, 21, 27, 30, 33},
		new unsigned short[]{12, 3, 30, 33, 3, 33, 12, 3, 12, 0, 21, 12, 33},
		new unsigned short[]{15, 9, 3, 12, 9, 12, 24, 3, 30, 12, 21, 12, 33, 30, 33, 12},
		new unsigned short[]{12, 12, 33, 21, 27, 33, 12, 27, 6, 33, 27, 3, 6},
		new unsigned short[]{15, 27, 21, 12, 27, 33, 21, 27, 3, 33, 6, 33, 3, 0, 24, 9},
		new unsigned short[]{9, 33, 21, 12, 33, 12, 6, 6, 12, 0},
		new unsigned short[]{12, 33, 21, 12, 33, 12, 6, 24, 9, 12, 9, 6, 12},
		new unsigned short[]{12, 6, 27, 30, 6, 21, 27, 6, 9, 21, 21, 12, 27},
		new unsigned short[]{15, 27, 30, 21, 27, 21, 12, 30, 6, 21, 24, 21, 0, 6, 0, 21},
		new unsigned short[]{15, 9, 21, 30, 9, 30, 6, 21, 12, 30, 3, 30, 0, 12, 0, 30},
		new unsigned short[]{6, 3, 30, 6, 24, 21, 12},
		new unsigned short[]{9, 12, 27, 3, 12, 3, 21, 21, 3, 9},
		new unsigned short[]{12, 12, 27, 3, 12, 3, 21, 0, 24, 3, 24, 21, 3},
		new unsigned short[]{6, 12, 0, 9, 21, 12, 9},
		new unsigned short[]{3, 12, 24, 21},
		new unsigned short[]{6, 27, 30, 24, 30, 33, 24},
		new unsigned short[]{9, 9, 0, 27, 9, 27, 33, 33, 27, 30},
		new unsigned short[]{9, 0, 3, 30, 0, 30, 24, 24, 30, 33},
		new unsigned short[]{6, 9, 3, 30, 33, 9, 30},
		new unsigned short[]{9, 3, 6, 33, 3, 33, 27, 27, 33, 24},
		new unsigned short[]{12, 9, 0, 27, 9, 27, 33, 3, 6, 27, 6, 33, 27},
		new unsigned short[]{6, 0, 6, 33, 24, 0, 33},
		new unsigned short[]{3, 9, 6, 33},
		new unsigned short[]{9, 6, 9, 24, 6, 24, 30, 30, 24, 27},
		new unsigned short[]{6, 27, 30, 6, 0, 27, 6},
		new unsigned short[]{12, 6, 9, 24, 6, 24, 30, 0, 3, 24, 3, 30, 24},
		new unsigned short[]{3, 3, 30, 6},
		new unsigned short[]{6, 3, 9, 24, 27, 3, 24},
		new unsigned short[]{3, 0, 27, 3},
		new unsigned short[]{3, 0, 9, 24},
		new unsigned short[]{0}
};
int* Chunk::edgeTable = new int[]{0x0, 0x109, 0x203, 0x30a, 0x406, 0x50f, 0x605, 0x70c,
                                                    0x80c, 0x905, 0xa0f, 0xb06, 0xc0a, 0xd03, 0xe09, 0xf00,
                                                    0x190, 0x99, 0x393, 0x29a, 0x596, 0x49f, 0x795, 0x69c,
                                                    0x99c, 0x895, 0xb9f, 0xa96, 0xd9a, 0xc93, 0xf99, 0xe90,
                                                    0x230, 0x339, 0x33, 0x13a, 0x636, 0x73f, 0x435, 0x53c,
                                                    0xa3c, 0xb35, 0x83f, 0x936, 0xe3a, 0xf33, 0xc39, 0xd30,
                                                    0x3a0, 0x2a9, 0x1a3, 0xaa, 0x7a6, 0x6af, 0x5a5, 0x4ac,
                                                    0xbac, 0xaa5, 0x9af, 0x8a6, 0xfaa, 0xea3, 0xda9, 0xca0,
                                                    0x460, 0x569, 0x663, 0x76a, 0x66, 0x16f, 0x265, 0x36c,
                                                    0xc6c, 0xd65, 0xe6f, 0xf66, 0x86a, 0x963, 0xa69, 0xb60,
                                                    0x5f0, 0x4f9, 0x7f3, 0x6fa, 0x1f6, 0xff, 0x3f5, 0x2fc,
                                                    0xdfc, 0xcf5, 0xfff, 0xef6, 0x9fa, 0x8f3, 0xbf9, 0xaf0,
                                                    0x650, 0x759, 0x453, 0x55a, 0x256, 0x35f, 0x55, 0x15c,
                                                    0xe5c, 0xf55, 0xc5f, 0xd56, 0xa5a, 0xb53, 0x859, 0x950,
                                                    0x7c0, 0x6c9, 0x5c3, 0x4ca, 0x3c6, 0x2cf, 0x1c5, 0xcc,
                                                    0xfcc, 0xec5, 0xdcf, 0xcc6, 0xbca, 0xac3, 0x9c9, 0x8c0,
                                                    0x8c0, 0x9c9, 0xac3, 0xbca, 0xcc6, 0xdcf, 0xec5, 0xfcc,
                                                    0xcc, 0x1c5, 0x2cf, 0x3c6, 0x4ca, 0x5c3, 0x6c9, 0x7c0,
                                                    0x950, 0x859, 0xb53, 0xa5a, 0xd56, 0xc5f, 0xf55, 0xe5c,
                                                    0x15c, 0x55, 0x35f, 0x256, 0x55a, 0x453, 0x759, 0x650,
                                                    0xaf0, 0xbf9, 0x8f3, 0x9fa, 0xef6, 0xfff, 0xcf5, 0xdfc,
                                                    0x2fc, 0x3f5, 0xff, 0x1f6, 0x6fa, 0x7f3, 0x4f9, 0x5f0,
                                                    0xb60, 0xa69, 0x963, 0x86a, 0xf66, 0xe6f, 0xd65, 0xc6c,
                                                    0x36c, 0x265, 0x16f, 0x66, 0x76a, 0x663, 0x569, 0x460,
                                                    0xca0, 0xda9, 0xea3, 0xfaa, 0x8a6, 0x9af, 0xaa5, 0xbac,
                                                    0x4ac, 0x5a5, 0x6af, 0x7a6, 0xaa, 0x1a3, 0x2a9, 0x3a0,
                                                    0xd30, 0xc39, 0xf33, 0xe3a, 0x936, 0x83f, 0xb35, 0xa3c,
                                                    0x53c, 0x435, 0x73f, 0x636, 0x13a, 0x33, 0x339, 0x230,
                                                    0xe90, 0xf99, 0xc93, 0xd9a, 0xa96, 0xb9f, 0x895, 0x99c,
                                                    0x69c, 0x795, 0x49f, 0x596, 0x29a, 0x393, 0x99, 0x190,
                                                    0xf00, 0xe09, 0xd03, 0xc0a, 0xb06, 0xa0f, 0x905, 0x80c,
                                                    0x70c, 0x605, 0x50f, 0x406, 0x30a, 0x203, 0x109, 0x0
};