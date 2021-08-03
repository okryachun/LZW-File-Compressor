//Author: Oleg Kryachun
//Date: 02/06/2020
//Class: CS360
//Assignment 1
#include <sequence.h>
#include <stdio.h>
#include <bitStream.h>
#include <dict.h>
#include <lzw.h>



bool lzwEncode (unsigned int bits, unsigned int maxBits,
                int (*readFunc)(void *context),
                void (*writeFunc)(unsigned char c, void *context),
                void *context) {

	// ceritfy parameters values are fit to execute this program
	if (bits < MIN || maxBits < 16 || maxBits < bits || maxBits > MAX || bits > 16) {
		return false;
	}
	
	unsigned long long hashSize = setHashSize(maxBits);
	Dict *dict = newDict(hashSize);
	initDict(dict, NULL, ENCODE, hashSize);
	BitStream *bs = openOutputBitStream(writeFunc, context);
	Sequence *x, *w;
	int c;
	w = newSequence(readFunc(context), hashSize);

	while (true) {
		if (bits < MAX && bits <= maxBits) { //check the bit value is correct
			checkMinBits(&bits,dict->count, ENCODE);
		}
		c = readFunc(context);  //read next byte
		if (c >= 0)	{//while not the end of file
		    x = copySequenceAppending(w, (unsigned char)c, hashSize);  //create new sequence x = w+c
		} else {  //if end of file
			searchDict(dict, w, &w->code); //find w
			outputBits(bs, bits, w->code); //output w
			deleteSequence(w);
			break;
		}
		if (searchDict(dict, x, &x->code)) {  //check if x is stored in the dictionary
		    deleteSequence(w);	//if it is, free w
			w = x;	//set w to x
		} else { //if x doesn't exist
		    searchDict(dict, w, &w->code);  //search for w
			outputBits(bs, bits, w->code);	//output w
			if (dict->count < hashSize) {	//update min bits
			    insertDict(dict, x, x->code);
			}
			deleteSequence(w); //free w
			w = newSequence((unsigned char)c, hashSize);
		}
	}
	
	//free memory and delete streams
	closeAndDeleteBitStream(bs);
	deleteDictDeep(dict);

	return true;
}
