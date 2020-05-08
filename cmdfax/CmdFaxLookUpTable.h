// HamFax -- an application for sending and receiving amateur radio facsimiles
// Copyright (C) 2002
// Christof Schmitt, DH1CS <cschmitt@users.sourceforge.net>
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#ifndef CMDFAXLOOKUPTABLE_H
#define CMDFAXLOOKUPTABLE_H

#include <cstddef>

template <class T> class LookUpTable {
public:
	LookUpTable(const size_t N);
	~LookUpTable(void);
	T& operator[](size_t i);
	void setIncrement(size_t i);
	T nextValue(void);
	size_t size(void);
	void reset(void);
private:
	T* table;
	size_t table_size;
	size_t next;
	size_t increment;
};

template <class T> LookUpTable<T>::LookUpTable(const size_t N)
	: table_size(N), next(0), increment(0)
{
	table=new T[table_size];
}

template <class T> LookUpTable<T>::~LookUpTable(void)
{
	delete[] table;
}

template <class T> inline T& LookUpTable<T>::operator[](size_t i)
{
	return table[i];
}

template <class T> inline void LookUpTable<T>::setIncrement(size_t i)
{
	increment=i;
}

template <class T> inline T LookUpTable<T>::nextValue(void)
{
	next+=increment;
	if(next>table_size) {
		next%=table_size;
	}
	return table[next];
}

template <class T> inline size_t LookUpTable<T>::size(void)
{
	return table_size;
}

template <class T> inline void LookUpTable<T>::reset(void)
{
	next=0;
}

#endif /* CMDFAXLOOKUPTABLE_H */
