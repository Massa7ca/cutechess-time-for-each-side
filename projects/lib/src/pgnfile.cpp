/*
    This file is part of Cute Chess.

    Cute Chess is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Cute Chess is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Cute Chess.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "pgnfile.h"
#include <QFile>


PgnFile::PgnFile()
	: m_variant(Chess::Variant::NoVariant),
	  m_lineNumber(1),
	  m_rewind(false)
{
}

PgnFile::PgnFile(const QString& filename, Chess::Variant variant)
{
	setVariant(variant);
	open(filename);
}

Chess::Board* PgnFile::board()
{
	return &m_board;
}

bool PgnFile::isOpen() const
{
	return (m_in.device() && m_in.device()->isOpen());
}

bool PgnFile::open(const QString& filename)
{
	m_lineNumber = 1;
	m_rewind = false;

	m_file.setFileName(filename);
	if (!m_file.open(QIODevice::ReadOnly))
		return false;

	m_in.setDevice(&m_file);
	return true;
}

qint64 PgnFile::lineNumber() const
{
	return m_lineNumber;
}

QChar PgnFile::readChar()
{
	if (m_rewind)
	{
		m_rewind = false;
		return m_lastChar;
	}
	
	m_in >> m_lastChar;
	if (m_lastChar == '\n')
		m_lineNumber++;
	
	return m_lastChar;
}

QString PgnFile::readLine()
{
	m_lineNumber++;
	return m_in.readLine();
}

void PgnFile::rewind()
{
	m_in.resetStatus();
	m_lineNumber = 1;
	m_rewind = false;
	m_in.seek(0);
}

void PgnFile::rewindChar()
{
	m_rewind = true;
}

void PgnFile::skipWhiteSpace()
{
	do
	{
		readChar();
	}
	while (m_lastChar.isSpace());
	m_rewind = true;
}

QTextStream::Status PgnFile::status() const
{
	return m_in.status();
}

Chess::Variant PgnFile::variant() const
{
	return m_variant;
}

void PgnFile::setVariant(Chess::Variant variant)
{
	m_variant = variant;
	if (!m_variant.isNone())
		m_board.setVariant(m_variant);
}