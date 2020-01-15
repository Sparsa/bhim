/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   automata.h
 * Author: Sparsa Roychowdhury
 * Mail: sparsa@cse.iitb.ac.in
 *
 * Created on 14 July, 2018, 10:17 AM
 */

#ifndef AUTOMATA_H
#define AUTOMATA_H
#include<vector>
#include<stack>
#include<string>
#include<set>
#include<climits>
#define INF (INT_MAX >> 1) //now infinity value is maximum integer divided by 2
using namespace std;
typedef unsigned long ul;
typedef  pair< unsigned long, vector<unsigned long> > states;
struct statesStack{
    states Pair1; // state valuation pair.
    states Pair2;
    //int stackLatestPush; // the index of the state after which the transition is fired.
    int transitionNumber; // which contains the push and is firable from state the state indicated by stackLatestPush
    //int timeTillPush; // time till push happened this is zero if there is no pending push
};
struct statesStackTimed{
    states Pair1;
    states Pair2;
    int transitionNumber;
    //int timeLapse;
    int timeTillLastPush;
};
//#define INF 999999 // declaring infinity by giving a large number like 5 9's
typedef unsigned us;
using namespace std;
class Guards 
{
public:
    int clock,upper_bound,lower_bound,clock_diag; //clock_diag is the second clock we need to store diagonal guards
    bool upper_open, lower_open,diagonal=false; //this diagonal flag is used to identify if the guard is a diagonal constraint or not
    void printGuard(ostream &output_stream) const;
};
class StackOp
{
public:
    int op=0; //0 for nothing, 1 for push, 2 for pop, for pop and then push 3 and 
    int stack_push,stack_pop;
    int stack_alphabet_push,stack_alphabet_pop,stack_pushed_clock; // if it is push or pop then which stack alphabet ? stack_clock_push is a special variable which includes the clock we want to push.
    unsigned long upper_bound,lower_bound; // if the op is pop then we need the time
    bool upper_open,lower_open,diagonal=false,special=false; // if the upper boun and lower bound is open or close.
    void printStackOp(ostream &output_stream);
};
class Transition
{
public:
    unsigned long source_state,destination_state;
    unsigned long number_of_reset;//,number_of_guards; //not necessary as we have the list with us we can directly compute it from the size() function
    //int reset_list = 0; // make it reset bit integer.
    vector<unsigned long> reset_vector; // keeps a list of clocks which are reset in this transition.
    string name;
    bool checkGuards(vector<unsigned long> valuation) const; // this function returns true or false if the guards matches with the valuation.
    vector<unsigned long> afterReset(vector<unsigned long> valuation)const ;// this function resets the valuation.
    bool firable(states s) const; // given a states pair this function checks if the transition is firable or not from the states.
    bool firable(statesStack s) const; // for the part with stacks.
    set<ul> checkStackConstraint(set<unsigned long> s) const;
    bool checkStackConstraint(unsigned long time) const;
    vector<Guards> guard_list;
    StackOp stackOp;
    int input_symbol;
    void printTransition(ostream &output_stream) const ;
};

class Automata 
{
public: 
    unsigned long number_of_states, number_of_transitions;
    vector<Transition> transition_list;
    int start_state,final_state;
    vector<string> list_of_clocks; //int number_of_clocks;
    int number_of_stacks;
    int number_of_alphabet;
    int number_of_stack_alphabet;
    unsigned long maximum_stack_constant,max_const;
    vector<unsigned long> maximum_constant; // map from list of clocks to its corresponding maximum constraint.
    bool diagonal;  // the diagonal flag is true if there is a diagonal constraint in the input.
    //bool **Complete;
    //vector<bool**> listOfE; // length depends on number of stacks in the automata.
    vector<stack <int > > stack_list;
    int contextBound; // this is the context switch bound on the multi stack.
    void printAutomata(ostream &output_stream);
    Automata(){}
    //utomata(char* inputfilename);
    Automata(char* inputfilename, int type);
    
    
    bool push_pop_check(unsigned long i,unsigned long j) const; // this function checks the push pop alphabet and stack are same are not, the first one is the push transition index and second one is the pop transition index

    
    //vector<int> normalization(vector<int> valuation); 
    //here we are taking a vector which is a map of clocks to time Natural numbers
    //clearly the index of the vector is the clock number
    //and the value at that index is the clock valuation
    // the maximumConstraint is maximum constraint in the 

};
void drawsystem(string outfile, const Automata& a) ;
#endif 

