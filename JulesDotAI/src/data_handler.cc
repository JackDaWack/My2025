#include "data_handler.hpp"
#include <iostream>
#include <sstream>

//Constructor and destructor
data_handler::data_handler()
{
    data_array = new std::vector<data *>;
    test_data = new std::vector<data *>;
    training_data = new std::vector<data *>;
    validation_data = new std::vector<data *>;
}
data_handler::~data_handler()
{
    delete data_array;
    delete test_data;
    delete training_data;
    delete validation_data;
}

void data_handler::read_feature_vector(std::string path)
{
    std::ifstream infile(path);
    if (!infile.is_open())
    {
        std::cerr << "File not found.\n";
        exit(1);
    }

    std::string line;
    while (std::getline(infile, line))
    {
        std::stringstream ss(line);
        std::string value;
        data* dat = new data();

        // Parse each value in the line
        while (std::getline(ss, value, ','))
        {
            try {
                float feature = std::stof(value);
                dat->append_feature_vector(feature);
            } catch (const std::invalid_argument&) {
                std::cerr << "Invalid value encountered: " << value << "\n";
                delete dat;
                exit(1);
            }
        }

        data_array->push_back(dat);
    }

    std::cout << "Data extraction success.\n"
    << data_array->size() << " feature vectors read and stored.\n";
}
void data_handler::read_feature_labels(std::string path)
{
    uint32_t header[2];//Magic, num of images, rowsize, colsize
    unsigned char bytes[2];
    FILE *f = fopen(path.c_str(),"r");
    if (f)
    {
        for(int i = 0; i < 2; i++)
        {if (fread(bytes, sizeof(bytes), 1, f)){header[i] = convert_to_little_endian(bytes);}}
        printf("Done getting label file header.\n");
        for(int i = 0; i < header[1]; i++)
        {
            uint8_t element[1];
            if(fread(element, sizeof(element), 1, f))
            {
                data_array->at(i)->set_label(element[0]);
            } else
            {
                printf("File reading error.\n");
                exit(1);
            }
        }
        printf("Data extraction success.\n %lu Labels read and stored.\n", data_array->size());
        fclose(f);
    } else
    {
        printf("File not found.\n");
        exit(1);
    }
}
void data_handler::split_data()
{
    std::unordered_set<int> used_indexes;
    int train_size = data_array->size() * TRAIN_SET_PERCENT;
    int test_size = data_array->size() * TEST_SET_PERCENT;
    int validation_size = data_array->size() * VALIDATION_PERCENT;

    //training data:
    int count = 0;
    while (count<train_size)
    {
        int random_index = rand() % data_array->size();
        if(used_indexes.find(random_index) == used_indexes.end())
        {
            training_data->push_back(data_array->at(random_index));
            used_indexes.insert(random_index);
            count++;
        }
    }
    //test data:
    count = 0;
    while (count<test_size)
    {
        int random_index = rand() % data_array->size();
        if(used_indexes.find(random_index) == used_indexes.end())
        {
            test_data->push_back(data_array->at(random_index));
            used_indexes.insert(random_index);
            count++;
        }
    }
    //validation data:
    count = 0;
    while (count<validation_size)
    {
        int random_index = rand() % data_array->size();
        if(used_indexes.find(random_index) == used_indexes.end())
        {
            validation_data->push_back(data_array->at(random_index));
            used_indexes.insert(random_index);
            count++;
        }
    }
    printf("training data size: %lu\n",training_data->size());
    printf("test data size: %lu\n",test_data->size());
    printf("validation data size: %lu\n",validation_data->size());
}
void data_handler::count_classes()
{
    int count = 0;
    for(unsigned i = 0; i <data_array->size(); i++)
    {
        if (class_map.find(data_array->at(i)->get_label()) == class_map.end())
        {
            class_map[data_array->at(i)->get_label()] = count;
            data_array->at(i)->set_enum_label(count);
            count++;
        }
    }
    num_classes = count;
    printf("Successfully extracted %lu unique classes.", num_classes);
}

uint32_t data_handler::convert_to_little_endian(const unsigned char* bytes)
{return (uint32_t) ((bytes[0] << 24) | (bytes[1] << 16) | (bytes[2] << 8) | (bytes[3]));}

std::vector<data *> * data_handler::get_training_data(){return training_data;}
std::vector<data *> * data_handler::get_test_data(){return test_data;}
std::vector<data *> * data_handler::get_validation_data(){return validation_data;}