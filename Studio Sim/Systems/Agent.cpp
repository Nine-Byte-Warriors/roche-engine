#include "stdafx.h"
#include "Agent.h"
#include <imgui/imgui.h>

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
	
	AddToEvent();
}

void Agent::Update(const float dt)
{
	m_pStateMachine->Clear();
	for (auto const& [key, value] : m_mapStates)
		m_pStateMachine->AddState(value);
	m_pStateMachine->UpdateMachine(dt);
}

void Agent::SpawnControlWindow(Vector2f fGO, Vector2f fTarg) noexcept
{
	if (ImGui::Begin("Agent AI", FALSE, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("Behaviour");
		static int activeBehaviour = 0;
		static std::string previewValueBehaviour = "Idle";
		static const char* behaviourList[]{ "Idle", "Seek", "Flee" };
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
				break;
			case 1:
				m_mapStates.find(AIStateTypes::Idle)->second->SetActivation(0.0f);
				m_mapStates.find(AIStateTypes::Seek)->second->SetActivation(1.0f);
				m_mapStates.find(AIStateTypes::Flee)->second->SetActivation(0.0f);
				break;
			case 2:
				m_mapStates.find(AIStateTypes::Idle)->second->SetActivation(0.0f);
				m_mapStates.find(AIStateTypes::Seek)->second->SetActivation(0.0f);
				m_mapStates.find(AIStateTypes::Flee)->second->SetActivation(1.0f);
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

void Agent::AddToEvent() noexcept
{
	EventSystem::Instance()->AddClient(EVENTID::MousePosition, this);
	EventSystem::Instance()->AddClient(EVENTID::PlayerPosition, this);
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
			m_vTargetPos = *(Vector2f*)event->GetData();
		break;
	default:
		break;
	}
}