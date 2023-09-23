#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdint.h>
#include "util.hh"
using namespace std;
void setUpper(int8_t up,  int16_t* full){
    *full = (*full & 0x00FF) | (static_cast<int16_t>(up) << 8);   
}
void setLower(int8_t low, int16_t* full){
    *full = (*full & 0xFF00) | (low & 0xFF);   
}

struct i2{
    int val1, val2;
};
struct i2 getMovArgs(uint8_t opcode){
    i2 result;
    result.val1 = (opcode - 0xC0) / 8;
    result.val2 = (opcode - 0xC0) % 8;
    return result;
}
int between(int val, int from, int to){
    if(val <= to && val >= from)return 1;
    return 0;
}

int main(){
    string filename = "program";
    vector<uint8_t> v = readFileBytes(filename);
    int16_t registers[4] = {0, 0, 0, 0};
    int offsets[] = {
        0, 4, 6, 2,//8bL
        1, 5, 7, 3,//8bH
        0, 2, 4, 6 //16b
    };
    for(int i =0; i < v.size();i++){
        //printf("%02x\n", v[i]);
                    //Mov       (REVIEW REQUIED)
        if(between(v[i],0xB0, 0xBB)){
            int offset = v[i] - 0xB0;
            if(offset < 8){
                uintptr_t address = reinterpret_cast<uintptr_t>(&registers) + offsets[offset];
                uint8_t* ptr = reinterpret_cast<uint8_t*>(address);
                *ptr = v[++i];
                //cout << registers << " + " << offsets[offset] << " = " << static_cast<void*>(ptr) << "\n";
            }else{
                uintptr_t address = reinterpret_cast<uintptr_t>(&registers) + offsets[offset];
                uint8_t* ptr = reinterpret_cast<uint8_t*>(address);
                int high = v[++i];
                *ptr = (((int16_t)v[++i] << 8) | high);
                cout << "Moving " << *ptr << " to " << offset << "\n";
            }
        }else       //Jmp       (100% WORKING)
        if(v[i] == 0xEB){
            int a = v[++i];
            if(a >= 128){a = -(255 - a) - 1;};
            cout << "Jumping by " << a << " bytes\n";
            i += a;
            cout << i << "\n";
        }else       //Mov reg
        if(v[i] == 0x88){
            int a = v[++i];
            struct i2 marg = getMovArgs(a);
            int8_t* ptr = (int8_t*)(registers + offsets[marg.val2]);
            *ptr = *((int8_t*)(registers + offsets[marg.val1]));
            cout << "Moving " << marg.val1 << " into " << marg.val2 << "\n";
        }else       //Add           (REVIEW REQUIED)
        if(v[i] == 0x01 || v[i] == 0x00){
            int a = v[++i];
            struct i2 marg = getMovArgs(a);
            int16_t* ptr = (int16_t*)(registers + offsets[marg.val2]);
            cout << "Adding " << *((int16_t*)(registers + offsets[marg.val1])) << " to\n";
            *ptr += *((int16_t*)(registers + offsets[marg.val1]));
            //cout << marg.val2 << "\n";
        }else       //Interupt      (TODO)
        if(v[i] == 0xCD){
            cout << "Interupt " << v[++i] << "\n"; 
        }else       //Reg dump      (TESTING)
        if(v[i] == 0x22){
            cout 
                << registers[0] << "\n"
                << registers[1] << "\n"
                << registers[2] << "\n"
                << registers[3] << "\n"; 
                //<< (registers[0]& 0xFF) << "\n"
                //<< (registers[1]& 0xFF) << "\n"
                //<< ((registers[0]>> 8)& 0xFF) << "\n"
                //<< ((registers[1]>> 8)& 0xFF) << "\n"; 
        }else       //Add value
        if(v[i] == 0x83){
            
        }
        b:
    }
}   
