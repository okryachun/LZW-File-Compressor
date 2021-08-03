#include <stdlib.h>
#include <dict.h>
#define ENCODE 1
#define DECODE 0

//Initialize dictionary
Dict *newDict (unsigned long long hashSize) {
	Dict *dict = calloc(sizeof(Dict), 1);
	dict->hashSize = hashSize;
	dict->count = 0;
	dict->table = calloc(sizeof(Sequence), hashSize);
	
    return dict;
}

//Delete dictionary and free memory
void deleteDictDeep (Dict *dict) {
	deleteTable(dict->table, dict->hashSize);
	free(dict);
}

//Initialize first 256 elements of the dictionary or sequence table
void initDict (Dict *dict, Sequence **s, int direction, unsigned long long hashSize){
    Sequence *seq;
    for (int i = 0; i < 256; i++) {
        if (direction == ENCODE) {  //For dictionary initialization
            seq = newSequence((unsigned char) i, dict->hashSize);
            seq->code = i;
            dict->table[seq->bucket] = seq;
            dict->count = i;
        } else {  //for sequence table initialization
            seq = newSequence((unsigned char) i, hashSize);
            seq->code = i;
            s[i] = seq;
        }
    }
}

//Search the dictionary to check if sequence exists
bool searchDict (Dict *dict, Sequence *key, unsigned int *code) {
    Sequence *temp = dict->table[key->bucket];
    while (temp != NULL) {	//if there is a collision, check the next
        if (identicalSequences(key, temp)) {
		    *code = temp->code;
			return true;  //if found then true
        }
		    temp = temp->next;
    }
	
    return false;	//if not found then false
}

//insert sequence into dictionary
void insertDict (Dict *dict, Sequence *key, unsigned int code) {
    Sequence *temp = dict->table[key->bucket];

    if (dict->table[key->bucket] != NULL) {   //if not empty then must be collision
        while (temp->next != NULL) {	//traverse to last next
            temp = temp->next;
        }
        temp->next = key;	//add sequence to end of linked list
        ++dict->count;
        temp->next->code = dict->count;
        
        return;
    }
	//else there is no collision and add new sequence
    ++dict->count;
    dict->table[key->bucket] = key;
    dict->table[key->bucket]->code = dict->count;
}


//Set min bits, increment bits as the program runs
void checkMinBits (unsigned int *bits, unsigned int code, int direction) {
    long long x = -1;
    unsigned int y = 0;
    y = (y|x);
    y >>= (32-(*bits));
    if(direction == ENCODE) //Encode and decode have different incrementing algorithms
        y++;
    if (y <= code)
        *bits = *bits + 1;
}
