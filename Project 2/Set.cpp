// Set.cpp

#include "Set.h"

Set::Set()
: m_size(0)
{}

bool Set::insert(const ItemType& value)
{
   if (m_size == DEFAULT_MAX_ITEMS)
       return false;
   int pos = findFirstAtLeast(value);
   if (pos < m_size  &&  m_data[pos] == value)
       return false;
   for (int k = m_size; k > pos; k--)
       m_data[k] = m_data[k-1];
   m_data[pos] = value;
   m_size++;
   return true;
}

bool Set::erase(const ItemType& value)
{
   int pos = findFirstAtLeast(value);
   if (pos == m_size  ||  m_data[pos] != value)
       return false;
   for ( ; pos < m_size - 1; pos++)
       m_data[pos] = m_data[pos+1];
   m_size--;
   return true;
}
    
bool Set::get(int i, ItemType& value) const
{
   if (i < 0  ||  i >= m_size)
       return false;
   value = m_data[i];
   return true;
}

void Set::swap(Set& other)
{
     // Swap elements.  Since the only elements that matter are those up to
     // m_size and other.m_size, only they have to be moved.

   int minSize = (m_size < other.m_size ? m_size : other.m_size);
   for (int k = 0; k < minSize; k++)
   {
       ItemType tempItem = m_data[k];
       m_data[k] = other.m_data[k];
       other.m_data[k] = tempItem;
   }

     // If the sizes are different, assign the remaining elements from the
     // longer one to the shorter.

   if (m_size > minSize)
       for (int k = minSize; k < m_size; k++)
           other.m_data[k] = m_data[k];
   else if (other.m_size > minSize)
       for (int k = minSize; k < other.m_size; k++)
           m_data[k] = other.m_data[k];

     // Swap sizes

   int tempSize = m_size;
   m_size = other.m_size;
   other.m_size = tempSize;
}

int Set::findFirstAtLeast(const ItemType& value) const
{
   int begin = 0;
   int end = m_size;
   while (begin < end)
   {
       int mid = (begin + end) / 2;
       if (value < m_data[mid])
           end = mid;
       else if (m_data[mid] < value)
           begin = mid + 1;
       else
           return mid;
   }
   return begin;
}
