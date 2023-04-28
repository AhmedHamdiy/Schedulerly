#pragma once
#include"List.h"
#include"../../System Classes/Process.h"
class PList : public List<Process*>
{
public:
	int search_by_ID(int id)
	{
		if (!Head)
			return 0;
				Node<Process*>* tmp = Head;
				int Pos = 1;
				while (tmp)
				{
					if (tmp->getItem()->getID() == id)
					{
						return Pos;
					}
					tmp = tmp->getNext();
					Pos++;
				}
				return 0;
	}
};