/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include<iostream>
#include<string>
#include<tinyxml.h>
#include<regex>
#include<cstdlib>
#include<ostream>
#include <automata.h>
#include<fstream>
#include<unordered_map>
//#include "concatenation.h"
using namespace std;


void
        Guards::printGuard (ostream & output_stream) const
{
    output_stream << clock << " " << lower_bound << " " << lower_open << " " <<
            upper_bound << " " << upper_open << endl;
}

void
Transition::printTransition (ostream & output_stream) const
{
    output_stream << source_state << " " << destination_state << " " << 0 << " "
            << guard_list.size () << " " << reset_vector.size() << endl;
    for (unsigned long i = 0; i < guard_list.size (); i++) {
        guard_list[i].printGuard (output_stream);
    }
    for(unsigned long i=0;i<reset_vector.size();i++){
    output_stream << reset_vector[i] << " ";
    }
    
    //have to write stack information printing code here.
    if (stackOp.op == 0)
        output_stream << 0 << endl;	// no stack operation
    else if (stackOp.op == 1) {	//stack push operation
        output_stream << stackOp.op << " " << stackOp.stack_alphabet_push << endl;
    }
    else if (stackOp.op == 2) {	//stack pop operation
        output_stream << "POP: " << endl;
        output_stream << stackOp.op<< " " << stackOp.stack_pop << " " << stackOp.
                stack_alphabet_pop << " " << stackOp.lower_bound << " " << stackOp.
                lower_open << " " << stackOp.upper_bound << " " << stackOp.
                upper_open << endl;
    }
    else if (stackOp.op == 3) {
        output_stream << stackOp.op << " " << stackOp.
                stack_alphabet_pop << " " << stackOp.lower_bound << " " << stackOp.
                lower_open << " " << stackOp.upper_bound << " " << stackOp.
                upper_open << " " << stackOp.stack_alphabet_push << endl;
    }
    output_stream << endl;
}

void
Automata::printAutomata (ostream & output_stream)
{
    output_stream << number_of_states << " " << number_of_transitions << " " <<
            list_of_clocks.size() << " " << number_of_alphabet << " " <<
            number_of_stack_alphabet << endl;
    output_stream << start_state << endl;
    output_stream << final_state << endl;
    for (unsigned long i = 0; i < number_of_transitions; i++) {
        transition_list[i].printTransition (output_stream);
    }
}

/* Automata::Automata (char *inputfilename)
{
    
    
   
}
 */






bool Transition::checkGuards(vector<unsigned long> valuation) const { // this function checks the given valuation for if the current valuation satisfies the guard in the transition or not.
    //the valuation matrix that I send here have 
    vector<Guards>::const_iterator guard_list_it;
    
    for(guard_list_it = guard_list.begin();guard_list_it!=guard_list.end();++guard_list_it){
        
        if((*guard_list_it).diagonal){ // if the guard is diagonal, we are assuming that the guards are closed.
            if(valuation[(*guard_list_it).clock-1] - valuation[(*guard_list_it).clock_diag-1] >= (*guard_list_it).lower_bound && valuation[(*guard_list_it).clock-1] - valuation[(*guard_list_it).clock_diag-1] <= (*guard_list_it).upper_bound){
                continue; // if the condigion is true we will move forward to check the next guard
            }
            else{
                return false; // if any one of the guard does not satisfy we will return false
            }
        }
        else{
            if(valuation[(*guard_list_it).clock-1] >= (*guard_list_it).lower_bound && valuation[(*guard_list_it).clock-1] <= (*guard_list_it).upper_bound){
                continue;
            }
            else{
                return false;
            }
        }
        
    }
    return true;
}

vector<unsigned long> Transition::afterReset(vector<unsigned long> valuation) const {
    // here we have to check for rests happened in the automata. but the resets are stored as bits ,we can store it as a vector and work on that.
    //in this transition we have a list called reset_vector which keeps the list of all clocks which are reset in this transition.
    vector<unsigned long> new_valuation;// after reset valuation
    vector<unsigned long>::iterator valuation_it; // int vector iterator
    for(valuation_it = valuation.begin();valuation_it != valuation.end();valuation_it++){ // iterating over the valuation vector
        new_valuation.push_back((*valuation_it)); // copy all the old valuation in the new one.
    }
    vector<unsigned long>::const_iterator reset_vector_iterator;
    for (reset_vector_iterator = reset_vector.begin();reset_vector_iterator!=reset_vector.end();reset_vector_iterator++){ // for all clocks in the reset vector we will reset the clock
        new_valuation[(*reset_vector_iterator)-1] = 0; // reset the new valuation to 0 for the clocks who are reset.
    }
    return new_valuation;// return the new_valuation vector containing valuation after reset.
}

bool Transition::firable(states s) const{ // given a states pair this function checks if the transition is firable or not from the states.
    if(s.first == source_state -1 && checkGuards(s.second)){ // check if the source is equal to the state given and the guards in the transition satisfies the valuation vector in the state.
        return true;
    }
    return false;
}

set<ul> Transition::checkStackConstraint(set<unsigned long> s) const{
    set<ul> returnSet;
    //set<unsigned long>::iterator set_it;
//    cout << "Source State " << source_state << " destination state " << destination_state ;
//    cout << "Lower bound: " << stackOp.lower_bound << " Upper bound: " << stackOp.upper_bound << " " ;
    for(auto set_it=s.begin();set_it!=s.end();++set_it){
        if(checkStackConstraint(*(set_it))){
            //cout << *(set_it) ;
            returnSet.insert(*(set_it));
        }
    }
    //cout << endl;
    return returnSet;
}
bool Transition::checkStackConstraint(unsigned long time) const{
    /* if(stackOp.upper_bound == 4 ){
        cout << stackOp.lower_bound << " " << time<< endl;
    } */
        if(stackOp.upper_bound == stackOp.lower_bound && stackOp.upper_bound == time){
            return true;
        }
   
        if( time >= stackOp.lower_bound && time <=stackOp.upper_bound){
            //cout << *(set_it) ;
            return true;
        }
    
    //cout << endl;
    return false;
}

bool Automata::push_pop_check(unsigned long push,unsigned long pop) const{ // push is the index of push transition and pop is the index of pop trasition.
    if(transition_list[push].stackOp.stack_alphabet_push == transition_list[pop].stackOp.stack_alphabet_pop && transition_list[push].stackOp.stack_push == transition_list[pop].stackOp.stack_pop){
        return true;
        
    }
    return false;
}
std::vector<std::string> split(std::string str,std::string sep){
    char* cstr=const_cast<char*>(str.c_str());
    char* current;
    std::vector<std::string> arr;
    current=strtok(cstr,sep.c_str());
    while(current!=NULL){
        arr.push_back(current);
        current=strtok(NULL,sep.c_str());
    }
    return arr;
}

ul search_map_return(string input,unordered_map<string,unsigned long> &obj,ul &count){
    if(obj[input]){
        return obj[input];
    }
    else{
        obj[input]= ++count;
        return count;
    }

}
Automata::Automata(char* filename, int type){ // when the type is 0 then 
    
    if(type == 1){ // boolean (single threaded) program

    }
    if(type == 2){ // concurrent boolean program
        final_state = -1;
        number_of_stacks=0;
        max_const = 0;
        vector<ul> end_list;
        unordered_map<string,unsigned long> config_map;
        unordered_map<string,unsigned int> stack_alpha;
        unordered_map<string,unsigned int> stack_map;
        unordered_map<string,unsigned int> end_states;
        unsigned long no_config=0;
        unsigned long no_of_trans=0;
        unsigned int no_stack_alpha=0;
        unsigned int stacks=0;
        Transition t;
        ifstream infile1(filename);
        string each_line;
        
        while(getline(infile1,each_line)){
            
            vector<string> line_tokens = split(each_line," ");
            switch(line_tokens.size() ){
                //this is for marking the start config
                case 2:
                if(line_tokens[0] == "START:"){
                    
                    config_map[line_tokens[1]] = ++no_config;
                    start_state = no_config;
                }
                if(line_tokens[0]=="END:"){
                    if(config_map[line_tokens[1]]){
                        if(!end_states[line_tokens[1]]){
                            end_list.push_back(config_map[line_tokens[1]]);
                            end_states[line_tokens[1]]=config_map[line_tokens[1]];
                        }
                        
                    }
                    else{//how to make sure I am adding multiple things here.
                        config_map[line_tokens[1]]= ++no_config;
                        end_states[line_tokens[1]] = no_config;
                        end_list.push_back(no_config);
                    }
                }
                break;
                case 6: 
                no_of_trans++;
                if(line_tokens[3] == "NOP"){
                    StackOp op;
                    op.op = 0;
                    t.stackOp=op; //mark it as NOP operations.
                    t.name=each_line;//mark the transition as the name of the tansition.
                    t.name+="\n";
                    if(config_map[line_tokens[0]]){
                        t.source_state = config_map[line_tokens[0]];
                    }
                    else{
                        config_map[line_tokens[0]] = ++no_config;
                        t.source_state = no_config;
                    }
                    if(config_map[line_tokens[2]]){
                        t.destination_state = config_map[line_tokens[2]];
                    }
                    else{
                        config_map[line_tokens[2]] = ++no_config;
                        t.destination_state = no_config;
                    }
                    
                }
                transition_list.push_back(t);//add this new transition to the transition list.
                break;
                case 9:
                no_of_trans++;
                Transition t;
                StackOp op;
                if(line_tokens[3] == "PUSH"){
                    op.op = 1;
                    if(config_map[line_tokens[0]]){
                        t.source_state = config_map[line_tokens[0]];
                    }
                    else{
                        config_map[line_tokens[0]] = ++no_config;
                        t.source_state = no_config;
                    }
                    if(config_map[line_tokens[2]]){
                        t.destination_state = config_map[line_tokens[2]];
                    }
                    else{
                        config_map[line_tokens[2]] = ++no_config;
                        t.destination_state = no_config;
                    }
                    if(stack_alpha[line_tokens[4]]){
                        op.stack_alphabet_push=stack_alpha[line_tokens[4]];
                    }
                    else{
                        stack_alpha[line_tokens[4]]=++no_stack_alpha;
                        op.stack_alphabet_push=no_stack_alpha;
                    }
                    op.stack_push = stoi(line_tokens[6])+1;//do I need to do a plus one to it?
                    if(!stack_map[line_tokens[6]]){
                        number_of_stacks++;
                        stack_map[line_tokens[6]] = number_of_stacks;
                    }

                }
            
                else if (line_tokens[3] == "POP"){
                    op.op=2;
                    op.lower_bound=0;
                    op.upper_bound=0;
                    op.lower_open=0;
                    op.upper_open=0;
                    if(config_map[line_tokens[0]]){
                        t.source_state = config_map[line_tokens[0]];
                    }
                    else{
                        config_map[line_tokens[0]] = ++no_config;
                        t.source_state = no_config;
                    }
                    if(config_map[line_tokens[2]]){
                        t.destination_state = config_map[line_tokens[2]];
                    }
                    else{
                        config_map[line_tokens[2]] = ++no_config;
                        t.destination_state = no_config;
                    }
                    if(stack_alpha[line_tokens[4]]){
                        op.stack_alphabet_pop=stack_alpha[line_tokens[4]];
                    }
                    else{
                        stack_alpha[line_tokens[4]]=++no_stack_alpha;
                        op.stack_alphabet_pop=no_stack_alpha;
                    }
                     if(!stack_map[line_tokens[6]]){
                        number_of_stacks++;
                        stack_map[line_tokens[6]] = number_of_stacks;
                    }
                    
                }
                t.stackOp=op;
                t.name = each_line;
                t.name+="\n";
                transition_list.push_back(t);
                break;
                

            }
            
            
        }
        final_state = ++no_config;
        for(auto i = 0; i < end_list.size();i++){
            StackOp op;
            op.op = 0;
            t.stackOp=op; //mark it as NOP operations.
            t.name="Epsilon";//mark the transition as the name of the tansition.
            t.source_state = end_list[i];
            t.destination_state = final_state;
            //cout << "The number of transitions of the current MPDA is " << transition_list.size() << endl;
            transition_list.push_back(t);
            //cout << "The number of transitions of the current MPDA is " << transition_list.size() << endl;
            //t.printTransition(cout);
        }
        //cout << "The number of transitions of the current MPDA is " << transition_list.size() << endl;
        for (pair<string, unsigned int> element : stack_alpha){
            for(int j = 0; j<number_of_stacks; j++){
                StackOp  op;
                op.op=2;
                op.lower_bound=0;
                op.upper_bound=0;
                op.lower_open=0;
                op.upper_open=0;
                op.stack_alphabet_pop = element.second;
                op.stack_pop = j;
                t.stackOp=op;
                t.name = "Epsilon";
                t.source_state = final_state;
                t.destination_state = final_state;
                //cout << "The number of transitions of the current MPDA is " << transition_list.size() << endl;
                transition_list.push_back(t);
                //cout << "The number of transitions of the current MPDA is " << transition_list.size() << endl;
                //t.printTransition(cout);
            }
	        
            //cout << "The number of transitions of the current MPDA is " << transition_list.size() << endl;

        }
        number_of_states=no_config;
        number_of_transitions=transition_list.size();
        //number_of_stacks = 2; // we also need to take care of the final state, and if the final state.in
        number_of_stack_alphabet=no_stack_alpha;
        number_of_alphabet=0;
        //create a new state which will be a new state and 
        //infile.close();

    }
    if(type == 3){ //uppal format input
     //maximum_constant =0;
        this->diagonal = false;
        //ifstream tiimeinfile ("in"); // timed system input file stream
        //ifstream tiimeinfile (inputfilename); // timed system input file stream
        
        
        // if there is any error opening the file           
        
        
        
        TiXmlDocument doc (filename);	// create a tinyxml doc file
        
            if( !(doc.LoadFile ()) )  { 
                cout << "\n****INPUT TIMED PUSH-DOWN FILE NOT FOUND ! ****\n"  << endl;
                cout << filename << endl;          
                exit(1);
            }
        
                // parse the file
        TiXmlElement *root = doc.RootElement ();	// root RootElement
        TiXmlElement *element;
        //cout << root->Value() << endl; 
        root = root->FirstChildElement ();	// now the root is in declaration
        //cout << root->Value() << endl;
        root = root->NextSiblingElement ();	// now the root is in template.
        //cout << root->Value() << endl;
        element = root->FirstChildElement ();	// this is the first tag called name
        //cout << element->Value() << endl; 
        while (strcmp (element->Value (), "location")) {
            element = element->NextSiblingElement ();	// this is declaration element inside template
        }
        unsigned long states = 0;
        //element = element->NextSiblingElement();
        string final = "-";
        for (; strcmp (element->Value (), "location") == 0; element = element->NextSiblingElement (), states++)	// this is location.
        {
            //cout << element->Value() << endl;
            //cout << element->Attribute("id") << endl;
            TiXmlElement *name = element->FirstChildElement ();	//extract the name of first child element
            if (name != nullptr) {
                if (strcmp (name->Value (), "name") == 0) {
                    TiXmlNode *n = name->FirstChild ();
                    if (strcmp (n->Value (), "end") == 0)
                        final = element->Attribute ("id");
                }
            }
        }
        
        //cout << "Total States of the Automaton: " << states << endl;
        //Automata a;
        //map<string,int> map_of_clocks; // map to get the clock number 
        map < string, int >map_of_stack_alphabet;	// map to get the stack_alphabet_number
        map < string, int >map_of_stack;	// count and give a number to the stacks used in the timed automata.
        map<string,int> map_of_clocks;
        map<int,int> map_max_constraints; // a map that maps clock to its maximum constraints.
        unsigned long number_of_clocks = 0;	// total number of clocks used
        number_of_stacks = 0;
        maximum_stack_constant = 0; // initialize it to zero
        number_of_states = states;	// numbe of states assingment in the automata
        //have to get the initial state
        //maximum_constant = 0;
        start_state = atoi (element->Attribute ("ref") + 2) + 1;	//first plus 2 increments pointer to the string, second +1 increments the integer
        element = element->NextSiblingElement ();	// start of transition;
        int no_of_transitions = 0;
        int stack_alphabet_number = 0;
        //int stack_alphabet_number =0;
        //vector<Transition>* transitions = new vector<Transition>();
        
        for (; element && strcmp (element->Value (), "transition") == 0;
                element = element->NextSiblingElement (), no_of_transitions++) {
            TiXmlElement *tElement = element->FirstChildElement ();
            //cout << tElement->Attribute("ref");
            //tElement = tElement->NextSiblingElement();
            //cout << tElement->Attribute("ref");
            //tElement = tElement->NextSiblingElement();
            //int number_of_reset_in_tr=0;
            Transition t;
            t.stackOp.op = 0;		//assume initially that no operaiton is there.
            t.number_of_reset = 0;
            while (tElement) {
                
                if (strcmp (tElement->Value (), "source") == 0)	//source of transition
                {
                    //this is a source of the transition.
                    //cout << tElement->Attribute("ref") << endl;
                    t.source_state = atoi (tElement->Attribute ("ref") + 2) + 1;
                    //cout  << t.source_state << endl;
                }
                else if (strcmp (tElement->Value (), "target") == 0)	//target of transition
                {
                    //this is the target of the transision
                    //cout << tElement->Attribute("ref") << endl;
                    t.destination_state = atoi (tElement->Attribute ("ref") + 2) + 1;
                    //cout << t.destination_state << endl;
                }
                else if (strcmp (tElement->Value (), "label") == 0)	//label of the transition
                {
                    //check if this label is guard
                    if (strcmp (tElement->Attribute ("kind"), "guard") == 0)	//if the attribute kind is guard
                    {
                        //extract the information inside the tag.
                        //vector<Guards>* list_of_guards = new vector<Guards>();
                        //cout << "guard" << endl;
                        TiXmlNode *guards = tElement->FirstChild ();
                        //cout << guards->Value() << endl;
                        string s = guards->Value ();
                        s.erase (remove_if (s.begin (), s.end (),::isspace), s.end ());	// removing all the spaces from the string
                        smatch cm, sm;	// list of matches
                        //stringstream ss (guards->Value()); //if more than one comma separated guards available
                        //split the guards and crete individual strings of guards.
                        regex rgx ("&&");	//using regular expression to split the string to substrings
                        vector < string > string_vector;	// after splitting the substrings should be stored in here
                        sregex_token_iterator iter (s.begin (), s.end (), rgx, -1);	//the splitting operation depending on the regular expression
                        //above statements splits a string in to tokens depending on the rgx value, here it will
                        //check for && and split strings in to part
                        sregex_token_iterator end;	//the last iterator
                        for (; iter != end; ++iter) {
                            string_vector.push_back (*iter);	//pusing the substrings to the vector
                        }
                        regex e_lessthan ("(.*)<(\\d*)");	//regular expression for only less than operation
                        regex e_lessthan_eq ("(.*)<=(\\d*)");	// regular expression for only less than equal to operation
                        regex e_greaterthan ("(.*)>(\\d*)");	// regular expression for greater than expression
                        regex e_greaterthan_eq ("(.*)>=(\\d*)");	//regular expression for greater than equalto expression
                        regex equality ("(.*)==(\\d*)");
                        regex diagonal ("(.*)-(.*)");	//this is the regular expression to check for diagonal constraint and extract the clocks from diagonal constraint. any one can be pop so pop cant be a name of diagolan constraint
                        map < int, Guards > map_clock_guard;
                        for (unsigned i = 0; i < string_vector.size (); ++i) {
                            Guards g;
                            //cout << string_vector[i] << endl;
                            //         string s1(cm[i].str());
                            //        cout << s1<< endl;
                            if (regex_match (string_vector[i], sm, equality)) {
                                //cout << "equality Matched " << endl;
                                string temporary = sm[1].str ();
                                if (regex_match (temporary, cm, diagonal))	// if the constraint is diagonal constraint .
                                {
                                    //write a code for diagonal element
                                    g.diagonal = true;	//setting diagonal flag to true its default value is false so we dont need to change it for others.
                                    this->diagonal= true; // mark the flag in the automata telling that the automata has atleast one diagonal constriant.
                                    if (map_of_clocks[cm[1].str ()])	//first clock of diagonal constraint exists in the map
                                    {
                                        g.clock = map_of_clocks[cm[1].str ()];
                                    }
                                    else {
                                        number_of_clocks++;	// increment the number of clocks
                                        list_of_clocks.push_back(cm[1].str());
                                        //maximum_constant.push_back(0); //the initial value of the clocks is this.
                                        map_max_constraints[number_of_clocks] = 0; // first initialize to zero.
                                        map_of_clocks[cm[1].str ()] = number_of_clocks;
                                        g.clock = number_of_clocks;
                                    }
                                    if (map_of_clocks[cm[2].str ()])	//second clock of diagonal constraint exists in the map
                                    {
                                        g.clock_diag = map_of_clocks[cm[2].str ()];
                                    }
                                    else {
                                        number_of_clocks++;	// increment the number of clocks
                                        list_of_clocks.push_back(cm[2].str());
                                        //maximum_constant.push_back(0); // same as above.
                                        map_max_constraints[number_of_clocks] = 0; // first initialize the maximum constraint to 0.
                                        map_of_clocks[cm[2].str ()] = number_of_clocks;
                                        g.clock_diag = number_of_clocks;
                                    }
                                    g.lower_bound = g.upper_bound = stoi (sm[2].str ());
                                    map_max_constraints[map_of_clocks[cm[2].str ()]] = max (map_max_constraints[map_of_clocks[cm[2].str ()]], g.lower_bound);	//updating maximum constraint
                                    map_max_constraints[map_of_clocks[cm[1].str ()]] = max (map_max_constraints[map_of_clocks[cm[1].str ()]], g.lower_bound);
                                    //cout << "Equality " << "Value :" << sm[2] << endl;
                                    g.lower_open = g.upper_open = false;
                                    t.guard_list.push_back (g);
                                }
                                else {
                                    if (map_of_clocks[sm[1].str ()])	// if exists
                                    {
                                        g.clock = map_of_clocks[sm[1].str ()];
                                    }
                                    else {
                                        number_of_clocks++;
                                        list_of_clocks.push_back(sm[1].str());
                                        map_max_constraints[number_of_clocks] = 0;
                                        map_of_clocks[sm[1].str ()] = number_of_clocks;
                                        g.clock = number_of_clocks;
                                    }
                                    if (map_clock_guard.count (g.clock))	// if we already added this clock to the map.
                                    {
                                        if (map_clock_guard[g.clock].upper_bound >
                                                stoi (sm[2].str ())
                                                || map_clock_guard[g.clock].upper_bound < 0) {
                                            map_clock_guard[g.clock].upper_bound =
                                                    stoi (sm[2].str ());
                                            map_clock_guard[g.clock].upper_open = false;
                                            map_max_constraints[g.clock]  = max (map_max_constraints[g.clock], map_clock_guard[g.clock].upper_bound);	//updating maximum constraint
                                        }
                                        if (map_clock_guard[g.clock].lower_bound <
                                                stoi (sm[2].str ())) {
                                            map_clock_guard[g.clock].lower_bound =
                                                    stoi (sm[2].str ());
                                            map_clock_guard[g.clock].lower_open = false;
                                            map_max_constraints[g.clock] = max (map_max_constraints[g.clock], map_clock_guard[g.clock].lower_bound);	//updating maximum constraint
                                        }
                                        //                                     else if(map_clock_guard[g.clock].upper_bound == stoi(sm[2].str()) && map_clock_guard[g.clock].upper_open == false)
                                        //                                         map_clock_guard[g.clock].upper_open = true;
                                        
                                    }
                                    else {
                                        g.lower_bound = g.upper_bound = stoi (sm[2].str ());
                                        map_max_constraints[g.clock] = max (map_max_constraints[g.clock], g.lower_bound);	//updating maximum constraint
                                        //cout << "Equality " << "Value :" << sm[2] << endl;
                                        g.lower_open = g.upper_open = false;
                                        //                                     g.lower_bound = 0;
                                        //                                     g.lower_open = false;
                                        map_clock_guard[g.clock] = g;
                                    }
                                    //t.guard_list.push_back(g);
                                }
                            }
                            else if (regex_match (string_vector[i], sm, e_lessthan)) {
                                //cout << "less than matched " << endl;
                                string temporary = sm[1].str ();
                                if (regex_match (temporary, cm, diagonal))	// if the constraint is diagonal constraint .
                                {
                                    //write a code for diagonal element
                                    g.diagonal = true;	//setting diagonal flag to true its default value is false so we dont need to change it for others.
                                    this->diagonal = true;// if at least one diagonal constraint 
                                    if (map_of_clocks[cm[1].str ()])	//first clock of diagonal constraint exists in the map
                                    {
                                        g.clock = map_of_clocks[cm[1].str ()];
                                    }
                                    else {
                                        number_of_clocks++;	// increment the number of clocks
                                        list_of_clocks.push_back(cm[1].str());
                                        map_max_constraints[number_of_clocks] = 0;
                                        map_of_clocks[cm[1].str ()] = number_of_clocks;
                                        g.clock = number_of_clocks;
                                    }
                                    if (map_of_clocks[cm[2].str ()])	//second clock of diagonal constraint exists in the map
                                    {
                                        g.clock_diag = map_of_clocks[cm[2].str ()];
                                    }
                                    else {
                                        number_of_clocks++;	// increment the number of clocks
                                        list_of_clocks.push_back(cm[2].str());
                                        map_max_constraints[number_of_clocks] = 0;
                                        map_of_clocks[cm[2].str ()] = number_of_clocks;
                                        g.clock_diag = number_of_clocks;
                                    }
                                    g.upper_bound = stoi (sm[2].str ());
                                    map_max_constraints[g.clock] = max (map_max_constraints[g.clock], g.lower_bound);
                                    map_max_constraints[g.clock_diag] = max (map_max_constraints[g.clock_diag], g.lower_bound);	//updating maximum constraint
                                    g.upper_open = true;
                                    g.lower_bound = 0;
                                    g.lower_open = false;
                                    t.guard_list.push_back (g);
                                }
                                else {
                                    if (map_of_clocks[sm[1].str ()])	// if exists
                                    {
                                        g.clock = map_of_clocks[sm[1].str ()];
                                    }
                                    else {
                                        number_of_clocks++;
                                        list_of_clocks.push_back(sm[1].str());
                                        map_max_constraints[number_of_clocks] = 0;
                                        map_of_clocks[sm[1].str ()] = number_of_clocks;
                                        g.clock = number_of_clocks;
                                    }
                                    if (map_clock_guard.count (g.clock))	// if we already added this clock to the map.
                                    {
                                        if (map_clock_guard[g.clock].upper_bound >
                                                stoi (sm[2].str ())
                                                || map_clock_guard[g.clock].upper_bound < 0) {
                                            map_clock_guard[g.clock].upper_bound =
                                                    stoi (sm[2].str ());
                                            map_clock_guard[g.clock].upper_open = true;
                                            map_max_constraints[g.clock] = max (map_max_constraints[g.clock], map_clock_guard[g.clock].upper_bound);	//updating maximum constraint
                                        }
                                        else if (map_clock_guard[g.clock].upper_bound ==
                                                stoi (sm[2].str ())
                                                && map_clock_guard[g.clock].upper_open == false)
                                            map_clock_guard[g.clock].upper_open = true;
                                        
                                    }
                                    else {
                                        g.upper_bound = stoi (sm[2].str ());
                                        map_max_constraints[g.clock] = max (map_max_constraints[g.clock], g.upper_bound);	//updating maximum constraint
                                        g.upper_open = true;
                                        g.lower_bound = 0;
                                        g.lower_open = false;
                                        map_clock_guard[g.clock] = g;
                                    }
                                    //t.guard_list.push_back(g);
                                }
                                
                            }
                            else if (regex_match (string_vector[i], sm, e_greaterthan)) {
                                //cout << "greater than matched " << endl;
                                string temporary = sm[1].str ();
                                if (regex_match (temporary, cm, diagonal))	// if the constraint is diagonal constraint .
                                {
                                    //write a code for diagonal element
                                    g.diagonal = true;	//setting diagonal flag to true its default value is false so we dont need to change it for others.
                                    this->diagonal = true;
                                    if (map_of_clocks[cm[1].str ()])	//first clock of diagonal constraint exists in the map
                                    {
                                        g.clock = map_of_clocks[cm[1].str ()];
                                    }
                                    else {
                                        number_of_clocks++;	// increment the number of clocks
                                        list_of_clocks.push_back(cm[1].str());
                                        map_max_constraints[number_of_clocks] = 0;
                                        map_of_clocks[cm[1].str ()] = number_of_clocks;
                                        g.clock = number_of_clocks;
                                    }
                                    if (map_of_clocks[cm[2].str ()])	//second clock of diagonal constraint exists in the map
                                    {
                                        g.clock_diag = map_of_clocks[cm[2].str ()];
                                    }
                                    else {
                                        number_of_clocks++;	// increment the number of clocks
                                        list_of_clocks.push_back(cm[2].str());
                                        map_max_constraints[number_of_clocks] = 0;
                                        map_of_clocks[cm[2].str ()] = number_of_clocks;
                                        g.clock_diag = number_of_clocks;
                                    }
                                    g.lower_bound = stoi (sm[2].str ());
                                    map_max_constraints[g.clock_diag] = max(map_max_constraints[g.clock_diag],g.lower_bound);
                                    map_max_constraints[g.clock] = max(map_max_constraints[g.clock],g.lower_bound);
                                    //maximum_constant = max (maximum_constant, g.lower_bound);	//updating maximum constraint
                                    g.lower_open = true;
                                    g.upper_bound = INF;
                                    g.upper_open = true;
                                    t.guard_list.push_back (g);
                                }
                                else {
                                    if (map_of_clocks[sm[1].str ()])	// if exists
                                    {
                                        g.clock = map_of_clocks[sm[1].str ()];
                                    }
                                    else {
                                        number_of_clocks++;
                                        list_of_clocks.push_back(sm[1].str());
                                        map_max_constraints[number_of_clocks] = 0;
                                        map_of_clocks[sm[1].str ()] = number_of_clocks;
                                        g.clock = number_of_clocks;
                                    }
                                    if (map_clock_guard.count (g.clock))	// if we already added this clock to the map.
                                    {
                                        if (map_clock_guard[g.clock].lower_bound <
                                                stoi (sm[2].str ())) {
                                            map_clock_guard[g.clock].lower_bound =
                                                    stoi (sm[2].str ());
                                            map_clock_guard[g.clock].lower_open = true;
                                            map_max_constraints[g.clock] = max (map_max_constraints[g.clock], map_clock_guard[g.clock].lower_bound);	//updating maximum constraint
                                        }
                                        else if (map_clock_guard[g.clock].lower_bound ==
                                                stoi (sm[2].str ())
                                                && map_clock_guard[g.clock].lower_open == false)
                                            map_clock_guard[g.clock].lower_open = true;
                                        
                                    }
                                    else {
                                        g.lower_bound = stoi (sm[2].str ());
                                        map_max_constraints[g.clock] = max (map_max_constraints[g.clock], g.lower_bound);
                                        g.lower_open = true;
                                        g.upper_bound = INF;
                                        g.upper_open = true;
                                        map_clock_guard[g.clock] = g;
                                    }
                                    //t.guard_list.push_back(g);
                                }
                                
                            }
                            else if (regex_match (string_vector[i], sm, e_lessthan_eq)) {
                                //cout << "less than equalto matched " << endl;
                                string temporary = sm[1].str ();
                                if (regex_match (temporary, cm, diagonal))	// if the constraint is diagonal constraint .
                                {
                                    //write a code for diagonal element
                                    g.diagonal = true;	//setting diagonal flag to true its default value is false so we dont need to change it for others.
                                    this->diagonal = true;
                                    if (map_of_clocks[cm[1].str ()])	//first clock of diagonal constraint exists in the map
                                    {
                                        g.clock = map_of_clocks[cm[1].str ()];
                                    }
                                    else {
                                        number_of_clocks++;	// increment the number of clocks
                                        list_of_clocks.push_back(cm[1].str());
                                        map_max_constraints[number_of_clocks] = 0;
                                        map_of_clocks[cm[1].str ()] = number_of_clocks;
                                        g.clock = number_of_clocks;
                                    }
                                    if (map_of_clocks[cm[2].str ()])	//second clock of diagonal constraint exists in the map
                                    {
                                        g.clock_diag = map_of_clocks[cm[2].str ()];
                                    }
                                    else {
                                        number_of_clocks++;	// increment the number of clocks
                                        list_of_clocks.push_back(cm[2].str());
                                        map_max_constraints[number_of_clocks] = 0;
                                        map_of_clocks[cm[2].str ()] = number_of_clocks;
                                        g.clock_diag = number_of_clocks;
                                    }
                                    g.upper_bound = stoi (sm[2].str ());
                                    map_max_constraints[g.clock] = max (map_max_constraints[g.clock], g.upper_bound);
                                    g.upper_open = false;
                                    g.lower_bound = 0;
                                    g.lower_open = false;
                                    t.guard_list.push_back (g);
                                }
                                else {
                                    if (map_of_clocks[sm[1].str ()])	// if exists
                                    {
                                        g.clock = map_of_clocks[sm[1].str ()];
                                    }
                                    else {
                                        number_of_clocks++;
                                        list_of_clocks.push_back(sm[1].str());
                                        map_max_constraints[number_of_clocks] = 0;
                                        map_of_clocks[sm[1].str ()] = number_of_clocks;
                                        g.clock = number_of_clocks;
                                    }
                                    if (map_clock_guard.count (g.clock))	// if we already added this clock to the map.
                                    {
                                        if (map_clock_guard[g.clock].upper_bound >
                                                stoi (sm[2].str ())
                                                || map_clock_guard[g.clock].upper_bound < 0) {
                                            map_clock_guard[g.clock].upper_bound =
                                                    stoi (sm[2].str ());
                                            map_max_constraints[g.clock] =
                                                    max (map_max_constraints[g.clock],
                                                    map_clock_guard[g.clock].upper_bound);
                                            map_clock_guard[g.clock].upper_open = false;
                                        }
                                        //                                     else if(map_clock_guard[g.clock].upper_bound == stoi(sm[2].str()) && map_clock_guard[g.clock].upper_open == true)
                                        //                                         map_clock_guard[g.clock].upper_open = true;
                                        
                                    }
                                    else {
                                        g.upper_bound = stoi (sm[2].str ());
                                        map_max_constraints[g.clock] = max (map_max_constraints[g.clock], g.upper_bound);
                                        g.upper_open = false;
                                        g.lower_bound = 0;
                                        g.lower_open = false;
                                        map_clock_guard[g.clock] = g;
                                    }
                                    //t.guard_list.push_back(g);
                                }
                            }
                            else if (regex_match (string_vector[i], sm, e_greaterthan_eq)) {
                                //cout << "greater than equalto matched " << endl;
                                string temporary = sm[1].str ();
                                if (regex_match (temporary, cm, diagonal))	// if the constraint is diagonal constraint .
                                {
                                    //write a code for diagonal element
                                    g.diagonal = true;	//setting diagonal flag to true its default value is false so we dont need to change it for others.
                                    this->diagonal = true;
                                    if (map_of_clocks[cm[1].str ()])	//first clock of diagonal constraint exists in the map
                                    {
                                        g.clock = map_of_clocks[cm[1].str ()];
                                    }
                                    else {
                                        number_of_clocks++;	// increment the number of clocks
                                        list_of_clocks.push_back(cm[1].str());
                                        map_max_constraints[number_of_clocks] = 0;
                                        map_of_clocks[cm[1].str ()] = number_of_clocks;
                                        g.clock = number_of_clocks;
                                    }
                                    if (map_of_clocks[cm[2].str ()])	//second clock of diagonal constraint exists in the map
                                    {
                                        g.clock_diag = map_of_clocks[cm[2].str ()];
                                    }
                                    else {
                                        number_of_clocks++;	// increment the number of clocks
                                        list_of_clocks.push_back(cm[2].str());
                                        map_max_constraints[number_of_clocks] = 0;
                                        map_of_clocks[cm[2].str ()] = number_of_clocks;
                                        g.clock_diag = number_of_clocks;
                                    }
                                    g.lower_bound = stoi (sm[2].str ());
                                    map_max_constraints[g.clock] = max (map_max_constraints[g.clock], g.lower_bound);
                                    //cout << "e_greaterthan_eq " << "Value :" << sm[2] << endl;
                                    g.lower_open = false;
                                    g.upper_bound = INF;
                                    g.upper_open = true;
                                    t.guard_list.push_back (g);
                                    
                                }
                                else {
                                    if (map_of_clocks[sm[1].str ()])	// if exists
                                    {
                                        g.clock = map_of_clocks[sm[1].str ()];
                                    }
                                    else {
                                        number_of_clocks++;
                                        list_of_clocks.push_back(sm[1].str());
                                        map_max_constraints[number_of_clocks ] = 0;
                                        map_of_clocks[sm[1].str ()] = number_of_clocks;
                                        g.clock = number_of_clocks;
                                    }
                                    if (map_clock_guard.count (g.clock))	// if we already added this clock to the map.
                                    {
                                        if (map_clock_guard[g.clock].lower_bound <
                                                stoi (sm[2].str ())) {
                                            map_clock_guard[g.clock].lower_bound =
                                                    stoi (sm[2].str ());
                                            map_max_constraints[g.clock] =
                                                    max (map_max_constraints[g.clock],
                                                    map_clock_guard[g.clock].lower_bound);
                                            map_clock_guard[g.clock].lower_open = false;
                                        }
                                        //                                     else if(map_clock_guard[g.clock].lower_bound == stoi(sm[2].str()) && map_clock_guard[g.clock].lower_open == true)
                                        //                                         map_clock_guard[g.clock].lower_open = true;
                                        //                                         
                                    }
                                    else {
                                        g.lower_bound = stoi (sm[2].str ());
                                        map_max_constraints[g.clock] =
                                                max (map_max_constraints[g.clock], g.lower_bound);
                                        //cout << "e_greaterthan_eq " << "Value :" << sm[2] << endl;
                                        g.lower_open = false;
                                        g.upper_bound = INF;
                                        g.upper_open = true;
                                        map_clock_guard[g.clock] = g;
                                    }
                                    //t.guard_list.push_back(g);
                                }
                            }
                            
                        }
                        
                        
                        for (map < int, Guards >::iterator it = map_clock_guard.begin ();
                                it != map_clock_guard.end (); ++it)
                            t.guard_list.push_back (it->second);
                        
                    }
                    else if (strcmp (tElement->Attribute ("kind"), "assignment") == 0) {
                        //extract the clocks which are reset in this transition.
                        //vector<int>* list_of_reset = new vector<int>();
                        
                        //cout << "reset" << endl;
                        TiXmlNode *resets = tElement->FirstChild ();
                        //cout << resets->Value () << endl;
                        regex reset ("(.*):=(\\d*)");	//= and := both used in uppaal for assignment
                        regex reset1 ("(.*)=(\\d*)");
                        string s (resets->Value ());
                        s.erase (remove_if (s.begin (), s.end (),::isspace), s.end ());	//remove all the unwanted spaces from the string
                        smatch resetmatch;
                        //t.reset_list = getResets(resets->Value());
                        stringstream ss (s);	//if more than one comma separated guards available
                        //split the guards and crete individual strings of guards.
                        vector < string > result;
                        while (ss.good ()) {
                            string substr;
                            getline (ss, substr, ',');	//separating the string based on the comma
                            result.push_back (substr);
                            //cout << substr << endl;;
                        }
                        t.number_of_reset = result.size ();
                        for (unsigned long i = 0; i < result.size (); i++) {
                            if (regex_match (result[i], resetmatch, reset) || regex_match (result[i], resetmatch, reset1))	// if any one of the above regular expression matches
                            {
                                if (map_of_clocks[resetmatch[1].str ()]){	// if we already gave the clock a number
                                    //t.reset_list |= a32[map_of_clocks[resetmatch[1].str ()]];	// updated the number of thing.
                                    t.reset_vector.push_back(map_of_clocks[resetmatch[1].str()]);
                                }
                                else		// else we need to give the clock a different number
                                {
                                    number_of_clocks++;
                                    list_of_clocks.push_back(resetmatch[1].str());
                                    map_max_constraints[number_of_clocks] = 0;
                                    map_of_clocks[resetmatch[1].str ()] = number_of_clocks;
                                    //t.reset_list |= a32[number_of_clocks];
                                    t.reset_vector.push_back(number_of_clocks);
                                }
                            }
                        }
                        //t.reset_list = list_of_reset;
                    }
                    
                    else if (strcmp (tElement->Attribute ("kind"), "synchronisation") == 0)	//comment for the stack operations
                    {
                        //extract push pop infomation from the transiton.
                        regex e_lessthan ("(.*)<(\\d*)");	//regular expression for only less than operation
                        regex e_lessthan_eq ("(.*)<=(\\d*)");	// regular expression for only less than equal to operation
                        regex e_greaterthan ("(.*)>(\\d*)");	// regular expression for greater than expression
                        regex e_greaterthan_eq ("(.*)>=(\\d*)");	//regular expression for greater than equalto expression
                        regex equality ("(.*)==(\\d*)");
                        regex diagonal ("(.*)-(.*)");	//this is the regular expression to check for diagonal constraint 
                        
                        //cout << "stack operation" << endl;
                        TiXmlNode *stackOp = tElement->FirstChild ();
                        //cout << stackOp->Value () << endl;
                        string backup = stackOp->Value ();
                        backup.erase (remove_if (backup.begin (), backup.end (),::isspace),
                                backup.end ());
                        stringstream ss (backup);	//if more than one comma separated guards available
                        //split the guards and crete individual strings of guards.
                        vector < string > result;
                        while (ss.good ()) {
                            string substr;
                            getline (ss, substr, ',');	//separating comma
                            result.push_back (substr);
                            //cout << substr << endl;;
                        }
                        //t.stack = getStack(stackOp->Value())
                        t.stackOp.op = 0;
                        t.stackOp.lower_bound = 0;	///default values
                        t.stackOp.lower_open = false;
                        t.stackOp.upper_bound = INF;
                        t.stackOp.upper_open = false;
                        if (result.size () == 7)	//this is both pop and push
                        {
                            //string temporary = sm[1].str();
                            //if(regex_match(temporary,cm,diagonal)) // if the constraint is diagonal constraint .
                            //    {
                            t.stackOp.op = 3;	// 3 for both push and pop check for 3 and reset point for pop operation.
                            //t.reset_list |= 1;	// for pop.
                            if (result[0].compare ("pop") != 0) {
                                cout << "This should be pop: " << result[0] << endl;
                                exit (0);
                            }
                            if (map_of_stack[result[1]]) {
                                t.stackOp.stack_pop = map_of_stack[result[1]];
                            }
                            else {
                                number_of_stacks++;
                                //stack<int> s;
                                map_of_stack[result[1]] = number_of_stacks;
                                t.stackOp.stack_pop = number_of_stacks;
                                //a.stack_list.push_back(s);
                            }
                            if (map_of_stack_alphabet[result[2]]) {
                                t.stackOp.stack_alphabet_pop = map_of_stack_alphabet[result[2]];
                            }
                            else {
                                stack_alphabet_number++;
                                map_of_stack_alphabet[result[2]] = stack_alphabet_number;
                                t.stackOp.stack_alphabet_pop = stack_alphabet_number;
                            }
                            
                            string result1 = result[3];
                            regex rgx ("&&");	//using regular expression to split the string to substrings
                            vector < string > string_vector;	// after splitting the substrings should be stored in here
                            sregex_token_iterator iter (result1.begin (), result1.end (), rgx, -1);	//the splitting operation depending on the regular expression
                            //above statements splits a string in to tokens depending on the rgx value, here it will
                            //check for && and split strings in to part
                            sregex_token_iterator end;	//the last iterator
                            for (; iter != end; ++iter) {
                                string_vector.push_back (*iter);	//pusing the substrings to the vector
                            }
                            smatch cm, sm;
                            for (unsigned i = 0; i < string_vector.size (); ++i) {
                                // Guards g;
                                //cout << string_vector[i] << endl;
                                //         string s1(cm[i].str());
                                //        cout << s1<< endl;
                                if (regex_match (string_vector[i], sm, equality)) {
                                    //cout << "equality Matched " << endl;
                                    string temporary = sm[1].str ();
                                    if (regex_match (temporary, cm, diagonal))	// if the constraint is diagonal constraint .
                                    {
                                        t.stackOp.diagonal = true;
                                        if (cm[1].str ().compare ("pop") != 0) {
                                            cout << "this must be pop: " << cm[1].str () << endl;
                                            exit (0);
                                        }
                                        if (map_of_clocks[cm[2].str ()])	// if we already gave the clock a number
                                            t.stackOp.stack_pushed_clock =
                                                    map_of_clocks[cm[2].str ()];
                                        else		// else we need to give the clock a different number
                                        {
                                            number_of_clocks++;
                                            list_of_clocks.push_back(cm[2].str());
                                            map_max_constraints[number_of_clocks] = 0;
                                            map_of_clocks[cm[2].str ()] = number_of_clocks;
                                            t.stackOp.stack_pushed_clock =
                                                    map_of_clocks[cm[2].str ()];
                                        }
                                        t.stackOp.lower_bound = t.stackOp.upper_bound =
                                                stoi (sm[2].str ());
                                        maximum_stack_constant = max(maximum_stack_constant,t.stackOp.lower_bound);
                                        //maximum_constant =
                                        //max (maximum_constant, t.stackOp.lower_bound);
                                        //cout << "Equality " << "Value :" << sm[2] << endl;
                                        t.stackOp.lower_open = t.stackOp.upper_open = false;
                                        
                                    }
                                    else {
                                        
                                        t.stackOp.lower_bound = max(int(t.stackOp.lower_bound),stoi(sm[2].str()));
                                        t.stackOp.upper_bound = min(int(t.stackOp.upper_bound),stoi (sm[2].str ()));
                                        maximum_stack_constant = max(maximum_stack_constant,t.stackOp.lower_bound);
                                        //maximum_constant =
                                        //  max (maximum_constant, t.stackOp.lower_bound);
                                        //cout << "Equality " << "Value :" << sm[2] << endl;
                                        t.stackOp.lower_open = t.stackOp.upper_open = false;
                                        //                                     g.lower_bound = 0;
                                        //                                     g.lower_open = false;
                                        //map_clock_guard[g.clock] = g
                                    }		//t.guard_list.push_back(g);
                                    
                                }
                                else if (regex_match (string_vector[i], sm, e_lessthan)) {
                                    //cout << "less than matched " << endl;
                                    string temporary = sm[1].str ();
                                    if (regex_match (temporary, cm, diagonal))	// if the constraint is diagonal constraint .
                                    {
                                        t.stackOp.diagonal = true;
                                        if (cm[1].str ().compare ("pop") != 0) {
                                            cout << "this must be pop: " << cm[1].str () << endl;
                                            exit (0);
                                        }
                                        if (map_of_clocks[cm[2].str ()])	// if we already gave the clock a number
                                            t.stackOp.stack_pushed_clock =
                                                    map_of_clocks[cm[2].str ()];
                                        else		// else we need to give the clock a different number
                                        {
                                            number_of_clocks++;
                                            list_of_clocks.push_back(cm[2].str());
                                            //maximum_constant.push_back(0);
                                            map_of_clocks[cm[2].str ()] = number_of_clocks;
                                            t.stackOp.stack_pushed_clock =
                                                    map_of_clocks[cm[2].str ()];
                                        }
                                        t.stackOp.upper_bound = stoi (sm[2].str ());
                                        maximum_stack_constant = max(maximum_stack_constant,t.stackOp.upper_bound);
                                        //maximum_constant =
                                        // max (maximum_constant, t.stackOp.upper_bound);
                                        t.stackOp.upper_open = true;
                                    }
                                    else {
                                        
                                        t.stackOp.upper_bound = stoi (sm[2].str ());
                                        maximum_stack_constant = max(maximum_stack_constant,t.stackOp.upper_bound);
                                        // maximum_constant =
                                        //  max (maximum_constant, t.stackOp.upper_bound);
                                        t.stackOp.upper_open = true;
                                        //t.stackOp.lower_bound = 0;
                                        //t.stackOp.lower_open = false;
                                        
                                        //t.guard_list.push_back(g);
                                    }
                                }
                                else if (regex_match (string_vector[i], sm, e_greaterthan)) {
                                    //cout << "greater than matched " << endl;
                                    string temporary = sm[1].str ();
                                    if (regex_match (temporary, cm, diagonal))	// if the constraint is diagonal constraint .
                                    {
                                        t.stackOp.diagonal = true;
                                        if (cm[1].str ().compare ("pop") != 0) {
                                            cout << "this must be pop: " << cm[1].str () << endl;
                                            exit (0);
                                        }
                                        if (map_of_clocks[cm[2].str ()])	// if we already gave the clock a number
                                            t.stackOp.stack_pushed_clock =
                                                    map_of_clocks[cm[2].str ()];
                                        else		// else we need to give the clock a different number
                                        {
                                            number_of_clocks++;
                                            list_of_clocks.push_back(cm[2].str());
                                            //maximum_constant.push_back(0);
                                            map_of_clocks[cm[2].str ()] = number_of_clocks;
                                            t.stackOp.stack_pushed_clock =
                                                    map_of_clocks[cm[2].str ()];
                                        }
                                        t.stackOp.lower_bound = stoi (sm[2].str ());
                                        maximum_stack_constant = max(maximum_stack_constant,t.stackOp.lower_bound);
                                        //maximum_constant =
                                        //  max (maximum_constant, t.stackOp.lower_bound);
                                        t.stackOp.lower_open = true;
                                    }
                                    else {
                                        t.stackOp.lower_bound = stoi (sm[2].str ());
                                        maximum_stack_constant = max(maximum_stack_constant,t.stackOp.lower_bound);
                                        //maximum_constant =
                                        //  max (maximum_constant, t.stackOp.lower_bound);
                                        t.stackOp.lower_open = true;
                                        //                                    t.stackOp.upper_bound = -1;
                                        //                                    t.stackOp.upper_open = true;
                                    }
                                    
                                }
                                else if (regex_match (string_vector[i], sm, e_lessthan_eq)) {
                                    //cout << "less than equalto matched " << endl;
                                    string temporary = sm[1].str ();
                                    if (regex_match (temporary, cm, diagonal))	// if the constraint is diagonal constraint .
                                    {
                                        t.stackOp.diagonal = true;
                                        if (cm[1].str ().compare ("pop") != 0) {
                                            cout << "this must be pop: " << cm[1].str () << endl;
                                            exit (0);
                                        }
                                        if (map_of_clocks[cm[2].str ()])	// if we already gave the clock a number
                                            t.stackOp.stack_pushed_clock =
                                                    map_of_clocks[cm[2].str ()];
                                        else		// else we need to give the clock a different number
                                        {
                                            number_of_clocks++;
                                            list_of_clocks.push_back(cm[2].str());
                                            //maximum_constant.push_back(0);
                                            map_of_clocks[cm[2].str ()] = number_of_clocks;
                                            t.stackOp.stack_pushed_clock =
                                                    map_of_clocks[cm[2].str ()];
                                        }
                                        
                                        t.stackOp.upper_bound = stoi (sm[2].str ());
                                        maximum_stack_constant = max(maximum_stack_constant,t.stackOp.upper_bound);
                                        //maximum_constant =
                                        //  max (maximum_constant, t.stackOp.upper_bound);
                                        t.stackOp.upper_open = false;
                                    }
                                    
                                    else {
                                        
                                        t.stackOp.upper_bound = min(int(t.stackOp.upper_bound),stoi (sm[2].str ()));
                                        t.stackOp.lower_bound = max(0,int(t.stackOp.lower_bound));
                                        t.stackOp.lower_open=false;
                                        maximum_stack_constant = max(maximum_stack_constant,t.stackOp.upper_bound);
                                        //maximum_constant =
                                        //  max (maximum_constant, t.stackOp.upper_bound);
                                        t.stackOp.upper_open = false;
                                        //                                    t.stackOp.lower_bound = 0;
                                        //                                    t.stackOp.lower_open = false;
                                    }
                                    //t.guard_list.push_back(g);
                                }
                                else if (regex_match (string_vector[i], sm, e_greaterthan_eq)) {
                                    //cout << "greater than equalto matched " << endl;
                                    string temporary = sm[1].str ();
                                    if (regex_match (temporary, cm, diagonal))	// if the constraint is diagonal constraint .
                                    {
                                        t.stackOp.diagonal = true;
                                        if (cm[1].str ().compare ("pop") != 0) {
                                            cout << "this must be pop: " << cm[1].str () << endl;
                                            exit (0);
                                        }
                                        if (map_of_clocks[cm[2].str ()])	// if we already gave the clock a number
                                            t.stackOp.stack_pushed_clock =
                                                    map_of_clocks[cm[2].str ()];
                                        else		// else we need to give the clock a different number
                                        {
                                            number_of_clocks++;
                                            list_of_clocks.push_back(cm[2].str());
                                            // maximum_constant.push_back(0);
                                            map_of_clocks[cm[2].str ()] = number_of_clocks;
                                            t.stackOp.stack_pushed_clock =
                                                    map_of_clocks[cm[2].str ()];
                                        }
                                        
                                        t.stackOp.lower_bound = stoi (sm[2].str ());
                                        maximum_stack_constant = max(maximum_stack_constant,t.stackOp.lower_bound);
                                        //maximum_constant =
                                        //  max (maximum_constant, t.stackOp.lower_bound);
                                        //cout << "e_greaterthan_eq " << "Value :" << sm[2] << endl;
                                        t.stackOp.lower_open = false;
                                    }
                                    else {
                                        t.stackOp.lower_bound = max(stoi (sm[2].str ()),int(t.stackOp.lower_bound));
                                        t.stackOp.upper_bound = min(INF,int(t.stackOp.upper_bound));
                                        maximum_stack_constant = max(maximum_stack_constant,t.stackOp.lower_bound);
                                        //maximum_constant =
                                        //  max (maximum_constant, t.stackOp.lower_bound);
                                        //cout << "e_greaterthan_eq " << "Value :" << sm[2] << endl;
                                        t.stackOp.lower_open = false;
                                        t.stackOp.upper_open = false;
                                        //                                    t.stackOp.upper_bound = -1;
                                        //                                    t.stackOp.upper_open = true;
                                        //map_clock_guard[g.clock] = g;
                                    }
                                    //t.guard_list.push_back(g);
                                }
                                //add the guards in the transition here.
                                //         for(map<int,Guards>::iterator it = map_clock_guard.begin();it!=map_clock_guard.end(); ++it)
                                //             t.guard_list.push_back(it->second);
                                //cout << i << endl;
                            }
                            if (result[4].compare ("push") != 0) {
                                cout << "This operation should be push " << endl;
                                exit (0);
                            }
                            if (map_of_stack[result[5]]) {
                                t.stackOp.stack_push = map_of_stack[result[5]];
                                //a.stack_list[map_of_stack[result[5]]-1].push()
                            }
                            else {
                                number_of_stacks++;
                                //stack<int> s;
                                map_of_stack[result[5]] = number_of_stacks;
                                t.stackOp.stack_push = number_of_stacks;
                                //a.stack_list.push_back(s);
                            }
                            if (map_of_stack_alphabet[result[6]]) {
                                t.stackOp.stack_alphabet_push =
                                        map_of_stack_alphabet[result[6]];
                                //a.stack_list[map_of_stack[result[5]]-1].push(map_of_stack_alphabet[result[6]]);
                            }
                            else {
                                stack_alphabet_number++;
                                map_of_stack_alphabet[result[6]] = stack_alphabet_number;
                                t.stackOp.stack_alphabet_push = stack_alphabet_number;
                                //a.stack_list[map_of_stack[result[5]]-1].push(map_of_stack_alphabet[result[5]]);
                            }
                            
                        }
                        else if (result.size () == 4)	// this is special pushonly
                        {
                            
                            if (result[0].compare ("push") == 0)	//that special push
                            {
                                //cout << "this is special push" << endl;
                                //format: push,a,x where push is the operation, a is the stack alphabet and x is the clock that is pushed.
                                t.stackOp.op = 1;	// 1 for special push
                                t.stackOp.special = true;
                                if (map_of_stack[result[1]]) {
                                    t.stackOp.stack_push = map_of_stack[result[1]];
                                }
                                else {
                                    number_of_stacks++;
                                    //stack<int> s;
                                    map_of_stack[result[1]] = number_of_stacks;
                                    t.stackOp.stack_push = number_of_stacks;
                                    //a.stack_list.push_back(s);
                                }
                                if (map_of_stack_alphabet[result[2]]) {
                                    t.stackOp.stack_alphabet_push =
                                            map_of_stack_alphabet[result[2]];
                                    //a.stack_list[map_of_stack[result[1]]-1].push(map_of_stack_alphabet[result[2]]);
                                }
                                else {
                                    stack_alphabet_number++;
                                    map_of_stack_alphabet[result[2]] = stack_alphabet_number;
                                    t.stackOp.stack_alphabet_push = stack_alphabet_number;
                                    //a.stack_list[map_of_stack[result[1]]-1].push(map_of_stack_alphabet[result[2]]);
                                }
                                if (map_of_clocks[result[3]])	// if we already gave the clock a number
                                    t.stackOp.stack_pushed_clock = map_of_clocks[result[3]];	// updated the number of thing.
                                else		// else we need to give the clock a different number
                                {
                                    number_of_clocks++;
                                    list_of_clocks.push_back(result[3]);
                                    //maximum_constant.push_back(0);
                                    map_of_clocks[result[3]] = number_of_clocks;
                                    t.stackOp.stack_pushed_clock = number_of_clocks;
                                }
                                
                            }
                            if (result[0].compare ("pop") == 0)	// have to change to some regular expression
                            {
                                t.stackOp.op = 2;	// 2 for pop
                                //t.reset_list |= 1;	// pop marking.
                                if (map_of_stack[result[1]]) {
                                    t.stackOp.stack_pop = map_of_stack[result[1]];
                                }
                                else {
                                    number_of_stacks++;
                                    //stack<int> s;
                                    map_of_stack[result[1]] = number_of_stacks;
                                    t.stackOp.stack_pop = number_of_stacks;
                                    //a.stack_list.push_back(s);
                                }
                                if (map_of_stack_alphabet[result[2]]) {
                                    t.stackOp.stack_alphabet_pop =
                                            map_of_stack_alphabet[result[2]];
                                }
                                else {
                                    stack_alphabet_number++;
                                    map_of_stack_alphabet[result[2]] = stack_alphabet_number;
                                    t.stackOp.stack_alphabet_pop = stack_alphabet_number;
                                }
                                
                                string result1 = result[3];
                                regex rgx ("&&");	//using regular expression to split the string to substrings
                                vector < string > string_vector;	// after splitting the substrings should be stored in here
                                sregex_token_iterator iter (result1.begin (), result1.end (), rgx, -1);	//the splitting operation depending on the regular expression
                                //above statements splits a string in to tokens depending on the rgx value, here it will
                                //check for && and split strings in to part
                                sregex_token_iterator end;	//the last iterator
                                for (; iter != end; ++iter) {
                                    string_vector.push_back (*iter);	//pusing the substrings to the vector
                                }
                                smatch cm, sm;
                                for (unsigned i = 0; i < string_vector.size (); ++i) {
                                    // Guards g;
                                    //cout << string_vector[i] << endl;
                                    //         string s1(cm[i].str());
                                    //        cout << s1<< endl;
                                    if (regex_match (string_vector[i], sm, equality)) {
                                        //cout << "equality Matched " << endl;
                                        string temporary = sm[1].str ();
                                        if (regex_match (temporary, cm, diagonal))	// if the constraint is diagonal constraint .
                                        {
                                            t.stackOp.diagonal = true;
                                            if (cm[1].str ().compare ("pop") != 0) {
                                                cout << "this must be pop: " << cm[1].str () << endl;
                                                exit (0);
                                            }
                                            if (map_of_clocks[cm[2].str ()])	// if we already gave the clock a number
                                                t.stackOp.stack_pushed_clock =
                                                        map_of_clocks[cm[2].str ()];
                                            else	// else we need to give the clock a different number
                                            {
                                                number_of_clocks++;
                                                list_of_clocks.push_back(cm[2].str());
                                                //maximum_constant.push_back(0);
                                                map_of_clocks[cm[2].str ()] = number_of_clocks;
                                                t.stackOp.stack_pushed_clock =
                                                        map_of_clocks[cm[2].str ()];
                                            }
                                            t.stackOp.lower_bound = t.stackOp.upper_bound =
                                                    stoi (sm[2].str ());
                                            maximum_stack_constant = max(maximum_stack_constant,t.stackOp.lower_bound);
                                            //maximum_constant =
                                            // max (maximum_constant, t.stackOp.lower_bound);
                                            //cout << "Equality " << "Value :" << sm[2] << endl;
                                            t.stackOp.lower_open = t.stackOp.upper_open = false;
                                        }
                                        else {
                                            t.stackOp.lower_bound = t.stackOp.upper_bound =
                                                    stoi (sm[2].str ());
                                            maximum_stack_constant = max(maximum_stack_constant,t.stackOp.lower_bound);
                                            //maximum_constant =
                                            // max (maximum_constant, t.stackOp.lower_bound);
                                            //cout << "Equality " << "Value :" << sm[2] << endl;
                                            t.stackOp.lower_open = t.stackOp.upper_open = false;
                                            //                                     g.lower_bound = 0;
                                            //                                     g.lower_open = false;
                                            //map_clock_guard[g.clock] = g
                                            //t.guard_list.push_back(g);
                                        }
                                    }
                                    else if (regex_match (string_vector[i], sm, e_lessthan)) {
                                        //cout << "less than matched " << endl;
                                        string temporary = sm[1].str ();
                                        if (regex_match (temporary, cm, diagonal))	// if the constraint is diagonal constraint .
                                        {
                                            t.stackOp.diagonal = true;
                                            if (cm[1].str ().compare ("pop") != 0) {
                                                cout << "this must be pop: " << cm[1].str () << endl;
                                                exit (0);
                                            }
                                            if (map_of_clocks[cm[2].str ()])	// if we already gave the clock a number
                                                t.stackOp.stack_pushed_clock =
                                                        map_of_clocks[cm[2].str ()];
                                            else	// else we need to give the clock a different number
                                            {
                                                number_of_clocks++;
                                                list_of_clocks.push_back(cm[2].str());
                                                //maximum_constant.push_back(0);
                                                map_of_clocks[cm[2].str ()] = number_of_clocks;
                                                t.stackOp.stack_pushed_clock =
                                                        map_of_clocks[cm[2].str ()];
                                            }
                                            t.stackOp.upper_bound = stoi (sm[2].str ());
                                            maximum_stack_constant = max(maximum_stack_constant,t.stackOp.upper_bound);
                                            //maximum_constant =
                                            //  max (maximum_constant, t.stackOp.upper_bound);
                                            t.stackOp.upper_open = true;
                                        }
                                        else {
                                            t.stackOp.upper_bound = stoi (sm[2].str ());
                                            maximum_stack_constant = max(maximum_stack_constant,t.stackOp.upper_bound);
                                            // maximum_constant =
                                            //  max (maximum_constant, t.stackOp.upper_bound);
                                            t.stackOp.upper_open = true;
                                            //t.stackOp.lower_bound = 0;
                                            //t.stackOp.lower_open = false;
                                            
                                            //t.guard_list.push_back(g);
                                        }
                                    }
                                    else if (regex_match (string_vector[i], sm, e_greaterthan)) {
                                        //cout << "greater than matched " << endl;
                                        string temporary = sm[1].str ();
                                        if (regex_match (temporary, cm, diagonal))	// if the constraint is diagonal constraint .
                                        {
                                            t.stackOp.diagonal = true;
                                            if (cm[1].str ().compare ("pop") != 0) {
                                                cout << "this must be pop: " << cm[1].str () << endl;
                                                exit (0);
                                            }
                                            if (map_of_clocks[cm[2].str ()])	// if we already gave the clock a number
                                                t.stackOp.stack_pushed_clock =
                                                        map_of_clocks[cm[2].str ()];
                                            else	// else we need to give the clock a different number
                                            {
                                                number_of_clocks++;
                                                list_of_clocks.push_back(cm[2].str());
                                                //maximum_constant.push_back(0);
                                                map_of_clocks[cm[2].str ()] = number_of_clocks;
                                                t.stackOp.stack_pushed_clock =
                                                        map_of_clocks[cm[2].str ()];
                                            }
                                            t.stackOp.lower_bound = stoi (sm[2].str ());
                                            maximum_stack_constant = max(maximum_stack_constant,t.stackOp.lower_bound);
                                            //maximum_constant =
                                            // max (maximum_constant, t.stackOp.lower_bound);
                                            t.stackOp.lower_open = true;
                                        }
                                        else {
                                            t.stackOp.lower_bound = stoi (sm[2].str ());
                                            maximum_stack_constant = max(maximum_stack_constant,t.stackOp.lower_bound);
                                            // maximum_constant =
                                            //  max (maximum_constant, t.stackOp.lower_bound);
                                            t.stackOp.lower_open = true;
                                            //                                    t.stackOp.upper_bound = -1;
                                            //                                    t.stackOp.upper_open = true;
                                            
                                        }
                                    }
                                    else if (regex_match (string_vector[i], sm, e_lessthan_eq)) {
                                        //cout << "less than equalto matched " << endl;
                                        string temporary = sm[1].str ();
                                        if (regex_match (temporary, cm, diagonal))	// if the constraint is diagonal constraint .
                                        {
                                            t.stackOp.diagonal = true;
                                            if (cm[1].str ().compare ("pop") != 0) {
                                                cout << "this must be pop: " << cm[1].str () << endl;
                                                exit (0);
                                            }
                                            if (map_of_clocks[cm[2].str ()])	// if we already gave the clock a number
                                                t.stackOp.stack_pushed_clock =
                                                        map_of_clocks[cm[2].str ()];
                                            else	// else we need to give the clock a different number
                                            {
                                                number_of_clocks++;
                                                list_of_clocks.push_back(cm[2].str());
                                                //maximum_constant.push_back(0);
                                                map_of_clocks[cm[2].str ()] = number_of_clocks;
                                                t.stackOp.stack_pushed_clock =
                                                        map_of_clocks[cm[2].str ()];
                                            }
                                            t.stackOp.upper_bound = stoi (sm[2].str ());
                                            maximum_stack_constant = max(maximum_stack_constant,t.stackOp.upper_bound);
                                            // maximum_constant =
                                            //  max (maximum_constant, t.stackOp.upper_bound);
                                            t.stackOp.upper_open = false;
                                        }
                                        else {
                                            
                                            t.stackOp.upper_bound = stoi (sm[2].str ());
                                            maximum_stack_constant = max(maximum_stack_constant,t.stackOp.upper_bound);
                                            //maximum_constant =
                                            //  max (maximum_constant, t.stackOp.upper_bound);
                                            t.stackOp.upper_open = false;
                                            //                                    t.stackOp.lower_bound = 0;
                                            //                                    t.stackOp.lower_open = false;
                                            
                                            //t.guard_list.push_back(g);
                                        }
                                    }
                                    else if (regex_match (string_vector[i], sm, e_greaterthan_eq)) {
                                        //cout << "greater than equalto matched " << endl;
                                        string temporary = sm[1].str ();
                                        if (regex_match (temporary, cm, diagonal))	// if the constraint is diagonal constraint .
                                        {
                                            t.stackOp.diagonal = true;
                                            if (cm[1].str ().compare ("pop") != 0) {
                                                cout << "this must be pop: " << cm[1].str () << endl;
                                                exit (0);
                                            }
                                            if (map_of_clocks[cm[2].str ()])	// if we already gave the clock a number
                                                t.stackOp.stack_pushed_clock =
                                                        map_of_clocks[cm[2].str ()];
                                            else	// else we need to give the clock a different number
                                            {
                                                number_of_clocks++;
                                                list_of_clocks.push_back(cm[2].str());
                                                //maximum_constant.push_back(0);
                                                map_of_clocks[cm[2].str ()] = number_of_clocks;
                                                t.stackOp.stack_pushed_clock =
                                                        map_of_clocks[cm[2].str ()];
                                            }
                                            t.stackOp.lower_bound = stoi (sm[2].str ());
                                            maximum_stack_constant = max(maximum_stack_constant,t.stackOp.lower_bound);
                                            //maximum_constant =
                                            //  max (maximum_constant, t.stackOp.lower_bound);
                                            //cout << "e_greaterthan_eq " << "Value :" << sm[2] << endl;
                                            t.stackOp.lower_open = false;
                                        }
                                        else {
                                            t.stackOp.lower_bound = stoi (sm[2].str ());
                                            maximum_stack_constant = max(maximum_stack_constant,t.stackOp.lower_bound);
                                            //maximum_constant =
                                            // max (maximum_constant, t.stackOp.lower_bound);
                                            //cout << "e_greaterthan_eq " << "Value :" << sm[2] << endl;
                                            t.stackOp.lower_open = false;
                                            //                                    t.stackOp.upper_bound = -1;
                                            //                                    t.stackOp.upper_open = true;
                                            //map_clock_guard[g.clock] = g;
                                            
                                            //t.guard_list.push_back(g);
                                        }
                                    }
                                    //add the guards in the transition here.
                                    //         for(map<int,Guards>::iterator it = map_clock_guard.begin();it!=map_clock_guard.end(); ++it)
                                    //             t.guard_list.push_back(it->second);
                                    //cout << i << endl;
                                }
                            }
                            else {
                                cout << result[0] << " should be a special push or a pop operation" <<
                                        endl;
                            }
                        }
                        else if (result.size () == 3)	// push
                        {
                            if (result[0].compare ("push") == 0) {
                                t.stackOp.op = 1;	// 1 for push
                                if (map_of_stack[result[1]]) {
                                    t.stackOp.stack_push = map_of_stack[result[1]];
                                }
                                else {
                                    number_of_stacks++;
                                    //stack<int> s;
                                    map_of_stack[result[1]] = number_of_stacks;
                                    t.stackOp.stack_push = number_of_stacks;
                                    //a.stack_list.push_back(s);
                                }
                                if (map_of_stack_alphabet[result[2]]) {
                                    t.stackOp.stack_alphabet_push =
                                            map_of_stack_alphabet[result[2]];
                                    //a.stack_list[map_of_stack[result[1]]-1].push(map_of_stack_alphabet[result[2]]);
                                }
                                else {
                                    stack_alphabet_number++;
                                    map_of_stack_alphabet[result[2]] = stack_alphabet_number;
                                    t.stackOp.stack_alphabet_push = stack_alphabet_number;
                                    //a.stack_list[map_of_stack[result[1]]-1].push(map_of_stack_alphabet[result[2]]);
                                }
                            }
                            else {
                                cout << "this should be push " << endl;
                                exit (0);
                            }
                            
                        }
                        
                        else {
                            cout << "Error Invalid Stack Input" << endl;
                            cout << " The stack Input should be like the following" << endl;
                            cout <<
                                    "1. for push it will be: push, stack_name, stack_alphabet." <<
                                    endl;
                            cout <<
                                    "2. for pop it will be : pop, stack_name, stack_alphabet, constraints. The constraints can contain diagonal constraits as well. "
                                    << endl;
                            cout <<
                                    "3. for both pop and push it will be : pop,stack_name,stack_alphabet,constraints, push, stack_name, stack_alphabet. "
                                    << endl;
                            exit (0);
                        }
                        
                    }
                    else if (strcmp (tElement->Attribute ("kind"), "select") == 0) {
                        TiXmlNode *transitionName = tElement->FirstChild ();
                        //cout << transitionName->Value () << endl;
                        t.name = transitionName->Value ();
                    }
                }
                tElement = tElement->NextSiblingElement ();
            }
            transition_list.push_back (t);
            
        }
        //cout << "Total Number of Transitions is : " << no_of_transitions << endl;
        number_of_transitions = no_of_transitions;
        //a.transition_list = transitions;
        if(final != "-"){
        final_state = atoi (final.c_str() + 2) + 1;
        }
        else {
            final_state = -1;
        }
        
        number_of_alphabet = 0;
        number_of_stack_alphabet = stack_alphabet_number;
        //cout << "Total Number of Clocks is : " << number_of_clocks << endl;
        
        //a.printAutomata(cout);//this is correct
        //Transition t1;
        //t1.destination_state = start_state;
        //t1.source_state = 0;
        //t1.input_symbol = 0;
        //t1.guard_list = 0;
        //t1.name = "init";
        
        //t1.reset_list = (b32[int (number_of_clocks)] & (~1));	//last bit represents the stack operation so we are concentrating on the clocks b32 is an array which says that all bits upto x are 1.
        //t1.stackOp = 0;
        //vector < Transition >::iterator it;
        //it = transition_list.begin ();
        //transition_list.insert (it, t1);	//this transition is now the first transition.
        
        for(int i = 0 ; i< number_of_clocks;i++){
            maximum_constant.push_back(0);
        }
        for(int i = 0; i < number_of_clocks; i++){
            maximum_constant[i] =map_max_constraints[i+1] + 1;
        }
        maximum_stack_constant +=1; // increase 1 to the maximum stack constant.
        //maximum_constant ++;
        //Complete = nullptr;
        max_const = maximum_stack_constant;
        for(int i =0; i < maximum_constant.size();i++){
            if(max_const < maximum_constant[i]){
                max_const = maximum_constant[i];
            }
        }
        //cout << "Number of stacks in this point is " << number_of_stacks << endl;
        
        
        
        //  prevtrans.resize (number_of_states + 1);	// #rows = S+1 in the 2D vector prevtrans and nexttrans // forget this
        //  nexttrans.resize (number_of_states + 1);	// forget this
        //
        //  for (i = 0; i <= a.number_of_transitions; i++) {
        //    prevtrans[transition_list[i].destination_state].push_back (i);
        //    nexttrans[transition_list[i].source_state].push_back (i);
        //  }

        }
}
