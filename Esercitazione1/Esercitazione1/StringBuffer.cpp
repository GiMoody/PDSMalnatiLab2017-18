#include "pch.h"
#include "StringBuffer.h"

using namespace std;

void StringBuffer::set(const char* str) {
	if (strlen(str) > this->sb_size) {
		delete this->ptr;
		this->ptr = NULL;
		this->sb_size = strlen(str) + START_SIZE;
		this->ptr = new char[this->sb_size];
	}
	memcpy(this->ptr, str, strlen(str) + 1);
}

void StringBuffer::set(const StringBuffer& sb) {
	if (strlen(sb.ptr) > this->sb_size) {
		delete this->ptr;
		this->ptr = NULL;
		this->sb_size = strlen(sb.ptr) + START_SIZE;
		this->ptr = new char[this->sb_size];
	}
	memcpy(this->ptr, sb.ptr, strlen(sb.ptr) + 1);
}

void StringBuffer::clear() {
	this->ptr[0] = '\0';
}

void StringBuffer::insert(const char *str, size_t pos) {
	// Strings lenght precalculation
	size_t end_dim = strlen(str) + this->size() + 1;
	size_t diff = abs(long(this->size() - pos + 1));
	if (long(this->size() - pos) < 0) end_dim += diff;
	
	// Intermediate containers
	char *tmp_final_string = new char[end_dim];
	char *tmp_middle_string = new char[diff];

	if (pos <= this->size()) {
		// If "pos" smaller than StringBuffer's string
		strncpy_s(tmp_final_string, end_dim, this->ptr, pos);
		strncpy_s(tmp_middle_string, diff, this->ptr+pos, this->size() - pos);
		strncat_s(tmp_final_string, end_dim, str, strlen(str));
		strncat_s(tmp_final_string, end_dim, tmp_middle_string, strlen(tmp_middle_string));
	}
	else {
		// If "pos" bigger than the string's lenght
		for (int i = 0; i < diff; i++) tmp_middle_string[i] = ' ';

		strncpy_s(tmp_final_string, end_dim, this->ptr, this->size());
		strncat_s(tmp_final_string, end_dim, tmp_middle_string, diff);
		strncat_s(tmp_final_string, end_dim, str, strlen(str));
	}

	// Update Stringbuffer and release memory
	this->sb_size = end_dim;
	delete this->ptr;
	
	this->ptr = new char[end_dim];
	memcpy(this->ptr, tmp_final_string, end_dim);

	delete tmp_final_string;
	delete tmp_middle_string;
}

/// For semplicity I call the other insert method 
void StringBuffer::insert(const StringBuffer& sb, size_t pos) {
	insert(sb.ptr, pos);
}


void StringBuffer::append(const char *str) {
	size_t end_dim = strlen(str) + this->size() + 1;

	// Check if the new string is bigger than the allocated buffer
	if ( end_dim > this->buf_size()) {
		char *temp_string = new char[strlen(str) + this->size() + 1];
		strncpy_s(temp_string, this->size()+1, this->ptr, this->size());
		strncat_s(temp_string, end_dim, str, strlen(str));

		delete this->ptr;
		this->sb_size = end_dim + START_SIZE;
		this->ptr = new char[sb_size];

		memcpy(this->ptr, temp_string, end_dim);

		delete temp_string;
	}
	else
		strncat_s(this->ptr, this->sb_size, str, strlen(str));
}

/// For semplicity I call the other append method 
void StringBuffer::append(const StringBuffer& sb) {
	append(sb.ptr);
}

size_t StringBuffer::size() { return strlen(ptr); }
size_t StringBuffer::buf_size() { return sb_size; }
const char* StringBuffer::c_str() { return ptr; }