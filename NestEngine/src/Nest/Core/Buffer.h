#pragma once

#include <vector>

#include "Nest/Core/Core.h"

namespace Nest
{
	class Buffer
	{
	public:
		Buffer() = default;
		Buffer(size_t bufSize, const uint8_t *data = nullptr);

		template<typename T>
		Buffer(const std::vector<T> &dataVec) :
			Buffer(sizeof(T) * dataVec.size())
		{
			m_bytesUsed = sizeof(T) * dataVec.size();
			memcpy(m_data, dataVec.data(), m_bytesUsed);
		}

		Buffer(const Buffer &other);
		Buffer(Buffer &&other) noexcept;

		~Buffer();

		inline size_t size() const { return m_bytesUsed; }
		inline size_t reserved() const { return m_reserved; }

		inline uint8_t* begin() { return m_data; }
		inline const uint8_t* begin() const { return m_data; }
		inline uint8_t* end() { return m_data + m_bytesUsed; }
		inline const uint8_t* end() const { return m_data + m_bytesUsed; }

		inline uint8_t& operator[](size_t i) { return *(m_data + i); }
		inline const uint8_t& operator[](size_t i) const { return *(m_data + i); }

		template <typename T>
		void push_back(const T &obj)
		{
			if (m_bytesUsed + sizeof(T) > m_reserved)
				reserve(3 * (m_reserved + sizeof(T)) / 2, true);

			memcpy(m_data + m_bytesUsed, &obj, sizeof(T));
			m_bytesUsed += sizeof(T);
		}

		void push_back(const uint8_t *src, size_t byteCount);

		template <typename T>
		void fill(const T &obj)
		{
			if (m_reserved % sizeof(T) != 0)
				return;

			setBytesUsed(0);
			for (size_t i = m_reserved / sizeof(T); i--;)
				push_back(obj);
		}

		inline void setBytesUsed(size_t bytesUsed) { m_bytesUsed = bytesUsed; }

		void reserve(size_t newSize, bool retainData);
		void clear();

		Buffer& operator=(const Buffer &other);
		Buffer& operator=(Buffer &&other) noexcept;

		template <typename ...Args>
		static Buffer Interweave(const std::vector<Args>&... dataVecs)
		{
			size_t bufSize = InterweaveBufSize(dataVecs...);

			// Return blank buffer if vectors are of different length
			if (bufSize == 0)
				return Buffer();

			Buffer result{ bufSize };
			result.m_bytesUsed = bufSize;
			size_t offset = 0;
			size_t stride = InterweaveStride(dataVecs...);
			Interweave(result, offset, stride, dataVecs...);
			return result;
		}

		template <typename T, typename ...Args>
		static void Interweave(Buffer &buf, size_t offset, size_t stride, const std::vector<T> &dataVec, const std::vector<Args>&... args)
		{
			Interweave(buf, offset, stride, dataVec);
			offset += sizeof(T);
			Interweave(buf, offset, stride, args...);
			return buf;
		}

		template <typename T>
		static void Interweave(Buffer &buf, size_t offset, size_t stride, const std::vector<T> &dataVec)
		{
			for (size_t i = 0; i < dataVec.size(); ++i)
			{
				memcpy((char*)buf.begin() + i * stride + offset, &dataVec[i], sizeof(T));
			}
		}

	private:
		size_t m_bytesUsed = 0, m_reserved = 0;
		uint8_t *m_data = nullptr;

		template <typename T, typename ...Args>
		static size_t InterweaveBufSize(const std::vector<T> &dataVec, const std::vector<Args>&... args)
		{
			size_t bufSize = 0, vecLen = dataVec.size();
			InterweaveBufSize(bufSize, vecLen, dataVec, args...);
			return bufSize;
		}

		template <typename T, typename ...Args>
		static void InterweaveBufSize(size_t &bufSize, size_t vecLen, const std::vector<T> &dataVec, const std::vector<Args>&... args)
		{
			InterweaveBufSize(bufSize, vecLen, dataVec);
			InterweaveBufSize(bufSize, vecLen, args...);
		}

		template <typename T>
		static void InterweaveBufSize(size_t &bufSize, size_t vecLen, const std::vector<T> &dataVec)
		{
			if (dataVec.size() != vecLen)
			{
				NE_WARN("Attempting to interweave vectors of different sizes.");
				bufSize = 0;
				return;
			}

			bufSize += sizeof(T) * dataVec.size();
		}

		template <typename T, typename ...Args>
		static size_t InterweaveStride(const std::vector<T> &dataVec, const std::vector<Args>&... args)
		{
			return InterweaveStride(args...) + sizeof(T);
		}

		template <typename T>
		static size_t InterweaveStride(const std::vector<T> &dataVec)
		{
			return sizeof(T);
		}
	};
}