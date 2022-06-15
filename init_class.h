#pragma once
#include "Header1.h"
#include "Header.h"
#include <fstream>
#include <tuple>

using namespace std;

template<typename T>
class Init_From_File
{
protected:

    inline vector<string> str_split_by_one_space(string str)// Делит строку на посдстроки по пробелу
    {
        vector<string> result;
        while (str.find(" ") != -1)
        {
            result.push_back(str.substr(0, str.find(" ") + 1));
            str = str.erase(0, str.find(" ") + 1);
        }
        if (!str.empty())
        {
            result.push_back(str);
        }
        return result;
    }
public:
    virtual SimpleHashTable<T>* Init_Data_Base() = 0;
};


class Init_Resident_Data_Base : Init_From_File<Resident*>
{
    string data_base_name;
    int size_of_base;

public:
    Init_Resident_Data_Base(int size_of_b, string data_base = "Resident.txt") : data_base_name{data_base}, size_of_base{size_of_b} {}
    SimpleHashTable<Resident*>* Init_Data_Base() override
    {
        string tmp; // для временного хранения строки
        vector<string> splitted_line; // вектор полей структуры
        SimpleHashTable<Resident*>* data_base = new SimpleHashTable<Resident*>{size_of_base};
        ifstream file_for_input(data_base_name);

        while (getline(file_for_input, tmp))
        {
            splitted_line = str_split_by_one_space(tmp);
   
            Resident* tmp_struct = new Resident{ splitted_line[0], stoi(splitted_line[1]), 
               stoi(splitted_line[2]), stoi(splitted_line[3])}; // инициализация полей структуры
            data_base->add_element(tmp_struct);
        }
        file_for_input.close();
        return data_base;
    }
};

class Init_Room_Data_Base : Init_From_File<Room*>
{
    string data_base_name;
    int size_of_base;

public:
    Init_Room_Data_Base(int size_of_b, string data_base = "Room.txt") : data_base_name{data_base}, size_of_base{size_of_b} {}
    SimpleHashTable<Room*>* Init_Data_Base() override
    {
        string tmp; // для временного хранения строки
        vector<string> splitted_line; // вектор полей структуры
        SimpleHashTable<Room*>* data_base = new  SimpleHashTable<Room*>{ size_of_base };
        ifstream file_for_input(data_base_name);

        while (getline(file_for_input, tmp))
        {
            splitted_line = str_split_by_one_space(tmp);
 
            Room* tmp_struct = new Room{stoi(splitted_line[0]), stoi(splitted_line[1]),
               stoi(splitted_line[2]) }; // инициализация полей структуры
            data_base->add_element(tmp_struct);
        }
        file_for_input.close();
        return data_base;
    }
};

class All_Data_Input
{
    SimpleHashTable<Room*>* room_data_base;
    SimpleHashTable<Resident*>* resident_data_base;
public:
    All_Data_Input(int room_data_base_size, int resident_data_base_size)
    {
        Init_Room_Data_Base room_init_object(room_data_base_size);
        Init_Resident_Data_Base resident_init_object(resident_data_base_size);
        room_data_base = room_init_object.Init_Data_Base();
        resident_data_base = resident_init_object.Init_Data_Base();
    }
    tuple<SimpleHashTable<Room*>*, SimpleHashTable<Resident*>*> get_data()
    {
        return make_tuple(room_data_base, resident_data_base);
    }
};

