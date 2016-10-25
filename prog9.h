/********************************************
 *  *   NAME        :  PROG8.H " THE N QUEENS PROBLEM"
 *  *   AUTHOR      :  AKASH Z1717009
 *  *   SUBMISSION  :  11/06/2015
 * ********************************************/

#include "/home/cs689/common/689.h"

#ifndef prog8_h
#define prog8_h

typedef struct { int id, arrTime, pickTime, waitTime;} cust;
typedef list<cust > L;
typedef queue<cust > Q;

typedef L::iterator LI;
typedef struct { int nextArr, nextCheckout, nextDept;} event;
typedef struct { int numDept, totShop, totWait, totServ;} stat;

const long SIM_TIME= 6*30*24*60;
const extern int MIN_INT_ARR=1;
const extern int MAX_INT_ARR =5;
const extern int MIN_PICK=1;
const extern int MAX_PICK =20;
const extern int MIN_SERV =1;
const extern int MAX_SERV =4;
const extern int SAMPLE_INT=2000;

int arr_time(const int& clocks) ;
int pick_time(const int& clocks) ;
int dept_time(const int& clocks);
void init_vars(event& e, stat& s);
void arrival(event& e, const int& id, L& l);
void checkout(event& e, L& l, Q& q);
void departure(event& e, Q& q, stat& s);
void update_fin_stat( stat& s, const cust& c, const int& clocks) ;
void print_fin_stat ( const stat& s);
int update_clock ( const event& e, const bool& f1, const bool& f2);
bool cmp ( const cust& c1, const cust& c2 );
int randTime(int low, int high) ;

#endif
