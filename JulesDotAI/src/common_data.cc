#include "common_data.hpp"

void common_data::set_training(std::vector<data *> * vect){training_data = vect;}
void common_data::set_test(std::vector<data *> * vect){test_data = vect;}
void common_data::set_validation(std::vector<data *> * vect){validation_data = vect;}