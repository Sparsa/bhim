#include<fstream>
#include<iostream>
#include<stdlib.h>
#include "automata.h"

using namespace std;




//using namespace std;


//char inputfilename[100]; // input file name for timed push-down system





void drawsystem(string outfile, const Automata& a) {
	
	
	ofstream tiimeoutfile(outfile); // timed automata output file for showing the automata 		
	tiimeoutfile << "digraph finite_state_machine {\n";
 	//tiimeoutfile << "ratio=\"fill\";\nsize=\"10,10!\";\n";
 	
 	

	tiimeoutfile << "\tnode [shape = point ]; qi" << 0 << ";\n";
	tiimeoutfile << "\tnode [shape = doublecircle];" << a.final_state << ";\n";
	tiimeoutfile << "\tnode [shape=circle];\n"; // all the states other than final states are labelled in a circle
	tiimeoutfile << "\tqi" << "0" << " -> " << a.start_state << ";\n";
	
	for(int i=0; i < a.number_of_transitions; i++) { // take input for all the transitions 		
		int source = a.transition_list[i].source_state; // previous and next state of the i-th transition 		
		int ns = a.transition_list[i].destination_state; 		
		if(!a.transition_list[i].name.empty()){
			tiimeoutfile << "\t" << source << " -> " << ns << " [ label = \"{" << a.transition_list[i].name; // output transition between two states 
		}
		else{
			tiimeoutfile << "\t" << source << " -> " << ns << " [ label = \"{tn:" << (i); // output transition between two states 
		}
		vector<Guards> guard = a.transition_list[i].guard_list; 
		vector<ul> reset =a.transition_list[i].reset_vector;		
		for(auto g : guard){
			int clock = g.clock;
			int lb = g.lower_bound;
			int ub = g.upper_bound; 
			bool openl = g.lower_open;
			bool openu = g.upper_bound;
			if(openl )
			{
				if(openu )
				{
					if(ub == INF) 				
						tiimeoutfile << "," <<  lb << "<" << "x" << clock << "<" << "inf"; 			
					else 				
						tiimeoutfile << "," << lb << "<" << "x" << clock << "<" << ub; 
				}
				else
				{
					if(ub == INF) 				
						tiimeoutfile << "," <<  lb << "<" << "x" << clock << "<=" << "inf"; 			
					else 				
						tiimeoutfile << "," << lb << "<" << "x" << clock << "<=" << ub; 
				}
			}   
			else
			{
				if(openu )
				{
					if(ub == INF) 				
						tiimeoutfile << "," <<  lb << "<=" << "x" << clock << "<" << "inf"; 			
					else 				
						tiimeoutfile << "," << lb << "<=" << "x" << clock << "<" << ub; 
				}
				else
				{
					if(ub == INF) 				
						tiimeoutfile << "," <<  lb << "<=" << "x" << clock << "<=" << "inf"; 			
					else 				
						tiimeoutfile << "," << lb << "<=" << "x" << clock << "<=" << ub; 
				}
			}
		}		
			
		//tiimeoutfile << ",a" << action; // output action number
			 		
		for(auto j: reset) { // reset clocks for i-th transition 'transitions[i]' 
				tiimeoutfile << ",x" << j << ":=0"; 	//***this line will be active other than maze****				
		}
		
		int op = a.transition_list[i].stackOp.op;
		int stack_ps = a.transition_list[i].stackOp.stack_push;
		int stack_alph_ps = a.transition_list[i]. stackOp.stack_alphabet_push;
		int stack_pp = a.transition_list[i].stackOp.stack_pop;
		int stack_alph_pp = a.transition_list[i].stackOp.stack_alphabet_pop;
		bool stack_openu = a.transition_list[i].stackOp.upper_open;
		bool stack_openl = a.transition_list[i].stackOp.lower_open;
		int stack_ub = a.transition_list[i].stackOp.upper_bound;
		int stack_lb = a.transition_list[i].stackOp.lower_bound;
		//stack_symbol = transitions[i].as ;
	
			 
		
		if( op == 1) { // push operation
			tiimeoutfile << ",push "<< stack_alph_ps << " stack " << stack_ps;
		}
		
		else if( op==2) { // pop operation
			tiimeoutfile << ",pop stack " << stack_pp;
			 if(stack_openl )
			{
				if(stack_openu )
				{
					if(stack_ub == INF) 				
						tiimeoutfile << "," <<  stack_lb << "<" <<  "ag(" << stack_alph_pp << ")" << "<" << "inf"; 			
					else 				
						tiimeoutfile << "," << stack_lb << "<" <<  "ag(" << stack_alph_pp << ")" << "<" << stack_ub; 
				}
				else
				{
					if(stack_ub == INF) 				
						tiimeoutfile << "," <<  stack_lb << "<" <<  "ag(" << stack_alph_pp << ")" << "<=" << "inf"; 			
					else 				
						tiimeoutfile << "," << stack_lb << "<" <<  "ag(" << stack_alph_pp << ")" << "<=" << stack_ub; 
				}
			}   
			else
			{
				if(stack_openu )
				{
					if(stack_ub == INF) 				
						tiimeoutfile << "," <<  stack_lb << "<=" <<  "ag(" << stack_alph_pp << ")" << "<" << "inf"; 			
					else 				
						tiimeoutfile << "," << stack_lb << "<=" <<  "ag(" << stack_alph_pp << ")" << "<" << stack_ub; 
				}
				else
				{
					if(stack_ub == INF) 				
						tiimeoutfile << "," <<  stack_lb << "<=" <<  "ag(" << stack_alph_pp << ")" << "<=" << "inf"; 			
					else 				
						tiimeoutfile << "," << stack_lb << "<=" <<  "ag(" << stack_alph_pp << ")" << "<=" << stack_ub; 
				}
			}
		}
		
		else if( op==3 ) { // pop operation
			tiimeoutfile << ",pop " << stack_alph_pp << " stack " << stack_pp;
			 if(stack_openl)
			{
				if(stack_openu )
				{
					if(stack_ub == INF) 				
						tiimeoutfile << "," <<  stack_lb << "<" <<  "ag(" << stack_alph_pp << ")" << "<" << "inf"; 			
					else 				
						tiimeoutfile << "," << stack_lb << "<" <<  "ag(" << stack_alph_pp << ")" << "<" << stack_ub; 
				}
				else
				{
					if(stack_ub == INF) 				
						tiimeoutfile << "," <<  stack_lb << "<" <<  "ag(" << stack_alph_pp << ")" << "<=" << "inf"; 			
					else 				
						tiimeoutfile << "," << stack_lb << "<" <<  "ag(" << stack_alph_pp << ")" << "<=" << stack_ub; 
				}
			}   
			else
			{
				if(stack_openu )
				{
					if(stack_ub == INF) 				
						tiimeoutfile << "," <<  stack_lb << "<=" <<  "ag(" << stack_alph_pp << ")" << "<" << "inf"; 			
					else 				
						tiimeoutfile << "," << stack_lb << "<=" <<  "ag(" << stack_alph_pp << ")" << "<" << stack_ub; 
				}
				else
				{
					if(stack_ub == INF) 				
						tiimeoutfile << "," <<  stack_lb << "<=" <<  "ag(" << stack_alph_pp << ")" << "<=" << "inf"; 			
					else 				
						tiimeoutfile << "," << stack_lb << "<=" <<  "ag(" << stack_alph_pp << ")" << "<=" << stack_ub; 
				}
			}
			tiimeoutfile << ",push "<< stack_alph_ps << " stack " << stack_ps;
		}
		tiimeoutfile << "}\" ];\n"; 
		
	}
		
		
	tiimeoutfile << "}"; // output the last closing brace 	// graph complete for the automata 	tiimeoutfile.close();
}




