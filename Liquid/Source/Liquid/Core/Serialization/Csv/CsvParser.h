#pragma once

namespace Liquid {

	class CsvRow
	{
	public:
		void ReadNextRow(std::istream& stream);

		StringView operator[](size_t index) const;
		size_t GetSize() const;
	private:
		String m_Line;
		std::vector<size_t> m_Data;
	};

	static std::istream& operator>>(std::istream& stream, CsvRow& row)
	{
		row.ReadNextRow(stream);
		return stream;
	}

	struct CsvIterator
	{
	public:
		typedef std::input_iterator_tag iterator_category;
		typedef CsvRow value_type;
		typedef size_t difference_type;
		typedef CsvRow* pointer;
		typedef CsvRow& reference;
	public:
		std::istream* Stream;
		CsvRow Row;
	public:
		CsvIterator()
			: Stream(nullptr) {}
		CsvIterator(std::istream& stream)
			: Stream(stream.good() ? &stream : nullptr)
		{
			(*this)++;
		}

		CsvIterator& operator++()
		{
			if (Stream)
			{
				if (!((*Stream) >> Row))
					Stream = nullptr;
			}
			
			return *this;
		}

		CsvIterator operator++(int)
		{
			CsvIterator tmpIterator(*this);
			++(*this);
			return tmpIterator;
		}

		const CsvRow& operator*() const { return Row; }
		const CsvRow* operator->() const { return &Row; }

		bool operator==(const CsvIterator& rhs) { return ((this == &rhs) || ((this->Stream == nullptr) && (rhs.Stream == nullptr))); }
		bool operator!=(const CsvIterator& rhs) { return !((*this) == rhs); }
	};

	struct CsvRange
	{
	public:
		std::istream& Stream;
	public:
		CsvRange(std::istream& stream)
			: Stream(stream) {}
		
		CsvIterator begin() const { return CsvIterator(Stream); }
		CsvIterator end() const { return CsvIterator(); }
	};

}