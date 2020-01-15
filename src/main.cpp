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
#include<iomanip>
#include<automata.h>
#include"boost/algorithm/string.hpp"
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
void bhim_usage(){
  cout << "bhim -h HOLE_BOUND -x INPUT_FILE" <<endl;
  cout <<"Please use -h and -x options to specify hole bound and input files respectively" << endl;
}
//bool isEmpty(const Automata& a); // takes an automata object and returns true if it is empty .
//bool isEmpty(const Concatenation& c, const Automata& a) ;// for timed system we need to work on the concatenation class.
int main(int argc, char** argv) {
    
    char choice;
    char* filename;
    short count = 0;
    int option,hole_bound;
  while ((choice = getopt (argc, argv, "x:b:c:h:")) != -1)
    switch (choice)
      {
      case 'b':
        option = 1;
        filename = optarg;
        count++;
        break;
      case 'c':
        option = 2;
        filename = optarg;
        count++;
        break;
      case 'x':
        option=3;
        filename = optarg;
        count++;
        break;
        case 'h' :
        count++;
        hole_bound = atoi(optarg);
        break;
      case '?':
        if (optopt == 'c'|| optopt == 'b' || optopt =='x')
          fprintf (stderr, "Please enter the file name after the option -%c.\n", optopt);
        else if (isprint (optopt))
          fprintf (stderr, "Unknown option `-%c'.\n", optopt);
        else
          fprintf (stderr,
                   "Unknown option character `\\x%x'.\n",
                   optopt);
        return 1;
      default:
        return 0;
      }

    if(argc < 2) {
        cout << "Input TPDA file must be given !" << endl;
        bhim_usage();
        return 0;
    }
    //cout << count << endl;
    if(count <2){
      bhim_usage();
      return 0;
    }
    clock_t start = clock(); // Start measuring time
    
    ////**********////
    /* int change = INF;
    if( argc ==3)
        change = atoi(argv[2]);
    if(argc == 4){
        change = atoi(argv[3]);
        repeated = true;
    } */


    //setGlobal(argv[1],change); // set all the global variables
    Automata a(filename,option); //open an input file and extract all information in an automata object a
    a.contextBound = hole_bound;
    if(a.final_state == -1){
      cout<< "No final states marked, the language accepted by the automaton is trivially Empty. Do you still want to continue with the process? Y/n" << endl;
      char c;
      cin >> c;
      if(!(c == 'Y' || c == 'y')){
          return 0;
      }
    }
    //cout <<"Here number of states is : " << a.number_of_states << endl;
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
    
    string dotFile(filename); 
    dotFile.append(".dot") ;
    drawsystem(dotFile,a);

     
    
    //a.printAutomata(cout);
    
    // now the reachability can be achieved by the Complete matrix.
    //        if(a.Complete[a.start_state-1][a.final_state-1]){
    //            cout << "The language recognized by the TPDA is NOT EMPTY" << endl;
    //        }
    
    Concatenation c(a);  // new concatenation object c declared using the constructor which takes an automata object.
//    c.timeLapse(a);
//    c.internal(a);
  c.witness_time = 0;
  string is_empty_mpda;
    bool b; // variable which stores the emptiness checking value.
    if(!repeated){
        b = c.isEmpty(a); // for the new program.
    if(b){
        cout << "The language recognized by the TPDA is EMPTY!" << endl;
        cout << "The Number of States generated " << c.list_of_states.size() << endl;
        is_empty_mpda="Yes";
                
    }
    else{
        cout << "The language recognized by the TPDA is NOT EMPTY!" << endl;
         cout << "The Number of States generated " << c.list_of_states.size() << endl;
         is_empty_mpda="No";
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
    
    
    cout << "#Stacks : " << a.number_of_stacks << endl;
     cout << "#clocks : " << a.list_of_clocks.size() << endl;
     cout << "#states : " << a.number_of_states << endl;
     cout << "#transitions : " << a.number_of_transitions << endl;
    cout << "Maximum constant(M) : " << a.max_const << endl;
    for(int i =0;i<a.maximum_constant.size();i++){
        cout << "Maximum Constraint for Clock "<< i << " is: " << a.maximum_constant[i] << endl;
    }
    
    //cout << "Tree-Width(K) : " << int(K-1) << endl;
    
    cout << endl << "---END OF EXECUTION---" << endl << endl;
    
    clock_t stop = clock(); // end of time calculation
    
    double elapsed = (double)(stop - start) * 1000.0 / CLOCKS_PER_SEC; // Time elapsed calculation 
    elapsed -= c.witness_time;
    cout << "Execution time : " << elapsed << " mili seconds." << endl << endl;
    cout << "Witness Generation time : " << c.witness_time << " mili seconds." << endl << endl;
    
    vector<string>  results;
    boost::split(results,filename,[](char c){return c == '/';});
    ret = getrusage(who,&usage);
    cout <<"Maximum Memory used is : " << usage.ru_maxrss << "KB" << endl;
    cout << "######### Corresponding Table Row ########## " << endl;

    if(a.list_of_clocks.size() == 0 && a.maximum_stack_constant <= 1){
      cout << "|" << setw(50)<< "Filename" << "|" << setw(10) << "#States" << "|"<<setw(12) << "#Transitions" << "|"<<setw(10)  << "#Stacks" << "|"<<setw(10) << "Hole Bound" << "|"<<setw(10) << "Total Time" <<"|"<<setw(12) << "Witness Time" << "|" <<setw(10) << "Memory(KB)" <<"|"<<setw(10) << "Empty?" << endl;
    cout << "|" << setw(50)<< results[results.size()-1] << "|" << setw(10) << a.number_of_states << "|"<<setw(12) << a.number_of_transitions << "|"<<setw(10)  << a.number_of_stacks << "|"<<setw(10) << hole_bound << "|"<<setw(10) << elapsed <<"|"<<setw(12) << c.witness_time << "|" <<setw(10) <<usage.ru_maxrss << "KB" <<"|"<<setw(10) << is_empty_mpda<< endl;
    }
    else{
      string aged;
      if(a.maximum_stack_constant > 1){
        aged = "Yes";
      }
      else{
        aged="No";
      }
    cout << "|" << setw(50)<< "Filename" << "|" << setw(10) << "#States" << "|"<<setw(12) << "#Transitions" << "|"<<setw(10)  << "#Stacks" << "|"<<setw(10) << "clocks"<< "|"<<setw(10) << "Cmax"<< "|"<<setw(10) << "Aged?"<< "|"<<setw(10) << "Hole Bound" << "|"<<setw(10) << "Total Time" <<"|"<<setw(12) << "Witness Time" << "|" <<setw(10) << "Memory(KB)" <<"|"<<setw(10) << "Empty?" << endl;
    cout << "|" << setw(50)<< results[results.size()-1]<< "|" << setw(10) << a.number_of_states << "|"<<setw(12) << a.number_of_transitions << "|"<<setw(10)  << a.number_of_stacks << "|"<<setw(10) << a.list_of_clocks.size() << "|"<<setw(10) << a.max_const << "|"<<setw(10) << aged << "|"<<setw(10) << hole_bound << "|"<<setw(10) << elapsed <<"|"<<setw(12) << c.witness_time << "|" <<setw(10) <<usage.ru_maxrss << "KB" <<"|"<<setw(10) << is_empty_mpda << endl;
    }
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
