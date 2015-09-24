#pragma once
#include <samchon/API.hpp>

#include <vector>
#include <string>

namespace samchon
{
	/**
	 * @brief Binary data class
	 *
	 * @details
	 * <p> ByteArray is the standard class for handling binary data in Samchon Framework. </p>
	 * <p> ByteArray provides methods for reading and writing binary data. </p>
	 * <p> In addition, zlib compression and decompression are supported. </p>
	 * 
	 * @note
	 * <p> ByteArray is a class only for advanced developer who can access data on byte level as the reason of: </p>
	 * 
	 *	\li Handling File Data
	 *	\li Custom protocol for network I/O
	 *	\li Optimizing the size of data for performance
	 *
	 * <p> If you're not trying to use ByteArray as the reason of what I've mentioned
	 * but only for representing entity data, I recomment you to use Entity instead. </p>
	 * 
	 * @author Jeongho Nam
	 */
	class SAMCHON_FRAMEWORK_API ByteArray
		: public std::vector<unsigned char>
	{
	private:
		typedef std::vector<unsigned char> super;

		/**
		 * @brief Current position of the ByteArray.\n
		 *
		 * @details
		 * If you call read, starting pointer will be the position
		 * \li (_Ty *ptr = (_Ty*)data() + position)
		 */
		size_t position;

	public:
		/* --------------------------------------------------------------
			CONSTRUCTROS
		-------------------------------------------------------------- */
		/**
		 * @brief Default Constructor
		 */
		ByteArray();
		/**
		 * @brief Copy Constructor
		 */
		ByteArray(const ByteArray &);
		/**
		 * @brief Move Constructor
		 */
		ByteArray(ByteArray &&);

		/* --------------------------------------------------------------
			POSITION
		-------------------------------------------------------------- */
		auto getPosition() const -> size_t;
		void setPosition(size_t);

		template <typename _Ty> static auto reverse(const _Ty &val) -> _Ty
		{
			_Ty res;
			int size = sizeof(_Ty);

			unsigned char *originalBytes = (unsigned char*)&val;
			unsigned char *reversedBytes = (unsigned char*)&res;

			for (int i = 0; i < size; i++)
				reversedBytes[i] = originalBytes[size - i - 1];

			return res;
		};

		/* --------------------------------------------------------------
			READ BYTES
		-------------------------------------------------------------- */
		/**
		 * @brief Read data
		 * @details Reads data(_Ty) from the ByteArray and adds the position following the size
		 * 
		 * @tparam _Ty A type of data to be returned
		 * @throw exception out of range
		 * @return Read data from the byte stream
		 */
		template <typename _Ty> auto read() const -> _Ty
		{
			_Ty *ptr = (_Ty*)(data() + position);
			((ByteArray*)this)->position += sizeof(_Ty);

			return *ptr;
		};
		template<> auto read() const -> std::string;

		template <typename _Ty> auto readReversely() const -> _Ty
		{
			_Ty val = read<_Ty>();
			return reserve(val);
		}

		/* --------------------------------------------------------------
			WRITE BYTES
		-------------------------------------------------------------- */
		/**
		 * @brief Write data
		 * @details Writes data(_Ty) to tail of the ByteArray
		 * 
		 * @tparam _Ty A type of data to write
		 * @param val Something to write on ByteArray
		 */
		template <typename _Ty> void write(const _Ty &val)
		{
			unsigned char *ptr = (unsigned char*)&val;
			insert(end(), ptr, ptr + sizeof(_Ty));
		};
		template<> void write(const std::string &str);
		template<> void write(const ByteArray &byteArray);

		template<typename _Ty> void writeReversely(const _Ty &val)
		{
			write(reverse(val));
		};

		/* --------------------------------------------------------------
			COMPRESS & DECOMPRESS
		-------------------------------------------------------------- */
		/**
		 * @brief Compress the binary data\n
		 * 
		 * @details
		 * <p> Generates a binary data compressed from the ByteArray. </p>
		 * <p> The binary data will be compressed by zlib library. </p>
		 * 
		 * @return ByteArray which is compressed
		 */
		auto compress() const -> ByteArray;

		/**
		 * @brief Decompress the binary data
		 *
		 * @details
		 * <p> Generates a decompressed binary data form the ByteArray. </p>
		 * <p> The binary data will be decompressed by zlib library. </p>
		 * 
		 * @return ByteArray that is decompressed
		 */
		auto decompress() const -> ByteArray;
	};
};