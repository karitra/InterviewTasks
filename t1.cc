//
// Copyright 2013 Alex Karev 
//
// Interview task:
// ===============
//
//  Find two missed numbers in sequence of natural numbers.
//
#include <algorithm>
#include <cassert>
#include <iostream>
#include <iterator>
#include <list>
#include <vector>

#define NELS(a) (sizeof(a) / sizeof(a[0]))

#define ADDVAL(c,e)      c.push_back(e)
#define GETOFFSET(a,i,d) ((a)[i] - (i) - 1 - (d))
#define DEF_SIZE 1024

#define DBGINIT()														\
  do {																	\
	dbg_current_recursion_depth = 0;									\
	dbg_max_recursion_depth = 0;										\
	dbg_find_hits = 0;													\
  } while(0)

#define DBGPROLOGUE() dbg_depth_t dbgmonitor(dbg_current_recursion_depth, dbg_max_recursion_depth)

static long dbg_find_hits;
static long dbg_current_recursion_depth;
static long dbg_max_recursion_depth;

struct dbg_depth_t {
  long &depth;
  long &max;

  dbg_depth_t(long &d, long &m) : depth(d), max(m) {
	if (++depth > max) 
	  max = depth;
  }

  ~dbg_depth_t() {
	--depth;
  }

};

template<typename T, typename Res>
inline
unsigned
findout(const T *a, const unsigned i, unsigned &d, Res &r )
{
  dbg_find_hits++;

  //switch(a[i] - i - 1 - d) {
  switch(GETOFFSET(a,i,d)) {
  case 0: return 0;
  case 1: d ++; ADDVAL(r,a[i] - 1);                      return 1;
  case 2: d+=2; ADDVAL(r,a[i] - 2); ADDVAL(r, a[i] - 1); return 2;
  };
  return 0;
}

template<typename T, typename Res>
inline
unsigned 
lookup_imp(const T *a, const unsigned i, const unsigned j, unsigned &d, Res &res) 
{
  DBGPROLOGUE();

  if (j == i) { // we hit the bottom of the heap
	return findout(a,i,d,res);
  } else { // still have intervals 
	int k = (i+j) >> 1; // median
	
	if (unsigned should_find = GETOFFSET(a,j,d)) {
	  if (lookup_imp(a,  i,k,d,res) == should_find) 
		return should_find;
	  if (lookup_imp(a,k+1,j,d,res) == should_find) 
		return should_find;
	}
  }

  return 0;
}

// TODO: forbit instance creation on non integer types
template<typename T, typename Res>
void 
lookup(const T *a, const unsigned sz, Res &r)  
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

  std::list<T> r;
  lookup(a,n,r);
  cout << header << ' ' << (r.size() == 2 ? "" : "not") << " found: ";
  std::copy(r.begin(), r.end(), std::ostream_iterator<T>(cout," "));
  cout << endl;

  if (check) {
	if (r.front() != check[0])
	  cout << " [0] wrong answer => " << r.front() << endl;

	if (r.back() != check[1])
	  cout << " [1] wrong answer => " << r.back() << endl;
  }
}


void 
run_test_with_input(std::istream &is)
{
  using std::cout;
  using std::endl;

  std::vector<int> v;
  v.reserve(DEF_SIZE);

  std::copy(std::istream_iterator<int>(is), std::istream_iterator<int>(),
			std::back_inserter(v) );

  // cout << "lookup in sequence: ";
  // std::copy(v.begin(), v.end(), std::ostream_iterator<int>(cout," "));
  // cout << endl;
  
  DBGINIT();

  std::list<int> r;
  lookup( &v[0], v.size(), r);
  
  cout << "found in standard input: ";
  std::copy(r.begin(), r.end(), std::ostream_iterator<int>(cout," "));
  cout << ", hits: " << dbg_find_hits << ", max depth: " << dbg_max_recursion_depth << endl;
}

int
main(int argc, char *argv[])
{
  // Simple tests, real (huge) test set will be provided from console input
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

  

  run_test_with_input(std::cin);
  return 0;
}
