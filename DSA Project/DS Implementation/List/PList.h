#pragma once
#include"List.h"
#include"../../System Classes/Process.h"
class PList : public List<Process*>
{
public:
	Process* search_by_ID(int id,int &x)
	{
		if (!Head)
			return nullptr;
				Node<Process*>* tmp = Head;
				int Pos = 1;
				while (tmp)
				{
					if (tmp->getItem()->getID() == id)
					{
						x = Pos;
						return tmp->getItem();
					}
					tmp = tmp->getNext();
					Pos++;
				}
				return nullptr;
	}
};