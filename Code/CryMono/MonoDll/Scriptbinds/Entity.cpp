#include "StdAfx.h"
#include "Scriptbinds\Entity.h"

#include "MonoEntity.h"
#include "NativeEntity.h"
#include "MonoEntityClass.h"

#include "MonoScriptSystem.h"

#include "MonoObject.h"
#include "MonoArray.h"
#include "MonoClass.h"

#include "MonoCVars.h"

#include <IEntityClass.h>

#include <IGameObjectSystem.h>
#include <IGameObject.h>
#include <IGameFramework.h>

std::vector<const char *> CScriptbind_Entity::m_monoEntityClasses = std::vector<const char *>();

CScriptbind_Entity::CScriptbind_Entity()
{
	REGISTER_METHOD(SpawnEntity);
	REGISTER_METHOD(RemoveEntity);

	REGISTER_METHOD(RegisterEntityClass);

	REGISTER_METHOD(GetEntity);
	REGISTER_METHOD(GetEntityId);
	REGISTER_METHOD(FindEntity);
	REGISTER_METHOD(GetEntitiesByClass);
	REGISTER_METHOD(GetEntitiesInBox);

	REGISTER_METHOD(QueryProximity);

	REGISTER_METHOD(SetPos);
	REGISTER_METHOD(GetPos);
	REGISTER_METHOD(SetWorldPos);
	REGISTER_METHOD(GetWorldPos);

	REGISTER_METHOD(SetRotation);
	REGISTER_METHOD(GetRotation);
	REGISTER_METHOD(SetWorldRotation);
	REGISTER_METHOD(GetWorldRotation);

	REGISTER_METHOD(LoadObject);
	REGISTER_METHOD(LoadCharacter);

	REGISTER_METHOD(GetBoundingBox);
	REGISTER_METHOD(GetWorldBoundingBox);

	REGISTER_METHOD(GetSlotFlags);
	REGISTER_METHOD(SetSlotFlags);

	REGISTER_METHOD(BreakIntoPieces);

	REGISTER_METHOD(GetStaticObjectFilePath);

	REGISTER_METHOD(SetWorldTM);
	REGISTER_METHOD(GetWorldTM);
	REGISTER_METHOD(SetLocalTM);
	REGISTER_METHOD(GetLocalTM);

	REGISTER_METHOD(GetName);
	REGISTER_METHOD(SetName);

	REGISTER_METHOD(GetFlags);
	REGISTER_METHOD(SetFlags);

	REGISTER_METHOD(SetVisionParams);
	REGISTER_METHOD(SetHUDSilhouettesParams);

	REGISTER_METHOD(PlayAnimation);
	REGISTER_METHOD(StopAnimationInLayer);
	REGISTER_METHOD(StopAnimationsInAllLayers);

	REGISTER_METHOD(AddEntityLink);
	REGISTER_METHOD(GetEntityLinks);
	REGISTER_METHOD(RemoveAllEntityLinks);
	REGISTER_METHOD(RemoveEntityLink);

	REGISTER_METHOD(GetEntityLinkName);
	REGISTER_METHOD(GetEntityLinkTarget);
	REGISTER_METHOD(GetEntityLinkRelativeRotation);
	REGISTER_METHOD(GetEntityLinkRelativePosition);
	REGISTER_METHOD(SetEntityLinkTarget);
	REGISTER_METHOD(SetEntityLinkRelativeRotation);
	REGISTER_METHOD(SetEntityLinkRelativePosition);
	REGISTER_METHOD(LoadLight);

	REGISTER_METHOD(FreeSlot);

	REGISTER_METHOD(AddMovement);

	// Attachments
	REGISTER_METHOD(GetAttachmentCount);
	REGISTER_METHOD(GetAttachmentByIndex);
	REGISTER_METHOD(GetAttachmentByName);

	REGISTER_METHOD(AttachmentUseEntityPosition);
	REGISTER_METHOD(AttachmentUseEntityRotation);

	REGISTER_METHOD(LinkEntityToAttachment);
	REGISTER_METHOD(GetAttachmentObject);

	REGISTER_METHOD(GetAttachmentAbsolute);
	REGISTER_METHOD(GetAttachmentRelative);
	REGISTER_METHOD(GetAttachmentDefaultAbsolute);
	REGISTER_METHOD(GetAttachmentDefaultRelative);

	REGISTER_METHOD(GetAttachmentMaterial);
	REGISTER_METHOD(SetAttachmentMaterial);
	// ~Attachment

	REGISTER_METHOD(GetJointAbsolute);
	REGISTER_METHOD(GetJointAbsoluteDefault);
	REGISTER_METHOD(GetJointRelative);
	REGISTER_METHOD(GetJointRelativeDefault);

	REGISTER_METHOD(SetJointAbsolute);

	REGISTER_METHOD(SetTriggerBBox);
	REGISTER_METHOD(GetTriggerBBox);
	REGISTER_METHOD(InvalidateTrigger);

	REGISTER_METHOD(AcquireAnimatedCharacter);

	REGISTER_METHOD(Hide);
	REGISTER_METHOD(IsHidden);

	REGISTER_METHOD(GetEntityFromPhysics);

	REGISTER_METHOD(SetUpdatePolicy);
	REGISTER_METHOD(GetUpdatePolicy);

	REGISTER_METHOD(IsPrePhysicsUpdateActive);
	REGISTER_METHOD(EnablePrePhysicsUpdate);

	REGISTER_METHOD(LoadParticleEmitter);

	REGISTER_METHOD(RemoteInvocation);

	//RegisterNativeEntityClass();

	gEnv->pEntitySystem->AddSink(this, IEntitySystem::OnSpawn | IEntitySystem::OnRemove, 0);
}

CScriptbind_Entity::~CScriptbind_Entity()
{
	if(gEnv->pEntitySystem)
		gEnv->pEntitySystem->RemoveSink(this);
	else
		MonoWarning("Failed to unregister CScriptbind_Entity entity sink!");
}

void CScriptbind_Entity::PlayAnimation(IEntity *pEntity, mono::string animationName, int slot, int layer, float blend, float speed, EAnimationFlags flags)
{
	// Animation graph input
	/*if(IGameObject *pGameObject = gEnv->pGameFramework->GetGameObject(pEntity->GetId()))
	{
		if(IAnimatedCharacter *pAniamtedCharacter = static_cast<IAnimatedCharacter*>(pGameObject->AcquireExtension("AnimatedCharacter")))	
		{
			pAniamtedCharacter->GetAnimationGraphState()->SetInput("Action / "Signal"
		}
	}*/

	ICharacterInstance *pCharacter = pEntity->GetCharacter(slot);
	if(!pCharacter)
		return;

	ISkeletonAnim *pSkeletonAnim = pCharacter->GetISkeletonAnim();
	if(!pSkeletonAnim)
		return;

	if(flags & EAnimFlag_CleanBending)
	{
		while(pSkeletonAnim->GetNumAnimsInFIFO(layer)>1)
		{
			if (!pSkeletonAnim->RemoveAnimFromFIFO(layer, pSkeletonAnim->GetNumAnimsInFIFO(layer)-1))
				break;
		}
	}

	if (flags & EAnimFlag_NoBlend)
		blend = 0.0f;

	CryCharAnimationParams params;
	params.m_fTransTime = blend;
	params.m_nLayerID = layer;
	params.m_fPlaybackSpeed = speed;
	params.m_nFlags = (flags & EAnimFlag_Loop ? CA_LOOP_ANIMATION : 0) | (flags & EAnimFlag_RestartAnimation ? CA_ALLOW_ANIM_RESTART : 0) | (flags & EAnimFlag_RepeatLastFrame ? CA_REPEAT_LAST_KEY : 0);
	pSkeletonAnim->StartAnimation(ToCryString(animationName),  params);
}

void CScriptbind_Entity::StopAnimationInLayer(IEntity *pEntity, int slot, int layer, float blendOutTime)
{
	ICharacterInstance *pCharacter = pEntity->GetCharacter(slot);
	if(!pCharacter)
		return;

	ISkeletonAnim *pSkeletonAnim = pCharacter->GetISkeletonAnim();
	if(!pSkeletonAnim)
		return;

	pSkeletonAnim->StopAnimationInLayer(layer, blendOutTime);
}

void CScriptbind_Entity::StopAnimationsInAllLayers(IEntity *pEntity, int slot)
{
	ICharacterInstance *pCharacter = pEntity->GetCharacter(slot);
	if(!pCharacter)
		return;

	ISkeletonAnim *pSkeletonAnim = pCharacter->GetISkeletonAnim();
	if(!pSkeletonAnim)
		return;

	pSkeletonAnim->StopAnimationsAllLayers();
}

bool CScriptbind_Entity::IsMonoEntity(const char *className)
{
	for each(auto entityClass in m_monoEntityClasses)
	{
		if(!strcmp(entityClass, className))
			return true;
	}

	return false;
}

void CScriptbind_Entity::OnSpawn(IEntity *pEntity,SEntitySpawnParams &params)
{
	const char *className = params.pClass->GetName();
	if(!IsMonoEntity(className))// && strcmp(className, "[NativeEntity]"))
		return;

	auto gameObject = gEnv->pGameFramework->GetIGameObjectSystem()->CreateGameObjectForEntity(pEntity->GetId());
	gameObject->ActivateExtension(className);
}

bool CScriptbind_Entity::OnRemove(IEntity *pIEntity)
{
	if(IMonoClass *pEntityClass = g_pScriptSystem->GetCryBraryAssembly()->GetClass("Entity"))
	{
		IMonoArray *pArgs = CreateMonoArray(1);
		pArgs->Insert(pIEntity->GetId());

		auto result = pEntityClass->InvokeArray(NULL, "InternalRemove", pArgs)->Unbox<bool>();

		SAFE_RELEASE(pArgs);

		if(!result)
			return false;
	}

	return true;
}

struct SMonoEntityCreator
	: public IGameObjectExtensionCreatorBase
{
	virtual IGameObjectExtension *Create() { return new CMonoEntityExtension(); }
	virtual void GetGameObjectExtensionRMIData(void **ppRMI, size_t *nCount) { return CMonoEntityExtension::GetGameObjectExtensionRMIData(ppRMI, nCount); }
};

bool CScriptbind_Entity::RegisterEntityClass(SEntityRegistrationParams params)
{
	const char *className = ToCryString(params.Name);
	if(gEnv->pEntitySystem->GetClassRegistry()->FindClass(className))
	{
		MonoWarning("Aborting registration of entity class %s, a class with the same name already exists", className); 
		return false;
	}

	std::vector<IEntityPropertyHandler::SPropertyInfo> properties;
	if(params.Folders != nullptr)
	{
		IMonoArray *pFolderArray = *params.Folders;

		int numFolders = pFolderArray->GetSize();
	
		for	(int iFolder = 0; iFolder < numFolders; ++iFolder)
		{
			IMonoObject *pFolderObject = pFolderArray->GetItem(iFolder);
			if(!pFolderObject)
				continue;

			auto folder = pFolderObject->Unbox<SMonoEntityPropertyFolder>();
			if(folder.properties == nullptr)
				continue;

			bool bDefaultFolder = !strcmp(ToCryString(folder.name), "Default") && iFolder == 0;

			if(!bDefaultFolder) // first element contains properties not organized into folders
			{
				IEntityPropertyHandler::SPropertyInfo folderInfo;
				folderInfo.name = ToCryString(folder.name);
				folderInfo.type = IEntityPropertyHandler::FolderBegin;

				properties.push_back(folderInfo);
			}

			IMonoArray *pPropertyArray = *folder.properties;

			for(int iProperty = 0; iProperty < pPropertyArray->GetSize(); iProperty++)
			{
				IMonoObject *pPropertyObject = pPropertyArray->GetItem(iProperty);
				if(pPropertyObject == nullptr)
					continue;

				auto property = pPropertyObject->Unbox<SMonoEntityProperty>();

				IEntityPropertyHandler::SPropertyInfo propertyInfo;

				propertyInfo.name = ToCryString(property.name);
				propertyInfo.description = ToCryString(property.description);
				propertyInfo.editType = ToCryString(property.editType);
				propertyInfo.type = property.type;
				propertyInfo.limits.min = property.limits.min;
				propertyInfo.limits.max = property.limits.max;

				properties.push_back(propertyInfo);
			}

			if(!bDefaultFolder)
			{
				IEntityPropertyHandler::SPropertyInfo folderInfo;
				folderInfo.name = ToCryString(folder.name);
				folderInfo.type = IEntityPropertyHandler::FolderEnd;

				properties.push_back(folderInfo);
			}
		}
	}

	IEntityClassRegistry::SEntityClassDesc entityClassDesc;	
	entityClassDesc.flags = params.Flags;
	entityClassDesc.sName = className;
	entityClassDesc.editorClassInfo.sCategory = ToCryString(params.Category);

	if(params.EditorHelper != nullptr)
		entityClassDesc.editorClassInfo.sHelper = ToCryString(params.EditorHelper);
	if(params.EditorIcon != nullptr)
		entityClassDesc.editorClassInfo.sIcon = ToCryString(params.EditorIcon);

	m_monoEntityClasses.push_back(className);

	bool result = gEnv->pEntitySystem->GetClassRegistry()->RegisterClass(new CEntityClass(entityClassDesc, properties));

	static SMonoEntityCreator creator;
	gEnv->pGameFramework->GetIGameObjectSystem()->RegisterExtension(className, &creator, nullptr);

	return result;
}

struct SMonoNativeEntityCreator
	: public IGameObjectExtensionCreatorBase
{
	virtual IGameObjectExtension *Create() { return new CNativeEntity(); }
	virtual void GetGameObjectExtensionRMIData(void **ppRMI, size_t *nCount) { return CNativeEntity::GetGameObjectExtensionRMIData(ppRMI, nCount); }
};

void CScriptbind_Entity::RegisterNativeEntityClass()
{
	const char *className = "[NativeEntity]";

	IEntityClassRegistry::SEntityClassDesc entityClassDesc;
	entityClassDesc.flags = 0;
	entityClassDesc.sName = className;
	entityClassDesc.editorClassInfo.sCategory = "Default";

	std::vector<IEntityPropertyHandler::SPropertyInfo> properties;

	bool result = gEnv->pEntitySystem->GetClassRegistry()->RegisterClass(new CEntityClass(entityClassDesc, properties));

	static SMonoNativeEntityCreator creator;
	gEnv->pGameFramework->GetIGameObjectSystem()->RegisterExtension(className, &creator, nullptr);
}

mono::object CScriptbind_Entity::SpawnEntity(EntitySpawnParams monoParams, bool bAutoInit, SMonoEntityInfo &entityInfo)
{
	const char *className = ToCryString(monoParams.sClass);

	if(IEntityClass *pClass = gEnv->pEntitySystem->GetClassRegistry()->FindClass(className))
	{
		SEntitySpawnParams spawnParams;
		spawnParams.pClass = pClass;
		spawnParams.sName = ToCryString(monoParams.sName);

		spawnParams.nFlags = monoParams.flags | ENTITY_FLAG_NO_SAVE;
		spawnParams.vPosition = monoParams.pos;
		spawnParams.qRotation = monoParams.rot;
		spawnParams.vScale = monoParams.scale;

		if(IEntity *pEntity = gEnv->pEntitySystem->SpawnEntity(spawnParams, bAutoInit))
		{
			entityInfo.pEntity = pEntity;
			entityInfo.id = pEntity->GetId();

			if(IGameObject *pGameObject = gEnv->pGameFramework->GetGameObject(spawnParams.id))
			{
				if(CMonoEntityExtension *pEntity = static_cast<CMonoEntityExtension *>(pGameObject->QueryExtension(className)))
					return pEntity->GetScript()->GetManagedObject();
			}
		}
	}

	return nullptr;
}

void CScriptbind_Entity::RemoveEntity(EntityId id, bool removeNow)
{
	gEnv->pEntitySystem->RemoveEntity(id, removeNow);
}

IEntity *CScriptbind_Entity::GetEntity(EntityId id)
{
	return gEnv->pEntitySystem->GetEntity(id);
}

EntityId CScriptbind_Entity::GetEntityId(IEntity *pEntity)
{
	return pEntity->GetId();
}

EntityId CScriptbind_Entity::FindEntity(mono::string name)
{
	if(IEntity *pEntity = gEnv->pEntitySystem->FindEntityByName(ToCryString(name)))
		return pEntity->GetId();

	return 0;
}

mono::object CScriptbind_Entity::GetEntitiesByClass(mono::string _class)
{
	IEntityClass *pDesiredClass = gEnv->pEntitySystem->GetClassRegistry()->FindClass(ToCryString(_class));

	std::vector<EntityId> classEntities;

	IEntityItPtr pIt = gEnv->pEntitySystem->GetEntityIterator();
	pIt->MoveFirst();

	while(!pIt->IsEnd())
	{
		if(IEntity *pEntity = pIt->Next())
		{
			if(pEntity->GetClass() == pDesiredClass)
				classEntities.push_back(pEntity->GetId());
		}
	}

	if(classEntities.size()<1)
		return nullptr;

	IMonoClass *pEntityIdClass = g_pScriptSystem->GetCryBraryAssembly()->GetClass("EntityId");

	IMonoArray *pArray = CreateMonoArray(classEntities.size());

	for(auto it = classEntities.begin(); it != classEntities.end(); ++it)
		pArray->Insert(pEntityIdClass->BoxObject(&mono::entityId(*it)));

	return pArray->GetManagedObject();
}

mono::object CScriptbind_Entity::GetEntitiesInBox(AABB bbox, int objTypes)
{
	IPhysicalEntity **pEnts = nullptr;

	IMonoClass *pEntityIdClass = g_pScriptSystem->GetCryBraryAssembly()->GetClass("EntityId");

	int numEnts = gEnv->pPhysicalWorld->GetEntitiesInBox(bbox.min, bbox.max, pEnts, objTypes);
	
	if(numEnts > 0)
		{
		IMonoArray *pEntities = CreateMonoArray(numEnts);

		for(int i = 0; i < numEnts; i++)
			pEntities->Insert(pEntityIdClass->BoxObject(&mono::entityId(gEnv->pPhysicalWorld->GetPhysicalEntityId(pEnts[i]))));

		return pEntities->GetManagedObject();
	}

	return nullptr;
}

mono::object CScriptbind_Entity::QueryProximity(AABB box, mono::string className, uint32 nEntityFlags)
{
	SEntityProximityQuery query;

	if(className != nullptr)
		query.pEntityClass = gEnv->pEntitySystem->GetClassRegistry()->FindClass(ToCryString(className));

	query.box = box;
	query.nEntityFlags = nEntityFlags;

	gEnv->pEntitySystem->QueryProximity(query);

	IMonoClass *pEntityIdClass = g_pScriptSystem->GetCryBraryAssembly()->GetClass("EntityId");
	if(query.nCount > 0)
	{
		IMonoArray *pEntities = CreateMonoArray(query.nCount);
		for(int i = 0; i < query.nCount; i++)
			pEntities->Insert(pEntityIdClass->BoxObject(&mono::entityId(query.pEntities[i]->GetId())));

		return pEntities->GetManagedObject();
	}

	return nullptr;
}

void CScriptbind_Entity::SetWorldTM(IEntity *pEntity, Matrix34 tm)
{
	pEntity->SetWorldTM(tm);
}

Matrix34 CScriptbind_Entity::GetWorldTM(IEntity *pEntity)
{
	return pEntity->GetWorldTM();
}

void CScriptbind_Entity::SetLocalTM(IEntity *pEntity, Matrix34 tm)
{
	pEntity->SetLocalTM(tm);
}

Matrix34 CScriptbind_Entity::GetLocalTM(IEntity *pEntity)
{
	return pEntity->GetLocalTM();
}

AABB CScriptbind_Entity::GetWorldBoundingBox(IEntity *pEntity)
{
	AABB boundingBox;
	pEntity->GetWorldBounds(boundingBox);

	return boundingBox;
}

AABB CScriptbind_Entity::GetBoundingBox(IEntity *pEntity)
{
	AABB boundingBox;
	pEntity->GetLocalBounds(boundingBox);

	return boundingBox;
}

void CScriptbind_Entity::SetPos(IEntity *pEntity, Vec3 newPos)
{
	pEntity->SetPos(newPos);
}

Vec3 CScriptbind_Entity::GetPos(IEntity *pEntity)
{
	return pEntity->GetPos();
}

void CScriptbind_Entity::SetWorldPos(IEntity *pEntity, Vec3 newPos)
{
	pEntity->SetWorldTM(Matrix34::Create(pEntity->GetScale(), pEntity->GetWorldRotation(), newPos));
}

Vec3 CScriptbind_Entity::GetWorldPos(IEntity *pEntity)
{
	return pEntity->GetWorldPos();
}

void CScriptbind_Entity::SetRotation(IEntity *pEntity, Quat newAngles)
{
	pEntity->SetRotation(newAngles);
}

Quat CScriptbind_Entity::GetRotation(IEntity *pEntity)
{
	return pEntity->GetRotation();
}

void CScriptbind_Entity::SetWorldRotation(IEntity *pEntity, Quat newAngles)
{
	pEntity->SetWorldTM(Matrix34::Create(pEntity->GetScale(), newAngles, pEntity->GetWorldPos()));
}

Quat CScriptbind_Entity::GetWorldRotation(IEntity *pEntity)
{
	return pEntity->GetWorldRotation();
}

void CScriptbind_Entity::LoadObject(IEntity *pEntity, mono::string fileName, int slot)
{
	pEntity->LoadGeometry(slot, ToCryString(fileName));
}

void CScriptbind_Entity::LoadCharacter(IEntity *pEntity, mono::string fileName, int slot)
{
	pEntity->LoadCharacter(slot, ToCryString(fileName));
}

EEntitySlotFlags CScriptbind_Entity::GetSlotFlags(IEntity *pEntity, int slot)
{
	return (EEntitySlotFlags)pEntity->GetSlotFlags(slot);
}

void CScriptbind_Entity::SetSlotFlags(IEntity *pEntity, int slot, EEntitySlotFlags slotFlags)
{
	pEntity->SetSlotFlags(slot, slotFlags);
}

void CScriptbind_Entity::BreakIntoPieces(IEntity *pEntity, int slot, int piecesSlot, IBreakableManager::BreakageParams breakageParams)
{
	gEnv->pEntitySystem->GetBreakableManager()->BreakIntoPieces(pEntity, slot, piecesSlot, breakageParams);
}

mono::string CScriptbind_Entity::GetStaticObjectFilePath(IEntity *pEntity, int slot)
{
	if(IStatObj *pStatObj = pEntity->GetStatObj(slot))
		return ToMonoString(pStatObj->GetFilePath());
	else if(ICharacterInstance *pCharacter = pEntity->GetCharacter(0))
		return ToMonoString(pCharacter->GetFilePath());

	return ToMonoString("");
}

mono::string CScriptbind_Entity::GetName(IEntity *pEntity)
{
	return ToMonoString(pEntity->GetName());
}

void CScriptbind_Entity::SetName(IEntity *pEntity, mono::string name)
{
	pEntity->SetName(ToCryString(name));
}

EEntityFlags CScriptbind_Entity::GetFlags(IEntity *pEntity)
{
	return (EEntityFlags)pEntity->GetFlags();
}

void CScriptbind_Entity::SetFlags(IEntity *pEntity, EEntityFlags flags)
{
	pEntity->SetFlags(flags);
}

void CScriptbind_Entity::SetVisionParams(IEntity *pEntity, float r, float g, float b, float a)
{
	IEntityRenderProxy *pRenderProxy = static_cast<IEntityRenderProxy *>(pEntity->GetProxy(ENTITY_PROXY_RENDER));
	if(!pRenderProxy)
		return;

	pRenderProxy->SetVisionParams(r, g, b, a);
}

void CScriptbind_Entity::SetHUDSilhouettesParams(IEntity *pEntity, float r, float g, float b, float a)
{
	IEntityRenderProxy *pRenderProxy = static_cast<IEntityRenderProxy *>(pEntity->GetProxy(ENTITY_PROXY_RENDER));
	if(!pRenderProxy)
		return;

	pRenderProxy->SetVisionParams(r, g, b, a);
}

IEntityLink *CScriptbind_Entity::AddEntityLink(IEntity *pEntity, mono::string linkName, EntityId otherId, Quat relativeRot, Vec3 relativePos)
{
	return pEntity->AddEntityLink(ToCryString(linkName), otherId, 0, relativeRot, relativePos);
}

mono::object CScriptbind_Entity::GetEntityLinks(IEntity *pEntity)
{
	// the first link
	IEntityLink *pLink = pEntity->GetEntityLinks();

	IMonoArray *pDynArray = CreateDynamicMonoArray();
	while(pLink != nullptr)
	{
		pDynArray->InsertAny(reinterpret_cast<intptr_t>(pLink));

		pLink = pLink->next;
	}

	return pDynArray->GetManagedObject();
}

void CScriptbind_Entity::RemoveAllEntityLinks(IEntity *pEntity)
{
	pEntity->RemoveAllEntityLinks();
}

void CScriptbind_Entity::RemoveEntityLink(IEntity *pEntity, IEntityLink *pLink)
{
	pEntity->RemoveEntityLink(pLink);
}

mono::string CScriptbind_Entity::GetEntityLinkName(IEntityLink *pLink)
{
	return ToMonoString(pLink->name);
}

EntityId CScriptbind_Entity::GetEntityLinkTarget(IEntityLink *pLink)
{
	return pLink->entityId;
}

Quat CScriptbind_Entity::GetEntityLinkRelativeRotation(IEntityLink *pLink)
{
	return pLink->relRot;
}

Vec3 CScriptbind_Entity::GetEntityLinkRelativePosition(IEntityLink *pLink)
{
	return pLink->relPos;
}

void CScriptbind_Entity::SetEntityLinkTarget(IEntityLink *pLink, EntityId id)
{
	pLink->entityId = id;
}

void CScriptbind_Entity::SetEntityLinkRelativeRotation(IEntityLink *pLink, Quat relRot)
{
	pLink->relRot = relRot;
}

void CScriptbind_Entity::SetEntityLinkRelativePosition(IEntityLink *pLink, Vec3 relPos)
{
	pLink->relPos = relPos;
}

int CScriptbind_Entity::LoadLight(IEntity *pEntity, int slot, SMonoLightParams params)
{
	CDLight light;

	if(const char *spec = ToCryString(params.specularCubemap))
	{
		if(strcmp(spec, ""))
			light.SetSpecularCubemap(gEnv->pRenderer->EF_LoadTexture(spec));
	}
	if(const char *diff = ToCryString(params.diffuseCubemap))
	{
		if(strcmp(diff, ""))
			light.SetDiffuseCubemap(gEnv->pRenderer->EF_LoadTexture(diff));
	}
	if(const char *lightImage = ToCryString(params.lightImage))
	{
		if(strcmp(lightImage, ""))
			light.m_pLightImage = gEnv->pRenderer->EF_LoadTexture(lightImage);
	}
	if(const char *lightAttenMap = ToCryString(params.lightAttenMap))
	{
		if(strcmp(lightAttenMap, ""))
			light.SetLightAttenMap(gEnv->pRenderer->EF_LoadTexture(lightAttenMap));
	}

	light.SetLightColor(params.color);
	light.SetPosition(params.origin);

	light.SetShadowBiasParams(params.shadowBias, params.shadowSlopeBias);

	light.m_fRadius = params.radius;
	light.SetSpecularMult(params.specularMultiplier);

	light.m_fHDRDynamic = params.hdrDynamic;

	light.m_fAnimSpeed = params.animSpeed;
	light.m_fCoronaScale = params.coronaScale;
	light.m_fCoronaIntensity = params.coronaIntensity;
	light.m_fCoronaDistSizeFactor = params.coronaDistSizeFactor;
	light.m_fCoronaDistIntensityFactor = params.coronaDistIntensityFactor;

	light.m_fShaftSrcSize = params.shaftSrcSize;
	light.m_fShaftLength = params.shaftLength;
	light.m_fShaftBrightness = params.shaftBrightness;
	light.m_fShaftBlendFactor = params.shaftBlendFactor;
	light.m_fShaftDecayFactor = params.shaftDecayFactor;

	light.m_fLightFrustumAngle = params.lightFrustumAngle;

	light.m_fShadowUpdateMinRadius = params.shadowUpdateMinRadius;
	light.m_nShadowUpdateRatio = params.shadowUpdateRatio;


	light.m_nLightStyle = params.lightStyle;
	light.m_nLightPhase = params.lightPhase;
	light.m_nPostEffect = params.postEffect;
	light.m_ShadowChanMask = params.shadowChanMask;

	return pEntity->LoadLight(slot, &light);
}

void CScriptbind_Entity::FreeSlot(IEntity *pEntity, int slot)
{
	pEntity->FreeSlot(slot);
}

void CScriptbind_Entity::AddMovement(IAnimatedCharacter *pAnimatedCharacter, SCharacterMoveRequest &moveRequest)
{
	if(pAnimatedCharacter)
		pAnimatedCharacter->AddMovement(moveRequest);

}

////////////////////////////////////////////////////
// Attachments
////////////////////////////////////////////////////
IAttachmentManager *GetAttachmentManager(IEntity *pEntity, int slot)
{
	if(auto pCharacter = pEntity->GetCharacter(slot))
		return pCharacter->GetIAttachmentManager();

	return nullptr;
}

int CScriptbind_Entity::GetAttachmentCount(IEntity *pEnt, int slot)
{
	if(auto pAttachmentManager = GetAttachmentManager(pEnt, slot))
		return pAttachmentManager->GetAttachmentCount();

	return 0;
}

IAttachment *CScriptbind_Entity::GetAttachmentByIndex(IEntity *pEnt, int index, int slot)
{
	if(auto pAttachmentManager = GetAttachmentManager(pEnt, slot))
		return pAttachmentManager->GetInterfaceByIndex(index);

	return nullptr;
}

IAttachment *CScriptbind_Entity::GetAttachmentByName(IEntity *pEnt, mono::string name, int slot)
{
	if(auto pAttachmentManager = GetAttachmentManager(pEnt, slot))
		return pAttachmentManager->GetInterfaceByName(ToCryString(name));

	return nullptr;
}

class CMonoEntityAttachment : public CEntityAttachment
{
public:
	CMonoEntityAttachment()
		: m_bUseEntityPosition(false)
		, m_bUseEntityRotation(false)
	{
	}

	void UseEntityPosition(bool use) { m_bUseEntityPosition = use; }
	void UseEntityRotation(bool use) { m_bUseEntityRotation = use; }

	virtual void UpdateAttachment(IAttachment *pIAttachment,const QuatT &m, float fZoomAdjustedDistanceFromCamera, uint32 OnRender ) override
	{
		const QuatT& quatT = pIAttachment->GetAttWorldAbsolute();

		IEntity *pEntity = gEnv->pEntitySystem->GetEntity(GetEntityId());
		if(pEntity)
			pEntity->SetPosRotScale((m_bUseEntityPosition ? pEntity->GetPos() : quatT.t), (m_bUseEntityRotation ? pEntity->GetRotation() : quatT.q), pEntity->GetScale(), ENTITY_XFORM_NO_PROPOGATE);
	}

private:
	bool m_bUseEntityPosition;
	bool m_bUseEntityRotation;
};

void CScriptbind_Entity::AttachmentUseEntityPosition(CMonoEntityAttachment *pEntityAttachment, bool use)
{
	pEntityAttachment->UseEntityPosition(use);
}

void CScriptbind_Entity::AttachmentUseEntityRotation(CMonoEntityAttachment *pEntityAttachment, bool use)
{
	pEntityAttachment->UseEntityRotation(use);
}

CMonoEntityAttachment *CScriptbind_Entity::LinkEntityToAttachment(IAttachment *pAttachment, EntityId id)
{
	pAttachment->ClearBinding();

	CMonoEntityAttachment *pEntityAttachment = new CMonoEntityAttachment();
	pEntityAttachment->SetEntityId(id);

	pAttachment->AddBinding(pEntityAttachment);

	return pEntityAttachment;
}

mono::string CScriptbind_Entity::GetAttachmentObject(IAttachment *pAttachment)
{
	if(IAttachmentObject *pObject = pAttachment->GetIAttachmentObject())
	{
		if(ICharacterInstance *pCharacterInstance = pObject->GetICharacterInstance())
			return ToMonoString(pCharacterInstance->GetFilePath());
		else if(IStatObj *pStatObj = pObject->GetIStatObj())
			return ToMonoString(pStatObj->GetFilePath());
	}

	return nullptr;
}

QuatT CScriptbind_Entity::GetAttachmentAbsolute(IAttachment *pAttachment)
{
	return pAttachment->GetAttWorldAbsolute();
}

QuatT CScriptbind_Entity::GetAttachmentRelative(IAttachment *pAttachment)
{
	return pAttachment->GetAttModelRelative();
}
	
QuatT CScriptbind_Entity::GetAttachmentDefaultAbsolute(IAttachment *pAttachment)
{
	return pAttachment->GetAttAbsoluteDefault();
}

QuatT CScriptbind_Entity::GetAttachmentDefaultRelative(IAttachment *pAttachment)
{
	return pAttachment->GetAttRelativeDefault();
}

IMaterial *CScriptbind_Entity::GetAttachmentMaterial(IAttachment *pAttachment)
{
	if(IAttachmentObject *pObject = pAttachment->GetIAttachmentObject())
		return pObject->GetMaterial();

	return nullptr;
}

void CScriptbind_Entity::SetAttachmentMaterial(IAttachment *pAttachment, IMaterial *pMaterial)
{
	if(IAttachmentObject *pObject = pAttachment->GetIAttachmentObject())
		pObject->SetMaterial(pMaterial);
}

QuatT CScriptbind_Entity::GetJointAbsolute(IEntity *pEntity, mono::string jointName, int characterSlot)
{
	if(ICharacterInstance *pCharacter = pEntity->GetCharacter(characterSlot))
	{
		if(ISkeletonPose *pSkeletonPose = pCharacter->GetISkeletonPose())
		{
			int16 id = pSkeletonPose->GetJointIDByName(ToCryString(jointName));
			if(id > -1)
				return pSkeletonPose->GetAbsJointByID(id);
		}
	}

	return QuatT();
}

QuatT CScriptbind_Entity::GetJointAbsoluteDefault(IEntity *pEntity, mono::string jointName, int characterSlot)
{
	if(ICharacterInstance *pCharacter = pEntity->GetCharacter(characterSlot))
	{
		if(ISkeletonPose *pSkeletonPose = pCharacter->GetISkeletonPose())
		{
			int16 id = pSkeletonPose->GetJointIDByName(ToCryString(jointName));
			if(id > -1)
				return pSkeletonPose->GetDefaultAbsJointByID(id);
		}
	}

	return QuatT();
}

QuatT CScriptbind_Entity::GetJointRelative(IEntity *pEntity, mono::string jointName, int characterSlot)
{
	if(ICharacterInstance *pCharacter = pEntity->GetCharacter(characterSlot))
	{
		if(ISkeletonPose *pSkeletonPose = pCharacter->GetISkeletonPose())
		{
			int16 id = pSkeletonPose->GetJointIDByName(ToCryString(jointName));
			if(id > -1)
				return pSkeletonPose->GetRelJointByID(id);
		}
	}

	return QuatT();
}

QuatT CScriptbind_Entity::GetJointRelativeDefault(IEntity *pEntity, mono::string jointName, int characterSlot)
{
	if(ICharacterInstance *pCharacter = pEntity->GetCharacter(characterSlot))
	{
		if(ISkeletonPose *pSkeletonPose = pCharacter->GetISkeletonPose())
		{
			int16 id = pSkeletonPose->GetJointIDByName(ToCryString(jointName));
			if(id > -1)
				return pSkeletonPose->GetDefaultRelJointByID(id);
		}
	}

	return QuatT();
}

void CScriptbind_Entity::SetJointAbsolute(IEntity *pEntity, mono::string jointName, int characterSlot, QuatT absolute)
{
	if(ICharacterInstance *pCharacter = pEntity->GetCharacter(characterSlot))
	{
		if(ISkeletonPose *pSkeletonPose = pCharacter->GetISkeletonPose())
		{
			int16 id = pSkeletonPose->GetJointIDByName(ToCryString(jointName));
			if(id > -1)
				pSkeletonPose->SetAbsJointByID(id, absolute);
		}
	}
}

void CScriptbind_Entity::SetTriggerBBox(IEntity *pEntity, AABB bounds)
{
	IEntityTriggerProxy *pTriggerProxy = static_cast<IEntityTriggerProxy *>(pEntity->GetProxy(ENTITY_PROXY_TRIGGER));
	if(!pTriggerProxy)
	{
		pEntity->CreateProxy(ENTITY_PROXY_TRIGGER);
		pTriggerProxy = static_cast<IEntityTriggerProxy *>(pEntity->GetProxy(ENTITY_PROXY_TRIGGER));
	}

	if (pTriggerProxy)
		pTriggerProxy->SetTriggerBounds(bounds);
}

AABB CScriptbind_Entity::GetTriggerBBox(IEntity *pEntity)
{
	AABB bbox;
	if(IEntityTriggerProxy *pTriggerProxy = static_cast<IEntityTriggerProxy *>(pEntity->GetProxy(ENTITY_PROXY_TRIGGER)))
		pTriggerProxy->GetTriggerBounds(bbox);

	return bbox;
}

void CScriptbind_Entity::InvalidateTrigger(IEntity *pEntity)
{
	if(IEntityTriggerProxy *pTriggerProxy = static_cast<IEntityTriggerProxy *>(pEntity->GetProxy(ENTITY_PROXY_TRIGGER)))
		pTriggerProxy->InvalidateTrigger();
}

IAnimatedCharacter *CScriptbind_Entity::AcquireAnimatedCharacter(EntityId id)
{
	if(IGameObject *pGameObject = gEnv->pGameFramework->GetGameObject(id))
		return static_cast<IAnimatedCharacter *>(pGameObject->AcquireExtension("AnimatedCharacter"));

	return nullptr;
}

void CScriptbind_Entity::Hide(IEntity *pEntity, bool hide)
{
	pEntity->Hide(hide);
}

bool CScriptbind_Entity::IsHidden(IEntity *pEntity)
{
	return pEntity->IsHidden();
}

IEntity *CScriptbind_Entity::GetEntityFromPhysics(IPhysicalEntity *pPhysEnt)
{
	return gEnv->pEntitySystem->GetEntityFromPhysics(pPhysEnt);
}

void CScriptbind_Entity::SetUpdatePolicy(IEntity *pEntity, EEntityUpdatePolicy policy)
{
	pEntity->SetUpdatePolicy(policy);
}

EEntityUpdatePolicy CScriptbind_Entity::GetUpdatePolicy(IEntity *pEntity)
{
	return pEntity->GetUpdatePolicy();
}

bool CScriptbind_Entity::IsPrePhysicsUpdateActive(IEntity *pEntity)
{
	return pEntity->IsPrePhysicsActive();
}

void CScriptbind_Entity::EnablePrePhysicsUpdate(IEntity *pEntity, bool enable)
{
	pEntity->PrePhysicsActivate(enable);
}

IParticleEmitter *CScriptbind_Entity::LoadParticleEmitter(IEntity *pEntity, int slot, IParticleEffect *pEffect, SpawnParams &spawnParams)
{
	int nSlot = pEntity->LoadParticleEmitter(slot, pEffect, &spawnParams);

	return pEntity->GetParticleEmitter(nSlot);
}

void CScriptbind_Entity::RemoteInvocation(EntityId entityId, EntityId targetId, mono::string methodName, mono::object args, ERMInvocation target, int channelId)
{
	CRY_ASSERT(entityId != 0);

	IGameObject *pGameObject = gEnv->pGameFramework->GetGameObject(entityId);
	CRY_ASSERT(pGameObject);

	CMonoEntityExtension::RMIParams params(*args, ToCryString(methodName), targetId);

	if(target & eRMI_ToServer)
		pGameObject->InvokeRMI(CMonoEntityExtension::SvScriptRMI(), params, target, channelId);
	else
		pGameObject->InvokeRMI(CMonoEntityExtension::ClScriptRMI(), params, target, channelId);
}