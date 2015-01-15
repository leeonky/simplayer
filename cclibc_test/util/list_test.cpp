#include "cclibc_test/util/list_test.h"
#include "cclibc/util/list.h"

using CCLIBC::dual_list;
using CCLIBC::single_list;


class mynode: public dual_list<mynode>::node{
public:
};

void list_test_class::empty_node() {
	mynode n;
	CPPUNIT_ASSERT(!n.next());
	CPPUNIT_ASSERT(!n.previous());
#ifndef NDEBUG
	CPPUNIT_ASSERT(!n.list());
#endif
}

void list_test_class::empty_list() {
	dual_list<mynode> l;
	CPPUNIT_ASSERT(!l.size());
	CPPUNIT_ASSERT(!l.first());
	CPPUNIT_ASSERT(!l.last());
}

void list_test_class::add_to_empty() {
	dual_list<mynode> l;
	mynode n;
	l.add(&n);
	CPPUNIT_ASSERT(l.size() == 1);
	CPPUNIT_ASSERT(l.first() == &n);
	CPPUNIT_ASSERT(l.last() == &n);
	CPPUNIT_ASSERT(!n.next());
	CPPUNIT_ASSERT(!n.previous());
#ifndef NDEBUG
	CPPUNIT_ASSERT(n.list() == &l);
#endif
}

void list_test_class::add_to_nonempty() {
	dual_list<mynode> l;
	mynode n1, n2;
	l.add(&n1);
	l.add(&n2);
	
	CPPUNIT_ASSERT(l.size() == 2);
	CPPUNIT_ASSERT(l.first() == &n1);
	CPPUNIT_ASSERT(l.last() == &n2);

	CPPUNIT_ASSERT(!n1.previous());
	CPPUNIT_ASSERT(n1.next() == &n2);
#ifndef NDEBUG
	CPPUNIT_ASSERT(n1.list() == &l);
#endif
	CPPUNIT_ASSERT(n2.previous() == &n1);
	CPPUNIT_ASSERT(!n2.next());
#ifndef NDEBUG
	CPPUNIT_ASSERT(n2.list() == &l);
#endif
}

void list_test_class::remove_only() {
	dual_list<mynode> l;
	mynode n;
	l.add(&n);

	CPPUNIT_ASSERT(l.remove(&n) == &n);
	CPPUNIT_ASSERT(!l.size());
	CPPUNIT_ASSERT(!l.first());
	CPPUNIT_ASSERT(!l.last());

	CPPUNIT_ASSERT(!n.next());
	CPPUNIT_ASSERT(!n.previous());
#ifndef NDEBUG
	CPPUNIT_ASSERT(!n.list());
#endif
}

void list_test_class::remove__first() {
	dual_list<mynode> l;
	mynode n1,n2,n3;
	l.add(&n1);
	l.add(&n2);
	l.add(&n3);

	CPPUNIT_ASSERT(l.remove(&n1) == &n1);

	CPPUNIT_ASSERT(l.size() == 2);
	CPPUNIT_ASSERT(l.first() == &n2);
	CPPUNIT_ASSERT(l.last() == &n3);

	CPPUNIT_ASSERT(!n1.next());
	CPPUNIT_ASSERT(!n1.previous());
#ifndef NDEBUG
	CPPUNIT_ASSERT(!n1.list());
#endif

	CPPUNIT_ASSERT(!n2.previous());
	CPPUNIT_ASSERT(n2.next() == &n3);
#ifndef NDEBUG
	CPPUNIT_ASSERT(n2.list() == &l);
#endif

	CPPUNIT_ASSERT(n3.previous() == &n2);
	CPPUNIT_ASSERT(!n3.next());
#ifndef NDEBUG
	CPPUNIT_ASSERT(n3.list() == &l);
#endif
}

void list_test_class::remove_middle() {
	dual_list<mynode> l;
	mynode n1,n2,n3;
	l.add(&n1);
	l.add(&n2);
	l.add(&n3);

	CPPUNIT_ASSERT(l.remove(&n2) == &n2);

	CPPUNIT_ASSERT(l.size() == 2);
	CPPUNIT_ASSERT(l.first() == &n1);
	CPPUNIT_ASSERT(l.last() == &n3);

	CPPUNIT_ASSERT(!n2.next());
	CPPUNIT_ASSERT(!n2.previous());
#ifndef NDEBUG
	CPPUNIT_ASSERT(!n2.list());
#endif

	CPPUNIT_ASSERT(!n1.previous());
	CPPUNIT_ASSERT(n1.next() == &n3);
#ifndef NDEBUG
	CPPUNIT_ASSERT(n1.list() == &l);
#endif

	CPPUNIT_ASSERT(n3.previous() == &n1);
	CPPUNIT_ASSERT(!n3.next());
#ifndef NDEBUG
	CPPUNIT_ASSERT(n3.list() == &l);
#endif

}
void list_test_class::remove_last() {
	dual_list<mynode> l;
	mynode n1,n2,n3;
	l.add(&n1);	l.add(&n2);
	l.add(&n3);

	CPPUNIT_ASSERT(l.remove(&n3) == &n3);

	CPPUNIT_ASSERT(l.size() == 2);
	CPPUNIT_ASSERT(l.first() == &n1);
	CPPUNIT_ASSERT(l.last() == &n2);

	CPPUNIT_ASSERT(!n3.next());
	CPPUNIT_ASSERT(!n3.previous());
#ifndef NDEBUG
	CPPUNIT_ASSERT(!n3.list());
#endif

	CPPUNIT_ASSERT(!n1.previous());
	CPPUNIT_ASSERT(n1.next() == &n2);
#ifndef NDEBUG
	CPPUNIT_ASSERT(n1.list() == &l);
#endif

	CPPUNIT_ASSERT(n2.previous() == &n1);
	CPPUNIT_ASSERT(!n2.next());
#ifndef NDEBUG
	CPPUNIT_ASSERT(n2.list() == &l);
#endif

}

void list_test_class::insert_before_empty(){
	dual_list<mynode> l;
	mynode n;
	l.insert_before(&n, NULL);
	CPPUNIT_ASSERT(l.size() == 1);
	CPPUNIT_ASSERT(l.first() == &n);
	CPPUNIT_ASSERT(l.last() == &n);
	CPPUNIT_ASSERT(!n.next());
	CPPUNIT_ASSERT(!n.previous());
#ifndef NDEBUG
	CPPUNIT_ASSERT(n.list() == &l);
#endif
}

void list_test_class::insert_before_null() {
	dual_list<mynode> l;
	mynode n1,n;
	l.add(&n1);
	l.insert_before(&n, NULL);
	CPPUNIT_ASSERT(l.size() == 2);
	CPPUNIT_ASSERT(l.first() == &n1);
	CPPUNIT_ASSERT(l.last() == &n);

	CPPUNIT_ASSERT(!n.next());
	CPPUNIT_ASSERT(n.previous() == &n1);
#ifndef NDEBUG
	CPPUNIT_ASSERT(n.list() == &l);
#endif

	CPPUNIT_ASSERT(n1.next() == &n);
	CPPUNIT_ASSERT(!n1.previous());
#ifndef NDEBUG
	CPPUNIT_ASSERT(n1.list() == &l);
#endif
}

void list_test_class::insert_before_first() {
	dual_list<mynode> l;
	mynode n1,n;
	l.add(&n1);
	l.insert_before(&n, &n1);
	CPPUNIT_ASSERT(l.size() == 2);
	CPPUNIT_ASSERT(l.first() == &n);
	CPPUNIT_ASSERT(l.last() == &n1);

	CPPUNIT_ASSERT(n.next() == &n1);
	CPPUNIT_ASSERT(!n.previous());
#ifndef NDEBUG
	CPPUNIT_ASSERT(n.list() == &l);
#endif

	CPPUNIT_ASSERT(!n1.next());
	CPPUNIT_ASSERT(n1.previous() == &n);
#ifndef NDEBUG
	CPPUNIT_ASSERT(n1.list() == &l);
#endif
}

void list_test_class::insert_before_mid() {
	dual_list<mynode> l;
	mynode n1,n2,n;
	l.add(&n1);
	l.add(&n2);
	l.insert_before(&n, &n2);
	CPPUNIT_ASSERT(l.size() == 3);
	CPPUNIT_ASSERT(l.first() == &n1);
	CPPUNIT_ASSERT(l.last() == &n2);

	CPPUNIT_ASSERT(n.next() == &n2);
	CPPUNIT_ASSERT(n.previous() == &n1);
#ifndef NDEBUG
	CPPUNIT_ASSERT(n.list() == &l);
#endif

	CPPUNIT_ASSERT(n1.next() == &n);
	CPPUNIT_ASSERT(!n1.previous());
#ifndef NDEBUG
	CPPUNIT_ASSERT(n1.list() == &l);
#endif

	CPPUNIT_ASSERT(!n2.next());
	CPPUNIT_ASSERT(n2.previous() == &n);
#ifndef NDEBUG
	CPPUNIT_ASSERT(n2.list() == &l);
#endif
}

void list_test_class::list_foreach(){
	dual_list<mynode> l;
	mynode ns[3];
	for(size_t i=0;i<3;i++){
		l.add(ns+i);
	}
	size_t k=0;
	LIST_FOREACH(mynode, n, l){
		CPPUNIT_ASSERT(n == ns+k);
		++k;
	}
}

void list_test_class::list_r_foreach() {
	dual_list<mynode> l;
	mynode ns[3];
	for(size_t i=0;i<3;i++){
		l.add(ns+i);
	}
	size_t k=2;
	LIST_FOREACH_R(mynode, n, l){
		CPPUNIT_ASSERT(n == ns+k);
		--k;
	}
}

void list_test_class::insert_after_empty(){
	dual_list<mynode> l;
	mynode n;
	l.insert_after(&n, NULL);
	CPPUNIT_ASSERT(l.size() == 1);
	CPPUNIT_ASSERT(l.first() == &n);
	CPPUNIT_ASSERT(l.last() == &n);
	CPPUNIT_ASSERT(!n.next());
	CPPUNIT_ASSERT(!n.previous());
#ifndef NDEBUG
	CPPUNIT_ASSERT(n.list() == &l);
#endif
}

void list_test_class::insert_after_null(){
	dual_list<mynode> l;
	mynode n1,n;
	l.add(&n1);
	l.insert_after(&n, NULL);
	CPPUNIT_ASSERT(l.size() == 2);
	CPPUNIT_ASSERT(l.first() == &n);
	CPPUNIT_ASSERT(l.last() == &n1);

	CPPUNIT_ASSERT(n.next() == &n1);
	CPPUNIT_ASSERT(!n.previous());
#ifndef NDEBUG
	CPPUNIT_ASSERT(n.list() == &l);
#endif

	CPPUNIT_ASSERT(!n1.next());
	CPPUNIT_ASSERT(n1.previous() == &n);
#ifndef NDEBUG
	CPPUNIT_ASSERT(n1.list() == &l);
#endif
}

void list_test_class::insert_after_last(){
	dual_list<mynode> l;
	mynode n1,n;
	l.add(&n1);
	l.insert_after(&n, &n1);
	CPPUNIT_ASSERT(l.size() == 2);
	CPPUNIT_ASSERT(l.first() == &n1);
	CPPUNIT_ASSERT(l.last() == &n);

	CPPUNIT_ASSERT(!n.next());
	CPPUNIT_ASSERT(n.previous() == &n1);
#ifndef NDEBUG
	CPPUNIT_ASSERT(n.list() == &l);
#endif

	CPPUNIT_ASSERT(n1.next() == &n);
	CPPUNIT_ASSERT(!n1.previous());
#ifndef NDEBUG
	CPPUNIT_ASSERT(n1.list() == &l);
#endif
}

void list_test_class::insert_after_mid(){
	dual_list<mynode> l;
	mynode n1,n2,n;
	l.add(&n1);
	l.add(&n2);
	l.insert_after(&n, &n1);
	CPPUNIT_ASSERT(l.size() == 3);
	CPPUNIT_ASSERT(l.first() == &n1);
	CPPUNIT_ASSERT(l.last() == &n2);

	CPPUNIT_ASSERT(n.next() == &n2);
	CPPUNIT_ASSERT(n.previous() == &n1);
#ifndef NDEBUG
	CPPUNIT_ASSERT(n.list() == &l);
#endif

	CPPUNIT_ASSERT(n1.next() == &n);
	CPPUNIT_ASSERT(!n1.previous());
#ifndef NDEBUG
	CPPUNIT_ASSERT(n1.list() == &l);
#endif

	CPPUNIT_ASSERT(!n2.next());
	CPPUNIT_ASSERT(n2.previous() == &n);
#ifndef NDEBUG
	CPPUNIT_ASSERT(n2.list() == &l);
#endif
}

void list_test_class::list_clear(){
	dual_list<mynode> l;
	mynode n1, n2;
	l.add(&n1);
	l.add(&n2);
	l.clear();
	CPPUNIT_ASSERT(!l.first());
	CPPUNIT_ASSERT(!l.last());
	CPPUNIT_ASSERT(!l.size());
	
	CPPUNIT_ASSERT(!n1.next());
	CPPUNIT_ASSERT(!n1.previous());
	
	CPPUNIT_ASSERT(!n2.next());
	CPPUNIT_ASSERT(!n2.previous());
	
#ifndef NDEBUG
	CPPUNIT_ASSERT(!n1.list());
	CPPUNIT_ASSERT(!n2.list());
#endif
}

void list_test_class::list_discard(){
	dual_list<mynode> l;
	mynode n1, n2;
	l.add(&n1);
	l.add(&n2);
	l.discard();
	CPPUNIT_ASSERT(!l.first());
	CPPUNIT_ASSERT(!l.last());
	CPPUNIT_ASSERT(!l.size());
	
	CPPUNIT_ASSERT(n1.next() == &n2);
	CPPUNIT_ASSERT(!n1.previous());
	
	CPPUNIT_ASSERT(!n2.next());
	CPPUNIT_ASSERT(n2.previous() == &n1);
	
#ifndef NDEBUG
	CPPUNIT_ASSERT(!n1.list());
	CPPUNIT_ASSERT(!n2.list());
#endif
}

class snode: public single_list<snode>::node {
};

void list_test_class::s_empty_node() {
	snode n;
	CPPUNIT_ASSERT(!n.next());
#ifndef NDEBUG
	CPPUNIT_ASSERT(!n.list());
#endif
}

void list_test_class::s_empty_list(){
	single_list<snode> l;
	CPPUNIT_ASSERT(!l.first());
	CPPUNIT_ASSERT(!l.last());
	CPPUNIT_ASSERT(!l.size());
}

void list_test_class::s_add_to_empty(){
	single_list<snode> l;
	snode n;
	l.add(&n);

	CPPUNIT_ASSERT(1 == l.size());
	CPPUNIT_ASSERT(l.first() == &n);
	CPPUNIT_ASSERT(l.last() == &n);

	CPPUNIT_ASSERT(!n.next());
#ifndef NDEBUG
	CPPUNIT_ASSERT(n.list() == &l);
#endif
}

void list_test_class::s_add_to_list() {
	single_list<snode> l;
	snode n1,n;
	l.add(&n1);
	l.add(&n);

	CPPUNIT_ASSERT(2 == l.size());
	CPPUNIT_ASSERT(l.first() == &n1);
	CPPUNIT_ASSERT(l.last() == &n);

	CPPUNIT_ASSERT(n1.next() == &n);
	CPPUNIT_ASSERT(!n.next());
#ifndef NDEBUG
	CPPUNIT_ASSERT(n.list() == &l);
	CPPUNIT_ASSERT(n1.list() == &l);
#endif
}

void list_test_class::s_remove_only() {
	single_list<snode> l;
	snode n;
	l.add(&n);
	CPPUNIT_ASSERT(&n == l.remove(&n));

	CPPUNIT_ASSERT(!n.next());
#ifndef NDEBUG
	CPPUNIT_ASSERT(!n.list());
#endif

	CPPUNIT_ASSERT(!l.first());
	CPPUNIT_ASSERT(!l.last());
	CPPUNIT_ASSERT(!l.size());
}

void list_test_class::s_remove_first() {
	single_list<snode> l;
	snode n1,n;
	l.add(&n);
	l.add(&n1);
	CPPUNIT_ASSERT(l.remove(&n) == &n);
	
	CPPUNIT_ASSERT(!n.next());
#ifndef NDEBUG
	CPPUNIT_ASSERT(!n.list());
#endif

	CPPUNIT_ASSERT(1 == l.size());
	CPPUNIT_ASSERT(l.first() == &n1);
	CPPUNIT_ASSERT(l.last() == &n1);

	CPPUNIT_ASSERT(!n1.next());
#ifndef NDEBUG
	CPPUNIT_ASSERT(n1.list() == &l);
#endif

}

void list_test_class::s_remove_other(){
	single_list<snode> l;
	snode n1, n2, n3;
	l.add(&n1);
	l.add(&n2);
	l.add(&n3);

	CPPUNIT_ASSERT(l.remove(&n2) == &n2);

	CPPUNIT_ASSERT(!n2.next());
#ifndef NDEBUG
	CPPUNIT_ASSERT(!n2.list());
#endif

	CPPUNIT_ASSERT(n1.next() == &n3);
#ifndef NDEBUG
	CPPUNIT_ASSERT(n1.list() == &l);
#endif

	CPPUNIT_ASSERT(!n3.next());
#ifndef NDEBUG
	CPPUNIT_ASSERT(n3.list() == &l);
#endif

	CPPUNIT_ASSERT(l.size() == 2);
	CPPUNIT_ASSERT(l.first() == &n1);
	CPPUNIT_ASSERT(l.last() == &n3);
}

void list_test_class::s_remove_last() {
	single_list<snode> l;
	snode n1,n;
	l.add(&n1);
	l.add(&n);
	CPPUNIT_ASSERT(l.remove(&n) == &n);
	
	CPPUNIT_ASSERT(!n.next());
#ifndef NDEBUG
	CPPUNIT_ASSERT(!n.list());
#endif

	CPPUNIT_ASSERT(1 == l.size());
	CPPUNIT_ASSERT(l.first() == &n1);
	CPPUNIT_ASSERT(l.last() == &n1);

	CPPUNIT_ASSERT(!n1.next());
#ifndef NDEBUG
	CPPUNIT_ASSERT(n1.list() == &l);
#endif
}

void list_test_class::s_remove_first_only() {
	single_list<snode> l;
	snode n;
	l.add(&n);
	CPPUNIT_ASSERT(&n == l.remove_first());

	CPPUNIT_ASSERT(!n.next());
#ifndef NDEBUG
	CPPUNIT_ASSERT(!n.list());
#endif

	CPPUNIT_ASSERT(!l.first());
	CPPUNIT_ASSERT(!l.last());
	CPPUNIT_ASSERT(!l.size());
}

void list_test_class::s_remove_first_other() {
	single_list<snode> l;
	snode n1,n;
	l.add(&n);
	l.add(&n1);
	CPPUNIT_ASSERT(l.remove_first() == &n);
	
	CPPUNIT_ASSERT(!n.next());
#ifndef NDEBUG
	CPPUNIT_ASSERT(!n.list());
#endif

	CPPUNIT_ASSERT(1 == l.size());
	CPPUNIT_ASSERT(l.first() == &n1);
	CPPUNIT_ASSERT(l.last() == &n1);

	CPPUNIT_ASSERT(!n1.next());
#ifndef NDEBUG
	CPPUNIT_ASSERT(n1.list() == &l);
#endif

}

void list_test_class::s_list_foreach() {
	single_list<snode> l;
	snode ns[3];
	l.add(ns);
	l.add(ns+1);
	l.add(ns+2);

	size_t i=0;
	LIST_FOREACH(snode, pn, l){
		CPPUNIT_ASSERT(pn == ns+i);
		i++;
	}
}

void list_test_class::s_remove2_only(){
	single_list<snode> l;
	snode n;
	l.add(&n);

	CPPUNIT_ASSERT(l.remove_next(NULL) == &n);
	CPPUNIT_ASSERT(!n.next());
#ifndef NDEBUG
	CPPUNIT_ASSERT(!n.list());
#endif

	CPPUNIT_ASSERT(!l.first());
	CPPUNIT_ASSERT(!l.last());
	CPPUNIT_ASSERT(!l.size());
}

void list_test_class::s_remove2_first(){
	single_list<snode> l;
	snode n1,n;
	l.add(&n);
	l.add(&n1);
	CPPUNIT_ASSERT(l.remove_next(NULL) == &n);
	
	CPPUNIT_ASSERT(!n.next());
#ifndef NDEBUG
	CPPUNIT_ASSERT(!n.list());
#endif

	CPPUNIT_ASSERT(1 == l.size());
	CPPUNIT_ASSERT(l.first() == &n1);
	CPPUNIT_ASSERT(l.last() == &n1);

	CPPUNIT_ASSERT(!n1.next());
#ifndef NDEBUG
	CPPUNIT_ASSERT(n1.list() == &l);
#endif
}

void list_test_class::s_remove2_other(){
	single_list<snode> l;
	snode n1, n2, n3;
	l.add(&n1);
	l.add(&n2);
	l.add(&n3);

	CPPUNIT_ASSERT(l.remove_next(&n1) == &n2);

	CPPUNIT_ASSERT(!n2.next());
#ifndef NDEBUG
	CPPUNIT_ASSERT(!n2.list());
#endif

	CPPUNIT_ASSERT(n1.next() == &n3);
#ifndef NDEBUG
	CPPUNIT_ASSERT(n1.list() == &l);
#endif

	CPPUNIT_ASSERT(!n3.next());
#ifndef NDEBUG
	CPPUNIT_ASSERT(n3.list() == &l);
#endif

	CPPUNIT_ASSERT(l.size() == 2);
	CPPUNIT_ASSERT(l.first() == &n1);
	CPPUNIT_ASSERT(l.last() == &n3);
}

void list_test_class::s_remove2_last(){
	single_list<snode> l;
	snode n1,n;
	l.add(&n1);
	l.add(&n);
	CPPUNIT_ASSERT(l.remove_next(&n1) == &n);
	
	CPPUNIT_ASSERT(!n.next());
#ifndef NDEBUG
	CPPUNIT_ASSERT(!n.list());
#endif

	CPPUNIT_ASSERT(1 == l.size());
	CPPUNIT_ASSERT(l.first() == &n1);
	CPPUNIT_ASSERT(l.last() == &n1);

	CPPUNIT_ASSERT(!n1.next());
#ifndef NDEBUG
	CPPUNIT_ASSERT(n1.list() == &l);
#endif
}

void list_test_class::s_foreach2(){
	single_list<snode> l;
	snode ns[3];
	l.add(ns);
	l.add(ns+1);
	l.add(ns+2);

	size_t i=0;
	SLIST_FOREACH_NOLAST(snode, p1, p2, l){
		CPPUNIT_ASSERT(p2 == ns+i);
		i++;
	}
}

void list_test_class::replace_only(){
	dual_list<mynode> l;
	mynode n, n1;
	l.add(&n);
	mynode *p = l.replace(&n, &n1);
	CPPUNIT_ASSERT(&n == p);
	CPPUNIT_ASSERT(!n.next());
	CPPUNIT_ASSERT(!n.previous());

	CPPUNIT_ASSERT(!n1.next());
	CPPUNIT_ASSERT(!n1.previous());
	CPPUNIT_ASSERT(l.first() == &n1);
	CPPUNIT_ASSERT(l.last() == &n1);
	CPPUNIT_ASSERT(1 == l.size());
}

void list_test_class::replace_other(){
	dual_list<mynode> l;
	mynode n1, n2, n3, n4;
	l.add(&n1);
	l.add(&n2);
	l.add(&n3);
	mynode *p = l.replace(&n2, &n4);
	CPPUNIT_ASSERT(&n2 == p);
	CPPUNIT_ASSERT(!n2.next());
	CPPUNIT_ASSERT(!n2.previous());

	CPPUNIT_ASSERT(n4.next() == &n3);
	CPPUNIT_ASSERT(n4.previous() == &n1);
	CPPUNIT_ASSERT(n3.previous() == &n4);
	CPPUNIT_ASSERT(n1.next() == &n4);
}

void list_test_class::s_list_clear(){
	single_list<snode> l;
	snode n1,n2;
	l.add(&n1);
	l.add(&n2);
	l.clear();
	
	CPPUNIT_ASSERT(!l.first());
	CPPUNIT_ASSERT(!l.last());
	CPPUNIT_ASSERT(!l.size());
	
	CPPUNIT_ASSERT(!n1.next());
	
	CPPUNIT_ASSERT(!n2.next());
	
#ifndef NDEBUG
	CPPUNIT_ASSERT(!n1.list());
	CPPUNIT_ASSERT(!n2.list());
#endif
}

void list_test_class::slist_discard(){
	single_list<snode> l;
	snode n1,n2;
	l.add(&n1);
	l.add(&n2);
	l.discard();
	
	CPPUNIT_ASSERT(!l.first());
	CPPUNIT_ASSERT(!l.last());
	CPPUNIT_ASSERT(!l.size());
	
	CPPUNIT_ASSERT(n1.next() == &n2);
	CPPUNIT_ASSERT(!n2.next());
	
#ifndef NDEBUG
	CPPUNIT_ASSERT(!n1.list());
	CPPUNIT_ASSERT(!n2.list());
#endif
}

void list_test_class::list_c_foreach(){
	dual_list<mynode> l;
	mynode ns[2];
	l.add(ns);
	l.add(ns+1);
	const dual_list<mynode> &rl = l;
	int j = 0;
	LIST_FOREACH(const mynode, n, rl){
		CPPUNIT_ASSERT(n == ns+j);
		++j;
	}
}

void list_test_class::list_c_rforeach(){
	dual_list<mynode> l;
	mynode ns[2];
	l.add(ns);
	l.add(ns+1);
	const dual_list<mynode> &rl = l;
	int j = 1;
	LIST_FOREACH_R(const mynode, n, rl){
		CPPUNIT_ASSERT(n == ns+j);
		--j;
	}
}

void list_test_class::slist_c_foreach(){
	single_list<snode> l;
	snode ns[2];
	l.add(ns);
	l.add(ns+1);
	const single_list<snode> &rl = l;
	int j = 0;
	LIST_FOREACH(const snode, n, rl){
		CPPUNIT_ASSERT(n == ns+j);
		++j;
	}
}

namespace{
	int des = 0;
	class testnode: public dual_list<testnode>::node{
	public:
		~testnode(){
			++des;
		}
	};
}
