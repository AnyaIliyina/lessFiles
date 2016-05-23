#include "item_factory.h"
#include "Resources.h"
#include "Departments.h"
#include "types.h"
#include "RegionItem.h"
#include "RegionItemChecked.h"

BaseItem* ItemFactory::loadFromDb(int type, QVariant id) {
	// Создаем верхний элемент
	BaseItem* item = createNew(type);

	switch (type)
	{
	case ItemTypes::RegionItemCheckedType:
		dynamic_cast<RegionItemChecked*>(item)->id = -1;
		break;
	default:
		break;
	}

	QList<BaseItem*> children = item->loadItemsFromDb(id);
	
	// Добавляем истинные значения верхнему элементу
	for (int i = 0; i < children.count(); i++)
		
		item->appendChild(children[i]);

	return item;
};

BaseItem* ItemFactory::createNew(int type) {
	switch (type)
	{
	case ItemTypes::ResourceType:
		return new Resources();
	case ItemTypes::DepartmentType:
		return new Departments();
	case ItemTypes::RegionItemType:
		return new RegionItem();
	case ItemTypes::RegionItemCheckedType:
		return new RegionItemChecked();
	
	default:
		return NULL;
	}
};