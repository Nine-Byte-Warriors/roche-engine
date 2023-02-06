#include "stdafx.h"
#include "Agent.h"

#if _DEBUG
#include <imgui/imgui.h>
#endif

using namespace AILogic;

Agent::Agent( const std::shared_ptr<Physics>& physics ) : m_physics( physics )
{
	m_fSpeed = 5.0f;

	m_pStateMachine = new AIStateMachine(this);

	AIState* pSeekState = m_pStateMachine->NewState(AIStateTypes::Seek);
	pSeekState->SetBounds(1.0f, 0.0f);
	pSeekState->SetActivation(0.0f);
	m_mapStates.emplace(AIStateTypes::Seek, pSeekState);

	AIState* pIdleState = m_pStateMachine->NewState(AIStateTypes::Idle);
	pIdleState->SetActivation(1.0f);
	m_mapStates.emplace(AIStateTypes::Idle, pIdleState);

	AIState* pFleeState = m_pStateMachine->NewState(AIStateTypes::Flee);
	pFleeState->SetBounds(1.0f, 0.0f);
	pFleeState->SetActivation(0.0f);
	m_mapStates.emplace(AIStateTypes::Flee, pFleeState);

	PatrolParams* patrolParams = new PatrolParams();
	patrolParams->fDistanceToWaypoint = 200.0f;
	patrolParams->fSensingRange = 10.0f;
	patrolParams->iWaypointCount = 6;
	patrolParams->ePatrolType = PatrolType::Loop;

	AIState* pPatrolState = m_pStateMachine->NewState(AIStateTypes::Patrol);
	pPatrolState->SetBounds(1.0f, 0.0f);
	pPatrolState->SetActivation(0.0f);
	pPatrolState->SetParams((void*)patrolParams);
	m_mapStates.emplace(AIStateTypes::Patrol, pPatrolState);

	FollowParams* followParams = new FollowParams();
	followParams->bKeepRange = true;
	followParams->fFollowRange = 200.0f;
	followParams->fRepulseRange = 100.0f;

	AIState* pFollowState = m_pStateMachine->NewState(AIStateTypes::Follow);
	pFollowState->SetBounds(1.0f, 0.0f);
	pFollowState->SetActivation(0.0f);
	pFollowState->SetParams(followParams);
	m_mapStates.emplace(AIStateTypes::Follow, pFollowState);

	WanderParams* pWanderParams = new WanderParams();
	pWanderParams->fWanderAngle = 5.0f;
	pWanderParams->fWanderDelay = 0.5f;

	AIState* pWanderState = m_pStateMachine->NewState(AIStateTypes::Wander);
	pWanderState->SetBounds(1.0f, 0.0f);
	pWanderState->SetActivation(0.0f);
	pWanderState->SetParams(pWanderParams);
	m_mapStates.emplace(AIStateTypes::Wander, pWanderState);

	AIState* pFireState = m_pStateMachine->NewState(AIStateTypes::Fire);
	pFireState->SetBounds(1.0f, 0.0f);
	pFireState->SetActivation(0.0f);
	m_mapStates.emplace(AIStateTypes::Fire, pFireState);
	
	AddToEvent();
}

void Agent::Update(const float dt)
{
	m_pStateMachine->Clear();
	for (auto const& [key, value] : m_mapStates)
		m_pStateMachine->AddState(value);
	m_pStateMachine->UpdateMachine(dt);
}

#if _DEBUG
void Agent::SpawnControlWindow(Vector2f fGO, Vector2f fTarg) noexcept
{
	if (ImGui::Begin("Agent AI", FALSE, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("Behaviour");
		static int activeBehaviour = 0;
		static std::string previewValueBehaviour = "Idle";
		static const char* behaviourList[]{ "Idle", "Seek", "Flee", "Patrol", "Follow", "Wander", "Fire" };
		if (ImGui::BeginCombo("##Active Behaviour", previewValueBehaviour.c_str()))
		{
			for (uint32_t i = 0; i < IM_ARRAYSIZE(behaviourList); i++)
			{
				const bool isSelected = i == activeBehaviour;
				if (ImGui::Selectable(behaviourList[i], isSelected))
				{
					activeBehaviour = i;
					previewValueBehaviour = behaviourList[i];
				}
			}

			switch (activeBehaviour)
			{
			case 0:
				m_mapStates.find(AIStateTypes::Idle)->second->SetActivation(1.0f);
				m_mapStates.find(AIStateTypes::Seek)->second->SetActivation(0.0f);
				m_mapStates.find(AIStateTypes::Flee)->second->SetActivation(0.0f);
				m_mapStates.find(AIStateTypes::Patrol)->second->SetActivation(0.0f);
				m_mapStates.find(AIStateTypes::Follow)->second->SetActivation(0.0f);
				m_mapStates.find(AIStateTypes::Wander)->second->SetActivation(0.0f);
				m_mapStates.find(AIStateTypes::Fire)->second->SetActivation(0.0f);
				break;
			case 1:
				m_mapStates.find(AIStateTypes::Idle)->second->SetActivation(0.0f);
				m_mapStates.find(AIStateTypes::Seek)->second->SetActivation(1.0f);
				m_mapStates.find(AIStateTypes::Flee)->second->SetActivation(0.0f);
				m_mapStates.find(AIStateTypes::Patrol)->second->SetActivation(0.0f);
				m_mapStates.find(AIStateTypes::Follow)->second->SetActivation(0.0f);
				m_mapStates.find(AIStateTypes::Wander)->second->SetActivation(0.0f);
				m_mapStates.find(AIStateTypes::Fire)->second->SetActivation(0.0f);
				break;
			case 2:
				m_mapStates.find(AIStateTypes::Idle)->second->SetActivation(0.0f);
				m_mapStates.find(AIStateTypes::Seek)->second->SetActivation(0.0f);
				m_mapStates.find(AIStateTypes::Flee)->second->SetActivation(1.0f);
				m_mapStates.find(AIStateTypes::Patrol)->second->SetActivation(0.0f);
				m_mapStates.find(AIStateTypes::Follow)->second->SetActivation(0.0f);
				m_mapStates.find(AIStateTypes::Wander)->second->SetActivation(0.0f);
				m_mapStates.find(AIStateTypes::Fire)->second->SetActivation(0.0f);
				break;
			case 3:
				m_mapStates.find(AIStateTypes::Idle)->second->SetActivation(0.0f);
				m_mapStates.find(AIStateTypes::Seek)->second->SetActivation(0.0f);
				m_mapStates.find(AIStateTypes::Flee)->second->SetActivation(0.0f);
				m_mapStates.find(AIStateTypes::Patrol)->second->SetActivation(1.0f);
				m_mapStates.find(AIStateTypes::Follow)->second->SetActivation(0.0f);
				m_mapStates.find(AIStateTypes::Wander)->second->SetActivation(0.0f);
				m_mapStates.find(AIStateTypes::Fire)->second->SetActivation(0.0f);
				break;
			case 4:
				m_mapStates.find(AIStateTypes::Idle)->second->SetActivation(0.0f);
				m_mapStates.find(AIStateTypes::Seek)->second->SetActivation(0.0f);
				m_mapStates.find(AIStateTypes::Flee)->second->SetActivation(0.0f);
				m_mapStates.find(AIStateTypes::Patrol)->second->SetActivation(0.0f);
				m_mapStates.find(AIStateTypes::Follow)->second->SetActivation(1.0f);
				m_mapStates.find(AIStateTypes::Wander)->second->SetActivation(0.0f);
				m_mapStates.find(AIStateTypes::Fire)->second->SetActivation(0.0f);
				break;
			case 5:
				m_mapStates.find(AIStateTypes::Idle)->second->SetActivation(0.0f);
				m_mapStates.find(AIStateTypes::Seek)->second->SetActivation(0.0f);
				m_mapStates.find(AIStateTypes::Flee)->second->SetActivation(0.0f);
				m_mapStates.find(AIStateTypes::Patrol)->second->SetActivation(0.0f);
				m_mapStates.find(AIStateTypes::Follow)->second->SetActivation(0.0f);
				m_mapStates.find(AIStateTypes::Wander)->second->SetActivation(1.0f);
				m_mapStates.find(AIStateTypes::Fire)->second->SetActivation(0.0f);
				break;
			case 6:
				m_mapStates.find(AIStateTypes::Idle)->second->SetActivation(0.0f);
				m_mapStates.find(AIStateTypes::Seek)->second->SetActivation(0.0f);
				m_mapStates.find(AIStateTypes::Flee)->second->SetActivation(0.0f);
				m_mapStates.find(AIStateTypes::Patrol)->second->SetActivation(0.0f);
				m_mapStates.find(AIStateTypes::Follow)->second->SetActivation(0.0f);
				m_mapStates.find(AIStateTypes::Wander)->second->SetActivation(0.0f);
				m_mapStates.find(AIStateTypes::Fire)->second->SetActivation(1.0f);
				break;
			}

			ImGui::EndCombo();
		}
		ImGui::NewLine();
		float floatTab[1] = { m_fSpeed };
		float* speed = floatTab;
		ImGui::Text("Speed");
		ImGui::SliderFloat("##EnemySpeed", speed, 1.0f, 10.0f, "%.1f");
		m_fSpeed = *speed;

		ImGui::NewLine();
		ImGui::Separator();
		ImGui::NewLine();

        ImGui::Text("Enemy");
        ImGui::Text(std::string("X: ").append(std::to_string(fGO.x)).c_str());
        ImGui::Text(std::string("Y: ").append(std::to_string(fGO.y)).c_str());

		ImGui::NewLine();
		float fDistance = fGO.Distance(fTarg);
		ImGui::Text(std::string("Distance: ").append(std::to_string(fDistance)).c_str());

		std::vector<Vector2f> vecWaypoints = m_mapStates.find(AIStateTypes::Patrol)->second->GetWaypoints();
		int iCurrentWaypoint = m_mapStates.find(AIStateTypes::Patrol)->second->GetCurrentWaypointIndex();
		if (vecWaypoints.size() > 0)
		{
			ImGui::NewLine();
			ImGui::Separator();
			ImGui::NewLine();

			ImGui::Text("Waypoints");
			ImGui::Text(std::string("Current Waypoint #").append(std::to_string(iCurrentWaypoint)).c_str());

			for (int i = 0; i < vecWaypoints.size(); i++)
			{
				ImGui::Text(std::string("Waypoint #").append(std::to_string(i)).c_str());

				ImGui::Text(std::string("X: ").append(std::to_string(vecWaypoints[i].x)).c_str());
				ImGui::Text(std::string("Y: ").append(std::to_string(vecWaypoints[i].y)).c_str());

				ImGui::NewLine();
			}
		}

		ImGui::NewLine();
		ImGui::Separator();
		ImGui::NewLine();

        ImGui::Text("Target");
		ImGui::Text(std::string("X: ").append(std::to_string(fTarg.x)).c_str());
        ImGui::Text(std::string("Y: ").append(std::to_string(fTarg.y)).c_str());
		static int targetGroup = 0;
		if (ImGui::RadioButton("Mouse", &targetGroup, 0))
			m_bTargetMouse = true;
		ImGui::SameLine();
		if (ImGui::RadioButton("Player##Field", &targetGroup, 1))
			m_bTargetMouse = false;
	}
	ImGui::End();
}
#endif

void Agent::SetBehaviour(AILogic::AIStateTypes behaviour)
{
	m_bTargetMouse = false;
	m_fSpeed = 5.0f;
	ResetBehaviour();

	m_mapStates.find(behaviour)->second->SetActivation(1.0f);
}

void Agent::ResetBehaviour()
{
	m_mapStates.find(AIStateTypes::Idle)->second->SetActivation(0.0f);
	m_mapStates.find(AIStateTypes::Seek)->second->SetActivation(0.0f);
	m_mapStates.find(AIStateTypes::Flee)->second->SetActivation(0.0f);
	m_mapStates.find(AIStateTypes::Patrol)->second->SetActivation(0.0f);
	m_mapStates.find(AIStateTypes::Follow)->second->SetActivation(0.0f);
	m_mapStates.find(AIStateTypes::Wander)->second->SetActivation(0.0f);
	m_mapStates.find(AIStateTypes::Fire)->second->SetActivation(0.0f);
}

void Agent::AddToEvent() noexcept
{
	EventSystem::Instance()->AddClient(EVENTID::MousePosition, this);
	EventSystem::Instance()->AddClient(EVENTID::PlayerPosition, this);
}

void Agent::RemoveFromEvent() noexcept
{
	EventSystem::Instance()->RemoveClient(EVENTID::MousePosition, this);
	EventSystem::Instance()->RemoveClient(EVENTID::PlayerPosition, this);
}

void Agent::HandleEvent(Event* event)
{
	switch (event->GetEventID())
	{
	case EVENTID::MousePosition:
		if (m_bTargetMouse)
			m_vTargetPos = *(Vector2f*)event->GetData();
		break;
	case EVENTID::PlayerPosition:
		if (!m_bTargetMouse) 
		{
			std::pair < Sprite*, Vector2f*>* dPair = (std::pair<Sprite*, Vector2f*>*)(event->GetData());
			m_vTargetPos = *dPair->second + Vector2f(dPair->first->GetWidthHeight() / 2);
		}
		break;
	default:
		break;
	}
}