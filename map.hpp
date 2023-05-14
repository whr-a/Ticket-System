/**
 * implement a container like std::map
 */
#ifndef SJTU_MAP_HPP
#define SJTU_MAP_HPP

// only for std::less<T>
#include <functional>
#include <cstddef>
#include <memory>
#include <iostream>
#include "utility.hpp"
#include "exceptions.hpp"

namespace sjtu {
enum type {right,wrong};
template<
	class Key,
	class T,
	class Compare = std::less<Key>
> class map {
public:
	/**
	 * the internal type of data.
	 * it should have a default constructor, a copy constructor.
	 * You can use sjtu::map as value_type by typedef.
	 */
	typedef pair<const Key, T> value_type;
	struct node
	{
		value_type value;
		int height;
		node *left,*right,*father;
		node(){left=right=father=nullptr;}
		node(value_type value_):value(value_){
			left=right=father=nullptr;
		}
	};
    int getheight(node *x){
        if(x==nullptr)return 0;
        if(x->left==nullptr && x->right==nullptr){
            x->height=1;
            return 1;
        }
        if(x->left==nullptr)x->height=x->right->height+1;
        else if(x->right==nullptr)x->height=x->left->height+1;
        else x->height=std::max(x->left->height , x->right->height) + 1;
        return x->height;
    }
	node* findbegin(node *x) const {
		if(x==nullptr)return nullptr;
		if(x->left==nullptr)return x;
		else return findbegin(x->left);
	}
	node* findend(node *x) const {
		if(x==nullptr)return nullptr;
		if(x->right==nullptr)return x;
		else return findend(x->right);
	}
	void rightrotate(node *a){
		if(a->father!=nullptr){
			if(a->father->left==a)a->father->left=a->left;
			else a->father->right=a->left;
		}
		if(a==root)root=a->left;
		node *b=a->left;
		b->father=a->father;
		a->left=b->right;
        if(b->right!=nullptr)b->right->father=a;
		b->right=a;
		a->father=b;
        getheight(a);
        getheight(a->father);
	}
	void leftrotate(node *a){
		if(a->father!=nullptr){
			if(a->father->left==a)a->father->left=a->right;
			else a->father->right=a->right;
		}
		if(a==root)root=a->right;
		node *b=a->right;
		b->father=a->father;
		a->right=b->left;
        if(b->left!=nullptr)b->left->father=a;
		b->left=a;
		a->father=b;
        getheight(a);
        getheight(a->father);
	}
	node* root;
	size_t length;
	/**
	 * see BidirectionalIterator at CppReference for help.
	 *
	 * if there is anything wrong throw invalid_iterator.
	 *     like it = map.begin(); --it;
	 *       or it = map.end(); ++end();
	 */
	class const_iterator;
	class iterator {
	friend class map;
	private:
		/**
		 * TODO add data members
		 *   just add whatever you want.
		 */
	public:
		const static type iterator_assignable = right;
		node *pointer;
		map<Key,T,Compare>* mp;
		iterator() {
			// TODO
			pointer=nullptr;
			mp=nullptr;
		}
		iterator(const iterator &other) {
			// TODO
			pointer=other.pointer;
			mp=other.mp;
		}
		/**
		 * TODO iter++
		 */
		node* findnext(node *x) const{
            if(x==nullptr)return nullptr;
			if(x->left==nullptr)return x;
			else return findnext(x->left);
		}
		node* findleftroot(node *x) const{
			if(x->father==nullptr)return nullptr;
			if(x->father->left==x)return x->father;
			return findleftroot(x->father);
		}
		iterator operator++(int) {
			if(pointer==nullptr)throw(invalid_iterator());
			iterator t(*this);
			if(pointer->father==nullptr)pointer=findnext(pointer->right);
			else{
				if(pointer->father->left==pointer){
					if(pointer->right!=nullptr)pointer=findnext(pointer->right);
					else pointer=pointer->father;
				}
				else{
					if(pointer->right!=nullptr)pointer=findnext(pointer->right);
					else pointer=findleftroot(pointer->father);
				}
			}
			return t;
		}
		/**
		 * TODO ++iter
		 */
		iterator & operator++() {
			if(pointer==nullptr)throw(invalid_iterator());
			if(pointer->father==nullptr)pointer=findnext(pointer->right);
			else{
				if(pointer->father->left==pointer){
					if(pointer->right!=nullptr)pointer=findnext(pointer->right);
					else pointer=pointer->father;
				}
				else{
					if(pointer->right!=nullptr)pointer=findnext(pointer->right);
					else pointer=findleftroot(pointer->father);
				}
			}
			return *this;
		}
		/**
		 * TODO iter--
		 */
		node* findlast(node *x)const {
			if(x->right==nullptr)return x;
			else return findlast(x->right);
		}
		node* findrightroot(node *x)const {
			if(x->father==nullptr)return nullptr;
			if(x->father->right==x)return x->father;
			else return findrightroot(x->father);
		}
		iterator operator--(int) {
            if(mp->root==nullptr)throw(invalid_iterator());
			iterator t(*this);
			if(pointer==nullptr){
				pointer=mp->findend(mp->root);
				return t;
			}
			if(pointer->father==nullptr){
				if(pointer->left==nullptr)throw(invalid_iterator());
				pointer=findlast(pointer->left);
			}
			else{
				if(pointer==pointer->father->left){
					if(pointer->left==nullptr){
						node *ans=findrightroot(pointer->father);
						if(ans==nullptr)throw(invalid_iterator());
						pointer=ans;
					}
					else pointer=findlast(pointer->left);
				}
				else{
                    if(pointer->left==nullptr)pointer=pointer->father;
                    else pointer=findlast(pointer->left);
                }
			}
			return t;
		}
		/**
		 * TODO --iter
		 */
		iterator & operator--() {
            if(mp->root==nullptr)throw(invalid_iterator());
			if(pointer==nullptr){
				pointer=mp->findend(mp->root);
				return *this;
			}
			if(pointer->father==nullptr){
				if(pointer->left==nullptr)throw(invalid_iterator());
				pointer=findlast(pointer->left);
			}
			else{
				if(pointer==pointer->father->left){
					if(pointer->left==nullptr){
						node *ans=findrightroot(pointer->father);
						if(ans==nullptr)throw(invalid_iterator());
						pointer=ans;
						ans=nullptr;
					}
					else pointer=findlast(pointer->left);
				}
				else{
                    if(pointer->left==nullptr)pointer=pointer->father;
                    else pointer=findlast(pointer->left);
                }
			}
			return *this;
		}
		/**
		 * a operator to check whether two iterators are same (pointing to the same memory).
		 */
		value_type & operator*() const {
			return pointer->value;
		}
		bool operator==(const iterator &rhs) const {
			if(mp==rhs.mp && pointer==rhs.pointer)return true;
			else return false;
		}
		bool operator==(const const_iterator &rhs) const {
			if(mp==rhs.mp && pointer==rhs.pointer)return true;
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

		/**
		 * for the support of it->first. 
		 * See <http://kelvinh.github.io/blog/2013/11/20/overloading-of-member-access-operator-dash-greater-than-symbol-in-cpp/> for help.
		 */
		value_type* operator->() const noexcept {
			return &(pointer->value);
		}
	};
	class const_iterator {
		friend class map;
		// it should has similar member method as iterator.
		//  and it should be able to construct from an iterator.
    private:
			// data members.
    public:
		const static type iterator_assignable = wrong;
        node *pointer;
        const map<Key,T,Compare>* mp;
        const_iterator() {
            // TODO
            pointer=nullptr;
            mp=nullptr;
        }
        const_iterator(const iterator &other) {
            // TODO
            pointer=other.pointer;
            mp=other.mp;
        }
        /**
         * TODO iter++
         */
        node* findnext(node *x) const{
            if(x==nullptr)return nullptr;
            if(x->left==nullptr)return x;
            else return findnext(x->left);
        }
        node* findleftroot(node *x) const{
            if(x->father==nullptr)return nullptr;
            if(x->father->left==x)return x->father;
            return findleftroot(x->father);
        }
        const_iterator operator++(int) {
            if(pointer==nullptr)throw(invalid_iterator());
            const_iterator t(*this);
            if(pointer->father==nullptr)pointer=findnext(pointer->right);
            else{
                if(pointer->father->left==pointer){
                    if(pointer->right!=nullptr)pointer=findnext(pointer->right);
                    else pointer=pointer->father;
                }
                else{
                    if(pointer->right!=nullptr)pointer=findnext(pointer->right);
                    else pointer=findleftroot(pointer->father);
                }
            }
            return t;
        }
        /**
         * TODO ++iter
         */
        const_iterator & operator++() {
            if(pointer==nullptr)throw(invalid_iterator());
            if(pointer->father==nullptr)pointer=findnext(pointer->right);
            else{
                if(pointer->father->left==pointer){
                    if(pointer->right!=nullptr)pointer=findnext(pointer->right);
                    else pointer=pointer->father;
                }
                else{
                    if(pointer->right!=nullptr)pointer=findnext(pointer->right);
                    else pointer=findleftroot(pointer->father);
                }
            }
            return *this;
        }
        /**
         * TODO iter--
         */
        node* findlast(node *x)const {
            if(x->right==nullptr)return x;
            else return findlast(x->right);
        }
        node* findrightroot(node *x)const {
            if(x->father==nullptr)return nullptr;
            if(x->father->right==x)return x->father;
            return findrightroot(x->father);
        }
        const_iterator operator--(int) {
            if(mp->root==nullptr)throw(invalid_iterator());
            const_iterator t(*this);
            if(pointer==nullptr){
                pointer=mp->findend(mp->root);
                return t;
            }
            if(pointer->father==nullptr){
                if(pointer->left==nullptr)throw(invalid_iterator());
                pointer=findlast(pointer->left);
            }
            else{
                if(pointer==pointer->father->left){
                    if(pointer->left==nullptr){
                        node *ans=findrightroot(pointer->father);
                        if(ans==nullptr)throw(invalid_iterator());
                        pointer=ans;
                    }
                    else pointer=findlast(pointer->left);
                }
                else{
                    if(pointer->left==nullptr)pointer=pointer->father;
                    else pointer=findlast(pointer->left);
                }
            }
            return t;
        }
        /**
         * TODO --iter
         */
        const_iterator & operator--() {
            if(mp->root==nullptr)throw(invalid_iterator());
            if(pointer==nullptr){
                pointer=mp->findend(mp->root);
                return *this;
            }
            if(pointer->father==nullptr){
                if(pointer->left==nullptr)throw(invalid_iterator());
                pointer=findlast(pointer->left);
            }
            else{
                if(pointer==pointer->father->left){
                    if(pointer->left==nullptr){
                        node *ans=findrightroot(pointer->father);
                        if(ans==nullptr)throw(invalid_iterator());
                        pointer=ans;
                        ans=nullptr;
                    }
                    else pointer=findlast(pointer->left);
                }
                else{
                    if(pointer->left==nullptr)pointer=pointer->father;
                    else pointer=findlast(pointer->left);
                }
            }
            return *this;
        }
        /**
         * a operator to check whether two iterators are same (pointing to the same memory).
         */
        bool operator==(const iterator &rhs) const {
            if(mp==rhs.mp && pointer==rhs.pointer)return true;
            else return false;
        }
        bool operator==(const const_iterator &rhs) const {
            if(mp==rhs.mp && pointer==rhs.pointer)return true;
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
        value_type* operator->() const noexcept {
            return &(pointer->value);
        }
        value_type & operator*() const {
            return pointer->value;
        }
	};
	/**
	 * TODO two constructors
	 */
	map() {length=0;root=nullptr;}
	void copytree(node *one,node *two){
		if(two->left!=nullptr){
			one->left=new node(two->left->value);
            one->left->height=two->left->height;
			one->left->father=one;
			copytree(one->left,two->left);
		}
		else one->left=nullptr;
		if(two->right!=nullptr){
			one->right=new node(two->right->value);
            one->right->height=two->right->height;
			one->right->father=one;
			copytree(one->right,two->right);
		}
		else one->right=nullptr;
	}
	void deletetree(node *one){
		if(one->left!=nullptr){
			deletetree(one->left);
			delete one->left;
			one->left=nullptr;
		}
		if(one->right!=nullptr){
			deletetree(one->right);
			delete one->right;
			one->right=nullptr;
		}
	}
	map(const map &other) {
		length=other.length;
		if(other.root!=nullptr){
			root=new node(other.root->value);
            root->height=other.root->height;
			copytree(root,other.root);
		}
		else root=nullptr;
	}
	/**
	 * TODO assignment operator
	 */
	map & operator=(const map &other) {
		if(this == &other) return *this;
		length=other.length;
		if(root!=nullptr){
			deletetree(root);
			delete root;
			root=nullptr;
		}
		if(other.root!=nullptr){
			root=new node(other.root->value);
            root->height=other.root->height;
			copytree(root,other.root);
		}
		else root=nullptr;
		return *this;
	}
	/**
	 * TODO Destructors
	 */
	~map() {
		if(root!=nullptr){
			deletetree(root);
			delete root;
			root=nullptr;
		}
	}
	/**
	 * TODO
	 * access specified element with bounds checking
	 * Returns a reference to the mapped value of the element with key equivalent to key.
	 * If no such element exists, an exception of type `index_out_of_bound'
	 */
	T & search(node *one,const Key &key)const{
		if(!(Compare()(one->value.first,key)) && !(Compare()(key,one->value.first)))return one->value.second;
		if(Compare()(key,one->value.first)){
			if(one->left!=nullptr)return search(one->left,key);
			else throw(index_out_of_bound());
		}
		else{
			if(one->right!=nullptr)return search(one->right,key);
			else throw(index_out_of_bound());
		}
	}
	const T & searchh(node *one,const Key &key) const {
		if(!(Compare()(one->value.first,key)) && !(Compare()(key,one->value.first)))return one->value.second;
		if(Compare()(key,one->value.first)){
			if(one->left!=nullptr)return search(one->left,key);
			else throw(index_out_of_bound());
		}
		else{
			if(one->right!=nullptr)return search(one->right,key);
			else throw(index_out_of_bound());
		}
	}
	T & at(const Key &key) {
		if(root==nullptr)throw(index_out_of_bound());
		return search(root,key);
	}
	const T & at(const Key &key) const {
		if(root==nullptr)throw(index_out_of_bound());
		return searchh(root,key);
	}
	/**
	 * TODO
	 * access specified element 
	 * Returns a reference to the value that is mapped to a key equivalent to key,
	 *   performing an insertion if such key does not already exist.
	 */
	pair<node*,bool> finds(node *x,const Key &key){//1==find 0==not find
		if(x==nullptr){
			value_type temp(key,T());
			root=new node(temp);
			root->height=1;
			return pair<node*,bool>(root,0);
		}
		if(!(Compare()(x->value.first,key)) && !(Compare()(key,x->value.first)))return pair<node*,bool>(x,1);
		if(Compare()(key,x->value.first)){
			if(x->left!=nullptr)return finds(x->left,key);
			else{
				value_type temp(key,T());
				x->left=new node(temp);
				x->left->height=1;
				x->left->father=x;
				return pair<node*,bool>(x->left,0);
			}
		}
		else{
			if(x->right!=nullptr)return finds(x->right,key);
			else{
				value_type temp(key,T());
				x->right=new node(temp);
				x->right->height=1;
				x->right->father=x;
				return pair<node*,bool>(x->right,0);
			}
		}
	}

	void LL(node *x){
		rightrotate(x);
	}
	void RR(node *x){
		leftrotate(x);
	}
	void LR(node *x){
		leftrotate(x->left);
		rightrotate(x);
	}
	void RL(node *x){
		rightrotate(x->right);
		leftrotate(x);
	}
	void balancetree(node *x){
		if(x==nullptr)return;
		getheight(x);
		if(x->left==nullptr && x->right==nullptr && x->father==nullptr)return;
		if(x->left==nullptr && x->right==nullptr){balancetree(x->father);return;}
       if(getheight(x->left)-getheight(x->right) == 2){
           if(getheight(x->left->left)>=getheight(x->left->right))LL(x);
           else LR(x);
           balancetree(x->father->father);
           return;
       }
       if(getheight(x->right)-getheight(x->left) == 2){
           if(getheight(x->right->right)>=getheight(x->right->left))RR(x);
           else RL(x);
           balancetree(x->father->father);
           return;
       }
       balancetree(x->father);
        return;
	}
	T & operator[](const Key &key) {
		pair<node*,bool> ans=finds(root,key);
		if(ans.second==1)return ((ans.first)->value).second;
		length++;
		balancetree(ans.first);
		return (ans.first->value).second;
	}
	/**
	 * behave like at() throw index_out_of_bound if such key does not exist.
	 */
	const T & operator[](const Key &key) const {
        if(root==nullptr)throw(index_out_of_bound());
		return search(root,key);
	}
	/**
	 * return a iterator to the beginning
	 */
	iterator begin() {
		iterator temp;
		temp.mp=this;
		temp.pointer=findbegin(root);
		return temp;
	}
	const_iterator cbegin() const {
		const_iterator temp;
		temp.mp = this;
		temp.pointer = findbegin(root);
		return temp;
	}
	/**
	 * return a iterator to the end
	 * in fact, it returns past-the-end.
	 */
	iterator end() {
		iterator temp;
		temp.mp=this;
		temp.pointer=nullptr;
		return temp;
	}
	const_iterator cend() const {
		const_iterator temp;
		temp.mp=this;
		temp.pointer=nullptr;
		return temp;
	}
	/**
	 * checks whether the container is empty
	 * return true if empty, otherwise false.
	 */
	bool empty() const {
		if(length==0)return true;
		else return false;
	}
	/**
	 * returns the number of elements.
	 */
	size_t size() const {
		return length;
	}
	/**
	 * clears the contents
	 */
	void clear() {
		length=0;
		if(root!=nullptr){
            deletetree(root);
            delete root;
        }
		root=nullptr;
	}
	/**
	 * insert an element.
	 * return a pair, the first of the pair is
	 *   the iterator to the new element (or the element that prevented the insertion), 
	 *   the second one is true if insert successfully, or false.
	 */
	pair<node*,bool> findss(node *x,const value_type &value_){//1==find 0==not find
		if(x==nullptr){
			root=new node(value_);
			root->height=1;
			return pair<node*,bool>(root,0);
		}
		if(!(Compare()(x->value.first,value_.first)) && !(Compare()(value_.first,x->value.first)))return pair<node*,bool>(x,1);
		if(Compare()(value_.first,x->value.first)){
			if(x->left!=nullptr)return findss(x->left,value_);
			else{
				x->left=new node(value_);
				x->left->height=1;
				x->left->father=x;
				return pair<node*,bool>(x->left,0);
			}
		}
		else{
			if(x->right!=nullptr)return findss(x->right,value_);
			else{
				x->right=new node(value_);
				x->right->height=1;
				x->right->father=x;
				return pair<node*,bool>(x->right,0);
			}
		}
	}
	pair<iterator, bool> insert(const value_type &value_) {
		pair<node*,bool> ans=findss(root,value_);
		if(ans.second==1){
			iterator temp;
			temp.mp=this;
			temp.pointer=ans.first;
			return pair<iterator, bool>(temp,0);
		}
		else{
			balancetree(ans.first);
            length++;
			iterator temp;
			temp.mp=this;
			temp.pointer=ans.first;
			return pair<iterator, bool>(temp,1);
		}
	}
	/**
	 * erase the element at pos.
	 *
	 * throw if pos pointed to a bad element (pos == this->end() || pos points an element out of this)
	 */
	node* findright(node *x)const {
		if(x->right==nullptr)return x;
		else return findright(x->right);
	}
	node* findleft(node *x)const {
		if(x->left==nullptr)return x;
		else return findleft(x->left);
	}
    void swaps(node *x,node *y){
        if(x==root)root=y;
        if(x->left==y){
            if(y->left!=nullptr)y->left->father=x;
            if(y->right!=nullptr)y->right->father=x;
            if(x->right!=nullptr)x->right->father=y;
            node *temp=x->right;
            x->left=y->left;
            x->right=y->right;
            y->left=x;
            y->right=temp;
            if(x->father!=nullptr){
                if(x->father->left==x)x->father->left=y;
                else x->father->right=y;
            }
            y->father=x->father;
            x->father=y;
            int tempp=x->height;
            x->height=y->height;
            y->height=tempp;
            return;
        }
        if(x->right==y){
            if(y->left!=nullptr)y->left->father=x;
            if(y->right!=nullptr)y->right->father=x;
            if(x->left!=nullptr)x->left->father=y;
            node *temp=x->left;
            x->left=y->left;
            x->right=y->right;
            y->right=x;
            y->left=temp;
            if(x->father!=nullptr){
                if(x->father->left==x)x->father->left=y;
                else x->father->right=y;
            }
            y->father=x->father;
            x->father=y;
            int tempp=x->height;
            x->height=y->height;
            y->height=tempp;
            return;
        }
        if(x->father!=nullptr){
            if(x->father->left==x)x->father->left=y;
            else x->father->right=y;
        }
        if(y->father!=nullptr){
            if(y->father->left==y)y->father->left=x;
            else y->father->right=x;
        }
        if(x->left!=nullptr)x->left->father=y;
        if(x->right!=nullptr)x->right->father=y;
        if(y->left!=nullptr)y->left->father=x;
        if(y->right!=nullptr)y->right->father=x;
        node *l=x->left,*r=x->right,*f=x->father;
        x->left=y->left;x->right=y->right;x->father=y->father;
        y->left=l;y->right=r;y->father=f;
        int tempp=x->height;
        x->height=y->height;
        y->height=tempp;
        return;
    }
	void erase(iterator pos) {
		if(pos.mp!=this || pos.pointer==nullptr)throw(invalid_iterator());
		length--;
		node *changer;
		if(pos.pointer->left==nullptr && pos.pointer->right==nullptr){
			if(pos.pointer->father!=nullptr){
				changer=pos.pointer->father;
				if(pos.pointer->father->left==pos.pointer)pos.pointer->father->left=nullptr;
				else pos.pointer->father->right=nullptr;
			}
			else{
                delete root;
                root=nullptr;
                pos.pointer=nullptr;
				return;
			}
			delete pos.pointer;
            getheight(changer);
		}
		else if(pos.pointer->left==nullptr){
			node *x=findleft(pos.pointer->right);
			swaps(pos.pointer,x);
			changer=pos.pointer->father;
			if(pos.pointer->father->left==pos.pointer)pos.pointer->father->left=pos.pointer->right;
			else pos.pointer->father->right=pos.pointer->right;
            if(pos.pointer->right!=nullptr)pos.pointer->right->father=pos.pointer->father;
			delete pos.pointer;
			getheight(changer);
		}
		else {
			node *x=findright(pos.pointer->left);
			swaps(pos.pointer,x);
			changer=pos.pointer->father;
			if(pos.pointer->father->left==pos.pointer)pos.pointer->father->left=pos.pointer->left;
			else pos.pointer->father->right=pos.pointer->left;
            if(pos.pointer->left!=nullptr)pos.pointer->left->father=pos.pointer->father;
			delete pos.pointer;
			getheight(changer);
		}
		balancetree(changer);
	}
	/**
	 * Returns the number of elements with key 
	 *   that compares equivalent to the specified argument,
	 *   which is either 1 or 0 
	 *     since this container does not allow duplicates.
	 * The default method of check the equivalence is !(a < b || b > a)
	 */
	bool searchn(node *one,const Key &key) const {
		if(!(Compare()(one->value.first,key)) && !(Compare()(key,one->value.first)))return true;
		if(Compare()(key,one->value.first)){
			if(one->left!=nullptr)return searchn(one->left,key);
			else return false;
		}
		else{
			if(one->right!=nullptr)return searchn(one->right,key);
			else return false;
		}
	}
	size_t count(const Key &key) const {
		if(root==nullptr)return 0;
		return searchn(root,key);
	}
	/**
	 * Finds an element with key equivalent to key.
	 * key value of the element to search for.
	 * Iterator to an element with key equivalent to key.
	 *   If no such element is found, past-the-end (see end()) iterator is returned.
	 */
	node* searchnode(node *one,const Key &key)const{
		if(one==nullptr)return nullptr;
		if(!(Compare()(one->value.first,key)) && !(Compare()(key,one->value.first)))return one;
		if(Compare()(key,one->value.first)){
			if(one->left!=nullptr)return searchnode(one->left,key);
			else return nullptr;
		}
		else{
			if(one->right!=nullptr)return searchnode(one->right,key);
			else return nullptr;
		}
	}
	iterator find(const Key &key) {
		iterator ans;
		ans.mp=this;
		ans.pointer=searchnode(root,key);
		return ans;
	}
	const_iterator find(const Key &key) const {
		const_iterator ans;
		ans.mp=this;
		ans.pointer=searchnode(root,key);
		return ans;
	}
};
template<typename T>
struct my_type_traits{
	const static bool value=T::iterator_assignable==right ? 1 : 0;
};
}

#endif
