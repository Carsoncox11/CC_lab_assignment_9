#include <stdio.h>
#include <stdlib.h>

struct RecordType
{
	int		id;
	char	name;
	int		order; 
	struct RecordType* next;
};

struct HashType
{
	struct RecordType* record;
};

//hashes the data by taking in the record id and modulus it by the table size
int hash(int x, int tableSize)
{
	return x % tableSize;
}

// parses input file to an integer array
int parseData(char* inputFileName, struct RecordType** ppData)
{
	FILE* inFile = fopen(inputFileName, "r");
	int dataSz = 0;
	int i, n;
	char c;
	struct RecordType *pRecord;
	*ppData = NULL;

	if (inFile)
	{
		fscanf(inFile, "%d\n", &dataSz);
		*ppData = (struct RecordType*) malloc(sizeof(struct RecordType) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i = 0; i < dataSz; ++i)
		{
			pRecord = *ppData + i;
			fscanf(inFile, "%d ", &n);
			pRecord->id = n;
			fscanf(inFile, "%c ", &c);
			pRecord->name = c;
			fscanf(inFile, "%d ", &n);
			pRecord->order = n;
		}

		fclose(inFile);
	}

	return dataSz;
}

// prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
	int i;
	printf("\nRecords:\n");
	for (i = 0; i < dataSz; ++i)
	{
		printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
	}
	printf("\n\n");
}

// insert record into the hash table
void insertRecord(struct HashType *hashTable, struct RecordType *record, int tableSize)
{
    // call the hash function to get the index, pass in the ID value
	int hashIndex = hash(record->id, tableSize);

	//if it is NULL at that index, assign that index to the record
	if((hashTable + hashIndex)->record == NULL)
		(hashTable + hashIndex)->record = record;
	else
	{
		//create a temporary pointer for traversal
		struct RecordType * temp = (hashTable + hashIndex)->record;

		//traverse to the end of the linked list if it is not NULL
		while(temp->next != NULL)
			temp = temp->next;
		
		//once at the end of the linked list, place the record at the end
		temp->next = record;
	}
}


// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType *pHashArray, int hashSz)
{
	int i;
	struct RecordType * temp;

	for (i=0;i<hashSz;++i)
	{
		// if index is occupied with any records, print all the records
		if((pHashArray + i)->record != NULL)
		{
			//temporary pointer for traversal
			temp = (pHashArray + i)->record;

			printf("Index %d ", i);

			//while temp is not NULL, print out the record and traverse the linked list
			while(temp != NULL)
			{
				printf("-> %d, %c, %d ", temp->id, temp->name, temp->order);
				temp = temp->next;
			}

			printf("-> NULL\n");
		}
	}
}

int main(void)
{
	struct RecordType *pRecords;
	int recordSz = 0;

	recordSz = parseData("input.txt", &pRecords);
	printRecords(pRecords, recordSz);

	//initialize the hash table and the table size
	struct HashType *hashTable;
	int hashTableSize = 11;

	//allocate space for the hash table, using calloc to set all to NULL
	hashTable = calloc(hashTableSize, sizeof(struct HashType));

	//insert each record into the hash table
	for(int i = 0; i < recordSz; i++)
		insertRecord(hashTable, pRecords + i, hashTableSize);
	
	displayRecordsInHash(hashTable, hashTableSize);

	//free the allocated memory
	free(hashTable);
	free(pRecords);

	return 0;
}