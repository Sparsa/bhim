/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: sparsa
 *
 * Created on 14 July, 2018, 11:13 AM
 */

#include <cstdlib>
#include<iostream>
#include<string>
#include<vector>
#include<unordered_map>

#include<automata.h>

#include<sys/resource.h>

//#include"dtimedPartStack.h"
#include<concatenationMultiStack.h>
//#include "concatenationOptimized.h"
using namespace std;

int who = RUSAGE_SELF;
struct rusage usage;
int ret;
bool repeated;
inline int  max(int a,int b);
//bool isEmpty(const Automata& a); // takes an automata object and returns true if it is empty .
//bool isEmpty(const Concatenation& c, const Automata& a) ;// for timed system we need to work on the concatenation class.
int main(int argc, char** argv) {
    
    if(argc < 2) {
        cout << "Input TPDA file must be given !" << endl;
        return 0;
    }
    if(argc < 3) {
        cout <<"The bound on the context switch is not given" << endl;
        cout <<"We will assume no limits on context switch" << endl;
    }
    clock_t start = clock(); // Start measuring time
    
    ////**********////
    int change = INF;
    if( argc ==3)
        change = atoi(argv[2]);
    if(argc == 4){
        change = atoi(argv[3]);
        repeated = true;
    }


    //setGlobal(argv[1],change); // set all the global variables
    Automata a(argv[1]); //open an input file and extract all information in an automata object a
    a.contextBound = change;
    if(a.final_state == -1){
      cout<< "No final states marked, the language accepted by the automaton is trivially Empty. Do you still want to continue with the process? Y/n" << endl;
      char c;
      cin >> c;
      if(!(c == 'Y' || c == 'y')){
          return 0;
      }
    }
    cout <<"Here number of states is : " << a.number_of_states << endl;
    // make sure when you are using completeListOfE() you called computeComplete() once.
    /*
     ///////////   THE UNTIMED CODE //////////////
     a.computeComplete(); // compute the complete matrix
     a.computeListOfE(); // compute the E matrices
     */
    //a.printAutomata(cout);
    //printBoolMatrix(a.Complete,a.number_of_states);
    if(a.number_of_stacks == 2 && argc == 2) {
        cout << "The example has 2 stacks and you have not specified the parameter" << endl;
        cout << "If you don't give the parameter the program might not terminate. " << endl;
        cout << "Please provide a parameter " << endl;
        cin >> a.contextBound;
        cout << "Thank You!" << endl;
        
    }
    
    if(a.number_of_stacks > 2)
    {
        cout << "This program only handles number of stack <= 2" << endl;
        cout << "The current number of stacks in the input is " << a.number_of_stacks << endl;
        return 0;
    }
    
    ///////  NEW CONCATENATION CODE ////////
    
    
    cout << "#Stacks : " << int(a.number_of_stacks) << endl;
    
    cout << "#clocks : " << int(a.list_of_clocks.size()) << endl;
    cout << "#states : " << int(a.number_of_states)-1 << endl;
    cout << "#transitions : " << int(a.number_of_transitions)-1 << endl;
    //cout << "Maximum constant(M) : " << int(a.maximum_constant) << endl;
    for(int i =0;i<a.maximum_constant.size();i++){
        cout << "Maximum Constraint for Clock "<< i << " is: " << a.maximum_constant[i] << endl;
    }
    
    
    a.printAutomata(cout);
    
    // now the reachability can be achieved by the Complete matrix.
    //        if(a.Complete[a.start_state-1][a.final_state-1]){
    //            cout << "The language recognized by the TPDA is NOT EMPTY" << endl;
    //        }
    
    Concatenation c(a);  // new concatenation object c declared using the constructor which takes an automata object.
//    c.timeLapse(a);
//    c.internal(a);
    bool b; // variable which stores the emptiness checking value.
    if(!repeated){
        b = c.isEmpty(a); // for the new program.
    if(b){
        cout << "The language recognized by the TPDA is EMPTY!" << endl;
        cout << "The Number of States generated " << c.list_of_states.size() << endl;
                
    }
    else{
        cout << "The language recognized by the TPDA is NOT EMPTY!" << endl;
         cout << "The Number of States generated " << c.list_of_states.size() << endl;
         
    }
    cout << endl;
    }
    else{
        b = c.isEmpty(a); // check for emptiness first
        set<ul> value = c.C(a.final_state-1,a.final_state-1);
        if( b && value.size() !=0){
            cout << "The system's liveness condition satisfied on the state S" << endl;
        }
    }
    
    
    
    //cout << "Tree-Width(K) : " << int(K-1) << endl;
    
    cout << endl << "---END OF EXECUTION---" << endl << endl;
    
    clock_t stop = clock(); // end of time calculation
    
    double elapsed = (double)(stop - start) * 1000.0 / CLOCKS_PER_SEC; // Time elapsed calculation 
    
    cout << "Execution time : " << (elapsed/1000) << " seconds." << endl << endl;
    
    
    
    ret = getrusage(who,&usage);
    cout <<"Maximum Memory used is : " << usage.ru_maxrss << "KB" << endl;
    
    return 0;
    
}




inline int max(int a,int b){
    if(a>b)
        return a;
    else return b;
}

//bool isEmpty(const Concatenation& c, const Automata& a) {
//    vector<int> in_valuation;
//    for(unsigned i = 0; i < a.list_of_clocks.size();i++){
//        in_valuation.push_back(0); // put all zeroes to the initial valuation matrix.
//    }
//    states start = make_pair(a.start_state-1,in_valuation);
//    if(c.stackP){
//    vector<states>::const_iterator it;
//    for(it = c.list_of_states.begin();it!=c.list_of_states.end();++it){
//        if(it->first == a.final_state-1){
//            if(c.getC(c.getStateMap(start)-1,c.getStateMap(*it)-1) >= 0){
//                return false;
//            }
//        }
//    }
//    }
//    else{
//            vector<states>::const_iterator it;
//    for(it = c.list_of_states.begin();it!=c.list_of_states.end();++it){
//        if(it->first == a.final_state-1){
//            if(c.getCb(c.getStateMap(start)-1,c.getStateMap(*it)-1)){
//                return false;
//            }
//        }
//    }
//        
//    }
//    return true;
//}
