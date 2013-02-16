#include "StdAfx.h"
#include "MonoFlowNode.h"

#include "MonoScriptSystem.h"
#include "FlowManager.h"

#include "MonoEntity.h"

#include <IGameFramework.h>

#include <MonoCommon.h>
#include <IMonoObject.h>
#include <IMonoArray.h>
#include <IMonoClass.h>

CFlowNode::CFlowNode(SActivationInfo *pActInfo)
	: m_pScript(nullptr)
	, m_pActInfo(pActInfo)
	, m_cloneType(eNCT_Instanced)
	, m_flags(0)
{
	// We *have* to get the id right away or inputs won't work, so lets use this fugly solution.
	pActInfo->pGraph->RegisterHook(this);
	// Keep it so we can unregister it after we've got what we came for.
	m_pHookedGraph = pActInfo->pGraph;
}

CFlowNode::~CFlowNode()
{
	SAFE_RELEASE(m_pScript);
}

bool CFlowNode::CreatedNode(TFlowNodeId id, const char *name, TFlowNodeTypeId typeId, IFlowNodePtr pNode) 
{ 
	if(pNode==this)
	{
		const char *typeName = gEnv->pFlowSystem->GetTypeName(typeId);

		IMonoObject *pScript = g_pScriptSystem->InstantiateScript(gEnv->pFlowSystem->GetTypeName(typeId), eScriptFlag_FlowNode);

		IMonoClass *pNodeInfo = g_pScriptSystem->GetCryBraryAssembly()->GetClass("NodeInfo", "CryEngine.FlowSystem.Native");
		
		IMonoObject *pResult = pScript->CallMethod("InternalInitialize", pNodeInfo->BoxObject(&SMonoNodeInfo(this, id, m_pActInfo->pGraph->GetGraphId())));
		
		m_pScript = pScript;
		if(pResult)
			return pResult->Unbox<bool>();

		CryLogAlways("Failed to create node %s", gEnv->pFlowSystem->GetTypeName(typeId));
		return false;
	}

	return true; 
}

IFlowNodePtr CFlowNode::Clone(SActivationInfo *pActInfo)
{
	switch(m_cloneType)
	{
	case eNCT_Singleton:
		return this;
	case eNCT_Instanced:
		return new CFlowNode(pActInfo);
	default:
		break;
	}

	return nullptr;
}

IEntity *CFlowNode::GetTargetEntity()
{
	return m_pActInfo->pEntity;
}

void CFlowNode::ProcessEvent(EFlowEvent event, SActivationInfo *pActInfo)
{	
	m_pActInfo = pActInfo;

	if(m_pHookedGraph && m_pScript != nullptr)
	{
		m_pHookedGraph->UnregisterHook(this);
		m_pHookedGraph = nullptr;
	}

	switch(event)
	{
	case eFE_Activate:
		{
			IFlowNodeData *pNodeData = pActInfo->pGraph->GetNodeData(pActInfo->myID);
			if(!pNodeData)
				return;

			int numInputPorts = pNodeData->GetNumInputPorts();
			if(m_flags & EFLN_TARGET_ENTITY)
				numInputPorts--; // last input is the entity port.

			for(int i = 0; i < numInputPorts; i++)
			{
				if(IsPortActive(i))
				{
					switch(GetPortType(pActInfo, i))
					{
					case eFDT_Void:
						m_pScript->CallMethod("OnPortActivated", i);
						break;
					case eFDT_Int:
						m_pScript->CallMethod("OnPortActivated", i, CFlowBaseNodeInternal::GetPortInt(pActInfo, i));
						break;
					case eFDT_Float:
						m_pScript->CallMethod("OnPortActivated", i, CFlowBaseNodeInternal::GetPortFloat(pActInfo, i));
						break;
					case eFDT_EntityId:
						m_pScript->CallMethod("OnPortActivated", i, CFlowBaseNodeInternal::GetPortEntityId(pActInfo, i));
						break;
					case eFDT_Vec3:
						m_pScript->CallMethod("OnPortActivated", i, CFlowBaseNodeInternal::GetPortVec3(pActInfo, i));
						break;
					case eFDT_String:
						m_pScript->CallMethod("OnPortActivated", i, CFlowBaseNodeInternal::GetPortString(pActInfo, i));
						break;
					case eFDT_Bool:
						m_pScript->CallMethod("OnPortActivated", i, CFlowBaseNodeInternal::GetPortBool(pActInfo, i));
						break;
					default:
						break;
					}
				}
			}

		}
		break;
	case eFE_Initialize:
		m_pScript->CallMethod("OnInit");
		break;
	case eFE_SetEntityId:
		{
			if(pActInfo->pEntity)
			{
				IMonoArray *pParams = CreateMonoArray(2);
				pParams->InsertNativePointer(pActInfo->pEntity);
				pParams->Insert(pActInfo->pEntity->GetId());

				m_pScript->GetClass()->InvokeArray(m_pScript, "InternalSetTargetEntity", pParams);
			}
		}
		break;
	}
}

void CFlowNode::GetConfiguration(SFlowNodeConfig &config)
{
	CRY_ASSERT(m_pScript);

	if(IMonoObject *pResult = m_pScript->CallMethod("GetNodeConfig"))
	{
		SMonoNodeConfig monoConfig = pResult->Unbox<SMonoNodeConfig>();

		config.nFlags |= monoConfig.flags;
		config.sDescription = _HELP(ToCryString(monoConfig.description));
		config.SetCategory(monoConfig.category);

		m_flags = config.nFlags;

		m_cloneType = monoConfig.cloneType;

		// Ports
		IMonoArray *pInputPorts = *monoConfig.inputs;
		int numInputs = pInputPorts->GetSize();

		auto pInputs = new SInputPortConfig[numInputs + 1];

		for(int i = 0; i < numInputs; i++)
			pInputs[i] = pInputPorts->GetItem(i)->Unbox<SMonoInputPortConfig>().Convert();

		pInputs[numInputs] = InputPortConfig_Null();

		config.pInputPorts = pInputs;

		SAFE_RELEASE(pInputPorts);

		IMonoArray *pOutputPorts = *monoConfig.outputs;
		int numOutputs = pOutputPorts->GetSize();

		auto pOutputs = new SOutputPortConfig[numOutputs + 1];

		for(int i = 0; i < numOutputs; i++)
			pOutputs[i] = pOutputPorts->GetItem(i)->Unbox<SMonoOutputPortConfig>().Convert();

		pOutputs[numOutputs] = OutputPortConfig_Null();

		config.pOutputPorts = pOutputs;

		SAFE_RELEASE(pOutputPorts);
	}
}