#ifndef SJTU_VECTOR_HPP
#define SJTU_VECTOR_HPP

#include "exceptions.hpp"

#include <climits>
#include <cstddef>
#include <memory>

namespace sjtu 
{
/**
 * a data container like std::vector
 * store data in a successive memory and support random access.
 */
template<typename T>
class vector 
{
public:
	/**
	 * TODO
	 * a type for actions of the elements of a vector, and you should write
	 *   a class named const_iterator with same interfaces.
	 */
	T* data;
	size_t length;//目前开的长度
	size_t now_length;//目前用的长度
	std::allocator<T> my_alloc;//内存分配器
	/**
	 * you can see RandomAccessIterator at CppReference for help.
	 */
	class const_iterator;
	class iterator 
	{
	// The following code is written for the C++ type_traits library.
	// Type traits is a C++ feature for describing certain properties of a type.
	// For instance, for an iterator, iterator::value_type is the type that the 
	// iterator points to. 
	// STL algorithms and containers may use these type_traits (e.g. the following 
	// typedef) to work properly. In particular, without the following code, 
	// @code{std::sort(iter, iter1);} would not compile.
	// See these websites for more information:
	// https://en.cppreference.com/w/cpp/header/type_traits
	// About value_type: https://blog.csdn.net/u014299153/article/details/72419713
	// About iterator_category: https://en.cppreference.com/w/cpp/iterator
	friend class vector;
	public:
		using difference_type = std::ptrdiff_t;
		using value_type = T;
		using pointer = T*;
		using reference = T&;
		using iterator_category = std::output_iterator_tag;

	private:
		/**
		 * TODO add data members
		 *   just add whatever you want.
		 */
		int position;
		vector<T>* vec_;
	public:
		/**
		 * return a new iterator which pointer n-next elements
		 * as well as operator-
		 */
		iterator (){}
		iterator (const iterator &other){
			position=other.position;
			vec_=other.vec_;
		}
		iterator operator=(const iterator &other){
			position=other.position;
			vec_=other.vec_;
			return *this;
		}
		iterator operator+(const int &n) const 
		{
			//TODO
			iterator temp;
			temp.position=position+n;
			temp.vec_=vec_;
			return temp;
		}
		iterator operator-(const int &n) const 
		{
			//TODO
			iterator temp;
			temp.position=position-n;
			temp.vec_=vec_;
			return temp;
		}
		// return the distance between two iterators,
		// if these two iterators point to different vectors, throw invaild_iterator.
		int operator-(const iterator &rhs) const 
		{
			//TODO
			if(vec_!=rhs.vec_)throw(invalid_iterator());
			return position-rhs.position;
		}
		iterator& operator+=(const int &n) 
		{
			//TODO
			position+=n;
			return *this;
		}
		iterator& operator-=(const int &n) 
		{
			//TODO
			position-=n;
			return *this;
		}
		/**
		 * TODO iter++
		 */
		iterator operator++(int) {
			iterator temp(*this);
			position++;
			return temp;
		}
		/**
		 * TODO ++iter
		 */
		iterator& operator++() {
			position++;
			return *this;
		}
		/**
		 * TODO iter--
		 */
		iterator operator--(int) {
			iterator temp(*this);
			position--;
			return temp;
		}
		/**
		 * TODO --iter
		 */
		iterator& operator--() {
			position--;
			return *this;
		}
		/**
		 * TODO *it
		 */
		T& operator*() const{
			return *(vec_->data+position);
		}
		/**
		 * a operator to check whether two iterators are same (pointing to the same memory address).
		 */
		bool operator==(const iterator &rhs) const {
			if(vec_==rhs.vec_ && position==rhs.position)return true;
			else return false;
		}
		bool operator==(const const_iterator &rhs) const {
			if(vec_==rhs.vec_ && position==rhs.position)return true;
			else return false;
		}
		/**
		 * some other operator for iterator.
		 */
		bool operator!=(const iterator &rhs) const {
			return !((*this)==rhs);
		}
		bool operator!=(const const_iterator &rhs) const {
			return !((*this)==rhs);
		}
	};
	/**
	 * TODO
	 * has same function as iterator, just for a const object.
	 */
	class const_iterator 
	{
	friend class vector;
	public:
		using difference_type = std::ptrdiff_t;
		using value_type = T;
		using pointer = T*;
		using reference = T&;
		using iterator_category = std::output_iterator_tag;
	
	private:
		/*TODO*/
		int position;
		const vector<T>* vec_;
	public:
		const_iterator(){}
		const_iterator(const const_iterator &other){
			position=other.position;
			vec_=other.vec_;
		}
		const_iterator(const iterator &other){
			position=other.position;
			vec_=other.vec_;
		}
		const_iterator operator+(const int &n) const 
		{
			//TODO
			const_iterator temp;
			temp.position=position+n;
			temp.vec_=vec_;
			return temp;
		}
		const_iterator operator-(const int &n) const 
		{
			//TODO
			const_iterator temp;
			temp.position=position-n;
			temp.vec_=vec_;
			return temp;
		}
		int operator-(const const_iterator &rhs) const 
		{
			//TODO
			if(vec_!=rhs.vec_)throw(invalid_iterator());
			return position-rhs.position;
		}
		const_iterator& operator+=(const int &n) 
		{
			//TODO
			position+=n;
			return *this;
		}
		const_iterator& operator-=(const int &n) 
		{
			//TODO
			position-=n;
			return *this;
		}
		/**
		 * TODO iter++
		 */
		const_iterator operator++(int) {
			const_iterator temp(*this);
			position++;
			return temp;
		}
		/**
		 * TODO ++iter
		 */
		const_iterator& operator++() {
			position++;
			return *this;
		}
		/**
		 * TODO iter--
		 */
		const_iterator operator--(int) {
			const_iterator temp(*this);
			position--;
			return temp;
		}
		/**
		 * TODO --iter
		 */
		const_iterator& operator--() {
			position--;
			return *this;
		}
		/**
		 * TODO *it
		 */
		T operator*() const{
			return *(vec_->data+position);
		}
		/**
		 * a operator to check whether two iterators are same (pointing to the same memory address).
		 */
		bool operator==(const iterator &rhs) const {
			if(vec_==rhs.vec_ && position==rhs.position)return true;
			else return false;
		}
		bool operator==(const const_iterator &rhs) const {
			if(vec_==rhs.vec_ && position==rhs.position)return true;
			else return false;
		}
		/**
		 * some other operator for iterator.
		 */
		bool operator!=(const iterator &rhs) const {
			return !((*this)==rhs);
		}
		bool operator!=(const const_iterator &rhs) const {
			return !((*this)==rhs);
		}
	};
	/**
	 * TODO Constructs
	 * At least two: default constructor, copy constructor
	 */

	//  T* data;
	// size_t length;//目前开的长度
	// size_t now_length;//目前用的长度
	// std::allocator<T> my_alloc;//内存分配器
	void merge_sort(T* left, T* right) {
		if (left >= right) return;

		T* middle = left + (right - left) / 2;

		merge_sort(left, middle);
		merge_sort(middle + 1, right);

		T* temp = new T[right - left + 1];
		T* p1 = left, *p2 = middle + 1, *p3 = temp;
		while (p1 <= middle && p2 <= right) {
			if (*p1 < *p2) {
				*p3++ = *p1++;
			}
			else {
				*p3++ = *p2++;
			}
		}
		while (p1 <= middle) {
			*p3++ = *p1++;
		}
		while (p2 <= right) {
			*p3++ = *p2++;
		}
		for (T* p = temp, *q = left; p3-- != temp;) {
			*q++ = *p++;
		}
		delete[] temp;
	}
	void sort(){
		if(now_length==0)return;
		merge_sort(data,data+now_length-1);
	}
	// int find_pos(T value){
	// 	int l=0,r=now_length;
	// 	while(l<=r){
	// 		int mid=(l+r)>>1;
	// 		if(data[mid]<value)l=mid+1;
	// 		if(value<data[mid])r=mid-1;
	// 		else return mid;
	// 	}
	// 	return -1;
	// }
	vector() {
		data=my_alloc.allocate(2);
		length=2;
		now_length=0;
	}
	vector(const vector &other) {
		data=my_alloc.allocate(other.length);
		length=other.length;
		now_length=other.now_length;
		for(int i=0;i<now_length;i++){
			my_alloc.construct(data+i,other.data[i]);
		}
	}
	vector(vector &&other){
		data=other.data;
		length=other.length;
		other.data=nullptr;
		other.length=0;
	}
	/**
	 * TODO Destructor
	 */
	~vector() {
		if(data!=nullptr){
			for(int i=0;i<now_length;i++)my_alloc.destroy(data+i);
			my_alloc.deallocate(data,length);
		}
	}
	/**
	 * TODO Assignment operator
	 */
	vector &operator=(const vector &other) {
		if(this==&other)return *this;
		for(int i=0;i<now_length;i++)my_alloc.destroy(data+i);
		my_alloc.deallocate(data,length);
		data=my_alloc.allocate(other.length);
		now_length=other.now_length;
		length=other.length;
		for(int i=0;i<now_length;i++){
			my_alloc.construct(data+i,other.data[i]);
		}
		return *this;
	}
	/**
	 * assigns specified element with bounds checking
	 * throw index_out_of_bound if pos is not in [0, size)
	 */
	T & at(const size_t &pos) {
		if(pos<0 || pos>=length)throw(index_out_of_bound());
		return data[pos];
	}
	const T & at(const size_t &pos) const {
		if(pos<0 || pos>=length)throw(index_out_of_bound());
		return data[pos];
	}
	/**
	 * assigns specified element with bounds checking
	 * throw index_out_of_bound if pos is not in [0, size)
	 * !!! Pay attentions
	 *   In STL this operator does not check the boundary but I want you to do.
	 */
	T & operator[](const size_t &pos) {
		if(pos<0 || pos>=length)throw(index_out_of_bound());
		return data[pos];
	}
	const T & operator[](const size_t &pos) const {
		if(pos<0 || pos>=length)throw(index_out_of_bound());
		return data[pos];
	}
	/**
	 * access the first element.
	 * throw container_is_empty if size == 0
	 */
	const T & front() const {
		if(now_length==0)throw(container_is_empty());
		return data[0];
	}
	/**
	 * access the last element.
	 * throw container_is_empty if size == 0
	 */
	const T & back() const {
		if(now_length==0)throw(container_is_empty());
		return data[now_length-1];
	}
	/**
	 * returns an iterator to the beginning.
	 */
	iterator begin() {
		iterator temp;
		temp.vec_=this;
		temp.position=0;
		return temp;
	}
	const_iterator cbegin() const {
		const_iterator temp;
		temp.vec_=this;
		temp.position=0;
		return temp;
	}
	/**
	 * returns an iterator to the end.
	 */
	iterator end() {
		iterator temp;
		temp.vec_=this;
		temp.position=now_length;
		return temp;
	}
	const_iterator cend() const {
		const_iterator temp;
		temp.vec_=this;
		temp.position=now_length;
		return temp;
	}
	/**
	 * checks whether the container is empty
	 */
	bool empty() const {
		if(now_length==0)return true;
		else return false;
	}
	/**
	 * returns the number of elements
	 */
	size_t size() const {
		return now_length;
	}
	/**
	 * clears the contents
	 */
	void clear() {
		for(int i=0;i<now_length;i++){
			my_alloc.destroy(data+i);
		}
		now_length=0;
	}
	/**
	 * inserts value before pos
	 * returns an iterator pointing to the inserted value.
	 */
	iterator insert(iterator pos, const T &value) {
		if(now_length==length){
			//double space
			T* data_;
			data_=my_alloc.allocate(length*2);
			for(int i=0;i<now_length;i++){
				my_alloc.construct(data_+i,data[i]);
			}
			for(int i=0;i<now_length;i++){
				my_alloc.destroy(data+i);
			}
			my_alloc.deallocate(data,length);
			length*=2;
			data=data_;
		}
		now_length++;
		if(pos.position==now_length-1){
			my_alloc.construct(data+now_length-1,value);
			return pos;
		}
		else{
			my_alloc.construct(data+now_length-1,data[now_length-2]);
			for(int i=now_length-1;i>=pos.position+1;i--){
				data[i]=data[i-1];
			}
			data[pos.position]=value;
			return pos;
		}
	}
	/**
	 * inserts value at index ind.
	 * after inserting, this->at(ind) == value
	 * returns an iterator pointing to the inserted value.
	 * throw index_out_of_bound if ind > size (in this situation ind can be size because after inserting the size will increase 1.)
	 */
	iterator insert(const size_t &ind, const T &value) {
		if(now_length==length){
			//double space
			T* data_;
			data_=my_alloc.allocate(length*2);
			for(int i=0;i<now_length;i++){
				my_alloc.construct(data_+i,data[i]);
			}
			for(int i=0;i<now_length;i++){
				my_alloc.destroy(data+i);
			}
			my_alloc.deallocate(data,length);
			length*=2;
			data=data_;
		}
		now_length++;
		if(ind==now_length-1){
			my_alloc.construct(data+now_length-1,value);
			iterator temp;
			temp.position=ind;
			temp.vec_=this;
			return temp;
		}
		else{
			my_alloc.construct(data+now_length-1,data[now_length-2]);
			for(int i=now_length-1;i>=ind+1;i--){
				data[i]=data[i-1];
			}
			data[ind]=value;
			iterator temp;
			temp.position=ind;
			temp.vec_=this;
			return temp;
		}
	}
	/**
	 * removes the element at pos.
	 * return an iterator pointing to the following element.
	 * If the iterator pos refers the last element, the end() iterator is returned.
	 */
	iterator erase(iterator pos) {
		now_length--;
		for(int i=pos.position;i<=now_length-1;i++)data[i]=data[i+1];
		my_alloc.destroy(data+now_length);
		return pos;
	}
	/**
	 * removes the element with index ind.
	 * return an iterator pointing to the following element.
	 * throw index_out_of_bound if ind >= size
	 */
	iterator erase(const size_t &ind) {
		if(ind>=now_length)throw(index_out_of_bound());
		now_length--;
		for(int i=ind;i<=now_length-1;i++)data[i]=data[i+1];
		my_alloc.destroy(data+now_length);
		iterator temp;
		temp.position=ind;
		temp.vec_=this;
		return temp;
	}
	/**
	 * adds an element to the end.
	 */
	void push_back(const T &value) {
		if(now_length==length){
			//double space
			T* data_;
			data_=my_alloc.allocate(length*2);
			for(int i=0;i<now_length;i++){
				my_alloc.construct(data_+i,data[i]);
			}
			for(int i=0;i<now_length;i++){
				my_alloc.destroy(data+i);
			}
			my_alloc.deallocate(data,length);
			length*=2;
			data=data_;
			data_=nullptr;
		}
		now_length++;
		my_alloc.construct(data+now_length-1,value);
	}
	/**
	 * remove the last element from the end.
	 * throw container_is_empty if size() == 0
	 */
	void pop_back() {
		if(size()==0)throw(container_is_empty());
		now_length--;
		my_alloc.destroy(data+now_length);
	}
	iterator find(iterator from,iterator to,const T &temp){
		iterator ans;
		ans.vec_=this;
		for(int i=from.position;i<to.position;i++){
			if(*(data+i)==temp){
				ans.position=i;
				return ans;
			}
		}
		ans.position=to.position;
		return ans;
	}
};


}

#endif
