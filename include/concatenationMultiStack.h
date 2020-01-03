/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   concatenationMultiStack.h
 * Author: sparsa
 *
 * Created on 6 September, 2018, 10:26 AM
 */

#ifndef CONCATENATIONMULTISTACK_H
#define CONCATENATIONMULTISTACK_H
/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <boost/numeric/ublas/matrix.hpp> // using high performing boost libs
#include <boost/numeric/ublas/io.hpp> // using high performing boost libs
#include<automata.h>
#include<iostream>
#include<unordered_map>
#include<string>
#include<set>
#include<linkedlist.h>
namespace bnu =  boost::numeric::ublas;
using namespace std;
typedef pair<ul,bool> run;
typedef pair<int, set<unsigned long > > entry; // entry for the Di matrix.
//class Fired {
//
//    set<int> timelapsed;
//    int Tr;
//
//public:
//    void insert_set(int element){
//        timelapsed.insert(element);
//    }
//    void setTr(int transition){
//        Tr=transition;
//    }
//
//    int getTr(){
//        return Tr;
//    }
//};
typedef struct {
    states state; // the state information
    unsigned long time; // the time will be -1 if it is the final state of the list of states else it will track total time duration of hole and complete context.
    ul h;
    //bool complete; //this boolean tells if this state added due to complete transition or not
} State;
//typedef struct{
//    int start;
//
//    int end;
//
//} Hole;
set<unsigned long> additionBags(set<unsigned long> a, set<unsigned long>b,unsigned long max_stack);
string toString(states Pair); // this function creates a string from the given state valuation pair (used for unordered_map).
//bool allMax(states s, const Automata& a); // this function returns true if all the clocks reached its maximum value.
string setToString(set<unsigned long> s);
class MultiStack;
class Concatenation{
public:
    //int maximum_constant; // maximum constant of the automata.
    unsigned long size_of_matrix; // the current size of the matrix.
    //int max_size; // this is done to check for the maximum valuation a clock can have in the system.
    bool diagonal; // this information will come from the automata which will be true if the underlying automata have diagonal guards or not.
    //int **C; // the matrix which actually holds the information.
    //Automata a;
    double witness_time;
    bool stackP;
    //int maximum_stack_constant;
    unsigned long maximum_stack_constant;
    states start_state;
    bool checkPossibleUseful(vector<ul> first, vector<ul> second);
    vector<bnu::matrix<vector<unsigned long> > > list_of_matrices; // this list is mapped using the valuation mapping -1.
    vector<bnu::matrix<vector<unsigned long> > > list_of_matrices2; // this list is mapped using the valuation mapping -1.
    bnu::matrix<vector<unsigned long> > getPossibleUselessTransitions(vector<unsigned long> valuations, const Automata& a); // this returns a matrix with
    void printPossibleUselessTransitions( bnu::matrix<vector<unsigned long> > useless, const Automata& a); // this prints possible useless transitionsb
    bnu::matrix<vector<unsigned long> > getPossibleUselessTransitions2(vector<unsigned long> valuations, const Automata& a); // this returns a matrix with
    vector<run> getPath(unsigned long index1,unsigned long index2, const Automata& a);
    vector<run> getPath3(unsigned long index1,unsigned long index2, const Automata& a);
      vector<run> getPath2(unsigned long index1,unsigned long index2, const Automata& a,unordered_map<string,int> &loop,unordered_map<string,int> &loop2);
    vector<run> computePath(bnu::matrix<vector<unsigned long> > &getUselessTransition,vector<unsigned long> valuation,unsigned long index1, unsigned long index2, const Automata& a);
     vector<run> computePath2(bnu::matrix<vector<unsigned long> > &getUselessTransition,vector<unsigned long> valuation,unsigned long index1, unsigned long index2, const Automata& a);
     vector<run> Witness2i(unsigned long index1, unsigned long time ,unsigned long index2, const Automata& a);
     void closureState(bnu::matrix<vector<ul> >& UselessMatrix,const Automata& a);
    bnu::matrix<int> InternalTr;
    bnu::matrix<set <unsigned long> > TimeLapseTr; // this is used to store the initial time lapses
    //bnu::matrix<bool> InternalTr; // this is initially used to store the initial internal transitions
    bnu::matrix<set <unsigned long> >   C; // for time pushdown systems
    bnu::matrix<bool> Cb; // for only time automata.
    bnu::matrix<entry > D1; // D1 matrix for stack 1
    bnu::matrix<entry> D2; // d2 matrix for stack2
    bnu::matrix<set<unsigned long> > D1c; // for D1 closure
    bnu::matrix<set<unsigned long> > D2c; // for D2 closure
//    bnu::matrix<linkedlist<Fired> > Witness; // this matrix will hold all the information regarding the witness.
    //Concatenation(const Automata &a,bool b);
    Concatenation(const Automata &a); // the constructor.
    int timeLapse(states s,const Automata &a); // what happens after all possible time lapsed.
    //vector<pair<states,bool> > timeLapse(states s,const Automata &a);
    int internal(states s,const Automata &a); //what happens if t transition fired after from the current matrix.    
    bool stackClosure(const Automata &a,bool timed);
    void computeD(const Automata &a); // this will compute D matrices we will assume that we already computed ConcatClosure
    void closureD( const Automata &a); // this will used to compute the closure of D matrices.
   // vector<pair<states,bool> > internal(states s,const Automata &a);
    //vector<pair< states, bool> > listNextStates(states s,const Automata& a,bool b);
    bool  closure(const Automata &a);
    bool semiClosure(const Automata &a, unsigned long i,unsigned long j);
//    string recursiveWitnessTimeLapse(unsigned long index1, unsigned long time, unsigned long index2, const Automata& a);
//    string recursiveWitnessInternal(unsigned long index1, unsigned long time, unsigned long index2, const Automata& a);
//    string recursiveWitness(unsigned long index1, unsigned long time,unsigned long index2, const Automata& a);
//    string recursiveWitness(unsigned long index1, unsigned long index2, const Automata& a);
    vector<run> Witness(unsigned long index1,unsigned long time, unsigned long index2, const Automata& a) ;
    //witness for stack push pop
    vector<run> Witness2(unsigned long index1, unsigned long time, unsigned long index2, const Automata& a);
    vector<run> Witness3(ul indexOfState,const vector<MultiStack>& listOfStates, const Automata& a,stack<run> &stack1,stack<run> &stack2);
    vector<run> computePushNComplete(State s1, State s2, ul time,const Automata & a,int stack_pop,int stack_alph_pop);
    int printRun(vector<run> therun,const Automata& a);
//    vector<int> normalization(vector<int> valuation, const Automata& a);
//    set<int> getC(int i,int j) const;
//    bool getCb(int i,int j) const;
//    int getStateMap(const states& s) const;
    int printWitness(unsigned long i, unsigned long j, const Automata& a);
    int printWitness2(unsigned long i, unsigned long j, const Automata& a);
    void parentPrint(unsigned long index,const vector<MultiStack>& listOfStates,const Automata &a);
    void printC();
    void printD(const bnu::matrix<entry > &D);
    void printDc(const bnu::matrix<set<unsigned long> > &D);
    unordered_map<string,unsigned long> state_map;
    unordered_map<string,unsigned long> valuation_map; // this maps valuations to a new state.
    unordered_map<string,unsigned long> valuation_map2; // this maps valuations to a new state.
    unordered_map<string,ul> indexmap;
    unordered_map<string,bool> functionCalled;
    ul indexmapcount =0;
    vector<states> list_of_states;
    //vector<int> push_transitions;
    //vector<int> pop_transitions;
    set<ul> getNextLeft(ul index, const Automata& a);
    bool isEmpty(const Automata &a);
};

    class MultiStack{
public:
    vector<State> currStates;
    int holes; // total number of holes
    int lastPush; // which stack pushed last.
    bool isPop; // is the last transition was pop we can do closure after pop.
    ul parentId; // this is the parent Id from where it is generated
    ul totalTimeLapsed;
    ul transitionId;
    short parentOp;  // this is the parent Op which is executed to get this operation, 1 for complete context, 2 for hole1, 3 for hole 2, 4 for pop 1, 5 for pop 2
    vector<unsigned long> Stack1; // the last entry will give us the detail of the hole created by Stack1 or Stack2
    vector<unsigned long> Stack2;
    vector<vector<State> > addComplete(const Concatenation &C, const Automata &a); // return complete states after applying complete states.
    vector <vector<State> > addHoleStack1( const Concatenation &C, const Automata &a); // add a hole of stack 1 to the state
    vector<vector<State > > addHoleStack2(const Concatenation &C, const Automata &a); // add hole of stack 2 to the state
    // vector<vector<State> > afterPop2(const Concatenation &C, ul transitionIt); // getting the possible states after the transitions.
    // vector<vector<State> > afterPop1(const Concatenation &C, ul transitinIt);

    // the pop will be nondeterministic.
//    vector<vector<State > > popStack1(const Concatenation &C,const Automata &a);
//    vector<vector<State> > popStack2(const Concatenation &C,const Automata &a);
    void print() const;
    MultiStack(const Concatenation &C){ // constructor of the MultiStack Object.
        State startState;
        startState.state=C.start_state;
        //    startState.stackSymbol = -1; // start state so no push happened yet
        startState.time = 0 ; //time is also zero.
        startState.h = 0; // zero means last point of the state.
        holes =0 ; // no holes created yet in the state
        lastPush = -1; // no stack pushed yet.
        currStates.push_back(startState);
        parentId = 0; // the self parent id
        parentOp = -1; // the parent op is -1 so when we reach parent op -1 we know we reached the first state no need to check the parent ID.
        totalTimeLapsed = 0; // this is the measure of total time lapsed from start to end.
        
    }// initialzes the new statelist.
    MultiStack(const MultiStack &M){ // copy constructor
        this->currStates = M.currStates;
        this->holes = M.holes;
        this->lastPush = M.lastPush;
        this->isPop = M.isPop;
        this->Stack1 = M.Stack1;
        this->Stack2 = M.Stack2;
        this->parentId = M.parentId;
        this->parentOp=M.parentOp;
        this->totalTimeLapsed = M.totalTimeLapsed;
       
    } 
    MultiStack(vector<State> &state,int holes, int lastPush,bool isPop,vector<unsigned long> S1,vector<unsigned long> S2,ul parentId, short parentOp,ul totalTimeLapse);
    string to_string();
};




#endif /* CONCATENATIONMULTISTACK_H */

