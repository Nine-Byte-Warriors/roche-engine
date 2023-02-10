#include "stdafx.h"
#include "Camera.h"
#include "Sprite.h"

#if _DEBUG
extern bool g_bDebug;
#endif

void Camera::SetProjectionValues(float width, float height, float nearZ, float farZ)
{
	m_vSizeOfScreen = { width, height };
	m_vPosition = { m_vSizeOfScreen.x / 2.0f, m_vSizeOfScreen.y / 2.0f };
	m_mOrthoMatrix = XMMatrixOrthographicOffCenterLH(0.0f, width, height, 0.0f, nearZ, farZ);

#if !_DEBUG
	m_bLockedToPlayer = true;
#endif

	m_vInitPosition = m_vPosition;
}

void Camera::SpawnControlWindow()
{
#if _DEBUG
	if (ImGui::Begin("Camera Editor", FALSE, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("Move Speed");
		ImGui::SliderFloat("##Move Speed", &m_fSpeed, 1.0f, 20.0f, "%.1f");
		ImGui::NewLine();
		if (ImGui::Checkbox("Lock To Player?", &m_bLockedToPlayer))
		{
			EventSystem::Instance()->AddEvent(EVENTID::TogglePlayerMovement, this);
		}
	}
	ImGui::End();
#endif
}

void Camera::Update(const float dt)
{
	if (!m_bLockedToPlayer)
	{
		m_mWorldMatrix = XMMatrixTranslation(
			-(m_vPosition.x - m_vSizeOfScreen.x / 2.0f),
			-(m_vPosition.y - m_vSizeOfScreen.y / 2.0f), 0.0f);
	}

	float cameraX = GetPosition().x - GetInitPosition().x;
	float cameraY = GetPosition().y - GetInitPosition().y;
	Vector2f* mouseCameraPos = new Vector2f(m_fMousePos.x + cameraX, m_fMousePos.y + cameraY);
	EventSystem::Instance()->AddEvent(EVENTID::MouseCameraPosition, mouseCameraPos);
}

void Camera::AddToEvent() noexcept
{
	EventSystem::Instance()->AddClient(EVENTID::WindowSizeChangeEvent, this);
	EventSystem::Instance()->AddClient(EVENTID::MoveUp, this);
	EventSystem::Instance()->AddClient(EVENTID::MoveLeft, this);
	EventSystem::Instance()->AddClient(EVENTID::MoveDown, this);
	EventSystem::Instance()->AddClient(EVENTID::MoveRight, this);
	EventSystem::Instance()->AddClient(EVENTID::PlayerPosition, this);
	EventSystem::Instance()->AddClient(EVENTID::MousePosition, this);
	EventSystem::Instance()->AddClient(EVENTID::ImGuiMousePosition, this);
	EventSystem::Instance()->AddClient(EVENTID::GameLevelChangeEvent, this);
}

void Camera::RemoveFromEvent() noexcept
{
	EventSystem::Instance()->RemoveClient(EVENTID::WindowSizeChangeEvent, this);
	EventSystem::Instance()->RemoveClient(EVENTID::MoveUp, this);
	EventSystem::Instance()->RemoveClient(EVENTID::MoveLeft, this);
	EventSystem::Instance()->RemoveClient(EVENTID::MoveDown, this);
	EventSystem::Instance()->RemoveClient(EVENTID::MoveRight, this);
	EventSystem::Instance()->RemoveClient(EVENTID::PlayerPosition, this);
	EventSystem::Instance()->RemoveClient(EVENTID::MousePosition, this);
	EventSystem::Instance()->RemoveClient(EVENTID::ImGuiMousePosition, this);
	EventSystem::Instance()->RemoveClient(EVENTID::GameLevelChangeEvent, this);
}

void Camera::HandleEvent(Event* event)
{
	switch (event->GetEventID())
	{
#if _DEBUG
	case EVENTID::ImGuiMousePosition:
	{
		if (!g_bDebug) return;
		Vector2f mousePos = *(Vector2f*)event->GetData();
		m_fMousePos = XMFLOAT2(mousePos.x, mousePos.y);
	}
	break;
#endif
	case EVENTID::MousePosition:
	{
#if _DEBUG
		if (g_bDebug) return;
#endif
		m_fMousePos = *(XMFLOAT2*)event->GetData();
	}
	break;
	case EVENTID::WindowSizeChangeEvent:
	{
		m_vSizeOfScreen = *static_cast<XMFLOAT2*>(event->GetData());
		if (m_vSizeOfScreen.x < 500.0f) m_vSizeOfScreen.x = 1260.0f;
		if (m_vSizeOfScreen.y < 400.0f) m_vSizeOfScreen.y = 500.0f;
		SetProjectionValues(m_vSizeOfScreen.x, m_vSizeOfScreen.y, 0.0f, 1.0f);
	}
	break;
	case EVENTID::GameLevelChangeEvent:
	{
		m_sCurrentLevel = *static_cast<std::string*>(event->GetData());
	}
	break;
	case EVENTID::PlayerPosition:
	{
		if (m_bLockedToPlayer)
		{
			std::pair<Sprite*, Vector2f*>* charSpriteandPos = static_cast<std::pair<Sprite*, Vector2f*>*>(event->GetData());
			Sprite* charSprite = charSpriteandPos->first;
			m_vPosition = XMFLOAT2(charSpriteandPos->second->x + charSprite->GetWidth()/2, charSpriteandPos->second->y + charSprite->GetHeight()/2);
			if (m_sCurrentLevel == "Game")
			{
				if (m_vPosition.x >= m_vSizeOfScreen.x)
					m_vPosition.x = m_vSizeOfScreen.x;
				if (m_vPosition.x <= 0)
					m_vPosition.x = 0;

				if (m_vPosition.y >= m_vSizeOfScreen.y)
					m_vPosition.y = m_vSizeOfScreen.y;
				if (m_vPosition.y <= 0)
					m_vPosition.y = 0;
			}
			else if (m_sCurrentLevel == "Shop")
			{
				m_vPosition.x = 1100.0f;
				m_vPosition.y = 1250.0f;
			}


			m_mWorldMatrix = XMMatrixTranslation(
				-(m_vPosition.x - m_vSizeOfScreen.x / 2.0f),
				-(m_vPosition.y - m_vSizeOfScreen.y / 2.0f), 0.0f);
		}
	}
	break;
	case EVENTID::MoveUp: if (!m_bLockedToPlayer) { m_vPosition.y -= m_fSpeed; } break;
	case EVENTID::MoveDown: if (!m_bLockedToPlayer) { m_vPosition.y += m_fSpeed; } break;
	case EVENTID::MoveRight: if (!m_bLockedToPlayer) { m_vPosition.x += m_fSpeed; } break;
	case EVENTID::MoveLeft: if (!m_bLockedToPlayer) { m_vPosition.x -= m_fSpeed; } break;
	}
}