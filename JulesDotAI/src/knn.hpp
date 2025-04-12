#ifndef __KNN_H
#define __KNN_H

#include "common_data.hpp"

class knn : public common_data
{
    int k;
    std::vector<data *> * training_data;
    std::vector<data *> * test_data;
    std::vector<data *> * validation_data;
    std::vector<data *> * neighbors;

    public:
    knn(int val);
    ~knn();

    void find_k_nearest(data* q_point);
    int predict();
    double calc_distance(data* q_point, data* input);
    double test_performance();
    double validate_performance();
    //setters:
    void set_k(int val);
};


#endif