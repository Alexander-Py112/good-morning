/****************************************************************************
  FileName     [ p2Json.cpp ]
  PackageName  [ p2 ]
  Synopsis     [ Define member functions of class Json and JsonElem ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2018-present DVLab, GIEE, NTU, Taiwan ]
****************************************************************************/
#include <iostream>
#include <string>
#include "p2Json.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iomanip>
using namespace std;

bool
Json::printFirst()
{
    cout << "THe first JsonElem: " << _obj[0] << endl;
    return true;
}


// Implement member functions of class Row and Table here
bool
Json::read(const string& jsonFile)
{
    ifstream mmm (jsonFile , ifstream::binary);
    string line;
    
    while (getline(mmm,line)){
        line = line + '\n';
        int count1 = 0 ;
        int count2 = 0 ;
        int quote_start_index = 0;
        int quote_end_index = 0;
        int quote_length = 0;
        int raw_value_start_index = 0;
        int raw_value_end_index = 0;
        int raw_value_length = 0;
        int value_start_index = 0;
        int value_end_index = 0;
        int value_length = 0;
        /* skip first and last open or close braket */  
        if ((string(line).find("{") != string::npos) || 
            (string(line).find("}") != string::npos) || 
            line == "\n"){
            continue;
        }
        /* cut down key */
        for (int i = 0;(unsigned) i < line.length(); i++){ 
            char c = line[i];
        /* label first " index */
            if (c == '\"' && count1 == 0){   
                quote_start_index = i; 
                count1++; 
            }
        /* label second " index */
            if (c == '\"' && count1 != 0){
                quote_end_index = i; 
            }
        }
        quote_length = quote_end_index - quote_start_index - 1;
        /* cout << ">>> quote length = " << quote_length << endl; */
        /* pick up key from json */
        string key = line.substr(quote_start_index + 1, quote_length);
        
        /* cut down raw_value */ 
        raw_value_start_index = quote_end_index + 1;
        raw_value_end_index = line.length() - 1;
        raw_value_length = raw_value_end_index - raw_value_start_index + 1;
        string raw_value = line.substr(raw_value_start_index, raw_value_length);
        /* clear raw_value */
        for (int i = 0 ;(unsigned) i < raw_value.length(); i++ ){
            if( count2 == 0 && (string(" :,\n\t").find(raw_value[i]) == string::npos))    
            {
               value_start_index = i;
               count2++;
            }
            if(count2 == 1 && string(":, \n \t").find(raw_value[i]) != string::npos) 
            {
               value_end_index= i-1;
               count2++;
            }          
        }
        value_length = value_end_index - value_start_index + 1; 
        string value = raw_value.substr(value_start_index,value_length);
        /* cout << ">>> key: " << key << ", and value: " << value << endl; */
        /* TODO : Insert the key and value into JsonEle obj (_obj)*/
        _obj.push_back(JsonElem(key, stoi(value)));
    }
    return true;
}

bool
Json::executeCommand(const string& cmd)
{
    /* TODO */

        bool is_execution_success = true;
        /* cout << "Executing the command:" << cmd << endl; */

        if(cmd == "FIRST"){
            is_execution_success = printFirst();
        }
        if(cmd == "PRINT"){
            print();
            /* TODO */
        }
        string str ("ADD");

        if(cmd.find(str) == 0 ){
            is_execution_success = ADD(cmd);
        }
        if(cmd == "MIN"){

            is_execution_success = MIN();
        }
        if(cmd == "MAX"){

            is_execution_success = MAX();
        }
        if(cmd == "SUM"){

            is_execution_success = SUM();
        }
        if(cmd == "AVE"){
            is_execution_success = AVE();
        }
        if(cmd == "EXIT"){
            return exit();
        }
        // If the exec. is not success, print error.
        if(!is_execution_success){
            if(cmd.find("ADD")){
                cout << "ERROR";
            }
            else if(cmd.find("SUM")){
                cout << "Error: No element found!! "<< endl;
            }
        }
        return true;
}

bool
Json::print(){   
    if(_obj.size() == 0){
        cout <<"Error: No element found!! " << endl;
    }
    else{
        cout<< "{\n";
        for( vector<JsonElem>::iterator it = _obj.begin(); it !=_obj.end(); ++it ){
           if( next(it) != _obj.end()){
              cout<< "  " << *it <<",\n" ;
           }
           else{
              cout<< "  " << *it <<"\n" ;
           }
        }

        cout<< "}\n";
    } 
    return true;     
}

bool
Json::ADD(const string& cmd){
    cout << cmd << endl;
    string command;
    string key;
    int value;
    istringstream iss(cmd);
    iss >> command >> key >>value;
    /* cout<< "command>>"<< command<<"key>>"<<key<<"value"<<value << endl; */
    _obj.push_back(JsonElem(key,value));

    return true;
}

bool
Json::SUM(){
    if(_obj.size() == 0){
        cout <<"Error: No element found!! " << endl;
    }
    else{
        float  summation_value = 0;
        for( vector<JsonElem>::iterator it = _obj.begin(); it !=_obj.end(); ++it ){
            summation_value += ((*it).getValue());
        }
        cout << summation_value << endl;   
    }
    return true;
}

bool
Json::AVE(){
    if(_obj.size() == 0){
        cout <<"Error: No element found!! " << endl;
    }
    else{
        float summation_value = 0;
        float N = 0;
        for( vector<JsonElem>::iterator it = _obj.begin(); it !=_obj.end(); ++it ){
            summation_value += ((*it).getValue());
            N++;
        }
        cout << fixed << setprecision(1) <<( summation_value / N ) << endl;   
    }
    return true;
}

bool 
Json::MAX(){
    if(_obj.size() == 0){
        cout <<"Error: No element found!! " << endl ;
    }
    else{
/*TO Do : bulid new  vector to store all value , and then cout  max and min function */  
        vector<int> value_vector;
        vector<int>::iterator pos;
        int i = 0;
        for( vector<JsonElem>::iterator it = _obj.begin(); it !=_obj.end(); ++it ){
            value_vector.push_back( (*it).getValue() );
        }
        pos = max_element(value_vector.begin(), value_vector.end());
        /* cout << "The maximum element is" << *pos << endl; */   
        /* cout << "max value is at:" << distance(value_vector.begin(), pos) << endl; */
        i = distance(value_vector.begin(), pos);
        cout << "The maximum element is: { " << _obj[i] <<" }." << endl;
    }
    return true;
}

bool 
Json::MIN(){
    if(_obj.size() == 0){
        cout <<"Error: No element found!! " << endl;
    }
    else{
/*TO Do : bulid new  vector to store all value , and then cout  max and min function */  
        vector<int> value_vector;
        vector<int>::iterator pos;
        int i = 0;
        for( vector<JsonElem>::iterator it = _obj.begin(); it !=_obj.end(); ++it ){
            value_vector.push_back( (*it).getValue() );
        }
        pos = min_element(value_vector.begin(), value_vector.end());
        /* cout << "The maximum element is" << *pos << endl; */   
        /* cout << "max value is at:" << distance(value_vector.begin(), pos) << endl; */
        i = distance(value_vector.begin(), pos);
        cout << "The minimum element is: { " << _obj[i] <<" }." << endl;
    }
    return true;
}

bool 
Json::exit(){
    return false;

}

ostream&
operator << (ostream& os, const JsonElem& j)
{
   return (os << "\"" << j._key << "\" : " << j._value);
}

