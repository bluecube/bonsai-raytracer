/// \file LList.h
/// Manipulation with unidirectional linked lists.

#ifndef LLIST_H
#define LLIST_H

/// A class that manages an unidirectional linked list.
/// The list item is of type T. The only requirement for the type is
/// that it has a member T* next.
/// This class keeps a pointer to beginning of the linked list
/// and a pointer to the next pointer of the last item to speed up appends.
/// \note The destructor of this class doesn't deallocate the list.
/// \note This class may be used exactly like T*, wit the only exception being
/// that assigning a T* to LList takes O(N) time, instead of O(1) (the last element must be found).
/// \note Prepending is a tiny bit slower than appending (but still in constant time).
template<class T>
class LList{
public:
	LList() : a(0), b(&a) {}

	LList(T *first) : a(first){
		find_last();
	}

	LList(T *first, T * last) : a(first), b(&(last -> next)) {}

	LList<T> operator=(T *first){
		a = first;
		find_last();
		return *this;
	}

	operator T* (){
		return a;
	}
	
	LList<T> append(LList<T> l);
	LList<T> append_single(T *i);
	LList<T> prepend(LList<T> l);
	LList<T> prepend_single(T *i);

	LList<T> split_after(T *i);
	T* drop_after(T *i);
	void delete_after(T *i);

	void delete_list();
private:
	void find_last();

	/// Pointer to the first list item;
	T *a;

	/// Pointer to where appended items go.
	/// (the pointer to last item's next, or pointer to a)
	T **b;
};

/// Appends a list in constant time.
template<class T>
LList<T> LList<T>::append(LList<T> l){
	*b = l;
	b = l.b;

	return *this;
}

/// Appends a single list item.
/// If a list longer than 1 item is given to this function,
/// then it creates a memory leak.
template<class T>
LList<T> LList<T>::append_single(T *i){
	*b = i;
	b = &(i -> next);
	i->next = NULL;

	return *this;
}

/// Prepend a list in constant time.
template<class T>
LList<T> LList<T>::prepend(LList<T> l){
	if(!a){
		a = l.a;
		b = l.b;
	}else{
		*(l.b) = a;
		a = l.a;
	}
	return *this;

}

/// Prepends a single list item.
/// If a list longer than 1 item is given to this function,
/// then either only the first item is added and all the other items
/// are lost (if the list we're prepending to is not empty), or all later
/// appends will generate wrong output (empty list).
template<class T>
LList<T> LList<T>::prepend_single(T *i){
	if(!a){
		a = i;
		b = &(i -> next);
	}else{
		i -> next = a;
		a = i;
	}
	
	return *this;
}

/// Splits the list into two, i remains as a last element in
/// this, its successors are returned as a new LList.
/// Assumes that i is a member of this list.
template<class T>
LList<T> LList<T>::split_after(T *i){
	LList<T> ret;
	ret.a = i -> next;
	ret.b = b;

	b = &(i->next);
	i->next = NULL;

	return ret;
}

/// Removes the successor of i from the list.
/// Assumes that i is a member of this list.
/// If i is last item, then nothing is done.
/// \return Pointer to the removed item or NULL.
template<class T>
T *LList<T>::drop_after(T *i){
	T *tmp = i->next;

	if(!tmp)
		return NULL;
	
	if(!tmp->next){
		b = &(i->next);
	}

	i->next = tmp->next;
	tmp->next = NULL;

	return tmp;
}

/// Like drop_after, but deletes the value instead of returning it.
template<class T>
void delete_after(T *i){
	T *tmp = drop_after(i);
	if(tmp)
		delete tmp;
}

/// Deletes all list items.
template<class T>
void LList<T>::delete_list(){
	T *tmp;

	while(a){
		tmp = a -> next;
		delete a;
		a = tmp;
	}

	b = &a;
}

/// Walks through the list in a and updates b.
/// Works correctly with empty list.
template <class T>
void LList<T>::find_last(){
	T *tmp = a;
	b = &a;

	while(tmp){
		b = &(tmp -> next);
		tmp = tmp -> next;
	}
}

#endif
