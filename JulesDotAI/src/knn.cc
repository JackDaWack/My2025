#include "knn.hpp"
#include <cmath>
#include <limits>
#include <map>
#include "stdint.h"
#include "data_handler.hpp"

knn::knn(int val)
{
    k = val;
}
knn::~knn()
{
    //free stuff later
}

//Calculates and stores the distance between data points. 
//Stored distances are then used to find the nearest K values.
//O(N^2) if K is close to N in value. May be near O(N) if K is small.
void knn::find_k_nearest(data* q_point)
{
    neighbors = new std::vector<data *>;
    double min = std::numeric_limits<double>::max();
    double prev_min = min;
    int indx = 0;
    //distance must be greater than the current min, but lower than previous min.
    for (int i = 0; i < k; i++)
    {
        if (i == 0)
        {
            for (int j = 0; j < training_data->size(); j++)
            {
                double distance = calc_distance(q_point, training_data->at(j));
                training_data->at(j)->set_distance(distance);
                if (distance < min)
                {
                    min = distance;
                    indx = j;
                }
            }
            neighbors->push_back(training_data->at(indx));
            prev_min = min;
            min = std::numeric_limits<double>::max();
        } else
        {
            for (int j = 0; j < training_data->size(); j++)
            {
                double distance = calc_distance(q_point, training_data->at(j));
                if (distance > prev_min && distance < min) 
                {
                    min = distance;
                    indx = j;
                }
            }
            neighbors->push_back(training_data->at(indx));
            prev_min = min;
            min = std::numeric_limits<double>::max();
        }
    }
}
int knn::predict()
{
    std::map<uint8_t, int> class_freq;
    for (int i = 0; i < neighbors->size(); i++)
    {
        if (class_freq.find(neighbors->at(i)->get_label()) == class_freq.end())
        {class_freq[neighbors->at(i)->get_label()] = 1;}
        else
        {class_freq[neighbors->at(i)->get_label()]++;}
    }
    int best = 0;
    int max = 0;
    for (auto kv : class_freq)
    {
        if (kv.second > max)
        {
            max = kv.second;
            best = kv.first;
        }
    }
    delete neighbors;
    return best;
}

//Calculates the distance between data points.
double knn::calc_distance(data* q_point, data* input)
{
    double distance = 0.0;
    if(q_point->get_feature_vector_size() != input->get_feature_vector_size())
    {
        printf("Error:Feature Vector Missmatch.\n");
        exit(1);
    }

#ifdef EUCLID
    for (unsigned i = 0; i < q_point->get_feature_vector_size(); i++)
    {distance = pow((q_point->get_feature_vector()->at(i) - input->get_feature_vector()->at(i)),2);}  
    distance = sqrt(distance);
#elif MANHATTAN
    //Manhattan Implementation here:
#endif
    return distance;
}
double knn::test_performance()
{
    double current_performance = 0.0;
    int count = 0;
    int data_indx = 0;
    for (data *q_point : *test_data)
    {
        find_k_nearest(q_point);
        int prediction = predict();
        if (prediction == q_point->get_label())
        {count++;}
        data_indx++;
        printf("Current Performance: %3f %%/n",(((double)count*100.0)/((double)data_indx)));
    }
    current_performance =((double)count*100.0)/((double)test_data->size());
    printf("Test Performance: %3f %%/n",(((double)count*100.0)/((double)test_data->size())));
    return current_performance;
}
double knn::validate_performance() 
{
    double current_performance = 0.0;
    int count = 0;
    int data_indx = 0;
    for (data *q_point : *validation_data)
    {
        find_k_nearest(q_point);
        int prediction = predict();
        if (prediction == q_point->get_label())
        {count++;}
        data_indx++;
        printf("Current Performance: %3f %%/n",(((double)count*100.0)/((double)data_indx)));
    }
    current_performance =((double)count*100.0)/((double)validation_data->size());
    printf("Validation Performance: %3f %%/n",(((double)count*100.0)/((double)validation_data->size())));
    return current_performance;
}
//setters:
void knn::set_k(int val){k = val;}