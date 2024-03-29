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
    child = 2 * i ;
    cout<<"child:"<<child<<endl;
    while (child < n){
        if (child < n && array[child+1].blockSize > array[child].blockSize)
            //check right side
            child = child + 1;
        //if the parent is bigger than child.
        if (temp > array[child].blockSize)
            break;
        //if child is bigger, we need to exchange.
        else if (temp <= array[child].blockSize){
            //exchange process.
            array[child / 2].blockSize = array[child].blockSize;
            child = 2 * child ;
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

int convertToInt(char* blocks, int heap_offset){
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
    return totalvalue;
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
        if (table[index_to_put_variable].type == -1) {
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
    
    
    
    cout<<"Error: Symbol Table is full."<<endl;
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
        //ascii_total_value % hash_table_size + probe
        index_to_put_variable = probe;
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
        if (block_size > variable_value.length()){
            for(int l = 0;l<variable_value.length(); l++){
                table[offset+l] = variable_value[l];
            }
            table[offset+variable_value.length()] = 255;
        }else{
            cout<<"Error: Insufficient memory to allocate variable"<<endl;
        }
        
    }else if(variable_type == "BST"){
        int value = atoi(variable_value.c_str());
        
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
        for(int l = 4; l<12; l++){
            table[offset + l] = 255;
        }
        
        
        
        
        
        
        
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




int string_update(unsigned char table[], string variable_value, string variable_value2, string variable_type, int offset, int block_size){
    if(variable_type == "CHAR"){
        
        variable_value += variable_value2;
        if (block_size > variable_value.length()){
            for(int l = 0;l<variable_value.length(); l++){
                table[offset+l] = variable_value[l];
            }
            table[offset+variable_value.length()] = 255;

        }else{
            cout<<"Error: LHS insufficient length to perform strcat"<<endl;
        }
            }else if(variable_type == "BST"){
        
        
    }
    return 0;
    
    
}




/*
bstNode BSTInsert(char * blocks, int root, int addedvalueindex, string key){
    
    
    char* newvalue = new char[12];
    char* parent = new char[12];
    char* current = new char[12];
    
    bstNode node = new bstNode[data];
    
    for(int l = 0; l<12; l++){
        current[l] = blocks[root + l];
    }
    while(current != null){
        for(int l = 0; l<12;l++){
            parent[l] = current[l];
        }
        convertToInt(<#char *blocks#>, <#int heap_offset#>)
        if (current.key <= addedvalueindex){
            current = current.right;
            
        }else{
            current = current.left;
            
        }
        
    }
    if (parent.key <= key){
        parent.right = node;
        
    }else{
        parent.left = node;
    }
    return root;
    
    
}
 */




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
    heapEntry *freeblocks = new heapEntry[size_of_physical_block/4];

    for (int i = 0; i<size_of_physical_block; i++) {
        blocks[i] = 0;
    }
    for (int i = 0; i <size_of_physical_block/4; i++){
        freeblocks[i].blockSize = 0;
        freeblocks[i].offset = 0;
    }
    freeblocks[0].blockSize = size_of_physical_block;
    freeblocks[0].offset = 0;

    
    //creating struct symbolTableEntry hash table using variable t
    symbolTableEntry *hash_table = new symbolTableEntry[t];
    //setting the offset equal to zero, for the purpose of matching later on by the offset.
    for(int i = 0; i<t;i ++){
        hash_table[i].offset = -1;
        hash_table[i].type = -1;
        hash_table[i].noBytes = 0;
    }
    
    
    
    
    
    
    
    
    
    
    
    
    string command_type;
    string variable_type;
    string variable_name;
    string variable_length;
    string variable_value_string;
    string variable_name2;
    int blocklength = 1;
    //cout<<"COMMAND:"<<c<<endl;
    for (int i = 0; i< c; i++){
        cin>>readLine;
        
        command_type = readLine;
        
        if (command_type == "allocate"){
            
            //cout<<"ALLOCATE"<<endl;
            cin>>readLine;
            variable_type = readLine;
            if(variable_type == "INT"){
                cin>>readLine;
                variable_name = readLine;
                
                cin>>variable_value_string;
                build_maxheap(freeblocks, blocklength);
                heap_extract_max(freeblocks, blocklength);
                if (freeblocks[0].blockSize <= 0){
                    cout<<"Error: Insufficient memory to allocate variable"<<endl;
                }else{
                    
                    //since its an integer, reduce by 4
                    hash_insert(hash_table, variable_name,t,variable_type,freeblocks[0].offset, 4);

                    myMalloc(blocks, variable_value_string, variable_type, freeblocks[0].offset, 4);
                    freeblocks[0].blockSize = freeblocks[0].blockSize - 4;
                    freeblocks[0].offset = freeblocks[0].offset + 4;
                    
                    //cout<<"Max Freeblock:"<<freeblocks[0].blockSize<<endl;
                    //cout<<"Position Freeblock:"<<freeblocks[0].offset<<endl;
                    // show hash table
                    /*for (int l = 0 ;l< t;l++){
                        cout<< hash_table[l].symbol<<" "<< hash_table[l].offset<<" "<<hash_table[l].noBytes <<endl;
                    }
                    cout<<endl;
                     */
                    
                }
            
                
                
                
                
            }else if(variable_type == "CHAR"){
                cin>>variable_name;
                cin>>variable_length;
                cin>>variable_value_string;
                build_maxheap(freeblocks, blocklength);
                heap_extract_max(freeblocks, blocklength);
                if (freeblocks[0].blockSize <= 0){
                    cout<<"Error: Insufficient memory to allocate variable"<<endl;
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
                    
                    //cout<<"Max Freeblock:"<<freeblocks[0].blockSize<<endl;
                    //cout<<"Position Freeblock:"<<freeblocks[0].offset<<endl;
                    /*
                    for (int l = 0 ;l< t;l++){
                        cout<< hash_table[l].symbol<<" "<< hash_table[l].offset<<" "<<hash_table[l].noBytes <<endl;
                    }
                     */
                    
                }
                
                
                
                
                
            }else if(variable_type == "BST"){
                cin>>readLine;
                variable_name = readLine;
                
                cin>>variable_value_string;
                build_maxheap(freeblocks, blocklength);
                heap_extract_max(freeblocks, blocklength);
                if (freeblocks[0].blockSize <= 0){
                    cout<<"Error: Insufficient memory to allocate variable"<<endl;
                }else{
                    
                    //since its an integer, reduce by 4
                    hash_insert(hash_table, variable_name,t,variable_type,freeblocks[0].offset, 12);
                    
                    myMalloc(blocks, variable_value_string, variable_type, freeblocks[0].offset, 12);
                    freeblocks[0].blockSize = freeblocks[0].blockSize - 12;
                    freeblocks[0].offset = freeblocks[0].offset + 12;
                }
                
            }else{
                cout<<"command type error"<<endl;
            }
            
            
        }
        
        if(command_type == "map"){
            cout<<"MAP"<<endl;
        
            int k = 0;
            
            while( k < size_of_physical_block){
                
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
                    
                    if (type == INT || type == BST ){
                        if (blocks[k] != '\0' && blocks[k] != 0){
                            printf("%02x ", (unsigned int)(blocks[k] & 0xFF));
                        }
                        if (blocks[k] == 0){
                            cout<<"00 ";
                        }
                        
                    }else if (type == CHAR){
                        if (blocks[k] != 255 && blocks[k] != 0){
                            printf(" %c ", (blocks[k] & 0xFF));
                        }
                        
                        if (blocks[k] == 255){
                            cout<<"\\0 ";
                        }
                        
                        if (blocks[k] == 0){
                            cout<<"00 ";
                        }
                        
                    }else if (type == -1){
                        cout<<" @ ";
                    }
                    k++;
                    
                }
                if (k%32 == 0){
                    cout<<endl;
                }
            }
            
        }
        if(command_type == "print"){
            cin>>variable_name;
            int heap_index = hash_search(hash_table, variable_name, t);
            if (heap_index != -1){
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
            }else{
                cout << "Error: \'"+ variable_name + "\' was freed"<<endl;
            }
        }
        if(command_type == "add"){
            cin>>variable_name;
            cin>>readLine;
            int heap_index = hash_search(hash_table, variable_name, t);
            if (heap_index != -1){
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
            }else{
                cout<<"Error: symbol table lookup failed."<<endl;
            }
            
            
            
        }
        if(command_type == "free"){
            cin>>variable_name;
            cout<<"FREE!"<<endl;

            int heap_index = hash_search(hash_table, variable_name, t);
            if (heap_index != -1){
                int heap_offset = hash_table[heap_index].offset;
                int heap_type = hash_table[heap_index].type;
                int heap_noBytes = hash_table[heap_index].noBytes;
                //hash_table[heap_index].offset = -1;
                hash_table[heap_index].type = -1;
                //hash_table[heap_index].noBytes = 0;
                for(int l = 0; l< SYMBOL_LENGTH; l++){
                    hash_table[heap_index].symbol[l] = 0;
                }
                
                for (int l = heap_offset; l <heap_offset+ heap_noBytes; l++){
                    blocks[l] = 0;
                }
                
                
                freeblocks[blocklength].offset = heap_offset;
                freeblocks[blocklength].blockSize = heap_noBytes;
                blocklength++;
                
                cout<<"elements:"<<blocklength<<endl;
            }else{
                cout<<"Error: symbol table lookup failed."<<endl;
            }
            

        }
        if(command_type == "strcat"){
            cin>>variable_name;
            cin>>variable_name2;
            int heap_index = hash_search(hash_table, variable_name, t);
            if(heap_index != -1){
                int heap_offset = hash_table[heap_index].offset;
                int heap_type = hash_table[heap_index].type;
                int heap_noBytes = hash_table[heap_index].noBytes;
                string totalvalue_string = "";
                string totalvalue_string1 = "";
                if(heap_type == CHAR){
                    if (variable_name2[0] == '"'){
                        //straight string
                        
                        for(int l = heap_offset; l<heap_offset+heap_noBytes; l++){
                            if(blocks[l] != 255 && blocks[l] != 0){
                                totalvalue_string += (blocks[l] & 0xFF);
                            }
                            
                        }
                        totalvalue_string1 = variable_name2.substr(1,variable_name2.length()-2);
                        string_update(blocks, totalvalue_string, totalvalue_string1,"CHAR", heap_offset, heap_noBytes);
                        
                        
                    }else{
                        string totalvalue_string = "";
                        string totalvalue_string1 = "";
                        for(int l = heap_offset; l<heap_offset+heap_noBytes; l++){
                            if(blocks[l] != 255 && blocks[l] != 0){
                                totalvalue_string += (blocks[l] & 0xFF);
                            }
                            
                        }
                        
                        
                        
                        
                        //string variable
                        int heap_index2 = hash_search(hash_table, variable_name2, t);
                        int heap_offset2 = hash_table[heap_index2].offset;
                        int heap_type2 = hash_table[heap_index2].type;
                        int heap_noBytes2 = hash_table[heap_index2].noBytes;
                        if(heap_type2 == CHAR){
                            for(int l = heap_offset2; l<heap_offset2+heap_noBytes2; l++){
                                if(blocks[l] != 255 && blocks[l] != 0){
                                    totalvalue_string1 += (blocks[l] & 0xFF);
                                }
                            }
                            string_update(blocks, totalvalue_string, totalvalue_string1,"CHAR", heap_offset, heap_noBytes);
                        }else{
                            cout<<"Error: RHS of strcat must be type CHAR"<<endl;
                        }
                        
                        
                        
                        
                    }
                }else{
                    cout<<"Error: LHS of strcat must be type CHAR."<<endl;
                }
            }else{
                cout<<"Error: symbol table lookup failed."<<endl;
            }
        }
        if(command_type == "insert"){
            string root;
            /*
            cin>>root;
            cin>>readLine;
            string value = readLine;
            
            
            build_maxheap(freeblocks, blocklength);
            heap_extract_max(freeblocks, blocklength);
            if (freeblocks[0].blockSize <= 0){
                cout<<"Error: Insufficient memory to allocate variable"<<endl;
            }else{
                //create a new space in memory
                int heap_index = hash_search(hash_table, root, t);
                //freeblocks[0].blockSize = freeblocks[0].blockSize - 12;
                //freeblocks[0].offset = freeblocks[0].offset + 12;
                if (heap_index != -1){
                    myMalloc(blocks, value, variable_type, freeblocks[0].offset, 12);
                    //int heap_offset = hash_table[heap_index].offset;
                    //int heap_type = hash_table[heap_index].type;
                    //int heap_noBytes = hash_table[heap_index].noBytes;
                    //hash_table[heap_index].offset = -1;
                    BSTInsert(heap_index, value);
                    
                    
                    
                    
                }else{
                    cout<<"Error: symbol table lookup failed."<<endl;
             
            }
            */
            
            
        }
        if(command_type == "compact"){
            
            for (int l = 0; l< blocklength; l ++){
                cout<<l<<":"<<freeblocks[l].offset<<" "<<freeblocks[l].blockSize<<endl;
            }
             
            int l = 0;
            int originalsize = blocklength;
            cout<<"blocksize:"<<blocklength<<endl;
        
            while( l <= blocklength){
                if ((freeblocks[l].offset + freeblocks[l].blockSize == freeblocks[l+1].offset)&&(freeblocks[l].blockSize != 0)){
                    
                    freeblocks[l].blockSize = freeblocks[l].blockSize + freeblocks[l+1].blockSize;
                    freeblocks[l+1].blockSize = freeblocks[l+2].blockSize;
                    freeblocks[l+1].offset = freeblocks[l+2].offset;
                    
                    //cout<<"COLLIDE:"<<l<<" "<<l+1<<endl;
                    blocklength --;
                    
                }else{
                    l++;
                }
               
                
            }
            cout<<"blocksize:"<<blocklength<<endl;
    
            for (int l = blocklength; l < originalsize;l++ ){
                freeblocks[l].blockSize = 0;
                freeblocks[l].offset = 0;
            }
            

            
            for (int l = 0; l< blocklength; l ++){
                cout<<l<<":"<<freeblocks[l].offset<<" "<<freeblocks[l].blockSize<<endl;
                
            }
            

            
            
        }
    }
   

    
    delete[] blocks;
    return size_of_physical_block;
}

