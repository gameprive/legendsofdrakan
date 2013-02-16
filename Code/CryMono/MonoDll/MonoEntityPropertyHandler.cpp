#include "StdAfx.h"
#include "MonoEntityPropertyHandler.h"

#include "MonoEntity.h"

#include <MonoCommon.h>
#include <IMonoScriptSystem.h>
#include <IMonoObject.h>

#include <IEntitySystem.h>

CEntityPropertyHandler::CEntityPropertyHandler(std::vector<IEntityPropertyHandler::SPropertyInfo> properties)
	: m_properties(properties)
{
}

int CEntityPropertyHandler::GetPropertyCount() const
{
	return m_properties.size();
}

bool CEntityPropertyHandler::GetPropertyInfo(int index, SPropertyInfo& info ) const
{
	info = m_properties.at(index);
	return true;
}

SQueuedProperty *CEntityPropertyHandler::GetQueuedProperties(EntityId id, int &numProperties)
{
	for each(auto pair in m_queuedProperties)
	{
		if(pair.first == id)
		{
			numProperties = pair.second.size();
			SQueuedProperty *pProperties = new SQueuedProperty[numProperties];

			int i = 0;
			for(auto it = pair.second.begin(); it != pair.second.end(); ++it)
			{
				pProperties[i] = *it;

				i++;
			}

			return pProperties;
		}
	}

	return nullptr;
}

void CEntityPropertyHandler::LoadEntityXMLProperties(IEntity *pEntity, const XmlNodeRef& xml)
{
	if(auto properties = xml->findChild("Properties"))
	{
		for(int i = 0; i < properties->getNumAttributes(); i++)
		{
			const char *name;
			const char *value;

			properties->getAttributeByIndex(i, &name, &value);

			int index = 0;
			bool exists = false;

			for(; index < GetPropertyCount(); index++)
			{
				SPropertyInfo info;
				GetPropertyInfo(index, info);

				if(!strcmp(info.name, name))
				{
					exists = true;
					break;
				}
			}

			if(exists)
				SetProperty(pEntity, index, value);
			else
				MonoWarning("Could not set property %s because it did not exist", name);
		}
	}
}

void CEntityPropertyHandler::SetProperty(IEntity *pIEntity, int index, const char *value)
{
	EntityId id = pIEntity->GetId();

	CMonoEntityExtension *pEntity = nullptr;
	if(IGameObject *pGameObject = gEnv->pGameFramework->GetGameObject(id))
		pEntity = static_cast<CMonoEntityExtension *>(pGameObject->QueryExtension(pIEntity->GetClass()->GetName()));

	// Only true after game has started, limiting this to changes made in Editor.
	if(pEntity && pEntity->IsInitialized())
		pEntity->SetPropertyValue(m_properties.at(index), value);
	else
	{
		bool exists = false;
		for(auto it = m_queuedProperties.begin(); it != m_queuedProperties.end(); ++it)
		{
			if((*it).first == id)
			{
				(*it).second.push_back(SQueuedProperty(m_properties.at(index), value));

				exists = true;
				break;
			}
		}
		if(!exists)
		{
			DynArray<SQueuedProperty> queuedPropertiesForEntity;
			queuedPropertiesForEntity.push_back(SQueuedProperty(m_properties.at(index), value));

			m_queuedProperties.insert(TQueuedPropertyMap::value_type(id, queuedPropertiesForEntity));
		}
	}
}

const char *CEntityPropertyHandler::GetProperty(IEntity *pIEntity, int index) const
{
	if(IGameObject *pGameObject = gEnv->pGameFramework->GetGameObject(pIEntity->GetId()))
	{
		if(CMonoEntityExtension *pEntity = static_cast<CMonoEntityExtension *>(pGameObject->QueryExtension(pIEntity->GetClass()->GetName())))
		{
			if(IMonoObject *pResult = pEntity->GetScript()->CallMethod("GetPropertyValue", m_properties.at(index).name))
				return ToCryString((mono::string)pResult->GetManagedObject());
		}
	}

	return "";
}