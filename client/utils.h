#ifndef UTILS_H
#define UTILS_H

#include <algorithm>

/// Append list2 to the end of list1.
/// @return The first node of the resulting list or 0 if both lists are empty.
/// @arg list1 the list to append to.
template <class T>
T *slow_append(T *list1, T *list2){
	if(!list1)
		return list2;
	if(!list2)
		return 0;
	
	T *i = list1;
	while(i -> next){
		i = i -> next;
	}

	i -> next = list2;

	return list1;
}

#endif
