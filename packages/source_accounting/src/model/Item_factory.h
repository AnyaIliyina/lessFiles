/*!

	\file
	\brief Фабрика элементов
	\author Чернышев Д.Р.
	\date Апрель 2016

*/

#pragma once

#include <QVariant>

class BaseItem;

class ItemFactory
{
public:
	static BaseItem* loadFromDb(int type, QVariant id = QVariant());

	static BaseItem* createNew(int type);
};