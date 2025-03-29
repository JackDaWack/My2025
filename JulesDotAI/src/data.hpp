#ifndef __DATA_H
#define __DATA_H
//Libraries
#include <vector>
#include "stdio.h"
#include "stdint.h"

class data
{
    std::vector<uint8_t> * feature_vector; //no class at end.
    uint8_t label;
    int enum_label; // A -> 1, B -> 2
    double distance;

    public:
    data();
    ~data();
    //Setter and appender methods.
    void set_feature_vector(std::vector<uint8_t> *);
    void append_feature_vector(uint8_t);
    void set_label(uint8_t);
    void set_enum_label(int);
    void set_distance(double);
    //Getter methods.
    std::vector<uint8_t> * get_feature_vector();
    int get_feature_vector_size();
    uint8_t get_label();
    uint8_t get_enum_label();
    double get_distance();



};




#endif