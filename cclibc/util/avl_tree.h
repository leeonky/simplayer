#ifndef CCLIBC_UTIL_AVL_TREE_H_
#define CCLIBC_UTIL_AVL_TREE_H_

#include "cclibc/env.h"
#include "cclibc/util/list.h"

#ifdef STACK_TRACE_FLAG

#define find(...) find(INVOKE_TRACE, ##__VA_ARGS__)

#endif

CCLIBC_BG

template<typename T>
class comparer{
public:
	int compare(const T &v1, const T &v2) const{
		if(v1 < v2){
			return -1;
		}else if(v1 == v2){
			return 0;
		}else{
			return 1;
		}
	}
};

template<>
class comparer<const char*>{
public:
	int compare(const char *str1, const char *str2) const{
		return strcmp(str1, str2);
	}
};

template<>
class comparer<char*>{
public:
	int compare(const char *str1, const char *str2) const{
		return strcmp(str1, str2);
	}
};

template<typename V, typename P, typename C = comparer<V> >
class avl_tree{
public:
	class node: public dual_list<node>::node{
	friend class avl_tree;
	private:
		const V _v;
		P _p;
		node *_left, *_right, *_father;
		int _bf;
	public:
		node(const V &v, const P &p): _v(v), _p(p), _left(NULL), _right(NULL), _father(NULL), _bf(0){}

		const V& value() const{
			return _v;
		}

		const P& payload() const{
			return _p;
		}

		P& payload(){
			return _p;
		}

		node* left() const{
			return _left;
		}

		node* right() const {
			return _right;
		}

		node* father() const {
			return _father;
		}

		int balance_factor() const {
			return _bf;
		}
	};
	
	private:
		dual_list<node> _list;
		node *_root;
		C _cmp;
		
		node* update_balance_2(node *n) {
			node *nr;
			int bf;
			bf = n->_left->_bf;
			if(-1 == bf){
				nr = n->_left->_right;
				n->_left->_father = nr;
				n->_left->_right = nr->_left;
				if(nr->_left){
					nr->_left->_father = n->_left;
				}
				if(nr->_bf == 1){
					n->_left->_bf = 0;
					n->_bf = -1;
				}else if(nr->_bf == -1){
					n->_left->_bf = 1;
					n->_bf = 0;
				}else{
					n->_left->_bf = 0;
					n->_bf = 0;
				}
				nr->_left = n->_left;
				n->_left = nr->_right;
				nr->_bf = 0;
			}else{
				nr = n->_left;
				n->_left = nr->_right;
				if(bf == 1){
					nr->_bf = 0;
					n->_bf = 0;
				}else if(bf == 0){
					nr->_bf = -1;
					n->_bf = 1;
				}
			}
			if(nr->_right){
				nr->_right->_father = n;
			}
			nr->_right = n;
			nr->_father = n->_father;
			n->_father = nr;
			if(nr->_father){
				if(nr->_father->_left == n){
					nr->_father->_left = nr;
				}else{
					nr->_father->_right = nr;
				}
			}
			if(_root == n){
				_root = nr;
			}
			return nr;
		}

		node* update_balance_n2(node *n) {
			node *nr;
			int bf;
			bf = n->_right->_bf;
			if(1 == bf){
				nr = n->_right->_left;
				n->_right->_father = nr;
				n->_right->_left = nr->_right;
				if(nr->_right){
					nr->_right->_father = n->_right;
				}
				if(nr->_bf == -1){
					n->_right->_bf = 0;
					n->_bf = 1;
				}else if(nr->_bf == 1){
					n->_right->_bf = -1;
					n->_bf = 0;
				}else{
					n->_right->_bf = 0;
					n->_bf = 0;
				}
				nr->_right = n->_right;
				n->_right = nr->_left;
				nr->_bf = 0;
			}else{
				nr = n->_right;
				n->_right = nr->_left;
				if(-1 == bf){
					nr->_bf = 0;
					n->_bf = 0;
				}else{
					nr->_bf = 1;
					n->_bf = -1;
				}
			}
			if(nr->_left){
				nr->_left->_father = n;
			}
			nr->_left = n;
			nr->_father = n->_father;
			n->_father = nr;
			if(nr->_father){
				if(nr->_father->_left == n){
					nr->_father->_left = nr;
				}else{
					nr->_father->_right = nr;
				}
			}
			if(_root == n){
				_root = nr;
			}
			return nr;
		}
		
		void swap_node(node *fn, node *tn){
			if(fn == _root){
				_root = tn;
			}
			std::swap(fn->_bf, tn->_bf);
			if(fn->_left != tn){
				std::swap(fn->_left, tn->_left);
				if(tn->_left){
					tn->_left->_father = tn;
				}	
			}else{
				fn->_left = tn->_left;
				tn->_left = fn;
			}
			if(fn->_right != tn){
				std::swap(fn->_right, tn->_right);
				if(tn->_right){
					tn->_right->_father = tn;
				}
			}else{
				fn->_right = tn->_right;
				tn->_right = fn;
			}
			if(tn->_left == fn || tn->_right == fn){
				tn->_father = fn->_father;
				fn->_father = tn;
			}else{
				std::swap(fn->_father, tn->_father);
				if(fn->_father){
					if(fn->_father->_left == tn){
						fn->_father->_left = fn;
					}else{
						fn->_father->_right = fn;
					}
				}
			}
			if(tn->_father){
				if(tn->_father->_left == fn){
					tn->_father->_left = tn;
				}else{
					tn->_father->_right = tn;
				}
			}
			if(fn->_left){
				fn->_left->_father = fn;
			}
			if(fn->_right){
				fn->_right->_father = fn;
			}
		}
		
	public:
		avl_tree(): _root(NULL){}

		size_t size() const{
			return _list.size();
		}

		node* first(){
			return _list.first();
		}

		const node* first() const{
			return _list.first();
		}

		node* last() {
			return _list.last();
		}

		const node* last() const{
			return _list.last();
		}

		node* root() {
			return _root;
		}

		const node* root() const{
			return _root;
		}

		void add FUNCTION_PARAMETER_LIST(node *r, int c, node *n){
			if(r){
				if(c<0){
					_list.insert_before(n, r);
					r->_left = n;
					n->_father = r;
				}else if(c>0){
					_list.insert_after(n, r);
					r->_right = n;
					n->_father = r;
				}
				do{
					if(n == r->_left){
						++(r->_bf);
					}else{
						--(r->_bf);
					}
					if(2 == r->_bf){
						update_balance_2(r);
					}else if(-2 == r->_bf){
						update_balance_n2(r);
					}else if(r->_bf){
						n = r;
						r = r->_father;
						continue;
					}
					break;
				}while(r);
			}else{
				_list.add(n);
				_root = n;
			}
		}
		
		int find FUNCTION_PARAMETER_LIST(node *&n, const V &v) {
			return const_cast<const avl_tree*>(this)->find(const_cast<const node *&>(n), v);
		}
		
		int find FUNCTION_PARAMETER_LIST(const node *&n, const V &v) const{
			int c;
			n = _root;
			if(n){
				for(;;){
					c = _cmp.compare(v, n->value());
					if(c < 0 && n->_left) {
						n = n->_left;
					}else if(c > 0 && n->_right) {
						n = n->_right;
					}else{
						break;
					}
				}
			}
			return c;
		}

		node* remove FUNCTION_PARAMETER_LIST(const V &v){
			node *res = _root;
			while(res){
				int c = _cmp.compare(v, res->value());
				if(c < 0) {
					res = res->_left;
				}else if(c > 0) {
					res = res->_right;
				}else{
					break;
				}
			}
			if(res){
				if(-1 == res->_bf){
					swap_node(res, res->next());
					while(res->_right){
						swap_node(res, res->_right);
					}
				}
				else if(res->_left){ // 1 == res->_bf || (0 == res->_bf && NULL!= res->_left)
					swap_node(res, res->previous());
					while(res->_left){
						swap_node(res, res->_left);
					}
				}
				node *n = res;
				if(node *r = n->_father){
					do{
						if(r->_left == n){
							--(r->_bf);
						}else{
							++(r->_bf);
						}
					}while(((2 == r->_bf && !(n = update_balance_2(r))->_bf)
							||(-2 == r->_bf && !(n = update_balance_n2(r))->_bf)
							||(!(n = r)->_bf)) && (r = n->_father));
					if(res->_father->_left == res){
						res->_father->_left = NULL;
					}else{
						res->_father->_right = NULL;
					}
					res->_father = NULL;
				}else{
					_root = NULL;
				}
				_list.remove(res);
			}
			return res;
		}

		void clear(){
			LIST_FOREACH(node, n, _list){
				n->_left = n->_right = n->_father = NULL;
				n->_bf = 0;
			}
			_root = NULL;
			_list.clear();
		}

		void discard(){
			LIST_FOREACH(node, n, _list){
				n->_left = n->_right = n->_father = NULL;
				n->_bf = 0;
			}
			_root = NULL;
			_list.discard();
		}
};

CCLIBC_END

#endif //CCLIBC_UTIL_AVL_TREE_H_
