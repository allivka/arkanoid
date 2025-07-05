
#include <iostream>
#include <math.h>
#include <sstream>
#include <vector>
#include <fstream>

template <typename T> T powder(T op1, unsigned long long op2) {
    T buff = 1;
    for(unsigned long long i = 0; i < op2; i++) {
        buff *= op1;
    }
    
    return buff;
}

long double calcOp(long double op1, char operation, long double op2) {
    
    long double result = 0;
    
    switch(operation) {
        case '+':
        result = op1 + op2;
        break;
        
        case '-':
        result = op1 - op2;
        break;
        
        case '*':
        result = op1 * op2;
        break;
        
        case '/':
        if(op2 == 0) {
            std::cout << "\nFailed dividing by zero!\n";
            throw "\n!Failed dividing by zero!\n";
        }
        result = op1 / op2;
        break;
        
        case '^':
        result = powder(op1, op2);
        break;
        
    }
    
    return result;
}

int main(int argc, char *argv[]) {
    
    try {
    
    std::vector<long double> values;    
    
    std::string buff;
    std::getline(std::cin, buff);
    
    std::stringstream stream(buff);
    
    long double numBuff = 0;
    
    while(!stream.eof()) {
        stream >> numBuff;
        values.push_back(numBuff);
    }
    
    char operation = '+';
    
    long double op2;
    
    std::cin >> operation >> op2;
    
    std::fstream file("output.txt", std::ios::out  | std::ios::app);
    
    for(const auto& op1 : values) {
        file << op1 << ' ' << operation << ' ' << op2 << " = " << calcOp(op1, operation, op2) << '\n';
    }
    
    } catch(const char *msg) {
        std::cout << msg;
        exit(EXIT_FAILURE);
    }
}