#pragma once

#include "../../rendering/Mesh.h"
#include "../../rendering/VertexArray.h"

#define CHUNK_SIZE 32
#define CHUNK_SIZE2 1024
#define CHUNK_SIZE3 32768

#define CHUNK_BOUND 32767

class Chunk {
protected:
	static int* edgeTable;
	static unsigned short** triTable;
	static float vertlist[36];
public:
	int x,y,z;

	unsigned char* blocks;
	Mesh mesh;

	Chunk();
	Chunk(int x, int y, int z);
	Chunk(unsigned char* blocks);
	Chunk(int x, int y, int z, unsigned char* blocks);
	~Chunk();

	void generate();

	void genVertexArray();
	void genIndexArray();
	void genMesh();
	void regenMesh();

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

	static bool outOfBounds(unsigned int i);
};

