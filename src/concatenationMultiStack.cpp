/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */


#include<iostream>
#include<vector>
#include<list>
#include<cmath>
//#include<unordered_map>
#include<concatenationMultiStack.h>
#include<automata.h>


using namespace std;
/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
string toStringParameter(ul index, ul time ,ul index2){
    string returnString = to_string(index);
    returnString = returnString + "-" ;
    returnString = returnString+ to_string(time);
    returnString = returnString + "-";
    returnString = returnString + to_string(index2);
    return returnString;
}
string toStringValuation(vector<unsigned long> valuation){ // vector to string.
    string returnstring;
    for(unsigned long i = 0;i < valuation.size();i++){
        returnstring += to_string(valuation[i]);
    }
    return returnstring;
}

string toString(states s){ // for unordered_map computation.
    string returnString(to_string(s.first));
    //string s(Pair.second.begin(),Pair.second.end());
    //vector<int>::iterator int_it;
    for(unsigned long int_it = 0;int_it!=s.second.size();++int_it){
        returnString+="-";
        returnString+=to_string((s.second[int_it]));

    }

    return returnString;
}

vector<unsigned long> usefulReset(vector<unsigned long> valuation, vector<unsigned long> reset){
    vector<unsigned long> usefulResetVector;
    for(unsigned long index=0;index<reset.size();index++){
        if(valuation[reset[index]-1]!=0){
            usefulResetVector.push_back(reset[index]); // push the clock whose reset is useful
        }
    }
    return usefulResetVector;
}

Concatenation::Concatenation(const Automata& a){ // computing all possible state space.
    //this->a = a;
    //stackP = false;
    stackP = true;
    maximum_stack_constant = a.maximum_stack_constant;
    /* if(a.number_of_stacks >0){
        stackP = true;
    } */
    diagonal = a.diagonal;
   

    
    vector<unsigned long> valuation(a.list_of_clocks.size());
    //vector <states> list_of_states;
    for(unsigned long i =0;i<a.list_of_clocks.size();i++){
        valuation[i] = 0;
    }
    start_state = make_pair(a.start_state-1,valuation);
    for(unsigned long s = 0; s<a.number_of_states;s++){ // for all states this will be the default valuation to all 0's.

        states first_state = make_pair(s,valuation);// first state valuation will be start_state and all the values is 0.

        //out << "Testing NumberOfStates " << list_of_states.size() << endl;
        list_of_states.push_back(first_state);
        state_map[toString(first_state)] = list_of_states.size(); //number_of_states;
    }
    unsigned long current_clock = 0;

    while(current_clock < a.list_of_clocks.size()){
        unsigned long  size = list_of_states.size();
        for(unsigned long i =0;i<size;i++){
            //for(int s = 0; s <a_number_of_states;s++){
            //if(list_of_states[i].first == s){
            for(unsigned long j = 0;j<=a.maximum_constant[current_clock];j++){ //////////Maximum constant here is interesting !!!! Please check this again!!!!
                states state = list_of_states[i];
                state.second[current_clock] = j;
                if(state_map[toString(state)]){continue;} // if the state is already created then continue the search.
                else{


                    list_of_states.push_back(state);
                    state_map[toString(state)] = list_of_states.size();
                    //cout <<toString(state) << " " << list_of_states.size() << endl;
                }
            }
            //}
            //}
        }
        current_clock++;
    }

    //cout << "size of the matrix " << list_of_states.size() << endl;
    size_of_matrix = list_of_states.size(); // we will keep growing this as long as we create a new state.
    //Witness = bnu::matrix<linkedlist<Fired> >(size_of_matrix,size_of_matrix); // for witness generation this will keep track of the transitions we fired to reach from one state to another (no time lapse)
    if(stackP){
        C = bnu::matrix<set< unsigned long> >(size_of_matrix,size_of_matrix); // resize the matrix to a bigger one and keep all the information intact.
        if(a.number_of_stacks>1){
            D1 = bnu::matrix<entry>(size_of_matrix,size_of_matrix);
            D2 = bnu::matrix<entry> (size_of_matrix,size_of_matrix);
            D1c = bnu::matrix<set<unsigned long> >(size_of_matrix,size_of_matrix);
            D2c = bnu::matrix<set<unsigned long> >(size_of_matrix,size_of_matrix);
        }
        // now fill the first location to zero.
        for(unsigned long i =0;i<size_of_matrix;i++){
            for(unsigned long j = 0; j <size_of_matrix;j++){
                if(i==j){
                    C(i,j).insert(0) ; // the time lapse from this state to again this state is zero.
                }
                else{
                   
                    continue; //check empty() for set emptiness
                }
            }

        }
    }
    else{
        Cb = bnu::matrix<bool>(size_of_matrix,size_of_matrix); // resize the matrix to a bigger one and keep all the information intact.
        C = bnu::matrix<set< unsigned long> >(size_of_matrix,size_of_matrix);
        // now fill the first location to zero.
        for(unsigned i =0;i<size_of_matrix;i++){
            for(unsigned j = 0; j <size_of_matrix;j++){
                if(i==j){
                    Cb(i,j) = true; // the time lapse from this state to again this state is zero.
                    C(i,j).insert(0);
                }
                else{
                    Cb(i,j) = false; // -1 indicates that the state j is not reachable from j.
                }
            }

        }

    }
    //cout << C << endl;
}

int Concatenation::timeLapse(states s,const Automata& a){ // this will compute all the time lapse possible


   
    // the entries of it is the state of the automata whose first is the state and second is another vector which we are going to add.
   


    for(unsigned long increment = 0; increment <= a.max_const;increment++){ // loop till all clocks reach their maximum constant value.
       
        vector<unsigned long> tools = s.second; // create a vector which will store after incremented values.
        for(unsigned long i = 0; i<tools.size();i++){ // loop to increment the values of the vector
            tools[i] = min(tools[i]+increment,a.maximum_constant.at(i)); // normalize on the fly
        }



        states AfterPopState  = make_pair(s.first,tools);
       
        if(stackP){ // if stack available then we have to update the matrix entries.
            C(state_map[toString(s)]-1,state_map[toString(AfterPopState)]-1).insert(increment); // the new matrix entry is the new value.
        }
        else{ // if not stack available just marking true is sufficient
            //do the same for only the timed automaton.
            Cb(state_map[toString(s)]-1,state_map[toString(AfterPopState)]-1) = true;
            C(state_map[toString(s)]-1,state_map[toString(AfterPopState)]-1).insert(increment);
        }



    }


    return 0;
}

int Concatenation::internal(states s,const Automata &a){ // take all possible transitions and if there is a transition which is fireable
    unsigned long it_transition;
    for(it_transition=0;it_transition<a.transition_list.size();++it_transition){
        if(0 == a.transition_list[it_transition].stackOp.op){ // we will do this only if there is not stack operation in the state.

            if(a.transition_list[it_transition].firable(s)){//check if the transition is firable from the state. then we will also check for the
                // now we will check the rest options.
                vector<unsigned long> after_reset = a.transition_list[it_transition].afterReset(s.second); // get the valuation of the clocks after the reset operation in the transition.
               
                states destinationState = make_pair(a.transition_list[it_transition].destination_state-1,after_reset);  // so the new state is the pair of destination state of the transition and the new valuation vector.
                if(stackP){
                    C(state_map[toString(s)]-1,state_map[toString(destinationState)]-1).insert(0); // we can reach this new state without thinking of time.
                }
                else{
                    Cb(state_map[toString(s)]-1,state_map[toString(destinationState)]-1) = true;
                    C(state_map[toString(s)]-1,state_map[toString(destinationState)]-1).insert(0);
                }
            }
        }
    }
    return 0;
}

void Concatenation::computeD(const Automata &a){ // compute the D_i matrices for stack i
    vector<states>::iterator states_it; // for all states we have to do this
    for(states_it=list_of_states.begin();states_it!=list_of_states.end();++states_it){// loop through all states.
        vector<Transition>::const_iterator it_transition;
        for(it_transition = a.transition_list.begin();it_transition!=a.transition_list.end();++it_transition){ // for all states for all transitions
            if((*it_transition).stackOp.op ==1){// if it is a push operation
                if((*it_transition).firable(*(states_it))){ // and this is fireable at current state
                    states destinationState = make_pair((*it_transition).destination_state-1,(*it_transition).afterReset(states_it->second)); //get the destination state
                    if(it_transition->stackOp.stack_push == 1) { // if first stack // if the stack where pushed is the first push
                        int stackSymbol = (*it_transition).stackOp.stack_alphabet_push;
                        vector<states>::iterator states_it1; // now check for those states which are reachable from destination states by some complete closure.
                        for(states_it1=list_of_states.begin();states_it1!=list_of_states.end();++states_it1){
                            if(!C(state_map[toString(destinationState)]-1,state_map[toString(*(states_it1))]-1).empty()){ //now the after push complete block.
                                set<unsigned long> bag = C(state_map[toString(destinationState)]-1,state_map[toString(*(states_it1))]-1); // take the entries in the bag.
                                D1(state_map[toString(*(states_it))]-1,state_map[toString(*(states_it1))]-1).first = stackSymbol;// progress the complete block with the information.
                                D1(state_map[toString(*(states_it))]-1,state_map[toString(*(states_it1))]-1).second.insert(bag.begin(),bag.end()); // insert the times in side
                                //D1c(state_map[toString(*(states_it))]-1,state_map[toString(*(states_it1))]-1).first = it_transition->stackOp.stack_alphabet_push; // progress the complete block with the information.
                                D1c(state_map[toString(*(states_it))]-1,state_map[toString(*(states_it1))]-1).insert(bag.begin(),bag.end());

                            }
                        }
                    }
                    if(it_transition->stackOp.stack_push == 2) { // do simililar for second stack.
                        int stackSymbol = (*it_transition).stackOp.stack_alphabet_push;
                        vector<states>::iterator states_it1;
                        for(states_it1=list_of_states.begin();states_it1!=list_of_states.end();++states_it1){
                            if(!C(state_map[toString(destinationState)]-1,state_map[toString(*(states_it1))]-1).empty()){
                                set<unsigned long> bag = C(state_map[toString(destinationState)]-1,state_map[toString(*(states_it1))]-1); // take the entries in the bag.
                                D2(state_map[toString(*(states_it))]-1,state_map[toString(*(states_it1))]-1).first = stackSymbol; // this will store the stack symbol inside the D1 matrix
                                D2(state_map[toString(*(states_it))]-1,state_map[toString(*(states_it1))]-1).second.insert(bag.begin(),bag.end()); // append the bag here.
                                //D2c(state_map[toString(*(states_it))]-1,state_map[toString(*(states_it1))]-1).first = it_transition->stackOp.stack_alphabet_push;
                                D2c(state_map[toString(*(states_it))]-1,state_map[toString(*(states_it1))]-1).insert(bag.begin(),bag.end());

                            }
                        }
                    }
                }
            }
        }
        //just to make sure of this.
        D1c(state_map[toString(*(states_it))]-1,state_map[toString(*(states_it))]-1).insert(0);
        D1(state_map[toString(*(states_it))]-1,state_map[toString(*(states_it))]-1).second.insert(0);
        D2c(state_map[toString(*(states_it))]-1,state_map[toString(*(states_it))]-1).insert(0);
        D2(state_map[toString(*(states_it))]-1,state_map[toString(*(states_it))]-1).second.insert(0);
    }
}

void Concatenation::closureD( const Automata &a){
    for(unsigned int k = 0;k<size_of_matrix;k++){ // fix a state
        for(unsigned int i =0;i<size_of_matrix;i++){ // if you can reach the state k from I
            for(unsigned int j =0;j<size_of_matrix;j++){ // if you can reach state j from k
                if(!D1c(i,k).empty() && !D1c(k,j).empty()){ // from i k must be reachable and from k j must be reachable
                    set<unsigned long> added = additionBags(D1c(i,k),D1c(k,j),a.max_const);
                    //set<int> old = C(i,j);
                    D1c(i,j).insert(added.begin(),added.end());// update current value of the C(i,j) to a new integer according to the concatenation
                }
                if(!D2c(i,k).empty() && !D2c(k,j).empty()){ // from i k must be reachable and from k j must be reachable
                    set<unsigned long> added = additionBags(D2c(i,k),D2c(k,j),a.max_const);
                    //set<int> old = C(i,j);
                    D2c(i,j).insert(added.begin(),added.end());// update current value of the C(i,j) to a new integer according to the concatenation
                }
            }
        }
    }
}

bool Concatenation::closure(const Automata& a){ // given the matrix it will compute the closure
    //cout << "IN closure" << endl;
    bool flag = true;
    unsigned int m,n;
    if(stackP){
        for(unsigned int k = 0;k<size_of_matrix;k++){ // fix a state
            for(unsigned int i =0;i<size_of_matrix;i++){ // if you can reach the state k from I
                for(unsigned int j =0;j<size_of_matrix;j++){ // if you can reach state j from k
                    if(!C(i,k).empty() && !C(k,j).empty()){ // from i k must be reachable and from k j must be reachable
                        set<unsigned long> added = additionBags(C(i,k),C(k,j),a.max_const);
                        //set<int> old = C(i,j);
                        C(i,j).insert(added.begin(),added.end());// update current value of the C(i,j) to a new integer according to the concatenation
                        if(state_map[toString(list_of_states[i])]==state_map[toString(start_state)] && list_of_states[j].first+1 == a.final_state){
                            if(added.empty()) { cout <<"Added is Empty!" << endl;}
                            flag = false;
                            m = i;
                            n = j;
                            
                            
                        }

                    }
                }
            }
        }
        if(flag == false){
            printWitness2(m,n,a);

            return false;
        }
    }
    else{
        for(unsigned k = 0;k<size_of_matrix;k++){ // fix a state
            for(unsigned i =0;i<size_of_matrix;i++){
                for(unsigned j =0;j<size_of_matrix;j++){
                    // from i k must be reachable and from k j must be reachable
                    Cb(i,j) = Cb(i,j)||(Cb(i,k) && Cb(k,j)); // update current value of the C(i,j) to a new integer according to the concatenation
                    if(!C(i,k).empty() && !C(k,j).empty()){ // from i k must be reachable and from k j must be reachable
                        set<unsigned long> added = additionBags(C(i,k),C(k,j),a.max_const);
                        //set<int> old = C(i,j);
                        C(i,j).insert(added.begin(),added.end());// update current value of the C(i,j) to a new integer according to the concatenation
                    }
                    if(Cb(i,j)){
                        if(toString(list_of_states[i]) == toString(start_state) && list_of_states[j].first+1 == a.final_state){
                            //cout << toString(list_of_states[i]) << " to " << toString(list_of_states[k]) << " to " << toString(list_of_states[j])<< endl;
                            //printWitness(i,j,a);

                            return false;
                        }
                    }
                    //cout<< "in closure" << endl;
                }
            }
        }
    }
    return true;

}

set<unsigned long> additionBags(set<unsigned long> a, set<unsigned long> b,unsigned long max_stack){
    set<unsigned long> returnSet;
    if(a.empty() || b.empty()){ // for security purposes please check if the sets are empty are not
        cout<< "Given Sets are empty" <<endl; // if empty then the set will be empty anyway.

    }
    else{ // if none of them are empty then we will start adding one to another and add it.
        set<unsigned long>::iterator set_it_a; // declare iterator for the set a
        set<unsigned long>::iterator set_it_b; // declare the iterator for set b
        for(set_it_a = a.begin();set_it_a != a.end(); ++set_it_a ){ // for every element in the set a we need to add the element with the set b
            for(set_it_b = b.begin();set_it_b!=b.end();++set_it_b){ // for every element of set b
                returnSet.insert(min(*(set_it_a)+*(set_it_b),max_stack)); // add the two elements of the set and insert it in the return set.
            }
        }
    }
    return returnSet;
}

bool Concatenation::isEmpty(const Automata &a){
    //cout <<"In isEmpty" << endl;
    vector<states>::iterator states_it;
    for(states_it=list_of_states.begin();states_it!=list_of_states.end();++states_it){ // for every possible state generate the matrix with numbers inside them.
        //cout << toString(*(states_it)) << endl;
        if(toString(*(states_it)) != toString(list_of_states[state_map[toString(*(states_it))]-1])){ // just o check mapping correctness.
            cout << "Something Wrong!" << endl;
        }
        timeLapse(*(states_it),a); // call time lapse operation on each state
        //cout << toString(*(states_it)) << endl;
        internal(*(states_it),a); // call internal operation on each state
    }
    //cout << "timelapse and internal done " << endl;
    if(!closure(a)){ //compute the closure of the matrix, if the closure returns false then we know that the automata is empty
        return false;
    }

    else if(stackP){ // if not empty then we have to progress forward, computing the necessary actions.
        //cout <<"Checking for timed automata This should not occur" << endl;
        //printC();
        if(!stackClosure(a,true)){ //  make it true for timed stack
            return false;
        }
            //        else if(!closure(a)){
            //            return false;
            //        }
        else if(a.number_of_stacks > 1){
            run barrier = make_pair(a.transition_list.size(),true);
            //printC();
            //cout <<"This will come for 2 stacks" << endl;
            computeD(a);// compute the D_i matrices.

            //now we have to do closure on D_ic matrics.
            closureD(a); // compute the D1 and D2 closure.
            //cout <<"Test" << endl;
            //printD(D1);
            //printDc(D1c);
            //printD(D2);
            //printDc(D2c);
            //now we can do the computation.
            unordered_map<string,unsigned long> multi_map;
            MultiStack M(*this); // then create a Multistack object by calling the constructor with the current concatenation object.
            vector<MultiStack> listOfMultiStack;//and a multistack list which will contain all the next multistack objects
            listOfMultiStack.push_back(M); // initialize the list using this.
            multi_map[M.to_string()] = listOfMultiStack.size(); // start to map the current state.
            //vector<MultiStack>::iterator MultiIt;
            //for( int MultiI = 0; MultiI <listOfMultiStack.size();MultiI++){
            ul MultiI =0;
            while(MultiI < listOfMultiStack.size()){ // loop till you reach the last point.
                MultiStack MultiIt(listOfMultiStack[MultiI]); // get the front
                //cout << MultiI << endl;
                //MultiIt.print();
                //do the operation creating hole and removing hole.
                if((MultiIt).holes <= a.contextBound){ // the number of holes does not go beyond the context bound provided in the input.
                    //cout << MultiIt.holes << " " << a.contextBound << "size " << MultiI<< endl;
                    /*** ADDING COMPLETE BLOCK****/
                    if(MultiI == 0 || (MultiIt).isPop ){ // if it is first state or the last operation is a pop then we have to append the Complete
                        vector< vector<State> > afterCompleteBlock = (MultiIt).addComplete(*this,a); //all possible states by adding the complete block
                        //                        vector<vector<State> >::iterator vector_it;
                        //cout << "In Complete" << endl;
                        for(unsigned long vector_it = 0;vector_it < afterCompleteBlock.size();++vector_it){ //loop through all the new multistack states
                            //bool newPop = false; //last operation is not a pop anymore.
                            State endState = afterCompleteBlock[vector_it][afterCompleteBlock[vector_it].size()-1];
                            State secondEndState = afterCompleteBlock[vector_it][afterCompleteBlock[vector_it].size()-2]; //mark the end state

                            ul timeLapsed=0;
                            for(ul index1 = 0;index1 < afterCompleteBlock[vector_it].size();index1++){
                                timeLapsed = min(timeLapsed+afterCompleteBlock[vector_it][index1].time,a.max_const); // just add the time.
                            }
                            
                            MultiStack Mc(afterCompleteBlock[vector_it],(MultiIt).holes,(MultiIt).lastPush,false,(MultiIt).Stack1,(MultiIt).Stack2,MultiI,1,timeLapsed); // because complete context doesnot change anything here.

                            if(endState.state.first+1 == a.final_state && (MultiIt).Stack1.size() == 0 && (MultiIt).Stack2.size() == 0){ // no pending push and we have reached to the final state of the automata
                                //cout << toString(endState.state) << endl; // this block is checking for final state and if it is then we will print the state and return false(which means non empty)
                                //Mc.print();
                                ///WITNESS PRINTING
                                listOfMultiStack.push_back(Mc);
                                stack<run> stack1,stack2;
                                stack1.push(barrier);
                                stack2.push(barrier);

                                /* cout << "The parent ID is " << MultiI << endl;
                                cout << "The parent op is " << listOfMultiStack[listOfMultiStack.size()-1].parentOp << endl;
                                cout << "The parent IDis " << listOfMultiStack[listOfMultiStack.size()-1].parentId << endl;
                                                            */
                                clock_t start = clock();
                                vector<run> runvector = Witness3(listOfMultiStack.size()-1,listOfMultiStack,a,stack1,stack2);
                                clock_t stop = clock(); // end of time calculation
    
                                witness_time = (double)(stop - start) * 1000.0 / CLOCKS_PER_SEC; // Time elapsed calculation 
    
                                
                                //cout << "Size of the run " << runvector.size() << endl;

                                cout << "<==========>THE WITNESS<==============>: " << endl;
                                printRun(runvector,a);
                                cout << "<==========>WITNESS END<==============>: " << endl;
                                //cout << "Witness Generation Execution time : " << (witness_time/1000) << " seconds." << endl << endl;
                                //parentPrint(listOfMultiStack.size()-1,listOfMultiStack,a);
                                //cout << "Adding a complete context and ending" << endl;
                                ////WITNESS PRINTING
                                ///
                                return false;
                            }
                            if(multi_map[Mc.to_string()]){} //if the state already created then we do nothing.
                            else{ // other wise add it, and print it and pushback to the list of states.
                                //Mc.print();
                                //cout <<"After complete the state is here " << endl;
                                //cout << "the id of the before state is " << listOfMultiStack.size() << endl;
                                listOfMultiStack.push_back(Mc); // push back this object to the list of multistack objects.
                                multi_map[Mc.to_string()] = listOfMultiStack.size();// update it in the map.
                            }
                        }
                    }
                    /*** ADDING HOLE OF STACK 1 **/
                    if(((MultiIt).isPop || (MultiIt).lastPush == -1 ||(MultiIt).lastPush == 2 ) && MultiIt.holes < a.contextBound){ // lastpush == -1 means it is the first.
                        vector< vector<State> >afterCompleteBlock = (MultiIt).addHoleStack1(*this,a); // create a hole of stack 1
                        //cout << "Adding a hole of Stack 1 " << endl;
                        for(unsigned long vector_it = 0;vector_it < afterCompleteBlock.size();++vector_it){
                            unsigned long indexEnd = afterCompleteBlock[vector_it].size()-1; // get the right most point of the hole
                            vector<unsigned long> newStack1 = (MultiIt).Stack1; // create a new stack for that
                            newStack1.push_back(indexEnd); // push that end point of hole here
                            ul totalTimeL = min(afterCompleteBlock[vector_it][indexEnd-1].time + MultiIt.totalTimeLapsed,a.max_const); //compute the total time lapsed
                            MultiStack Mc(afterCompleteBlock[vector_it],(MultiIt).holes + 1,1,false,newStack1,(MultiIt).Stack2,MultiI,2,totalTimeL); // because complete context doesnot change anything here.

                            if(multi_map[Mc.to_string()]){}
                            else{
                                /* Mc.print(); */
                                listOfMultiStack.push_back(Mc); // push back this object to the list of multistack objects.
                                multi_map[Mc.to_string()] = listOfMultiStack.size();
                            }
                        }
                    }
                    /*** ADDING HOLE OF STACK 2 ***/
                    if(((MultiIt).isPop || (MultiIt).lastPush == -1 || (MultiIt).lastPush == 1 ) && MultiIt.holes < a.contextBound){
                        //cout << "Adding a hole of Stack 2 " << endl;
                        vector< vector<State> > afterCompleteBlock = (MultiIt).addHoleStack2(*this,a);
                        //vector<vector<State> >::iterator vector_it;
                        for(unsigned long vector_it = 0;vector_it < afterCompleteBlock.size();++vector_it){ // creating a hole of stack 2.
                            unsigned long indexEnd = afterCompleteBlock[vector_it].size()-1;//the last state
                            vector<unsigned long> newStack2 = (MultiIt).Stack2;
                            newStack2.push_back(indexEnd);
                            ul totalTimeL = min(afterCompleteBlock[vector_it][indexEnd-1].time+MultiIt.totalTimeLapsed,a.max_const); // the time is saved in the last.

                            MultiStack Mc(afterCompleteBlock[vector_it],(MultiIt).holes + 1,2,false,(MultiIt).Stack1,newStack2,MultiI,3,totalTimeL); // because complete context doesnot change anything here.
                            if(multi_map[Mc.to_string()]){}
                            else{
                             /*    Mc.print(); */
                                listOfMultiStack.push_back(Mc); // push back this object to the list of multistack objects.
                                multi_map[Mc.to_string()] = listOfMultiStack.size();
                            }
                        }
                    }
                    //**** ADDING POP *** //
                    if((MultiIt).lastPush != -1){ //if it is not first state then we will always try to add a pop.
                        states lastState = ((MultiIt).currStates[(MultiIt).currStates.size()-1]).state; // extract the last state(state valuation pair) information from the multistack object.
                        
                        for(unsigned long transitionIt = 0;transitionIt < a.transition_list.size();transitionIt++){ // for all transitions check for transitions with pop
                            if(a.transition_list[transitionIt].stackOp.op == 2 && a.transition_list[transitionIt].firable(lastState)){ // if the transition have a pop operation and it is firable from the last state.
                                /* ### THE POP OF STACK 2 ###*/
                                //compute the next possible state after pop
                                ul nextState = a.transition_list[transitionIt].destination_state-1;
                                vector<ul> nextValuation = a.transition_list[transitionIt].afterReset(lastState.second);
                                states afterPop = make_pair(nextState,nextValuation);
                                State AfterPopState;
                                AfterPopState.state = afterPop;
                                AfterPopState.time = 0;
                                AfterPopState.h = 0;
                                // check which stack operation is it
                                if((MultiIt).lastPush !=2 && a.transition_list[transitionIt].stackOp.stack_pop == 2 && (MultiIt).Stack2.size() != 0){
                                    
                                     //we dont care consecutive push pop of same stack as it is handled already

                                    int stackSymbolT = a.transition_list[transitionIt].stackOp.stack_alphabet_pop;//extract the stack symbol from the transition
                                    ul HoleLastPoint = MultiIt.Stack2.back(); // get the last element of the stack.
                                    ul HoleLeftPoint = HoleLastPoint -1; // the index of the point where the time information of the hole is stored
                                    ul timeInHole = MultiIt.currStates[HoleLeftPoint].time; // get the time in hole information

                                    states HoleLeftState = MultiIt.currStates[HoleLeftPoint].state; // extract the state information of the holes left point
                                    states HoleRightState = MultiIt.currStates[HoleLastPoint].state; // extract the state information of the holes right point
                                    ul indexOfHoleLeftState = state_map[toString(HoleLeftState)]-1; // compute the index of the holes left point
                                    ul indexOfHoleRightState = state_map[toString(HoleRightState)]-1;  /// compute the index of the holes left point
                                    
                                    ul totalTimeForStack = 0;
                                    for(ul index1 = HoleLastPoint; index1 < MultiIt.currStates.size();index1++){
                                        totalTimeForStack = min(totalTimeForStack+ MultiIt.currStates[index1].time,a.max_const); // compute total time lapsed.
                                    }
                                    for(unsigned long stateIterator = 0; stateIterator < list_of_states.size();stateIterator++){ // loop for all states to get a good states
                                    set<ul> leftBag = D2c(indexOfHoleLeftState,stateIterator); // get the possible value of times
                                    set<ul> rightBag = D2(stateIterator,indexOfHoleRightState).second;
                                    int stack_push_alph = D2(stateIterator,indexOfHoleRightState).first;
                                    for(auto left: leftBag){
                                        for(auto right:rightBag){
                                            if(min(left+right,a.max_const) == timeInHole && a.transition_list[transitionIt].checkStackConstraint(min(right+totalTimeForStack,a.max_const)) && stack_push_alph == stackSymbolT){
                                                vector<State> presentStates = MultiIt.currStates;
                                                //this part is for closing the hole
                                                if(indexOfHoleLeftState == stateIterator && left == 0){ // right will retain the information of the time.
                                                    // then we will close the hole.
                                                    //cout << "Closing the Hole of Stack 2 " << endl;
                                                    presentStates.erase(presentStates.begin()+HoleLeftPoint); // erasing the stuff.
                                                    vector<ul> newStack1 = MultiIt.Stack1;
                                                    for(auto stack1Id = 0; stack1Id<newStack1.size();stack1Id++){
                                                        if(newStack1[stack1Id]> HoleLeftPoint){
                                                            newStack1[stack1Id]= MultiIt.Stack1[stack1Id]-1; // reduce the values in the stack.
                                                        }
                                                    }
                                                    presentStates[HoleLeftPoint].h =0; // update the hole information to 0
                                                    presentStates[HoleLeftPoint].time = min(presentStates[HoleLeftPoint].time + right, a.max_const);
                                                    //add the after pop state to the end of this list of states and make sure you check the othe stack contents.
                                                    if(!MultiIt.Stack1.empty() && MultiIt.currStates.size()-1 == MultiIt.Stack1.back()){
                                                        
                                                        presentStates.push_back(AfterPopState); // if the last point is the last hole point of some other stack 
                                                    }
                                                    else{
                                                        presentStates[presentStates.size()-1]=AfterPopState; // if the last point is not a last hole point.
                                                    }
                                                    //update the stack info of other thing
                                                    //update the time by adding "right" to the right point of the hole.
                                                    // then update the last point by adding the pop point. and replacing it.
                                                    vector<ul> newStack2 = MultiIt.Stack2;
                                                    newStack2.pop_back();
                                                    MultiStack Mclose(presentStates,MultiIt.holes-1,0,true,newStack1,newStack2,MultiI,7,MultiIt.totalTimeLapsed);
                                                    //check if this state is already in the list or not. if not add it in the list also check if this is reaching some state :D
                                                    if(multi_map[Mclose.to_string()]){}
                                                    else{
                                              /*           Mclose.print(); */
                                                        listOfMultiStack.push_back(Mclose); // push back this object to the list of multistack objects.
                                                        multi_map[Mclose.to_string()] = listOfMultiStack.size();
                                                        if(afterPop.first+1 == a.final_state && Mclose.Stack1.size() == 0 && (Mclose).Stack2.size() == 0){ // no pending push and we have reached to the final state of the automata
                                                    //here we will call the witness generation
                                                    ///WITNESS PRINTING
                                                    
                                                    stack<run> stack1,stack2;
                                                    stack1.push(barrier);
                                                    stack2.push(barrier);
                                                    clock_t start = clock();
                                                    vector<run> runvector = Witness3(listOfMultiStack.size()-1,listOfMultiStack,a,stack1,stack2);
                                                    clock_t stop = clock(); // end of time calculation
    
                                                    witness_time = (double)(stop - start) * 1000.0 / CLOCKS_PER_SEC; // Time elapsed calculation 
    
                                                     cout << "<==========>THE WITNESS<==============>: " << endl;
                                                    printRun(runvector,a);
                                                    cout << "<==========>WITNESS END<==============>: " << endl;
                                                    //cout << "Witness Generation Execution time : " << (witness_time/1000) << " seconds." << endl << endl;
                                                    //parentPrint(listOfMultiStack.size()-1,listOfMultiStack,a);
                                                    
                                                    ////WITNESS PRINTING
                                                    ///
                                                    return false;
                                                }
                                                        
                                                    }


                                                }
                                                presentStates = MultiIt.currStates;//get old thing back togeter
                                                //This part is when we decide not to close the hole.
                                                //here we will get one next possible state and 
                                                State MidState;
                                                MidState.state = list_of_states[stateIterator];
                                                MidState.time = min(right+presentStates[HoleLastPoint].time,a.max_const);
                                                MidState.h = presentStates[HoleLastPoint].h;
                                                presentStates[HoleLeftPoint].time = left; // update the time in the hole left point.
                                                presentStates[HoleLastPoint] = MidState; // update the hole last point

                                                if(!MultiIt.Stack1.empty() && MultiIt.currStates.size()-1 == MultiIt.Stack1.back()){
                                                        
                                                        presentStates.push_back(AfterPopState); // if the last point is the last hole point of some other stack 
                                                    }
                                                    else{
                                                        presentStates[presentStates.size()-1]=AfterPopState; // if the last point is not a last hole point.
                                                    }
                                                // here if we are not removing any points replace the current right point by the new one by updating the information.

                                                MultiStack Mc(presentStates,MultiIt.holes,0,true,MultiIt.Stack1,MultiIt.Stack2,MultiI,6,MultiIt.totalTimeLapsed);
                                                //check if this state already exists or not, if not then add this state in list
                                                if(multi_map[Mc.to_string()]){}
                                                else{
                                                    /* Mc.print(); */
                                                    listOfMultiStack.push_back(Mc); // push back this object to the list of multistack objects.
                                                    multi_map[Mc.to_string()] = listOfMultiStack.size();
                                                }

                                            }
                                        }
                                    }
                                    
                                    }

                                    
                                        
                                    
                                }
                                //**** THE POP OF STACK 1 **** //
                                if((MultiIt).lastPush !=1 && a.transition_list[transitionIt].stackOp.stack_pop == 1 && (MultiIt).Stack1.size() != 0){
                                    
                                     //we dont care consecutive push pop of same stack as it is handled already

                                    int stackSymbolT = a.transition_list[transitionIt].stackOp.stack_alphabet_pop;//extract the stack symbol from the transition
                                    ul HoleLastPoint = MultiIt.Stack1.back(); // get the last element of the stack.
                                    ul HoleLeftPoint = HoleLastPoint -1; // the index of the point where the time information of the hole is stored
                                    ul timeInHole = MultiIt.currStates[HoleLeftPoint].time; // get the time in hole information

                                    states HoleLeftState = MultiIt.currStates[HoleLeftPoint].state; // extract the state information of the holes left point
                                    states HoleRightState = MultiIt.currStates[HoleLastPoint].state; // extract the state information of the holes right point
                                    ul indexOfHoleLeftState = state_map[toString(HoleLeftState)]-1; // compute the index of the holes left point
                                    ul indexOfHoleRightState = state_map[toString(HoleRightState)]-1;  /// compute the index of the holes left point
                                    
                                    ul totalTimeForStack = 0;
                                    for(ul index1 = HoleLastPoint; index1 < MultiIt.currStates.size();index1++){
                                        totalTimeForStack = min(totalTimeForStack+ MultiIt.currStates[index1].time,a.max_const); // compute total time lapsed.
                                    }
                                    for(unsigned long stateIterator = 0; stateIterator < list_of_states.size();stateIterator++){ // loop for all states to get a good states
                                    set<ul> leftBag = D1c(indexOfHoleLeftState,stateIterator); // get the possible value of times
                                    set<ul> rightBag = D1(stateIterator,indexOfHoleRightState).second;
                                    int stack_push_alph = D1(stateIterator,indexOfHoleRightState).first;
                                    for(auto left: leftBag){
                                        for(auto right:rightBag){
                                            if(min(left+right,a.max_const) == timeInHole && a.transition_list[transitionIt].checkStackConstraint(min(right+totalTimeForStack,a.max_const)) && stack_push_alph == stackSymbolT){
                                                vector<State> presentStates = MultiIt.currStates;
                                                //this part is for closing the hole
                                                if(indexOfHoleLeftState == stateIterator && left == 0){ // right will retain the information of the time.
                                                    // then we will close the hole.
                                                    //cout << "Closing the Hole of Stack 1 " << endl;
                                                    presentStates.erase(presentStates.begin()+HoleLeftPoint); // erasing the stuff.
                                                    vector<ul> newStack2 = MultiIt.Stack2;
                                                    for(auto stack2Id = 0; stack2Id<newStack2.size();stack2Id++){
                                                        if(newStack2[stack2Id]> HoleLeftPoint){
                                                            newStack2[stack2Id]= MultiIt.Stack2[stack2Id]-1; // reduce the values in the stack.
                                                        }
                                                    }
                                                    presentStates[HoleLeftPoint].h =0; // update the hole information to 0
                                                    presentStates[HoleLeftPoint].time = min(presentStates[HoleLeftPoint].time + right, a.max_const);
                                                    //add the after pop state to the end of this list of states and make sure you check the othe stack contents.
                                                    if(!MultiIt.Stack2.empty() && MultiIt.currStates.size()-1 == MultiIt.Stack2.back()){
                                                        
                                                        presentStates.push_back(AfterPopState); // if the last point is the last hole point of some other stack 
                                                    }
                                                    else{
                                                        presentStates[presentStates.size()-1]=AfterPopState; // if the last point is not a last hole point.
                                                    }
                                                    //update the stack info of other thing
                                                    //update the time by adding "right" to the right point of the hole.
                                                    // then update the last point by adding the pop point. and replacing it.
                                                    vector<ul> newStack1 = MultiIt.Stack1;
                                                    newStack1.pop_back();
                                                    MultiStack Mclose(presentStates,MultiIt.holes-1,0,true,newStack1,newStack2,MultiI,5,MultiIt.totalTimeLapsed);
                                                    //check if this state is already in the list or not. if not add it in the list also check if this is reaching some state :D
                                                    if(multi_map[Mclose.to_string()]){}
                                                    else{
                                                        //Mclose.print();
                                                        listOfMultiStack.push_back(Mclose); // push back this object to the list of multistack objects.
                                                        multi_map[Mclose.to_string()] = listOfMultiStack.size();
                                                        if(afterPop.first+1 == a.final_state && Mclose.Stack1.size() == 0 && (Mclose).Stack2.size() == 0){ // no pending push and we have reached to the final state of the automata
                                                    //here we will call the witness generation
                                                    ///WITNESS PRINTING
                                                    
                                                    stack<run> stack1,stack2;
                                                    stack1.push(barrier);
                                                    stack2.push(barrier);
                                                    clock_t start = clock();
                                                     vector<run> runvector = Witness3(listOfMultiStack.size()-1,listOfMultiStack,a,stack1,stack2);
                                                    clock_t stop = clock(); // end of time calculation
    
                                                    witness_time = (double)(stop - start) * 1000.0 / CLOCKS_PER_SEC; // Time elapsed calculation 
    
                                                    cout << "<==========>THE WITNESS<==============>: " << endl;    
                                                     printRun(runvector,a);
                                                     cout << "<==========>WITNESS END<==============>: " << endl;
                                                     //cout << "Witness Generation Execution time : " << (witness_time/1000) << " seconds." << endl << endl;
                                                    //parentPrint(listOfMultiStack.size()-1,listOfMultiStack,a);
                                                    ////WITNESS PRINTING
                                                    ///
                                                    return false;
                                                }
                                                        
                                                    }


                                                }
                                                presentStates = MultiIt.currStates;//get old thing back togeter
                                                //This part is when we decide not to close the hole.
                                                //here we will get one next possible state and 
                                                State MidState;
                                                MidState.state = list_of_states[stateIterator];
                                                MidState.time = min(right+presentStates[HoleLastPoint].time,a.max_const);
                                                MidState.h = presentStates[HoleLastPoint].h;
                                                presentStates[HoleLeftPoint].time = left; // update the time in the hole left point.
                                                presentStates[HoleLastPoint] = MidState; // update the hole last point

                                                if(!MultiIt.Stack2.empty() && MultiIt.currStates.size()-1 == MultiIt.Stack2.back()){
                                                        
                                                        presentStates.push_back(AfterPopState); // if the last point is the last hole point of some other stack 
                                                    }
                                                    else{
                                                        presentStates[presentStates.size()-1]=AfterPopState; // if the last point is not a last hole point.
                                                    }
                                                // here if we are not removing any points replace the current right point by the new one by updating the information.

                                                MultiStack Mc(presentStates,MultiIt.holes,0,true,MultiIt.Stack1,MultiIt.Stack2,MultiI,4,MultiIt.totalTimeLapsed);
                                                //check if this state already exists or not, if not then add this state in list
                                                if(multi_map[Mc.to_string()]){}
                                                else{
                                                    /* Mc.print(); */
                                                    listOfMultiStack.push_back(Mc); // push back this object to the list of multistack objects.
                                                    multi_map[Mc.to_string()] = listOfMultiStack.size();
                                                }

                                            }
                                        }
                                    }
                                    
                                    }

                                    
                                        
                                    
                                }
                            }
                        }
                    }
                }
                MultiI++;
            }
        }

        else{
            //printC();
            //closure(a);
            return true;
        }
    }
    return true;
}





bool Concatenation::stackClosure(const Automata& a,bool timed){
    bool flag = true ; //looping constant we want to loop as long as any changes happen in  the matrix
    //unordered_pair<int,int> mapOfdone;
    while(flag){
        flag = false; // initially this is true to get in to loop
        for(unsigned long k =0;k<list_of_states.size();k++){ // for all states
            for(unsigned long i =0; i<a.transition_list.size();i++){ // check if any push transitinons is firable
                if(a.transition_list[i].stackOp.op == 1 && a.transition_list[i].firable(list_of_states[k])){ // if firable then create a new state for destination index
                    //a.transition_list[i].printTransition(cout);
                    states AfterPushState = make_pair(a.transition_list[i].destination_state-1,a.transition_list[i].afterReset(list_of_states[k].second));
                    unsigned long index = state_map[toString(AfterPushState)]-1; // get the new states index
                    //if(!(mapOfdone[k])){ // make sure we are not adding same stack again. if its not empty then it is already mapped.
                    //cout << "States after Push: " << toString(AfterPopState)<< " " <<  index << " " << toString(list_of_states[index]) <<endl;
                    for(unsigned long n =0;n<list_of_states.size();n++){//check if any states are reachable from that destination index

                        if(!C(index,n).empty()){ // check if the bag in the location is empty which means we cant reach from the index to the state n
                            //if(toString(list_of_states[index]) == "30") { cout << "this should be 20 " << toString(list_of_states[n]) << endl; }
                            for(unsigned long j =0;j<a.transition_list.size();j++){ // if not empty then check if any pop transition is firable from the reachable state n
                                if(a.transition_list[j].stackOp.op ==2 && a.transition_list[j].firable(list_of_states[n])){ // check timing constraints
                                    //a.transition_list[j].printTransition(cout);
                                    if(a.push_pop_check(i,j)){ // check if the alphabet and stack in the push is equal to the alphabet and stack in the pop.
                                        // get the bag.
                                        // if the stack is timed stack
                                        set<ul>   satisfied = a.transition_list[j].checkStackConstraint(C(index,n)); // check if any of the time is satisfied
                                        //cout << "satisfied set" << setToString(satisfied) << endl;
                                        if(!satisfied.empty()){ // if the set satisfied is not empty
                                            //mapOfdone[k] = 1;
                                            //cout << "TEST" << endl;
                                            states destinationState = make_pair(a.transition_list[j].destination_state-1,a.transition_list[j].afterReset(list_of_states[n].second)); // then check the destination state.
                                            unsigned long index2 = state_map[toString(destinationState)]-1; // get the index of the after pop state // get the new index
                                            set<unsigned long> oldset = C(k,index2); // keep the old size so that you can check if anything changed in this iteration or not.
                                            //                                            int set_size1 = C(k,index).size();
                                            // int set_size2 = C(n,index2).size();

                                            //C(k,index).insert(0); // push transition
                                            //C(n,index2).insert(0);// pop transition 
                                            C(k,index2).insert(satisfied.begin(),satisfied.end()); // insert all the elements that satisfies the pop constraint if no one 
                                            if(toString(list_of_states[k])==toString(start_state) && destinationState.first+1 == a.final_state){ // check for final state in the destination.
                                                //cout << "here" << endl;
                                                printWitness2(k,state_map[toString(destinationState)]-1,a);
                                                return false;
                                            }
                                            //satisfies the condition then we  
                                            //flag = true;
                                            //printC();
                                            // if we can reach to final state via this closure then we should stop wasting time. 
                                            //cout << set_size << " " << satisfied.size() << " " << C(k,index2).size() << endl;
                                            if(oldset!= C(k,index2)){ // any change of the matrix will lead to 
                                                semiClosure(a,k,index2);
                                                flag = true; // mark the flag true to denote that a change happened
                                            }
                                        }
                                    }
                                }
                            }
                        }

                    }
                }
                //}

            }

        }
        if(flag){
            if(!closure(a)) { cout << "closure inside stack closure " << endl; return false;}
        }

    }
    return true;
}

void Concatenation::printC(){ // this is used to print the bags in the matrix C 
    cout << "\t";
    for(unsigned long j =0;j<list_of_states.size();j++){
        cout << toString(list_of_states[j]) << "("<< j<<")" <<"\t";
    }
    cout << endl;
    for(unsigned long i =0 ;i < list_of_states.size();i++){
        cout << toString(list_of_states[i]) << "\t";
        for(unsigned long j =0;j<list_of_states.size();j++){
            cout << setToString(C(i,j)) << "\t" ;
        }
        cout << endl;
    }
}
void Concatenation::printD(const bnu::matrix<entry > &D){ // this is used to print the bags in the matrix C 
    cout << "\t";
    for(unsigned long j =0;j<list_of_states.size();j++){
        cout << toString(list_of_states[j]) <<"\t";
    }
    cout << endl;
    for(unsigned long i =0 ;i < list_of_states.size();i++){
        cout << toString(list_of_states[i]) << "\t";
        for(unsigned long j =0;j<list_of_states.size();j++){
            cout << D(i,j).first << "," << setToString(D(i,j).second) << "\t" ;
        }
        cout << endl;
    }
}

void Concatenation::printDc(const bnu::matrix<set<unsigned long> > &D){ // this is used to print the bags in the matrix C
    cout << "\t";
    for(unsigned long j =0;j<list_of_states.size();j++){
        cout << toString(list_of_states[j]) <<"\t";
    }
    cout << endl;
    for(unsigned long i =0 ;i < list_of_states.size();i++){
        cout << toString(list_of_states[i]) << "\t";
        for(unsigned long j =0;j<list_of_states.size();j++){
            cout << setToString(D(i,j)) << "\t" ;
        }
        cout << endl;
    }
}
string setToString(set<unsigned long> s){ // convert a set to a string to ease printing it in terminal.
    string returnstring;
    returnstring = "{";
    set<unsigned long>::iterator set_it;
    for(set_it = s.begin();set_it!=s.end();++set_it){
        if(set_it == s.begin())
            returnstring+=to_string(*(set_it));
        else
            returnstring = returnstring +","+to_string(*(set_it)); // get a comma in between the times for clarification.
    }
    returnstring += "}";
    return returnstring;
}

vector<vector<State> > MultiStack::addComplete( const Concatenation &Con, const Automata &a){ //this function appends a complete context afte the run.
    unsigned long lastStateIndex = Con.state_map.at(toString(currStates[currStates.size()-1].state))-1;// take the last state valuation pair of the state
    //cout <<"In addComplete" << endl;
    vector< vector<State> > returnVector; // create a return vector which will contain all possible states to return.
    for(unsigned long i = 0; i<Con.list_of_states.size(); i++){ // loop for all the sates
        set<unsigned long > bag = Con.C(lastStateIndex,Con.state_map.at(toString(Con.list_of_states[i]))-1); // check if a complete block is possible.
        if(!bag.empty()){ // we will only add the reachable states.
            for(auto it: bag){ // for all time elapses in the bag we have to create a new state.it is the time we are selecting here
                vector<State> possibilities = currStates;//copy a dummy state from the existing state. currstates is the class variable of the currentt lsit of states.
                // if(possibilities.size() >=2){ // if the current states more than 2
                //     possibilities[possibilities.size()-2].time = min(possibilities[possibilities.size()-2].time + it,Con.maximum_stack_constant); // assign the timelapse
                // }
                //possibilities[possibilities.size()-1].stackSymbol= -1; // because it is a complete block
                if(possibilities.size() == 1){ // the first state}
                    possibilities[possibilities.size()-1].time = it;
                    State AfterPopState; // create the next possible state in the sequence
                    AfterPopState.state = Con.list_of_states[i]; // assigne the values of the next possible state
                //cout << toString(AfterPopState.s) <<endl;
                    AfterPopState.h=0; // this is a flag which tells if this is the start of a hole or not and if then which stack?
                    AfterPopState.time = 0; // this time will be used to add to total time lapsed during the computation.
                    possibilities.push_back(AfterPopState);
                }
                else{
                    possibilities[possibilities.size()-2].time = min(possibilities[possibilities.size()-2].time+it,a.max_const);
                    State AfterPopState;
                    AfterPopState.state = Con.list_of_states[i];
                    AfterPopState.h = 0;
                    AfterPopState.time=0;
                    possibilities[possibilities.size()-1]=AfterPopState;
                }
                //AfterPopState.stackSymbol = -1; // clearly for the last state the stack symbol should be negetive.
                 // now push_back the state to the vector.
                returnVector.push_back(possibilities); // 
            }

        }
    }
    return returnVector;
}
vector <vector<State> > MultiStack::addHoleStack1( const Concatenation &Con, const Automata &a){ // this function adds a hole of stack1 after the given run.

    //cout << "In addHoleStack1" << endl;
    
    bool flag= false;
    if(!Stack2.empty()){ // get the right side of other hole
        if(currStates.size()-1==Stack2[Stack2.size()-1])
            flag = true; // this means we need to add a new state before we can add new hole.
    }   
    unsigned long lastStateIndex = Con.state_map.at(toString(currStates[currStates.size()-1].state))-1;//get the last state of the list 
    vector< vector<State> > returnVector; // create a return vector which will contain all possible states to return.
    for(unsigned long i = 0; i<Con.list_of_states.size(); i++){ // loop for all the states to check which one will satisfy the hole stack requirement.
        set<unsigned long > bag = Con.D1c(lastStateIndex,Con.state_map.at(toString(Con.list_of_states[i]))-1); // check if a complete block is possible.
        if(!bag.empty()){
                    for(auto it: bag){ // for all time elapses in the push closure,
                        
                            vector<State> possibilities = currStates;//copy a dummy state from the existing state.
                            if(flag){
                            possibilities[possibilities.size()-1].time = 0; // assign the timelapse
                            //possibilities[possibilities.size()-1].h = 0;
                            //possibilities[possibilities.size()-1].stackSymbol = stackSymbol; // because it is a hole of stack 1
                            State AfterPopState; // create the next possible state in the sequence
                            AfterPopState.state = possibilities[possibilities.size()-1].state;
                            AfterPopState.time = it;
                            AfterPopState.h = 0;
                            possibilities.push_back(AfterPopState);
                            AfterPopState.state = Con.list_of_states[i]; // assigne the values of the next possible state
                            //cout << toString(AfterPopState.s) <<endl;
                            AfterPopState.time = 0; // the time lapse is negetive because this is now the next end state
                            AfterPopState.h = 1;
                            //AfterPopState.stackSymbol = -1; // clearly for the last state the stack symbol should be negetive.
                            possibilities.push_back(AfterPopState); // now push_back the state to the vector.
                            returnVector.push_back(possibilities); // 
                            }
                            else{
                            possibilities[possibilities.size()-1].time = it; // assign the timelapse
                            possibilities[possibilities.size()-1].h = 0;
                            //possibilities[possibilities.size()-1].stackSymbol = stackSymbol; // because it is a hole of stack 1
                            State AfterPopState; // create the next possible state in the sequence
                            AfterPopState.state = Con.list_of_states[i]; // assigne the values of the next possible state
                            //cout << toString(AfterPopState.s) <<endl;
                            AfterPopState.time = 0; // the time lapse is negetive because this is now the next end state
                            AfterPopState.h = 1;
                            //AfterPopState.stackSymbol = -1; // clearly for the last state the stack symbol should be negetive.
                            possibilities.push_back(AfterPopState); // now push_back the state to the vector.
                            returnVector.push_back(possibilities); // 
                            }
                        
                        
                    }

                
            
        }
    
    }
    return returnVector;
}

vector <vector<State> > MultiStack::addHoleStack2( const Concatenation &Con, const Automata &a){ // this function adds a hole of stack1 after the given run.

    bool flag= false;
    if(!Stack1.empty()){ // get the right side of other hole
        if(currStates.size()-1==Stack1[Stack1.size()-1])
            flag = true; // this means we need to add a new state before we can add new hole.
    }   
    unsigned long lastStateIndex = Con.state_map.at(toString(currStates[currStates.size()-1].state))-1;//get the last state of the list 
    vector< vector<State> > returnVector; // create a return vector which will contain all possible states to return.
    for(unsigned long i = 0; i<Con.list_of_states.size(); i++){ // loop for all the states to check which one will satisfy the hole stack requirement.
        set<unsigned long > bag = Con.D2c(lastStateIndex,Con.state_map.at(toString(Con.list_of_states[i]))-1); // check if a complete block is possible.
        if(!bag.empty()){
                    for(auto it: bag){ // for all time elapses in the push closure,
                        
                            vector<State> possibilities = currStates;//copy a dummy state from the existing state.
                            if(flag){
                            possibilities[possibilities.size()-1].time = 0; // assign the timelapse
                            //possibilities[possibilities.size()-1].h = 0;
                            //possibilities[possibilities.size()-1].stackSymbol = stackSymbol; // because it is a hole of stack 1
                            State AfterPopState; // create the next possible state in the sequence
                            AfterPopState.state = possibilities[possibilities.size()-1].state;
                            AfterPopState.time = it;
                            AfterPopState.h = 0;
                            possibilities.push_back(AfterPopState);
                            AfterPopState.state = Con.list_of_states[i]; // assigne the values of the next possible state
                            //cout << toString(AfterPopState.s) <<endl;
                            AfterPopState.time = 0; // the time lapse is negetive because this is now the next end state
                            AfterPopState.h = 2;
                            //AfterPopState.stackSymbol = -1; // clearly for the last state the stack symbol should be negetive.
                            possibilities.push_back(AfterPopState); // now push_back the state to the vector.
                            returnVector.push_back(possibilities); // 
                            }
                            else{
                            possibilities[possibilities.size()-1].time = it; // assign the timelapse
                            possibilities[possibilities.size()-1].h = 0;
                            //possibilities[possibilities.size()-1].stackSymbol = stackSymbol; // because it is a hole of stack 1
                            State AfterPopState; // create the next possible state in the sequence
                            AfterPopState.state = Con.list_of_states[i]; // assigne the values of the next possible state
                            //cout << toString(AfterPopState.s) <<endl;
                            AfterPopState.time = 0; // the time lapse is negetive because this is now the next end state
                            AfterPopState.h = 2;
                            //AfterPopState.stackSymbol = -1; // clearly for the last state the stack symbol should be negetive.
                            possibilities.push_back(AfterPopState); // now push_back the state to the vector.
                            returnVector.push_back(possibilities); // 
                            }
                        
                        
                    }

                
            
        }
    
    }
    return returnVector;
}



MultiStack::MultiStack(vector<State> &state,int holes, int lastPush,bool isPop,vector<unsigned long> Stack1, vector<unsigned long> Stack2,ul parentId, short parentOp, ul totalTime){
    this->currStates = state;
    this->holes = holes;
    this->lastPush = lastPush;
    this->isPop = isPop;
    this->Stack1 = Stack1;
    this->Stack2 = Stack2;
    this->parentId = parentId;
    this->parentOp = parentOp;
    this->totalTimeLapsed = totalTime;
    
}

bool Concatenation::semiClosure(const Automata& A, unsigned long i,unsigned long j){
    for(unsigned long a = 0; a < list_of_states.size();a++){
        for(unsigned long b =0;b < list_of_states.size(); b++){
            if(!C(a,i).empty() && !C(j,b).empty()){
                set<unsigned long> newAdd = additionBags(C(a,i),C(j,b),A.max_const);
                newAdd = additionBags(newAdd,C(i,j),A.max_const);
                C(a,b).insert(newAdd.begin(),newAdd.end());
                if(state_map[toString(list_of_states[a])]==state_map[toString(start_state)] && list_of_states[b].first+1 == A.final_state){

                    return false;
                }
            }
        }
    }
    return true;
}

void MultiStack::print() const{ // will come back to it.

    for(unsigned long i = 0;i<currStates.size()-1;i++){
        cout << toString(currStates[i].state) << " " ;
        if(currStates[i+1].h == 0){} // this means it is not a hole
            ///else if(currStates[i].h == 1){ cout << "C " ;}
        else if(currStates[i+1].h == 1) {cout << "H1 " ;}
        else if(currStates[i+1].h == 2) {cout << "H2 ";}
    }
    cout <<toString(currStates[currStates.size()-1].state) << endl;
    //cout << endl;
}

string MultiStack::to_string(){ // this is a list of possible sequence of state valuation pairs with holes and other things between them.
    string returnString = "";
    for(unsigned long i = 0; i<currStates.size();i++){
        returnString = returnString+toString(currStates[i].state);// convert the sate valuation pair to string
        returnString +=",";
//        returnString = returnString+std::to_string(currStates[i].h);
//        returnString += ",";
        returnString= returnString+std::to_string(currStates[i].time); // convert the integer to string.
        returnString +=",";
        for(unsigned long i = 0; i < Stack1.size(); i++){
            returnString += std::to_string(Stack1[i]); // convert the stack1 entry to string
            returnString += ",";
        }

        for(unsigned long i =0; i<Stack2.size();i++){ // convert the stack2 entry to string
            returnString += std::to_string(Stack2[i]);
            returnString+= ",";
        }
    }
    return returnString;
}


int Concatenation::printWitness2(unsigned long i,unsigned long j, const Automata &a) { // this prints run for the single stack system.
    clock_t witnessTime = clock();
    cout <<"There exists an accepting run of the system as follows:" << endl;
    set<unsigned long> times = C(i, j); // get the set of times
    if(times.empty() ) { cout << "Empty" << endl;}
    cout <<"The size of the satisfying number is " << times.size() << endl;
    vector<run> returnstring ;
    for (auto times_it = times.begin(); times_it != times.end(); ++times_it) { // loop for all possible time lapses
        cout <<"The time from start to final is : " << *times_it << endl;
        //printC();
        returnstring = Witness2(i, *times_it, j, a); // call the witness for the time lapse.
        cout << "The length of run " << returnstring.size() << endl;
        cout << "The source state is " << toString(list_of_states[i]) << endl; // print the source state
        cout << "The destination state is " << toString(list_of_states[j]) << endl; // print the destination state
        //cout <<"The sequence of Transitions and time lapses is :" << endl;
        cout << endl;
        if(!returnstring.empty()){
            
            cout << "<==========>THE WITNESS<==============>: " << endl;
            int ret_i = printRun(returnstring,a);
            cout << "<==========>WITNESS END<==============>: " << endl;
            clock_t witnessEndTime = clock();
            witness_time = (double)(witnessEndTime - witnessTime ) * 1000.0 / CLOCKS_PER_SEC;
            //cout << "TIME FOR WITNESS GENERATION: " << witness_time/1000<< " seconds." << endl << endl;
            return ret_i;

        }
            

    }
    
    cout << "Error" << endl;
    return -1;
    //return returnstring;
}

int Concatenation::printRun(vector<run> runVector,const Automata& a) {
    if (!runVector.empty() ) {
        ul time =0;
        ul length=0;
        for(auto it = runVector.begin(); it!=runVector.end();it++){ //
        
            if((*it).second){//
                if(time > 0){
                    cout << "TL(" << time <<") ";
                }
                cout << a.transition_list[(*it).first].name << " " ;
                length++;
                time =0;
            }
            else {
                time += it->first  ;
            }
        }
        cout << endl;
        cout << endl;
        cout <<"Witness Length " << length << endl;
        
        return 0;
    }
    else{
        cout <<"This run is empty " << endl;
    }
    return -1;
}




vector<run> Concatenation::getPath2(unsigned long index1,unsigned long index2, const Automata& a,unordered_map<string,int> &loop,unordered_map<string,int> &loop2) { // this function gets path for useless reset
    // indices are the state valuation pair indices
    vector<run> list_of_transitions; // initially empty
    ul stateIndex1 = list_of_states[index1].first;
    ul stateIndex2 = list_of_states[index2].first;
    vector<ul> valuationIndex1 = list_of_states[index1].second;
    vector<ul> valuationIndex2 = list_of_states[index2].second;
    if(valuationIndex1 != valuationIndex2){
        cout <<"The valuations are not same so can't progress" << endl;
        list_of_transitions.push_back(make_pair(0,false));
        return list_of_transitions;
    }
    if(stateIndex1 == stateIndex2){ //if the calling index is equal to the destination index then we will return a zero false pair
        list_of_transitions.push_back(make_pair(0,false)); // path from same thing to same thing should be non empty.
        return list_of_transitions;
    }
    for(unsigned long transition_it=0;transition_it<a.transition_list.size();transition_it++){ // iterating throught the transition list
        if(a.transition_list[transition_it].stackOp.op ==0 && a.transition_list[transition_it].source_state != a.transition_list[transition_it].destination_state) { // this will only take care of the transitions without any stack operations.
            
                vector<unsigned long> usefulreset = usefulReset(valuationIndex1,
                                                      a.transition_list[transition_it].reset_vector);// get list of useful reset between the newly reached valuation and the previous valuation
                if (usefulreset.empty() && a.transition_list[transition_it].firable(make_pair(stateIndex1,
                                                                                              valuationIndex1))) { // if there is no useful reset and the transition if firable then only we can do the following things.\
                    //we may have to add an emptiness chekcing here.
                    ul mid_dest = a.transition_list[transition_it].destination_state-1;
                    states mid_dest_s = make_pair(mid_dest,valuationIndex1);
                    ul mid_dest_id = state_map[toString(mid_dest_s)]-1;
                    //cout << toString(mid_dest_s) << " " << toString(list_of_states[index2]) << endl;
                    ostringstream os;
                    os<< mid_dest_id << "|"<<transition_it<<"|" << index2 ;
                    string pair = os.str();
                    if(!C(mid_dest_id,index2).empty() && loop[pair]!=1){
                        loop[pair]=1;
                        list_of_transitions.push_back(make_pair(transition_it,true));
                    //recursion
                        vector<run> next_sequence = getPath2(mid_dest_id,index2,a,loop,loop2);
                       
                        list_of_transitions.insert(list_of_transitions.end(),next_sequence.begin(),next_sequence.end());
                        return list_of_transitions;
                    }
                    
                }

            
        }
    }
    for(ul left_transition_id = 0; left_transition_id < a.number_of_transitions; left_transition_id++){
        if(a.transition_list[left_transition_id].stackOp.op==1 && a.transition_list[left_transition_id].firable(list_of_states[index1]) ){
            vector<unsigned long> usefulreset = usefulReset(valuationIndex1,a.transition_list[left_transition_id].reset_vector);
            if(usefulreset.empty()){
                ul stack_push = a.transition_list[left_transition_id].stackOp.stack_push;
                ul stack_alphabet_push = a.transition_list[left_transition_id].stackOp.stack_alphabet_push;
                ul left_destination_state = a.transition_list[left_transition_id].destination_state-1;
                states left_destination = make_pair(left_destination_state,valuationIndex1);
                ul left_id = state_map[toString(left_destination)]-1;
                for(ul right_transition_id = 0; right_transition_id<a.number_of_transitions;right_transition_id++){
                    if( a.transition_list[right_transition_id].stackOp.op==2 && a.transition_list[right_transition_id].stackOp.stack_pop == stack_push && a.transition_list[right_transition_id].stackOp.stack_alphabet_pop == stack_alphabet_push){
                        ul right_dest_state = a.transition_list[right_transition_id].destination_state-1 ;
                        states right_dest = make_pair(right_dest_state,valuationIndex1);
                        ul right_dest_id = state_map[toString(right_dest)]-1;
                        ul right_start_state = a.transition_list[right_transition_id].source_state-1;
                        states right_start = make_pair(right_start_state,valuationIndex1);
                        ul right_id = state_map[toString(right_start)]-1;
                        vector<unsigned long> usefulreset2 = usefulReset(valuationIndex1,a.transition_list[right_transition_id].reset_vector);
                        ostringstream os1,os2;
                        os1<< left_id<< "|" << right_id ;
                        string pair1 = os1.str();
                        os2<<right_dest_id <<"|" << index2 ;
                        string pair2 = os2.str();
                        if(!C(left_id,right_id).empty() && usefulreset2.empty() && (left_id != index1 || right_id != index2) && !C(right_dest_id,index2).empty() && loop2[pair1]!=1 && loop2[pair2]!=1){
                            loop2[pair1]=1;
                            loop2[pair2]=1;
                            vector<run> list_of_transitions2 = getPath2(left_id,right_id,a,loop,loop2);
                            vector<run> list_of_transitions3 = getPath2(right_dest_id,index2,a,loop,loop2);
                            list_of_transitions.push_back(make_pair(left_transition_id,true));
                            list_of_transitions.insert(list_of_transitions.end(),list_of_transitions2.begin(),list_of_transitions2.end());
                            list_of_transitions.push_back(make_pair(right_transition_id,true));
                            list_of_transitions.insert(list_of_transitions.end(),list_of_transitions3.begin(),list_of_transitions3.end());
                            return list_of_transitions;
                        }
                    }
                }

            }
            
        }
    }
    //cout << "This should not happen! " << "index1: " << index1 << "index2: " << index2 << endl;
return list_of_transitions;
}



vector<run> Concatenation::Witness2(unsigned long index1, unsigned long time ,unsigned long index2, const Automata& a){
   /*  cout << "CALLING WITNESS 2 Again" << endl;
    cout << "Index1: " << index1 << " time: " << time << "Index2: " << index2 << endl; */
    vector<run> empty;
    /* if(time == 0){
    empty = getPath2(index1,index2,a);
    
    if(!empty.empty()){
        return empty;
    }
    } */
    //ul total_elapsed_time = 0; // we need to make sure we elapse the time amount of time in this function
    for(unsigned long s = 0 ; s < a.number_of_states ; s++) { // guess middle state

//Here we are making the possible mid state assuming no time loss and also no useful transition.
        vector<unsigned long> val = list_of_states[index1].second; // extract the valuation from the first state
        states mid = make_pair(s, val); // assume the mid state.
        unsigned long midId = state_map[toString(mid)]-1;
         vector<run> path;
         //cout <<"first: " << index1 << " Second: " << midId << endl;
         if(!C(index1,midId).empty()){
             unordered_map<string,int> loop;
            unordered_map<string,int> loop2;
             path= getPath2(index1, midId,a,loop,loop2);// gets a possible path with only useless transitions and no time lapse from index1 to midId.
         }
       
//        cout << "Path from " << list_of_states[index1].first << " to " << list_of_states[midId].first << " Valuation " << toStringValuation(val) << endl;
        /* cout <<"first: " << index1 << " Second: " << midId << endl;
        cout << "Getpath function output called from witness 2" << endl; */
       //printRun(path,a);
//if we already reached the final state and the time is 0 then we are done.
        if(s == list_of_states[index2].first){ // if we reach final state just by doing this useless transitions then we will get the list of transitions.
            if(!path.empty() && time == 0){ // print the path

                return path;
            }
        }
        //time passing
        if(!path.empty()) {
            //check for possible next states reachable by only time lapse
            //if the time is greater than 0.
            if(time > 0){
            for (unsigned long t1 = 1; t1 <= a.max_const; t1++) { // select one time lapse one by one and apply the time lapse to the state.                
            vector<unsigned long> newTime(val.size());
                for (unsigned long i = 0; i < val.size(); i++) { // loop to increment the values of the vector
                    newTime[i] = min(val[i] + t1, a.maximum_constant.at(i)); // normalize on the fly
                }

                //this state is reachable by lapsing time t1
                states stateNewTime = make_pair(s, newTime); // make new state by elapsing time.
                unsigned long stateNewTimeId =
                        state_map[toString(stateNewTime)] - 1; // get the index of the newly genrated state.
                        //if(stateNewTimeId != index1){ //if time lapse gives same state after that then dont continue
                set<unsigned long> Mid2Last = C(stateNewTimeId,
                                                index2); // get all possible time from mid state to destination state.
                //if (!Mid2Last.empty()) { // if other state is reachable from the middle state.
                for (auto iteration = Mid2Last.begin();
                     iteration != Mid2Last.end(); iteration++) { // for all possible time lapse values.
                    if (min(*iteration + t1, a.max_const) ==
                        time) { // if the time matches with the possible time lapse, this is essential
                        //cout << "The first recursion call Witness2 " << endl; 
                        vector<run> midpath = Witness2(stateNewTimeId, *iteration, index2, a);
                        if (!midpath.empty()) {
                            //cout << "Time Lapsed: " << t1 << endl;
                            //cout << "Path found after time Lapse:" << endl;
                            //printRun(midpath, a);
                            run firstStep = make_pair(t1, false); // first step was a time delay so add time delay at the beginning
                            midpath.insert(midpath.begin(), firstStep);
                            //cout << "New path after added the first time lapse to the mid path " << endl;
                            //printRun(midpath, a);
                            //cout << "Before inserting midpath to the main path" << endl;
                            //printRun(path, a);
                            //cout << "After inserting the midpath to the main path" << endl;
                            path.insert(path.end(), midpath.begin(), midpath.end());
                            //printRun(path, a);
                            //cout << path.size() << endl;
                            return path;
                        }

                    }
                }
                        //}

            }
            }
            // the useful transition part
            for (unsigned long transition_it = 0;
                 transition_it < a.transition_list.size(); transition_it++) {// for all transitions
                if (a.transition_list[transition_it].firable(
                        list_of_states[midId]) && a.transition_list[transition_it].stackOp.op == 0  ) { // check for all NOP transitions which are firable from the transitions.
                    vector<unsigned long> usefulreset = usefulReset(list_of_states[midId].second,
                                                          a.transition_list[transition_it].reset_vector);// get list of useful resets
                    if (!usefulreset.empty()) {//check if there are atleast some resets which are useful
                        vector<unsigned long> after_reset = a.transition_list[transition_it].afterReset(
                                list_of_states[midId].second); // get the valuation of the clocks after the reset operation in the transition.
                        //vector<int> after_normalize = normalization(after_reset,a); // we have to normalize the valuation.
                        states destinationState = make_pair(a.transition_list[transition_it].destination_state - 1,
                                                            after_reset);
                        unsigned long destinationStateIndex =
                                state_map[toString(destinationState)] - 1; // get the index of the destination state.
                        
                        vector<run> midpath ;

                        if( destinationStateIndex != midId){ // make sure you are not looping on the same state.
                        //cout <<"The second recursion call Witness2" << endl;
                        midpath= Witness2(destinationStateIndex, time, index2, a); // recursively call witness from mid path
                        }

                        if (!midpath.empty()) {//if we can not get another mid state from here then it is worrisome.

                            run addtr = make_pair(transition_it, true);
                            midpath.insert(midpath.begin(), addtr);
                            path.insert(path.end(), midpath.begin(), midpath.end());
                            //cout << path.size() << endl;
                            return path;
                        }
                    }
                }

            }


            //if the next possible transitions is a push transition
            // then we need to guess the possible pop transition and along with all the possibilities
            // like if the last pop is not the possible pop.
            for(ul tr = 0; tr < a.transition_list.size(); tr++){
                if(a.transition_list[tr].stackOp.op == 1 && a.transition_list[tr].firable(mid)) { // it should be a push transition and firable from the current state valuation pair.;
                     // store the stack name and alphabet
                    int stack_name_push = a.transition_list[tr].stackOp.stack_push;
                    int stack_alph_push = a.transition_list[tr].stackOp.stack_alphabet_push; // store the stack alphabet.
                    // get the destination state of the transiton
                    ul destination_state = a.transition_list[tr].destination_state-1; 
                    //get the valuation after the transition is fired.
                    vector<ul> afterReset = a.transition_list[tr].afterReset(mid.second); 
                     // create the new state after the transition fired.
                    states afterPushState = make_pair(destination_state,afterReset);
                    // get the index of the state after push state valuation pair.
                    ul afterPushStateId = state_map[toString(afterPushState)]-1; 
                    //for all possible state valuation pairs please check if there exists a possible mid path?.
                    for(ul stateIter = 0 ; stateIter < list_of_states.size();stateIter++){
                        set<ul> possibleTimeLapse = C(afterPushStateId,stateIter) ; // get the possible time lapse between the after pushstate and the guessed
                        //this guessed state is assumed to be the start state of the possible pop transition which matches with the push we discussed earlier.
                        //iterate through the possible time lapses
                        for(ul tr1 = 0; tr1<a.transition_list.size();tr1++){
                            //check if the transition tr1 is a pop transition and firable from the specified state
                            if(a.transition_list[tr1].stackOp.op==2 && a.transition_list[tr1].firable(list_of_states[stateIter])){
                                //store the stack and stack alphabet of the pop transition tr1
                                int stack_name_pop = a.transition_list[tr1].stackOp.stack_pop;
                                int stack_alph_pop = a.transition_list[tr1].stackOp.stack_alphabet_pop;
                                //check if the push stack push stack alphabt matches with the pop alphabet and pop stack
                                if(stack_name_pop == stack_name_push && stack_alph_pop == stack_alph_push){
                                    //check if the pop satisfies the stack timing constrinats.
                                    set<ul> satisfiedStackConst = a.transition_list[tr1].checkStackConstraint(possibleTimeLapse);
                                    if(!satisfiedStackConst.empty()){ // if some stack constraint is satisfied.
                                        //store the destination state of the pop transition
                                        ul destination_state_pop = a.transition_list[tr1].destination_state-1; // get the destination state of pop transition
                                        //get the valuation after reset in the pop transition
                                        vector<ul> afterResetPop = a.transition_list[tr1].afterReset(list_of_states[stateIter].second);
                                        //get the pop state after the transition
                                        states afterPopState = make_pair(destination_state_pop,afterResetPop);
                                        //get the state id of the afterpopstate
                                        ul afterPopStateIt = state_map[toString(afterPopState)]-1;
                                        //get the possible time lapses after the pop till the last index
                                        set<ul> possibleTimeAfterPop = C(afterPopStateIt,index2);
                                        //now it the time to recurse !!!!!
                                        //for all time that satisfies the stack constraint,
                                        for(auto satConst = satisfiedStackConst.begin(); satConst != satisfiedStackConst.end(); satConst++){
                                            vector<ul> valuation_afterPush = afterPushState.second; // the valuation vector of the state after push
                                            vector<ul> valuation_afterPop = afterPopState.second; // the valuation vector of the state after pop state
                                            vector<ul> valuation_stateIter = list_of_states[stateIter].second; // the valuation vector of the state guessed
                                            vector<ul> valuation_index2 = list_of_states[index2].second; // the valuation vector of the index2 state.
                                            vector<run> betweenPushPop;
                                            vector<run> afterPop;
                                            
                                            //if the time between push and pop is 0 then
                                            if(*(satConst) == 0 ){
                                                if(afterPushStateId==stateIter){ // if the afterpush state id is equal to the source state of the pop
                                                    vector<run> another;
                                                    another.push_back(make_pair(tr,true));
                                                    another.push_back(make_pair(tr1,true));
                                                    return another;
                                                }
                                                //if there are no useful transitions then the valuation will not change
                                                //and we will be able to use getPath2 to get possible path between the two states.
                                                    if(toStringValuation(valuation_afterPush) == toStringValuation(valuation_stateIter)&& !C(afterPushStateId,stateIter).empty()){
                                                        unordered_map<string,int> loop1;
                                                        unordered_map<string,int> loop21;
                                                        betweenPushPop = getPath2(afterPushStateId,stateIter,a,loop1,loop21);
                                                    }
                                                    else if(!checkPossibleUseful(valuation_afterPush,valuation_stateIter)){
                                                        //this is not possible because we are not spending time in between
                                                        // so only reset is possible which reduces the value of some clock to 0
                                                        // and doesnot increase value of any clocks in the second vector
                                                        //betweenPushPop = getPathUseful(afterPushStateId,stateIter)
                                                        cout <<"This is not possible Witness 2" << endl;
                                                    }
                                                    else{
                                                        // this is when we have a useful reset.
                                                        cout <<"When Time between push and pop is 0 and there is a possible useful transition "<< endl;
                                                    }

                                                    // if(functionCalled[toStringParameter(afterPopStateIt,time,index2)]){}
                                                    // else{
                                                    //     afterPop = Witness2(afterPopStateIt,time,index2,a);
                                                    //     functionCalled[toStringParameter(afterPopStateIt,time,index2)] = functionCalled.size() + 1;
                                                    // }
                                                //     if(afterPopStateIt == index2 ){
                                                //         afterPop.push_back(make_pair(time,false));
                                                //     }
                                                //     else{
                                                //         afterPop = Witness2(afterPopStateIt,time,index2,a);
                                                //     }
                                                
                                             }
                                            else if(*(satConst) <= time  && *(satConst) > 0){
                                                //cout << "The time between push and pop is " << *satConst << endl;
                                                //time after pop will from the set possibleTimeAfterPop
                                                //and will be added with *(satConst) so that the addition equals
                                                //cout << "The time after pop " << time - *satConst << endl;
                                                //here we will get the valuation vector for the start state middle state and the end state
                                               
                                                //cout << toStringValuation(valuation_afterPush) << " " << toStringValuation(valuation_stateIter) << endl;
                                                //checking if there exists a run without stack
                                                betweenPushPop = Witness2(afterPushStateId,*(satConst),stateIter,a);
                                                if(betweenPushPop.empty()){ // if it is not possible to go 
                                                //cout << "Third recursion call witness2 " << endl;
                                                    betweenPushPop = Witness2(afterPushStateId,*(satConst),stateIter,a);
                                                }
                                                
                                                //cout <<"Print the possible between push pop run inside witness 2 " << endl;
                                                //printRun(betweenPushPop,a);
                                                
                                                
                                                //cout << "afterPop state: " << afterPopState.first <<":"<< toStringValuation(valuation_afterPop) << "totalTime: " << time <<"satTime: "<< *(satConst)  << "index 2 state: "<< list_of_states[index2].first << ":" << toStringValuation(valuation_index2) << endl;
                                                
                                                for(auto afterPopTime = possibleTimeAfterPop.begin();afterPopTime!=possibleTimeAfterPop.end();afterPopTime++){
                                                    if(min(*(afterPopTime)+*(satConst), a.max_const)==time){
                                                        if(*(afterPopTime) == 0 ){
                                                    if( toStringValuation(valuation_afterPop) == toStringValuation(valuation_index2) && !C(afterPopStateIt,index2).empty()){
                                                        //check for useless transitions
                                                        //in this case only useless transitions are possible becuase no time lapsed
                                                        //and the start valuation and the end valuation are same
                                                        //if there was a useful transition then it will reduce atleast one clock value
                                                        //and to get this clock to be the same as the start valuation we need time lapse. Contradiction
                                                        unordered_map<string,int> loop2;
                                                        unordered_map<string,int> loop22;
                                                        afterPop = getPath2(afterPopStateIt,index2,a,loop2,loop22);
                                                        
                                                    }
                                                    else if (!checkPossibleUseful(valuation_afterPop,valuation_index2)){
                                                        //this is not possible because we are not spending time in between
                                                        // so only reset is possible which reduces the value of some clock to 0
                                                        // and doesnot increase value of any clocks in the second 
                                                        cout <<"This is not possible witness 2" << endl;
                                                    }
                                                    else{
                                                        // this is when  we have a useful transition in between.
                                                        cout << "This is useful transitions after pushpop inside witness 2"<< endl;
                                                    }
                                                }
                                                else{
                                                    //when the time is not zero
                                                    cout << "Fourth recursion call Witness2" << endl;
                                                
                                                    afterPop = Witness2(afterPopStateIt,*(afterPopTime),index2,a);
                                                    cout <<"Print the run after pop witness 2" << endl;
                                                    //printRun(afterPop,a);
                                                }

                                                    }
                                                    if(!afterPop.empty())
                                                    {break;}
                                                }

                                                



                                                
                                            }


                                            if(!betweenPushPop.empty() &&!afterPop.empty() ){
                                                    run pushTr = make_pair(tr,true);
                                                    run popTr = make_pair(tr1,true);
                                                    //cout << "before adding push or pop" << endl;
                                                    //printRun(betweenPushPop,a);
                                                    betweenPushPop.insert(betweenPushPop.begin(),pushTr);
                                                    //cout <<"afteradding push transition: " << endl;
                                                     //printRun(betweenPushPop,a);
                                                    betweenPushPop.insert(betweenPushPop.end(),popTr);
                                                    //cout <<"after adding pop transition: " << endl;
                                                     //printRun(betweenPushPop,a);
                                                    
                                                        betweenPushPop.insert(betweenPushPop.end(),afterPop.begin(),afterPop.end());
                                                     
                                                    path.insert(path.end(),betweenPushPop.begin(),betweenPushPop.end());
                                                    //cout<<"printing push-pop path" << endl;
                                                    //printRun(path,a);
                                                    return path;
                                                }
                                            // do something different when the time lapse is zero.
                                            // else if(*(satConst) == 0){
                                            // //this means time difference between push and pop is 0
                                            // // here check if the two states are same 
                                            //  string indexmapstring = to_string(index1) + "-" +to_string(index2);
   
                                            // if(indexmap[indexmapstring]){
                                            // return empty;
                                            // }
                                            // else{
                                            //     indexmapcount++;
                                            //     indexmap[indexmapstring] = indexmapcount;
                                            //     }
                                            // }
                                        }
                                        

                                    }
                                }
                            }
                        }
                    }
                }
            }


        }


    }
    return empty; // this will return if nothing is returned

}


vector<run> Concatenation::Witness3(ul index,const vector<MultiStack>& listOfStates, const Automata& a,stack<run> &stack1,stack<run> &stack2){
    /* cout << "Print the length of the list passed " << listOfStates.size() << endl;
    cout << "The value of index is " << index << endl;
    cout << "The value of the totaltime " << listOfStates[index].totalTimeLapsed<< endl;
    cout << "The value of the parentID " << listOfStates[index].parentId << endl;
    cout << "The value of ParentOp " << listOfStates[index].parentOp << endl;
    cout << "PRINTING THE STATES " << endl; */
    //listOfStates[index].print();
    vector<run> returnVariable;
    ul totalTime = listOfStates[index].totalTimeLapsed;
    ul parentId = listOfStates[index].parentId;
    MultiStack parent = listOfStates[parentId];
    MultiStack current = listOfStates[index];
    states LastStateOfParent = parent.currStates[parent.currStates.size()-1].state;
    ul LastStateOfParentId = state_map[toString(LastStateOfParent)]-1;
    states LastState = current.currStates[current.currStates.size()-1].state;
    ul LastStateId = state_map[toString(LastState)]-1;
    ul parentTotalTime = parent.totalTimeLapsed;
    short parentOp = listOfStates[index].parentOp;
    State endOfHole1, endOfHole2,leftOfHole1,leftOfHole2, PendOfHole1,PendOfHole2,PleftOfMiddle1,PleftOfMiddle2;
    if(current.Stack1.size()>0){
         endOfHole1 = current.currStates[current.Stack1[current.Stack1.size()-1]]; // this is the end hole of stack 1
         leftOfHole1 = current.currStates[current.Stack1[current.Stack1.size()-1]-1];
         //this is necessary when we are shrinking hole size.
    }
    if(current.Stack2.size()>0){
         endOfHole2 = current.currStates[current.Stack2[current.Stack2.size()-1]]; // this is the end hole of stack 2
         leftOfHole2 = current.currStates[current.Stack2[current.Stack2.size()-1]-1];
         // this is useful when we are shrinking hole size
    }
    if(parent.Stack1.size()>0){
         PendOfHole1 = parent.currStates[parent.Stack1[parent.Stack1.size()-1]]; // this is important when we know that the operation is closed hole.
         PleftOfMiddle1 = parent.currStates[parent.Stack1[parent.Stack1.size()-1]-1];
    }
    if(parent.Stack2.size()>0){
         PendOfHole2 = parent.currStates[parent.Stack2[parent.Stack2.size()-1]]; // this is useful when we know that the operation is closed hole.
         PleftOfMiddle2 = parent.currStates[parent.Stack2[parent.Stack2.size()-1]-1];
    }
    stack<run> newStack1,newStack2; // this stacks is to keep track of the hole transitions.
    vector<run> completeRun;
    int stack_alph_pop,stack_alph,push;
    run barrier = make_pair(a.transition_list.size()+1,true); // this will work as a barrier in the stack.
    run check;
    ul push_transition,pop_transition;
    short stack_pop,stack_push;
    if(parentOp==1){ // for all parent ops we will do different things.
    // this one is for the complete context addition, we know that there is a complete context
        completeRun = Witness2(LastStateOfParentId,totalTime - parentTotalTime,LastStateId,a);
        returnVariable = Witness3(parentId,listOfStates,a,stack1,stack2); // get the run of initial part by recursion
        returnVariable.insert(returnVariable.end(),completeRun.begin(),completeRun.end()); // append the run.
        //cout << "This is when the a complete block is added " << endl;
        //printRun(returnVariable,a);
      }
    else if (parentOp == 2){
        

        while(stack1.top().first != a.transition_list.size()+1){
            returnVariable.insert(returnVariable.begin(),stack1.top());
            stack1.pop();
        }
        stack1.pop(); // remove the barrier
        completeRun = Witness3(parentId,listOfStates,a,stack1,stack2);
        returnVariable.insert(returnVariable.begin(),completeRun.begin(),completeRun.end());
        //cout <<"This is when the first hole created " << endl;
        //printRun(returnVariable,a);
        //this is when the program has push of stack 1
    }
    else if(parentOp==3 ){ // means push of stakc 2
        while(stack2.top().first != a.transition_list.size()+1){
            returnVariable.insert(returnVariable.begin(),stack2.top());
            stack2.pop();
        }
        stack2.pop(); // remove the barrier
        completeRun = Witness3(parentId,listOfStates,a,stack1,stack2);

        returnVariable.insert(returnVariable.begin(),completeRun.begin(),completeRun.end());
        //cout <<"This is when the second stack hole created "<<endl;
        //printRun(returnVariable,a);
        //this is when the state is generated by push of stack 2
    }
    else if(parentOp == 4 ){// we have to get the pop transition first.
        // this is when the state is generated by pop of stack 1 and doesnot close the hole
        run popTr ;
        for(ul transition_it =0;transition_it < a.transition_list.size();transition_it++){
            // checking for the possible stack pop transiton
            if(a.transition_list[transition_it].stackOp.op == 2 && a.transition_list[transition_it].firable(LastStateOfParent)){ // if the transition has a pop and is firable.
                ul destination_state = a.transition_list[transition_it].destination_state-1;
                vector<ul> value = a.transition_list[transition_it].afterReset(LastStateOfParent.second);
                states possibleNextState = make_pair(destination_state,value); // get the possible state.
                if(toString(possibleNextState) == toString(LastState)){ // check if this is the transition which is matched.s
                    stack_alph_pop = a.transition_list[transition_it].stackOp.stack_alphabet_pop; // get the pop alphabet.
                    pop_transition = transition_it; // also record the trnasition number as we need it.
                    stack_pop = a.transition_list[transition_it].stackOp.stack_pop;
                    popTr = make_pair(transition_it,true);
                }

            }
        }
        ul time = PleftOfMiddle1.time - leftOfHole1.time;
        vector<run> afterPushNComplete = computePushNComplete(endOfHole1,PendOfHole1,time,a,stack_pop,stack_alph_pop);
       //this vector run returns the possible vector which we will push to the stack.1
        for(auto it =afterPushNComplete.begin(); it != afterPushNComplete.end();it++){
            stack1.push(*it);
        }
        returnVariable = Witness3(parentId,listOfStates,a,stack1,stack2);
        returnVariable.push_back(popTr);
        //cout << "This is when a pop added without closing the hole" << endl;
        //printRun(returnVariable,a);

    }
    else if(parentOp == 5){
        // this is when the state is generated by pop of stack 1 and the hole is closed
        run popTr ;
        vector<run> afterPushNComplete;
        for(ul transition_it =0;transition_it < a.transition_list.size();transition_it++){
            // checking for the possible stack pop transiton
            if(a.transition_list[transition_it].stackOp.op == 2 && a.transition_list[transition_it].firable(LastStateOfParent)){ // if the transition has a pop and is firable.
                ul destination_state = a.transition_list[transition_it].destination_state-1;
                ul stackid = a.transition_list[transition_it].stackOp.stack_pop;
                vector<ul> value = a.transition_list[transition_it].afterReset(LastStateOfParent.second);
                states possibleNextState = make_pair(destination_state,value);
                
                if(toString(possibleNextState) == toString(LastState)){ // check if this is the transition which is matched.s
                    stack_alph_pop = a.transition_list[transition_it].stackOp.stack_alphabet_pop; // get the pop alphabet.
                    pop_transition = transition_it; // also record the trnasition number as we need it.
                    stack_pop = a.transition_list[transition_it].stackOp.stack_pop;
                    popTr = make_pair(transition_it,true);
                }
                afterPushNComplete = computePushNComplete(PleftOfMiddle1,PendOfHole1,PleftOfMiddle1.time,a,stack_pop,stack_alph_pop);
                if(!afterPushNComplete.empty()){
                    break;
                }
            }
        }

        
       //this vector run returns the possible vector which we will push to the stack.1
       //cout << "This is stack1 afterPushNcomplete" << endl;
       //printRun(afterPushNComplete,a);
        stack1.push(barrier);
        for(auto it =afterPushNComplete.begin(); it != afterPushNComplete.end();it++){
            stack1.push(*it);
        }
        returnVariable = Witness3(parentId,listOfStates,a,stack1,stack2);
        returnVariable.push_back(popTr);
        //cout << "This is when a pop of first stack added closing the hole " << endl;
        //printRun(returnVariable,a);
    }
    else if(parentOp == 6){
        // this is when the state is generated by pop of stack 2 and the hole is not closed
        run popTr ;
        for(ul transition_it =0;transition_it < a.transition_list.size();transition_it++){
            // checking for the possible stack pop transiton
            if(a.transition_list[transition_it].stackOp.op == 2 && a.transition_list[transition_it].firable(LastStateOfParent)){ // if the transition has a pop and is firable.
                ul destination_state = a.transition_list[transition_it].destination_state-1;
                vector<ul> value = a.transition_list[transition_it].afterReset(LastStateOfParent.second);
                states possibleNextState = make_pair(destination_state,value);
                if(toString(possibleNextState) == toString(LastState)){ // check if this is the transition which is matched.s
                    stack_alph_pop = a.transition_list[transition_it].stackOp.stack_alphabet_pop; // get the pop alphabet.
                    pop_transition = transition_it; // also record the trnasition number as we need it.
                    stack_pop = a.transition_list[transition_it].stackOp.stack_pop;
                    popTr = make_pair(transition_it,true);
                }

            }
        }
        ul time = PleftOfMiddle2.time - leftOfHole2.time;
        vector<run> afterPushNComplete = computePushNComplete(endOfHole2,PendOfHole2,time,a,stack_pop,stack_alph_pop);
       //this vector run returns the possible vector which we will push to the stack.1
        for(auto it =afterPushNComplete.begin(); it != afterPushNComplete.end();it++){
            stack2.push(*it);
        }
        returnVariable = Witness3(parentId,listOfStates,a,stack1,stack2);
        returnVariable.push_back(popTr);
        //cout <<"This is when a pop was added of stack 2 without closing the hole " << endl;
        //printRun(returnVariable,a);
    }
    else if(parentOp == 7){
        // this is when the state is generated by pop of stack 2 and the hole is closed.
        run popTr ;
        vector<run> afterPushNComplete;
        for(ul transition_it =0;transition_it < a.transition_list.size();transition_it++){
            // checking for the possible stack pop transiton
            if(a.transition_list[transition_it].stackOp.op == 2 && a.transition_list[transition_it].firable(LastStateOfParent)){ // if the transition has a pop and is firable.
                ul destination_state = a.transition_list[transition_it].destination_state-1;
                vector<ul> value = a.transition_list[transition_it].afterReset(LastStateOfParent.second);
                states possibleNextState = make_pair(destination_state,value);
                if(toString(possibleNextState) == toString(LastState)){ // check if this is the transition which is matched.s
                    stack_alph_pop = a.transition_list[transition_it].stackOp.stack_alphabet_pop; // get the pop alphabet.
                    pop_transition = transition_it; // also record the trnasition number as we need it.
                    stack_pop = a.transition_list[transition_it].stackOp.stack_pop;
                    popTr = make_pair(transition_it,true);
                }
                afterPushNComplete  = computePushNComplete(PleftOfMiddle2,PendOfHole2,PleftOfMiddle2.time,a,stack_pop,stack_alph_pop);
                if(!afterPushNComplete.empty()){
                    break;
                }
            }
        }
        
        //cout <<"This is stack2 after PushNComplete" << endl;
        //printRun(afterPushNComplete,a);
        stack2.push(barrier);
       //this vector run returns the possible vector which we will push to the stack.1
        for(auto it =afterPushNComplete.begin(); it != afterPushNComplete.end();it++){
            stack2.push(*it);
        }
        returnVariable = Witness3(parentId,listOfStates,a,stack1,stack2);
        returnVariable.push_back(popTr);
        //cout << "This is when a pop of stack 2 added and the hole is closed " << endl;
        //printRun(returnVariable,a);
    }


    return returnVariable;
}


vector<run> Concatenation::computePushNComplete(State source ,State destination,ul time, const Automata& a, int stack_pop, int stack_alph_pop){
    vector<run> vectorRun;
    states currMidHole = source.state;
    states PcurrMidHole = destination.state;
     //cout << "Checking inside the computePushNComplete function" << toString(currMidHole) << " " << toString(PcurrMidHole) << endl;
    ul expectedTime = time ;
    //cout << "Expected Time " << expectedTime << endl;
    for(ul transition_it = 0 ;transition_it < a.transition_list.size(); transition_it++){
        //for all transtiions check for the transitions which are push transitions and are of stack given here.
        Transition t = a.transition_list[transition_it];
        if(t.stackOp.op==1 && t.stackOp.stack_push == stack_pop && t.stackOp.stack_alphabet_push == stack_alph_pop && t.firable(currMidHole)){
            //now check if this transition is firable from the child state to the first state.
            ul destination = t.destination_state-1;
            vector<ul> valuation_v = t.afterReset(currMidHole.second);
            states possibleMid = make_pair(destination,valuation_v); // got the middle state.
            //check if you can reach from the middle state to the destination state using only complete contexts.
            set<ul> completeTime = C(state_map[toString(possibleMid)]-1,state_map[toString(PcurrMidHole)]-1); //got the possible time lapses
            //cout << "Checking inside the computePushNComplete function" << toString(possibleMid) << " " << toString(PcurrMidHole) << endl;
            if(!completeTime.empty()){ // if the time spend between them is not empty.
                if(completeTime.find(expectedTime) != completeTime.end()){
                    run possibleTransition = make_pair(transition_it,true);
                    vectorRun.push_back(possibleTransition);
                    vector<run> getComplete = Witness2(state_map[toString(possibleMid)]-1,expectedTime,state_map[toString(PcurrMidHole)]-1,a);
                    vectorRun.insert(vectorRun.end(),getComplete.begin(),getComplete.end());
                    
                    return vectorRun;
                }
            }

        }
    }
    return vectorRun;
}

// void Concatenation::parentPrint(ul index,const vector<MultiStack>& listOfStates, const Automata &a){
//     int parentOp;
//     stack<MultiStack> sequenceState;
// do{
//     MultiStack m = listOfStates[index];
//     sequenceState.push(m);
//     ul TotalTime = m.totalTimeLapsed;
//     parentOp = m.parentOp;
//     m.print();
//     cout << TotalTime << endl;
//     index = m.parentId;
// }while(parentOp!=-1);

// }
bool Concatenation::checkPossibleUseful(vector<ul> first, vector<ul> second){ 
    //we call this function when we are sure that no time lapse is happening between the transitions
    //other wise this function makes no sense
    //and also we checked equality between the vectors.
    if(first.size()!= second.size()){ // the size of the vectors must be same 
        cout <<" Error!! in valuation comparison" << endl;
        return false;
    }
    for(ul index = 0; index< first.size(); index++){ // for all indices of the vectors compare the clock value index wise.
        if(first[index] < second[index]){ 
            //if there is no time lapse in between then it can not be the case that value of one clock is smaller than other.
            return false;
        }
    }
    return true; // means that there is a possible useful transition here.
}



set<ul> Concatenation::getNextLeft(ul index, const Automata& a){ // this function returns all possible state reachable from the left most index.
    set<ul> returnSet;
    states newLeft = list_of_states[index];
     for(unsigned long increment = 1; increment <= a.max_const;increment++){ // loop till all clocks reach their maximum constant value.
        //cout << "bleh" << endl;
        vector<unsigned long> tools = newLeft.second; // create a vector which will store after incremented values.
        for(unsigned long i = 0; i<tools.size();i++){ // loop to increment the values of the vector
            tools[i] = min(tools[i]+increment,a.maximum_constant.at(i)); // normalize on the fly
        }
        ul newIndex = state_map[toString(make_pair(newLeft.first,tools))] -1;
        returnSet.insert(newIndex);
     }

     for(ul it_transition=0;it_transition<a.transition_list.size();++it_transition){
        if(0 == a.transition_list[it_transition].stackOp.op){ // we will do this only if there is not stack operation in the state.

            if(a.transition_list[it_transition].firable(newLeft)){//check if the transition is firable from the state. then we will also check for the
                // now we will check the rest options.
                vector<unsigned long> after_reset = a.transition_list[it_transition].afterReset(newLeft.second); // get the valuation of the clocks after the reset operation in the transition.
                //vector<int> after_normalize = normalization(after_reset,a); // we have to normalize the valuation.
                states destinationState = make_pair(a.transition_list[it_transition].destination_state-1,after_reset); 
                ul newIndex = state_map[toString(destinationState)] -1;
                if(newIndex != index){
                    returnSet.insert(newIndex);
                }
            }
        }
     }
     return returnSet;
                

}

void Concatenation::printPossibleUselessTransitions( bnu::matrix<vector<unsigned long> > useless, const Automata& a){
    for(ul i =0; i <a.number_of_states; i++){
        for(ul j = 0; j < a.number_of_states;j++){
            cout << "{";
            for (auto m = useless(i,j).begin(); m!= useless(i,j).end();m++){
                cout << a.transition_list[*m].name << "," ;
            }
            cout << "}\t";
        }
        cout << endl;
    }
}