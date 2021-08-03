#include <stdlib.h>
#include <stdio.h>
#include <bitStream.h>
#define INPUT 	1
#define OUTPUT 	0
#define BYTE	8

//Uses outputBits, use only for outputting encoded information
BitStream *openInputBitStream (int (*readFunc)(void *context), void *context) {
	BitStream *bs = malloc(sizeof(BitStream));
	bs->writeFunc = NULL;
	bs->readFunc = readFunc;
	bs->direction = INPUT;
	bs->context = context;
	bs->extraBits = bs->extraCount = 0;
	
	return bs;
}

//Uses readInBits, use only for decoding bits
BitStream *openOutputBitStream (void (*writeFunc)(unsigned char c,void *context),
															 void *context) {
	BitStream *bs = malloc(sizeof(BitStream));
	bs->writeFunc = writeFunc;
	bs->readFunc = NULL;
	bs->direction = OUTPUT;
	bs->context = context;
	bs->extraBits = bs->extraCount =  0;
	
	return bs;
}

void closeAndDeleteBitStream (BitStream *bs) {
	if (bs->direction == OUTPUT)
		flushOutBits(bs);
	free(bs);
}

unsigned long long setHashSize (unsigned int maxBits) {
	unsigned long long hashSize = 1; //set most significant bit
	hashSize <<= maxBits;
	return hashSize;
}

//shift and flush the rest of the bits 
void flushOutBits (BitStream *bs) {
	if (bs->extraCount != 0) {
		bs->extraBits <<=  (BYTE - bs->extraCount);
		bs->writeFunc((unsigned char)bs->extraBits, bs->context);
	}
}

//output nBits to writeFunc
void outputBits (BitStream *bs, unsigned int nBits, unsigned int code) {
	unsigned int c1;
	bs->extraBits <<= nBits;
	bs->extraBits = bs->extraBits | code;
	bs->extraCount += nBits;
	while (bs->extraCount >= BYTE) {
		c1 = bs->extraBits;
		c1 >>= (bs->extraCount - BYTE);
		bs->writeFunc((unsigned char) c1, bs->context);
		bs->extraCount -= BYTE;
	}
}

//read nBits from readFunc, store in buffer and shift to get right bit length
bool readInBits (BitStream *bs, unsigned int nBits, unsigned int *code) {
	unsigned int c1;
	int c;
	while (bs->extraCount < nBits) {
	    c = bs->readFunc(bs->context);
	    if (c == EOF) { return false; }
	    bs->extraBits <<= BYTE;
	    bs->extraBits = bs->extraBits | c;
	    bs->extraCount += BYTE;
	}
	c1 = bs->extraBits;
	c1 >>= (bs->extraCount - nBits);
	*code = c1;
	bs->extraCount -= nBits;
	bs->extraBits <<= (sizeof(unsigned int)*8 - bs->extraCount);
	bs->extraBits >>= (sizeof(unsigned int)*8 - bs->extraCount);
	if (bs->extraCount == 0) //if extraCount is zero then reset buffer to zero
	    bs->extraBits = 0;
	return true;
}

