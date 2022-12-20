#include "LiquidPCH.h"
#include "CsvParser.h"

namespace Liquid {

	void CsvRow::ReadNextRow(std::istream& stream)
	{
		std::getline(stream, m_Line);

		m_Data.clear();
		m_Data.emplace_back(-1);

		size_t currentPos = 0;
		while ((currentPos = m_Line.find(',', currentPos)) != String::npos)
		{
			m_Data.emplace_back(currentPos);
			currentPos++;
		}

		currentPos = m_Line.size();
		m_Data.emplace_back(currentPos);
	}

	StringView CsvRow::operator[](size_t index) const
	{
		const char* data = &m_Line[m_Data[index] + 1];
		size_t count = (m_Data[index + 1]) - (m_Data[index] + 1);

		return StringView(data, count);
	}

	size_t CsvRow::GetSize() const
	{
		return m_Data.size() - 1;
	}

}