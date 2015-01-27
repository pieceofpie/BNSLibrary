#pragma systemFile

#ifndef __BNS_DATA_STRUCTURES_C
#define __BNS_DATA_STRUCTURES_C

#ifndef __BNS_DATA_STRUCTURES_H
#include "BNSDataStructures.h"
#endif

#ifndef __BNS_HEAP_H
#include "BNSHeap.h"
#endif

#ifndef __BNS_CORE_H
#include "BNSCore.h"
#endif

// Initializes a dynamic array in memory
// Default size is in BNSDataStructures.h as
//  DEFAULT_DYNAMIC_ARRAY_SIZE
void DynamicArrayInit(struct DynamicArray *array)
{
	DynamicArrayClear(array);

	array->pointer = bnsMalloc(DEFAULT_DYNAMIC_ARRAY_SIZE);

	if(array->pointer != -1)
	{
		array->maxSize = DEFAULT_DYNAMIC_ARRAY_SIZE;
		array->size = 0;
		array->inUse = true;
	}
}

// Automatically adds a new element into DynamicArray
// If new space is needed, it will automatically allocate that
//   memory
bool DynamicArrayAdd(struct DynamicArray *array, float value)
{
	if(array->size < array->maxSize)
	{
		bnsSetHeapElement(array->pointer + array->size, value);
		array->size++;
	}
	else
	{
		writeDebugStreamLine("%d Trying to extend", array->size);
		int res = bnsExtend(array->pointer, array->maxSize*2);
		if(res != -1)
		{
			array->pointer = res;
			array->maxSize *= 2;
			bnsSetHeapElement(array->pointer + array->size, value);
			array->size++;
		}
		else
		{
			BNS_ERROR("DYNAMIC ARRAY", "Ran out of memory! Check DynamicArrayAdd(...)");
			return false;
		}
	}

	return true;
}

// Retrivies a point of memory from the array
float DynamicArrayGet(struct DynamicArray *array, int where)
{
	return bnsGetHeapElement(array->pointer + where);
}

// Removes a certain element from the array
bool DynamicArrayRemoveAt(struct DynamicArray *array, int where)
{
	if(where < array->maxSize && where >= 0)
  {
  	for(int i = where; i < array->maxSize-1; i++)
  	{
  		bnsSetHeapElement(array->pointer + i, bnsGetHeapElement(array->pointer + i+1));
  	}
  	bnsSetHeapElement(array->pointer + array->maxSize-1, 0);
  }
  else
  {
  	BNS_ERROR("DYNAMIC ARRAY", "Trying to remove unallocated memory, check calls to DynamicArrayRemoveAt(...)");
  	return false;
  }

  return true;
}

// Clears the entire array to a size of zero
// Note: Memory is still allocated!
void DynamicArrayClear(struct DynamicArray *array)
{
	array->size = 0;
}

// Returns how large the array is
int DynamicArraySize(struct DynamicArray *array)
{
	return array->size;
}

// Removes the array from memory entirely
// Note: You will have to call DynamicArrayInit to
//  be able to use this array again
void DynamicArrayDelete(struct DynamicArray *array)
{
	array->size = 0;
	if(array->inUse == true)
	{
		bnsFree(array->pointer);
		array->pointer = -1;
	}
}

// Initialize the stack
void StackInit(struct Stack *object)
{
	object->pos = -1;
	DynamicArrayInit(object->array);
}

// Removes the top-most number and returns it
//  to the user
float StackPop(struct Stack *object)
{
	if(object->pos < 0)
	{
		BNS_ERROR("STACK ERROR", "Cannot pop a stack of size zero!");
		return 0;
  }

	float returnVal = DynamicArrayGet(object->array, object->pos);
	object->pos--;

	return returnVal;
}

bool StackPush(struct Stack *object, float value)
{
	bool good = DynamicArrayAdd(object->array, value);

	if(good)
	{
		object->pos++;
		return true;
	}
	else
	{
		return false;
	}
}

float StackPeek(struct Stack *object)
{
	if(object->pos < 0)
	{
	  BNS_ERROR("STACK ERROR", "Cannot peek a stack of size zero!");
		return 0;
  }
	return DynamicArrayGet(object->array, object->pos);
}

// Returns true if the stack is completely empty
bool StackIsEmpty(struct Stack *object)
{
	if(object->pos < 0)
		return true;


	return false;
}

#endif