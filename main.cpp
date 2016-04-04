//
//  main.cpp
//  project2
//
//  Created by Enoch Ko on 3/14/16.
//  Copyright © 2016 Enoch Ko. All rights reserved.
//
#define SYMBOL_LENGTH   20      // Maximum length of a variable name (identifier)
#define INT             0       // Integer type
#define CHAR            1       // Character string type
#define BST             2       // Binary search tree type
#define STRTERM         '\0'    // String termination character
#define toDigit(c) (c-'0')
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include <cstring>
#include <sstream>
#include <math.h>

using namespace std;
struct heapEntry{
    int blockSize;  // Size of free block in bytes
    int offset;     // Start address of free block given as offset into memory block
};

struct symbolTableEntry{
    char symbol[ SYMBOL_LENGTH ]; // Variable identifier
    int type;       // The type of the variable: one of INT, CHAR, or BST
    int offset;     // Start address of variable in physical memory given as offset
    int noBytes;    // Size in bytes used to store variable in physical memory
};

struct bstNode{
    int key; // A node of a binary search tree ordered on this key value
    int left;  // Left child
    int right; // Right child
};

void max_heapify(heapEntry *array, int i, int n){
    
    int child, temp;
    temp = array[i].blockSize;
    child = 2 * i;
    while (child < n){
        if (child < n && array[child+1].blockSize > array[child].blockSize)
            child = child + 1;
        if (temp > array[child].blockSize)
            break;
        else if (temp <= array[child].blockSize){
            array[child / 2].blockSize = array[child].blockSize;
            child = 2 * child;
        }
    }
    array[child/2].blockSize = temp;
    return;
}
void build_maxheap(heapEntry *array,int arraylength){
    if (arraylength > 1){
        int i;
        for(i = arraylength/2; i >= 0; i--){
            max_heapify(array,i,arraylength);
        }
    }else{
        return;
    }
    
    
}
heapEntry heap_extract_max(heapEntry *array , int size ){
    heapEntry heapmax = array[0];
    array[0].blockSize = array[size-1].blockSize;
    array[0].offset = array[size-1].offset;
    return heapmax;
    
}



int hash_insert(symbolTableEntry table[], string key, int hash_table_size, string variable_type, int offset, int block_size){
    int ascii_total_value = 0;
    for (int i = 0; i< key.length(); i++) {
        ascii_total_value += (int)key[i];
        
        
    }
    int index_to_put_variable = -1;
    int probe = 0;
    for (probe = 0; probe != hash_table_size;probe++) {
        index_to_put_variable = ascii_total_value % hash_table_size + probe;
        if (table[index_to_put_variable].offset == -1) {
            for (int j = 0; j< SYMBOL_LENGTH; j++) {
                table[index_to_put_variable].symbol[j] = key[j];
            }
            if (variable_type == "INT"){
                table[index_to_put_variable].type = INT;
            }else if (variable_type == "CHAR"){
                table[index_to_put_variable].type = CHAR;
            }else if (variable_type == "BST"){
                table[index_to_put_variable].type = BST;
            }
            
            table[index_to_put_variable].offset = offset;
            table[index_to_put_variable].noBytes = block_size;
            return index_to_put_variable;
            
        }
    }
    
    
    
    
    return index_to_put_variable;
    
}




int hash_search(symbolTableEntry table[], string key, int hash_table_size){
    int ascii_total_value = 0;
    for (int i = 0; i< key.length(); i++) {
        ascii_total_value += (int)key[i];
        
        
    }
    int index_to_put_variable = -1;
    int found_index = -1;
    int probe = 0;
    for (probe = 0; probe != hash_table_size;probe++) {
        
        index_to_put_variable = ascii_total_value % hash_table_size + probe;
        if (table[index_to_put_variable].symbol == key) {
            found_index = index_to_put_variable;
            
        }
    }
    
    return found_index;
    
    
    
}




bool is_integer(const std::string& s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

//HEAP INSERT
int myMalloc(unsigned char table[], string variable_value, string variable_type, int offset, int block_size){
    if (variable_type == "INT"){
        int value = atoi(variable_value.c_str());
        cout<<"value:"<<value<<endl;
        
        
        if (value/4294967296 >= 1){
            
        }
        if (value/16777216 >= 1){
            table[offset + 3] = value/16777216;
            value -= (value/16777216) * 16777216;
        }
        if (value/65536 >= 1){
            table[offset + 2] = value/65536;
            value -= (value/65536) * 65536;
            
        }
        if (value/256 >= 1){
            table[offset + 1] = value/256;
            value -= (value/256) * 256;
        }
        
        table[offset] = value;
    }else if(variable_type == "CHAR"){
        variable_value = variable_value.substr(1,variable_value.length()-2);
        for(int l = 0;l<variable_value.length(); l++){
            table[offset+l] = variable_value[l];
        }
        table[offset+variable_value.length()] = 255;
    }else if(variable_type == "BST"){
        
        
    }
    return 0;
    
    
}

int add_update(unsigned char table[], string variable_value, string variable_value2, string variable_type, int offset, int block_size){
    if (variable_type == "INT"){
        int value = atoi(variable_value.c_str());
        int value2 = atoi(variable_value2.c_str());
        value += value2;
        
        if (value/4294967296 >= 1){
            
        }
        if (value/16777216 >= 1){
            table[offset + 3] = value/16777216;
            value -= (value/16777216) * 16777216;
        }
        if (value/65536 >= 1){
            table[offset + 2] = value/65536;
            value -= (value/65536) * 65536;
            
        }
        if (value/256 >= 1){
            table[offset + 1] = value/256;
            value -= (value/256) * 256;
        }
        
        table[offset] = value;
    }else if(variable_type == "CHAR"){
        variable_value = variable_value.substr(1,variable_value.length()-2);
        for(int l = 0;l<variable_value.length(); l++){
            table[offset+l] = variable_value[l];
        }
        table[offset+variable_value.length()] = 255;
    }else if(variable_type == "BST"){
        
        
    }
    return 0;
    
    
}




int myFree(){
    return 1;
}




int main(){
    string readLine;
    //passing through attributes of the txt file
    cin>>readLine;
    int k = atoi(readLine.c_str());
    cin>>readLine;
    int t = atoi(readLine.c_str());
    cin>>readLine;
    int c = atoi(readLine.c_str());
    
    //creating Free space implementation using variable k.
    int size_of_physical_block = 1;
    for(int counter = 0;counter < k;counter++){
        size_of_physical_block = size_of_physical_block *2;
    }
    
    unsigned char *blocks = new unsigned char [size_of_physical_block];   //allocating n bytes.
    
    
    //initial block starts at offset 0, and size of whole physical block.
    heapEntry *freeblocks = new heapEntry[1];
    freeblocks[0].blockSize = size_of_physical_block;
    freeblocks[0].offset = 0;
    
    
    
    
    for (int i = 0; i<size_of_physical_block; i++) {
        blocks[i] = 0;
    }

    
    //creating struct symbolTableEntry hash table using variable t
    symbolTableEntry *hash_table = new symbolTableEntry[t];
    //setting the offset equal to zero, for the purpose of matching later on by the offset.
    for(int i = 0; i<t;i ++){
        hash_table[i].offset = -1;
    }
    
    
    
    
    
    
    
    
    
    
    
    
    string command_type;
    string variable_type;
    string variable_name;
    string variable_length;
    string variable_value_string;
    string variable_name2;
    int blocklength = 1;
    cout<<"COMMAND:"<<c<<endl;
    for (int i = 0; i< c; i++){
        cin>>readLine;
        
        command_type = readLine;
        
        if (command_type == "allocate"){
            cout<<"ALLOCATE"<<endl;
            cin>>readLine;
            variable_type = readLine;
            if(variable_type == "INT"){
                cin>>readLine;
                variable_name = readLine;
                
                cin>>variable_value_string;
                build_maxheap(freeblocks, blocklength);
                heap_extract_max(freeblocks, blocklength);
                if (freeblocks[0].blockSize <= 0){
                    cout<<"Insufficient memory to allocate variable"<<endl;
                }else{
                    //since its an integer, reduce by 4
                    hash_insert(hash_table, variable_name,t,variable_type,freeblocks[0].offset, 4);
                    myMalloc(blocks, variable_value_string, variable_type, freeblocks[0].offset, 4);
                    freeblocks[0].blockSize = freeblocks[0].blockSize - 4;
                    freeblocks[0].offset = freeblocks[0].offset + 4;

                    cout<<"Max Freeblock:"<<freeblocks[0].blockSize<<endl;
                    cout<<"Position Freeblock:"<<freeblocks[0].offset<<endl;
                    for (int l = 0 ;l< t;l++){
                        cout<< hash_table[l].symbol<<" "<< hash_table[l].offset<<" "<<hash_table[l].noBytes <<endl;
                    }
                }
            
                
                
                
                
            }else if(variable_type == "CHAR"){
                cin>>variable_name;
                cin>>variable_length;
                cin>>variable_value_string;
                build_maxheap(freeblocks, blocklength);
                heap_extract_max(freeblocks, blocklength);
                if (freeblocks[0].blockSize <= 0){
                    cout<<"Insufficient memory to allocate variable"<<endl;
                }else{
                    int variable_length_int = atoi(variable_length.c_str());
                    int dividend = variable_length_int/4;
                    if (variable_length_int%4){
                        dividend ++;
                    }
                    //dividend *4 is ceiled.
                    hash_insert(hash_table, variable_name,t,variable_type,freeblocks[0].offset, dividend*4);
                    myMalloc(blocks, variable_value_string, variable_type, freeblocks[0].offset, dividend*4);
                    freeblocks[0].blockSize = freeblocks[0].blockSize - dividend*4;
                    freeblocks[0].offset = freeblocks[0].offset + dividend*4;
                    
                    cout<<"Max Freeblock:"<<freeblocks[0].blockSize<<endl;
                    cout<<"Position Freeblock:"<<freeblocks[0].offset<<endl;
                    for (int l = 0 ;l< t;l++){
                        cout<< hash_table[l].symbol<<" "<< hash_table[l].offset<<" "<<hash_table[l].noBytes <<endl;
                    }
                    
                }
                
                
                
                
                
            }else{
                cout<<"command type error"<<endl;
            }
            
            
        }
        
        if(command_type == "map"){
            cout<<"MAP"<<endl;
            int num_rows = size_of_physical_block/32;
            for (int j = 0; j<num_rows; j++) {
                int k = 0;
                
                while( k <32){
                    int skip = 0;
                    int type = 0;
                    int l = -1;
                    
                    for (l = 0; l<t; l++){
                        if (hash_table[l].offset == k ){
                            skip = hash_table[l].noBytes;
                            type = hash_table[l].type;
                        }
                    }
                    if (skip == 0){
                        k++;
                        cout<<"00 ";
                    }
                    
                    for(int m = 0; m<skip; m++){
                        
                        if (type == INT){
                            if (blocks[k] != '\0' && blocks[k] != 0){
                                printf("%02x ", (unsigned int)(blocks[k] & 0xFF));
                            }
                            if (blocks[k] == 0){
                                cout<<"00 ";
                            }
                            
                        }else if (type == CHAR){
                            if (blocks[k] != 255 && blocks[k] != 0){
                                printf("%c ", (blocks[k] & 0xFF));
                            }
                            
                            if (blocks[k] == 255){
                                cout<<"\\0 ";
                            }
                            
                            if (blocks[k] == 0){
                                cout<<"00 ";
                            }
                        }
                        k++;
                        
                    }
                    
                }
                cout<<endl;
            }
            
        }
        if(command_type == "print"){
            cin>>variable_name;
            int heap_index = hash_search(hash_table, variable_name, t);
            int heap_offset = hash_table[heap_index].offset;
            int heap_type = hash_table[heap_index].type;
            int heap_noBytes = hash_table[heap_index].noBytes;
            cout<<"PRINT:";
            if (heap_type == INT){
                int totalvalue = 0;
                if (heap_type == 0){
                    
                    
                    
                    if(blocks[heap_offset+3]){
                        
                        totalvalue = totalvalue+ 16777216*blocks[heap_offset+3];
                    }
                    if(blocks[heap_offset+2]){
                        totalvalue = totalvalue+ 65536*blocks[heap_offset+2];
                    }
                    if(blocks[heap_offset+1]){
                        totalvalue = totalvalue+ 256*blocks[heap_offset+1];
                    }
                    if(blocks[heap_offset]){
                        
                        totalvalue = totalvalue+ blocks[heap_offset+0];
                    }
                    
                    
                }
                
                cout<<totalvalue;
            }
            if (heap_type == CHAR){
                //cout<<"offset:"<<heap_offset<<endl;
                //cout<<"noBytes:"<<heap_noBytes<<endl;
                for(int k = heap_offset; k < heap_noBytes + heap_offset; k++){
                    if (blocks[k] != 255 && blocks[k] != 0){
                        printf("%c ", (blocks[k] & 0xFF));
                    }
                }
                
            }
            
            cout<<endl;
            
            
            
        }
        if(command_type == "add"){
            cin>>variable_name;
            cin>>readLine;
            int heap_index = hash_search(hash_table, variable_name, t);
            int heap_offset = hash_table[heap_index].offset;
            int heap_type = hash_table[heap_index].type;
            if (heap_type == INT){
                int heap_noBytes = hash_table[heap_index].noBytes;
                if(is_integer(readLine)){
                    int totalvalue = 0;
                    if(blocks[heap_offset+3]){
                        totalvalue = totalvalue+ 16777216*blocks[heap_offset+3];
                    }
                    if(blocks[heap_offset+2]){
                        totalvalue = totalvalue+ 65536*blocks[heap_offset+2];
                    }
                    if(blocks[heap_offset+1]){
                        totalvalue = totalvalue+ 256*blocks[heap_offset+1];
                    }
                    if(blocks[heap_offset]){
                        totalvalue = totalvalue+ blocks[heap_offset+0];
                    }
                    //convert int totalvalue to string, so it can be used for add_update
                    stringstream ss;
                    ss << totalvalue;
                    string totalvalue_string = ss.str();
                    //add_update does all the work
                    add_update(blocks, totalvalue_string, readLine,"INT", heap_offset, heap_noBytes);
                    
                    
                }else{
                    int heap_index1 = hash_search(hash_table, readLine, t);
                    int heap_offset1 = hash_table[heap_index1].offset;
                    int heap_type1 = hash_table[heap_index1].type;
                    int totalvalue1 = 0;
                    if(blocks[heap_offset1+3]){
                        totalvalue1 = totalvalue1+ 16777216*blocks[heap_offset1+3];
                    }
                    if(blocks[heap_offset1+2]){
                        totalvalue1 = totalvalue1+ 65536*blocks[heap_offset1+2];
                    }
                    if(blocks[heap_offset1+1]){
                        totalvalue1 = totalvalue1+ 256*blocks[heap_offset1+1];
                    }
                    if(blocks[heap_offset1]){
                        totalvalue1 = totalvalue1+ blocks[heap_offset1+0];
                    }
                    //convert int totalvalue to string, so it can be used for add_update
                    stringstream ss;
                    ss << totalvalue1;
                    string totalvalue_string1 = ss.str();
                    //add_update does all the work
                    
                    int totalvalue = 0;
                    if(blocks[heap_offset+3]){
                        totalvalue = totalvalue+ 16777216*blocks[heap_offset+3];
                    }
                    if(blocks[heap_offset+2]){
                        totalvalue = totalvalue+ 65536*blocks[heap_offset+2];
                    }
                    if(blocks[heap_offset+1]){
                        totalvalue = totalvalue+ 256*blocks[heap_offset+1];
                    }
                    if(blocks[heap_offset]){
                        totalvalue = totalvalue+ blocks[heap_offset+0];
                    }
                    //convert int totalvalue to string, so it can be used for add_update
                    stringstream ss2;
                    ss2 << totalvalue;
                    string totalvalue_string = ss2.str();
                    add_update(blocks, totalvalue_string, totalvalue_string1,"INT", heap_offset, heap_noBytes);
                    
                    
                    
                    
                }
            }else{
                cout<<"Error: LHS of add must be type INT."<<endl;
            }
            
            
        }
        if(command_type == "free"){
            cin>>variable_name;
            cout<<"FREE!"<<endl;
            /*
            //increasing the size of dynamic array by creating a new one and replacing the old one.
            
            heapEntry *increasedfreeblocks = new heapEntry[blocklength+1];
            for (int l = 0;l< blocklength ;l++){
                increasedfreeblocks[l] = freeblocks[l];
            }
            //delete[] freeblocks;
            blocklength++;
            heapEntry *freeblocks = new heapEntry[blocklength];
            for (int l = 0;l< blocklength ;l++){
                freeblocks[l] = increasedfreeblocks[l];
            }
            cout<<"elements:"<<blocklength<<endl;
            delete[] increasedfreeblocks;
             */
        }
        if(command_type == "strcat"){
            cin>>variable_name;
            cin>>variable_name2;
            
            if (variable_name2[0] == '"'){
                //straight string
                
            }else{
                //string variable
                
            }
            cout<<"STRCAT!"<<endl;
        }
        if(command_type == "compact"){
            cout<<"COMPACT!"<<endl;
        }
    }
   
    
    
    
    
    delete[] blocks;
    return size_of_physical_block;
}
