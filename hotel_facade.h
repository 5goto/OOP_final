#pragma once
#include "Header1.h"
#include "Header.h"
#include "init_class.h"
#include <functional>

auto find_free_rooms = [](Room* data) {return (data->check_is_it_free() == true) ? true : false; };

template<typename Iter, typename Result_type, typename Data_type>
Stack<Result_type*> custom_add_if(Iter& first, Iter& last, std::function<bool(Data_type)> func)
{
	Stack<Result_type*> result;
	for (first; first != last; ++first)
		if (func(first->data))
			result.insert(*first);
	return result;
}

class HotelFacade // Паттерн фасад для усправления основными сущностями
{
	SimpleHashTable<Resident*>* client_data_base;
	SimpleHashTable<Room*>* room_data_base;
	Stack<Record*> record_data_base;
	bool is_it_initialised;
public:
	HotelFacade() : is_it_initialised{ false }, client_data_base{ nullptr }, room_data_base{nullptr}{}
	~HotelFacade()
	{
		delete client_data_base;
		delete room_data_base;
	}

	void Init_Data_Bases(size_t room_data_base_size, size_t resident_data_base_size) // инициализация основных баз данных
	{
		All_Data_Input init_object(room_data_base_size, resident_data_base_size);
		std::tie(room_data_base, client_data_base) = init_object.get_data();
		is_it_initialised = true;
	}

	void create_record()
	{
		if (!is_it_initialised)
			throw logic_error("Not initialized");
		Stack<Resident*> client = custom_add_if(client_data_base->begin(), client_data_base->end(),
			[](Room* data) {return (data->check_is_it_free() == true) ? true : false; });
	}
	
	

	void show_all_rooms()
	{
		if (!is_it_initialised)
			throw logic_error("Not initialized");
		for (auto room : *room_data_base)
			if(room.cell_status == status::occupied)
			cout << *room.data << endl;
		cout << endl;
	}

	void show_all_residents()
	{
		if (!is_it_initialised)
			throw logic_error("Not initialized");
		for (auto resident : *client_data_base)
			if (resident.cell_status == status::occupied)
				cout << *resident.data << endl;
		cout << endl;
	}

};



