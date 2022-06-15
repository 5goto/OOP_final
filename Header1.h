#pragma once
#include <iostream>
#include <string>
#include <cstdint>
#include <exception>
#include <stdlib.h>
using namespace std;

auto id_genetator = []() -> uint16_t {return rand() % (10000 + 1); };

class Resident
{
	struct date
	{
		int day;
		int mounth;
		int year;
		date(int day_, int mounth_, int year_) : day{ day_ }, mounth{ mounth_ }, year{ year_ } {}
		date() {}

	};
	string name;
	int resident_id;
	date registration_date;


public:
	Resident(string name_, int day, int mounth, int year, int id = id_genetator())
		: name{ name_ }, resident_id{ id_genetator() }
	{
		registration_date.day = day;
		registration_date.mounth = mounth;
		registration_date.year = year;
	}
	Resident() { name = "#"; resident_id = 0; }

	friend ostream& operator << (ostream& out, Resident& object)
	{
		out << "[" << object.resident_id << "]" << object.name << " | " << object.registration_date.day << "." <<
			object.registration_date.mounth << "." << object.registration_date.year;
		return out;
	}

	operator int() const
	{
		return resident_id;
	}
};

enum class capacity // ����� - ������������ ��� ���� � ������ room
{
	single = 0, doubl, triple, extra
};

std::ostream& operator << (std::ostream& out, const capacity& t)
{
	switch (t) {
	case capacity::single: return (out << "Single");
	case capacity::doubl: return (out << "Doubl");
	case capacity::triple: return (out << "Triple");
	case capacity::extra: return (out << "Extra");
	}
	return (out);
}

class Room // ����� ����� �����
{
	capacity room_type; // ��� �������
	int room_size;   // ������ �������
	bool is_it_free;  // �������� �� �������
	int room_number;  // ���������� ����� �������
public:
	Room(int room_type_, int room_size_, int room_number_, bool is_it_free_ = false)
		: room_type{ room_type_ }, room_size{ room_size_ }, room_number{ room_number_ }, is_it_free{ is_it_free_ } {}
	Room() : room_type{ capacity::single }, room_size{}, room_number{}, is_it_free{} {}
	void change_status(bool is_it_free_)
	{
		is_it_free = is_it_free_;
	}
	uint16_t get_room_number() { return room_number; }

	bool check_is_it_free()
	{
		return is_it_free;
	}

	operator int() const
	{
		return room_number;
	}

	friend ostream& operator << (ostream& out, Room& object)
	{
		out << "Room number: " << object.room_number << " | Size:" << object.room_size << " m. | " << object.room_type << 
			" | " << object.is_it_free;
		return out;
	}
};


class Record // ����� - ������/�����
{
	bool is_it_active;  //������������ �� �����
	struct discription
	{
		int room_number; // ��������� ����� �������
		int resident_id; // � ���������� ��������������� ������������
		discription(int id, int num) : resident_id{ id }, room_number{ num } {}
		discription() : room_number{}, resident_id{} {}
		bool operator ==(const discription& other)
		{
			return room_number == other.room_number && resident_id == other.resident_id;
		}
	};
	discription info;
public:
	bool operator ==(const Record& other)
	{
		return info == other.info && is_it_active == other.is_it_active;
	}
	Record(int id, int number, bool active = false) : is_it_active{active}
	{
		info = discription(id, number);
	}
	Record() : is_it_active{ false }, info{ discription{} } {}

	void do_active()
	{
		is_it_active = true;
	}
};