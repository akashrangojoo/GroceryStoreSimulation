/****************************************************************
 *      PROGRAM:   PROG9.CC
 *      AUTHOR:    AKASH RANGOJOO
 *      LOGON ID:  Z1717009
 *      DUE DATE:  11/6/2015
 *      FUNCTION:  PROGRAM TO SIMULATE STORE.
 *****************************************************************/

#include "prog9.h"

long clocks;

int main(){
   L l;
   Q q;
   event e;
   stat s;
   init_vars(e,s);
   int id=1;
   while(clocks<SIM_TIME){
      // cout<<"while "<<clocks<<endl;
      if(e.nextArr==clocks) {
         arrival(e,id++,l);
      }
      if(e.nextCheckout==clocks) {
         //    cout<<"check out : "<<endl;
         checkout(e,l,q);
      }
      if(e.nextDept==clocks) {
         //  cout<<"Depart : "<<endl;
         departure(e,q,s);
      }
      // clocks++;
      bool f1= !l.empty ( );
      bool f2= !q.empty ( );
      clocks=update_clock(e,f1,f2);
   }
   print_fin_stat(s);

   return 0;
}


 /*************************************************
  *  * FUNCTION  :  arrival
  *  * INPUTS    :  event&e,const int& id, L& l
  *  * USAGE     :  This method is called when a new cust enters 
  *               the store.
  *********************************************/

void arrival(event&e,const int& id, L& l) {
   cust c;
   c.id=id;
   c.arrTime=e.nextArr;
   c.pickTime= pick_time(clocks);
   c.waitTime=0;
   l.push_back(c);
   // cout<<"Pushed into list : "<<c.id<<endl;
   e.nextArr= arr_time(clocks);
   // cout<<"Next arrival at : "<<e.nextArr<<endl;
   LI i =min_element(l.begin(),l.end(),cmp);
   e.nextCheckout= (*i).pickTime;

}

 /*************************************************
  *  * FUNCTION  :  checkout 
  *  * INPUTS    :  event& e, L& l, Q& q
  *  * USAGE     :  This method is called when a cust enters 
  *               the checkout line.
  * ***********************************************/

void checkout(event& e, L& l, Q& q) {
   LI li = min_element(l.begin(), l.end(), cmp);

   if(q.empty())  e.nextDept = dept_time(clocks);
   q.push(*li);
   l.erase(li);

   if(!l.empty())
   {
      li = min_element(l.begin(), l.end(), cmp);
      e.nextCheckout = (*li).pickTime;
   }
   else  e.nextCheckout = SIM_TIME + 1;
}


/*************************************************
  *  * FUNCTION  :  departure
  *  * INPUTS    :  event& e, Q& q, stat& s
  *  * USAGE     :  this method is called when a cust 
  *               departs the store.
  * ***********************************************/

void departure(event& e, Q& q, stat& s) {

   cust c=q.front();
   q.pop();
   update_fin_stat(s,c,clocks);
   if(!q.empty()) {
      q.front().waitTime = e.nextDept-q.front().pickTime;
      e.nextDept=dept_time(clocks);
   }
   else e.nextDept = SIM_TIME + 1;
//   update_fin_stat(s,c,e.nextDept);
}

 /*************************************************
  *  * FUNCTION  :  cmp
  *  * INPUTS    :  const cust& c1, const cust& c2
  *  * USAGE     :  compare two entities .
  * ***********************************************/

bool cmp ( const cust& c1, const cust& c2 ) {
   return c1.pickTime < c2.pickTime;
}
 /*************************************************
  *  * FUNCTION  :  randTime
  *  * INPUTS    :  int low, int high
  *  * USAGE     :  generate random number between low and
  *               high.
  ************************************************/

int randTime(int low, int high) {
   high++;
   int randNumber = rand()%(high-low)+low;
   return randNumber;
}

 /*************************************************
  *  * FUNCTION  :  arr_time
  *  * INPUTS    :  const int& clocks
  *  * USAGE     :  generate random next arrival time.
  * ***********************************************/

int arr_time(const int& clocks) {
   return clocks+randTime(MIN_INT_ARR,MAX_INT_ARR);
}

 /*************************************************
  *  * FUNCTION  :  pick_time
  *  * INPUTS    :  const int& clocks
  *  * USAGE     :  generate random pick up time.
  * ***********************************************/

int pick_time(const int& clocks) {
   return clocks+randTime(MIN_PICK,MAX_PICK);
}


/*************************************************
 *  * FUNCTION  :  dept_time
 *  * INPUTS    :  const int& clocks
 *  * USAGE     :  generate random departure time.
 * ***********************************************/

int dept_time(const int& clocks) {
   return clocks+randTime(MIN_SERV,MAX_SERV);
}

/*************************************************
 *  * FUNCTION  :  update_fin_stat
 *  * INPUTS    :  stat& s, const cust& c, const int& clocks
 *  * USAGE     :  update the statistics every time a customer
 *                departs.
 * ***********************************************/

void update_fin_stat( stat& s, const cust& c, const int& clocks) {
   s.numDept++;
   s.totServ+= (clocks - (c.pickTime + c.waitTime));
   s.totShop+= (c.pickTime-c.arrTime);
   s.totWait+= c.waitTime;

   if(s.numDept% SAMPLE_INT== 0)
      cout<<"clock = "<<clocks<<"      :     id = "<<c.id<<"     :     shop-time = "<<c.pickTime-c.arrTime<<"     :      wait-time = "<<c.waitTime<< "     :     serve-time = "<<(clocks - (c.pickTime + c.waitTime))<<endl;
}

 
/*************************************************
 *  * FUNCTION  :  print_fin_stat
 *  * INPUTS    :  const stat& s
 *  * USAGE     :  Calculate Average values and print them
 ************************************************/

void print_fin_stat ( const stat& s){
   double avgShopping, avgWaiting, avgServe;

   avgShopping =(double)s.totShop/s.numDept;
   avgServe=(double)s.totServ/s.numDept;
   avgWaiting=(double)s.totWait/s.numDept;
   cout<<"Average Serving Time : "<<avgServe<<endl<<"Average Shopping Time : "<<avgShopping<<endl<<"Average Waiting Time : "<<avgWaiting<<endl;
}


/*************************************************
 *  * FUNCTION  :  update_clock
 *  * INPUTS    :  const event& e, const bool& f1, const bool& f2
 *  * USAGE     :  update the clock value to earliest
 *                 event
 ************************************************/
 
int update_clock ( const event& e, const bool& f1, const bool& f2 )
{
   int min = SIM_TIME + 1;
   if(e.nextArr < min) min= e.nextArr;
   if((f1 == true) && (min > e.nextCheckout))  min=  e.nextCheckout;
   if((f2 == true) && (min > e.nextDept)) min=  e.nextDept;
   return min;
}

  
/*************************************************
 *  * FUNCTION  :  init_vars
 *  * INPUTS    :  event& e, stat& s
 *  * USAGE     :  Initialize variables structures
 *                event and stat
 *************************************************/
 
void init_vars(event& e, stat& s){             
   s.numDept=0;
   s.totServ=0;
   s.totShop=0;
   s.totWait=0;
   e.nextArr=0;
   e.nextCheckout= SIM_TIME+ 1;
   e.nextDept=SIM_TIME+ 1;
}

