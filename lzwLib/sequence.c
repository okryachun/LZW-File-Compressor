#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sequence.h>
#define BYTE 	8


//bernstein hash function
unsigned long long hashFunc(unsigned char *str) {
	unsigned long long hash = 5381;
	int c;
	while (c = *str++) {
	    hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
	}
	return hash;
}

Sequence* newSequence(unsigned char firstByte, unsigned long long hashSize) {
	Sequence *seq = calloc(sizeof(Sequence),1);
	seq->bytes = calloc(sizeof(unsigned char),2);
	seq->bytes[0] = firstByte;
	seq->bytes[1] = '\0';
	seq->length = 1;
	seq->hash = hashFunc(seq->bytes);
	seq->bucket = seq->hash % hashSize;
	seq->next = NULL;
	seq->code = 0;
	return seq;
}

//Initalize table of length hashSize
Sequence **tableSequence(unsigned long long hashSize) {
	Sequence **table = calloc(sizeof(Sequence),hashSize);
	return table;
}

//Delete single sequence
void deleteSequence(Sequence* sequence) {
	Sequence *temp = sequence;
	while(sequence != NULL) {
		sequence = sequence->next;
		free(temp->bytes);
		free(temp);
		temp = sequence;
	}
}

//Delete table sequence
void deleteTable(Sequence **table, unsigned long long hashSize) {
	for(int i=0; i < hashSize; i++) {
		deleteSequence(table[i]);
	}
	free(table);
}

//Append character to sequence and return new sequence
Sequence* copySequenceAppending(Sequence* sequence, unsigned char addByte, unsigned long long hashSize) {
	Sequence *seq = calloc(sizeof(Sequence),1);
	seq->bytes = calloc(sizeof(unsigned char), (sequence->length+2));  //malloc byte string + 2 for new byte and null
	int i = 0;
	//copy the bytes from one sequence to the new sequence
	while(sequence->bytes[i] != '\0') {
		seq->bytes[i] = sequence->bytes[i];
		i++;
	}
	seq->bytes[i] = addByte;  //add the new byte
	seq->bytes[++i] = '\0';	//add null terminator
	seq->length = (sequence->length + 1); //update length
	seq->hash = hashFunc(seq->bytes); //hash the bytes
	seq->bucket = seq->hash % hashSize; //set the bucket
	seq->next = NULL;
	seq->code = 0;
	return seq;
}

//Print sequence when decoding
void outputSequence(Sequence* sequence,
										void (*writeFunc)(unsigned char c, void* context), void* context) {
	int i = 0;
	//while the byte is not null then write it
	while(sequence->bytes[i] != '\0')
		writeFunc(sequence->bytes[i++], context);
}

//Check if two sequences are identical
bool identicalSequences(Sequence* a, Sequence* b) {
	int i = 0;
	//compare bytes in both sequences, false if not identical, true otherwise
	while(a->bytes[i] != '\0' || b->bytes[i] != '\0') {
		if(a->bytes[i] != b->bytes[i])
			return false;
	    	i++;
		}
	return true;
}
