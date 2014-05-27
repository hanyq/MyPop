#include "CellMgr.h"
#include <time.h>
#include "../../controller/Controllers.h"
#include "../CountDown.h"

CellMgr::CellMgr()
:_cells(nullptr)
, _elements(nullptr)
, _sizeX(0)
, _sizeY(0)
, _tagCount(0)
{
	srand((int)time(NULL));
}

CellMgr::~CellMgr()
{
	if (_cells)
	{
		for (int i = 0; i < _sizeX * _sizeY; i++)
		{
			CC_SAFE_RELEASE_NULL(_cells[i]);
		}
		CC_SAFE_DELETE(_cells);
	}
	if (_elements)
	{
		for (int i = 0; i < _sizeX * _sizeY; i++)
		{
			CC_SAFE_RELEASE_NULL(_elements[i]);
		}
		CC_SAFE_DELETE(_elements);
	}
}

void CellMgr::init(int sizeX, int sizeY)
{
	_sizeX = sizeX;
	_sizeY = sizeY;

	_tagCount = 5;//��ʼ��5�����͵�Ԫ��

	_cells = new Cell *[sizeX * sizeY];
	for (int i = 0; i < sizeX * sizeY; i++)
	{
		_cells[i] = Cell::create();
		_cells[i]->retain();
	}

	for (int x = 0; x < sizeX; x++)
	{
		for (int y = 0; y < sizeY; y++)
		{
			get(x, y)->initWith(x, y);
		}
	}

	_elements = new Element *[sizeX * sizeY];
	for (int i = 0; i < sizeX * sizeY; i++)
	{
		_elements[i] = nullptr;//����ÿ�����ӵ�Ԫ��ΪNULL
	}
}

Cell *CellMgr::get(int x, int y)
{
	CCASSERT(x >= 0 && x < _sizeX, "cell out of bound x.");
	CCASSERT(y >= 0 && y < _sizeY, "cell out of bound y.");

	return _cells[x + y * _sizeX];
}

Element *CellMgr::getElement(int x, int y)
{
	CCASSERT(x >= 0 && x < _sizeX, "cell out of bound x.");
	CCASSERT(y >= 0 && y < _sizeY, "cell out of bound y.");

	return _elements[x + y * _sizeX];
}

void CellMgr::setElement(int x, int y, Element *element)
{
	CCASSERT(x >= 0 && x < _sizeX, "cell out of bound x.");
	CCASSERT(y >= 0 && y < _sizeY, "cell out of bound y.");

	CC_SAFE_RETAIN(element);
	CC_SAFE_RELEASE(_elements[x + y * _sizeX]);

	_elements[x + y * _sizeX] = element;
}

int CellMgr::randTag()
{
	return rand() % _tagCount + 1;
}

void CellMgr::start()
{
	for (int x = 0; x < _sizeX; x++)
	{
		for (int y = 0; y < _sizeY; y++)
		{
			Element *element = Element::create();
			element->setX(x);
			element->setY(y);

			element->setTag(randTag());

			setElement(x, y, element);
		}
	}

	//�׳���Ϸ��ʼ�¼�
	Controllers::getGameController()->start();
}


bool CellMgr::mark()
{
	bool canErase = false;
	for (int x = 0; x < _sizeX; x++)
	{
		Element *preY = nullptr;
		for (int y = 0; y < _sizeY; y++)
		{
			Element *element = getElement(x, y);
			if(element)
			{
				element->setCountX(1);
				element->setCountY(1);

				if(x > 0)
				{
					//���X�᷽����ͬTagԪ�ظ���
					Element *preX = getElement(x - 1, y);
					if(preX && preX->getTag() == element->getTag())
					{
						element->setCountX(preX->getCountX() + 1);

						if(element->getCountX() >= 3)
						{
							markEraseX(x, y, element->getCountX());

							canErase = true;
						}
					}
				}

				//���Y�᷽����ͬTagԪ�ظ���
				if(preY && preY->getTag() == element->getTag()){
					element->setCountY(preY->getCountY() + 1);

					if(element->getCountY() >= 3)
					{
						markEraseY(x, y, element->getCountY());

						canErase = true;
					}
				}
			}

			preY = element;
		}
	}

	return canErase;
}

void CellMgr::markEraseX(int x, int y, int countX)
{
	for(int i = 0; i < countX; i++)
	{
		Element *element = getElement(x - i, y);
		if(!element->getEraseX()){
			element->setEraseX(true);
		}
		else
		{
			break;
		}
	}
}

void CellMgr::markEraseY(int x, int y, int countY)
{
	for(int i = 0; i < countY; i++)
	{
		Element *element = getElement(x, y - i);
		if(!element->getEraseY()){
			element->setEraseY(true);
		}
		else
		{
			break;
		}
	}
}

void CellMgr::erase()
{
	//��ʼ����
	Controllers::getGameController()->beginErase();

	for (int x = 0; x < _sizeX; x++)
	{
		for (int y = 0; y < _sizeY; y++)
		{
			Element *element = getElement(x, y);
			if(element)
			{
				if(element->getEraseX() || element->getEraseY()){
					//������ǰλ�õ�Ԫ��
					Controllers::getGameController()->erase(x, y, element->getEraseX(), element->getEraseY());

					setElement(x, y, nullptr);
				}
			}
		}
	}
}

void CellMgr::drop()
{
	//��ʼ����
	Controllers::getGameController()->beginDrop();
	for (int x = 0; x < _sizeX; x++)
	{
		int upY = 0;
		for (int y = 0; y < _sizeY; y++)
		{
			Element *element = getElement(x, y);
			if(!element)
			{
				Element *upElement = nullptr;
				if(upY <= y){
					upY = y + 1;
				}
				for(; upY < _sizeY; upY++)
				{
					upElement = getElement(x, y);
					if(upElement)
					{
						break;
					}
				}
				if(upElement){
					//�����ƶ�Ԫ��
					int upX = upElement->getX();
					int upY = upElement->getY();
					upElement->setX(x);
					upElement->setY(y);

					setElement(x, y, upElement);
					setElement(upX, upY, nullptr);

					Controllers::getGameController()->drop(upX, upY, x, y);
				}
				else
				{
					//����Ԫ��
					Element *newElement = Element::create();
					newElement->setX(x);
					newElement->setY(y);

					newElement->setTag(randTag());

					setElement(x, y, newElement);

					Controllers::getGameController()->drop(x, _sizeY, x, y);
				}
			}
		}
	}
}