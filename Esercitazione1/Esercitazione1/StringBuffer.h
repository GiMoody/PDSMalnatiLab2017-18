#pragma once

/**********************************
* Esercitazione 1 2017/2018 - PDS *
***********************************/
#define START_SIZE 100

/**
* StringBuffer is a dynamic string container where the string can have variable size 
*/
class StringBuffer
{
	
	public:
		char *ptr;		 // Chars containers
		size_t sb_size;  // Buffer size

		/**
		* Default Constructor - Initial dimension of ptr is START_SIZE
		*/
		StringBuffer() {
			ptr = new char[START_SIZE];
			sb_size = START_SIZE;
			size_t test = sizeof(ptr);
		};

		/**
		* Standard Constructor - Create a new StringBuffer contained the string "str" as copy
		* @param str : input string
		*/
		StringBuffer(const char *str) {
			sb_size = strlen(str) + START_SIZE + 1;
			ptr = new char[sb_size];
			memcpy(ptr, str, sb_size);
		};

		/**
		* Copy Constructor - Initializate a copy of the input StringBuffer
		* @param sb : StringBuffer input object to copy
		*/
		StringBuffer(const StringBuffer& sb) {
			sb_size = sb.sb_size + START_SIZE;
			ptr = new char[sb_size];
			memcpy(ptr, sb.ptr, sb_size);
		};

		~StringBuffer() { delete ptr; };

		/// General public methods
		/**
		* Return the string size
		* @return size_t : string's lenght
		*/
		size_t size();

		/**
		* Return the size of the buffer
		* @return size_t buf_size : buffer's size
		*/
		size_t buf_size();

		/**
		* Return the string contained into the buffer
		* @return cost char* c_str : char's array
		*/
		const char* c_str();
		

		/**
		* Subsitutes the current string with the one received as input, if necessary the buffer will be reallocated
		* @param str : input string
		*/
		void set(const char* str);

		/**
		* Subsitutes the current string with the one contained in the input StringBuffer, if necessary the current buffer will be reallocated
		* @param sb : StringBuffer input object
		*/
		void set(const StringBuffer& sb);

		/**
		* It sets to 0 the used char
		*/
		void clear();


		/**
		* Insert the string str in the position pos of the current buffer
		* @param str : input string
		* @param pos : position in wich insert the string
		*/
		void insert(const char *str, size_t pos);
		
		/**
		* Insert the string contained in sb StringBuffer in the position pos of the current buffer
		* @param sb : StringBuffer input object to copy
		* @param pos : position in wich insert the string
		*/
		void insert(const StringBuffer& sb, size_t pos);

		/**
		* Append the string str to the buffer's string
		* @param str : input string to append 
		*/
		void append(const char *str);
		
		/**
		 * Append the string str to the buffer's string
		 * @param sb : input StrinbBuffer to append
		 */
		void append(const StringBuffer& sb);
		
};