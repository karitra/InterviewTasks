//
// Copyright 2013 Alex Karev 
//
// Interview task:
//
//  In sequnce of continues discrete values (without gaps) find missing two values. 
//
#include <cassert>
#include <stack>
#include <iostream>
#include <algorithm>
#include <iterator>

#define NELS(a) (sizeof(a) / sizeof(a[0]))
#define ADDVAL(c,e) c.push_back(Val<T>(e));
#define GETOFFSET(a,i,d) ((a)[i] - (i) - 1 - (d))

template<typename T>
struct Val {
  T val;
  bool nill;

  Val() : nill(true) {}
  Val(const T &init) : val(init), nill(false) {}

  void set(const T &init) {
	val  = init;
	nill = false;
  }

  T get() const {
	return val;
  }

  void remove() {
	nill = true;
  }

  bool isNill() const { return nill; }

  bool operator<(const Val<T> &other) const {
	if (other.nill)
	  return false;

	if (this->nill)
	  return false;

	return this->get() < other.get();
  }

  Val &operator=(Val<T> other) {
	std::swap<>(val,  other.val  );
	std::swap<>(nill, other.nill );
	return *this;
  }
};

template<typename T>
std::ostream &operator<<(std::ostream &o, const Val<T> v) {
  o << v.get();
  return o;
}

template<typename T>
inline
unsigned
findout(const T *a, const unsigned i, unsigned &d, std::deque< Val<T> > &r )
{
  //switch(a[i] - i - 1 - d) {
  switch(GETOFFSET(a,i,d)) {
  case 0: return 0;
  case 1: d ++; ADDVAL(r,a[i] - 1);                      return 1;
  case 2: d+=2; ADDVAL(r,a[i] - 2); ADDVAL(r, a[i] - 1); return 2;
  };
  return 0;
}

template<typename T>
inline
unsigned 
lookup_imp(const T *a, const unsigned i, const unsigned j, unsigned &d, std::deque< Val<T> > &res) 
{
  if (j == i) { // we hit the bottom of the heap
	return findout(a,i,d,res);
  } else { // still have intervals 
	int k = (i+j) >> 1; // median
	
	//if (unsigned should_find = a[j] - j - 1 - d) {
	if (unsigned should_find = GETOFFSET(a,j,d)) {
	  if (lookup_imp(a,  i,k,d,res) == should_find) 
		return should_find;
	  if (lookup_imp(a,k+1,j,d,res) == should_find) 
		return should_find;
	}
  }

  return 0;
}

template<typename T>
void 
lookup(const T *a, int sz, std::deque< Val<T> > &r)  
{
  unsigned d = 0;
  // TODO: check & explain return value evaluation
  lookup_imp(a, 0, sz-1, d, r);
  switch(d) {
  case 1:
	ADDVAL(r, a[sz-1]+1);
	break;
  case 0:
	ADDVAL(r, a[sz-1]+1);
	ADDVAL(r, a[sz-1]+2);
	break;
  }
}


//
// Note: if check array is present, it length should be 2
//
template<typename T>
void 
run_test(const T *a, int n, const char *header, const T *check) {
  using std::cout;
  using std::endl;

  std::deque<Val<T> > r;

  lookup(a,n,r);
  cout << header << ' ' << (r.size() == 2 ? "" : "not") << " found: ";
  std::copy(r.begin(), r.end(), std::ostream_iterator<Val<T> >(cout," "));
  cout << endl;

  if (check) {
	std::sort(r.begin(), r.end());

	if (r[0].get() != check[0])
	  cout << " [0] wrong answer => " << r[0].get() << endl;

	if (r[1].get() != check[1])
	  cout << " [1] wrong answer => " << r[1].get() << endl;
  }

}

int
main(int argc, char *argv[])
{
  static const int t0[] = {2}; static const int r0[] = {1, 3};
  static const int t1[] = {1,2,3,4,5,6,7,8,9,10,11};
  static const int t2[] = {1,2,3,4,5,6,7,8,9,10};
  static const int t3[] = {1,2,3,4,5,6,7    ,10};     static const int r3[] = {8, 9};
  static const int t4[] = {1,2,3,4    ,7,8,9,10,11};  static const int r4[] = {5, 6};
  static const int t5[] = {  2,3,4,5,6,7,8,9,   11};  static const int r5[] = {1,10};
  static const int t6[] = {1,2,3,  5,6,7,8  ,10,11};  static const int r6[] = {4, 9};
  static const int t7[] = {1};                        static const int r7[] = {2, 3};

  run_test<int>(t0, NELS(t0), "t0", r0 );
  run_test<int>(t1, NELS(t1), "t1",  0 );
  run_test<int>(t2, NELS(t2), "t2",  0 );
  run_test<>(   t3, NELS(t3), "t3", r3 );
  run_test<>(   t4, NELS(t4), "t4", r4 );
  run_test<>(   t5, NELS(t5), "t5", r5 );
  run_test<>(   t6, NELS(t6), "t6", r6 );
  run_test<int>(t7, NELS(t7), "t7", r7 );

  return 0;
}
