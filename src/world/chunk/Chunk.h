#pragma once

#include "../../rendering/VertexArray.h"
#include "../../rendering/Mesh.h"

#define CHUNK_SIZE 64
#define CHUNK_SIZE2 4096
#define CHUNK_SIZE3 262144

#define v0 4096
#define v1 4097
#define v2 1
#define v3 0
#define v4 4160
#define v5 4161
#define v6 65
#define v7 64

class Chunk {
protected:
	static int* edgeTable;
	static unsigned short** triTable;
	static float vertlist[36];
public:
	unsigned char* blocks;
	Mesh mesh;

	Chunk();
	Chunk(unsigned char* thing);
	~Chunk();

	void generate();

	void genVertexArray();
	void genIndexArray();
	void genMesh();

	unsigned char getBlock(unsigned char x, unsigned char y, unsigned char z);
	unsigned char getBlock(unsigned int index);
	void setBlock(unsigned char x, unsigned char y, unsigned char z, unsigned char block);
	void setBlock(unsigned int index, unsigned char block);


	void printy();

	static unsigned int up(unsigned int i, unsigned char n);
	static unsigned int up(unsigned int i);
	static unsigned int down(unsigned int i);
	static unsigned int down(unsigned int i, unsigned char n);

	static unsigned int east(unsigned int i);
	static unsigned int east(unsigned int i, unsigned char n);
	static unsigned int west(unsigned int i);
	static unsigned int west(unsigned int i, unsigned char n);

	static unsigned int north(unsigned int i);
	static unsigned int north(unsigned int i, unsigned char n);
	static unsigned int south(unsigned int i);
	static unsigned int south(unsigned int i, unsigned char n);

	static unsigned int getIndex(unsigned char x, unsigned char y, unsigned char z);
};

