#include <sequence.h>
#include <stdio.h>
#include <bitStream.h>
#include <dict.h>
#include <sequence.h>
#include <lzw.h>

bool lzwDecode(unsigned int bits, unsigned int maxBits,
               int  (*readFunc )(void* context),
               void (*writeFunc)(unsigned char c, void* context),
               void* context) {

	if(bits < MIN || maxBits < 16 || maxBits < bits || maxBits > MAX || bits > 16)
		return false;

	unsigned long long hashSize = setHashSize(maxBits);
	Sequence **seq = tableSequence(hashSize);
	initDict(NULL, seq, DECODE, hashSize);	//initialize dictionary
	BitStream *bs = openInputBitStream(readFunc, context);
	unsigned int previousCode, currentCode, count = 255;
	unsigned char c;
	Sequence *s;
	readInBits(bs,bits,&previousCode); //read first bit
	outputSequence(seq[previousCode], writeFunc, context);  //ouput first bit
	//if starting out with 8 bits, edge case, increment to 9
	if(bits == 8)
		bits = 9;
	//while there is more byts to read
	while(readInBits(bs,bits,&currentCode)) {
		if(currentCode < count) {	//check if currentCode read is less than count
			c = seq[currentCode]->bytes[0];
		} else {
			c = seq[previousCode]->bytes[0];
		}
		if(count < hashSize) {  //if there is room for a new sequence in the tabe
			s = copySequenceAppending(seq[previousCode], c, hashSize);  //create new sequence with new character appended
			seq[++count] = s;	//store sequence in table
		}
		outputSequence(seq[currentCode], writeFunc, context);  //output sequence at currentCode location
		previousCode = currentCode;	 //set previous code

		if(bits < MAX && bits <= maxBits)
			checkMinBits(&bits,count, DECODE);
	}
	//close and delete bit stream and sequence table
	closeAndDeleteBitStream(bs);
	deleteTable(seq, hashSize);
	return true;
}
