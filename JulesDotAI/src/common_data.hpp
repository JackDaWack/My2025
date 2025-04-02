#ifndef COMMON_DATA_H
#define COMMON_DATA_H

#include "data.hpp"
#include <vector>

class common_data
{
    protected:
    std::vector<data *> * training_data;
    std::vector<data *> * test_data;
    std::vector<data *> * validation_data;

    public:

    //Setter methods:
    void set_training(std::vector<data *> * vect);
    void set_test(std::vector<data *> * vect);
    void set_validation(std::vector<data *> * vect);

};


#endif