/*
* Viry3D
* Copyright 2014-2019 by Stack - stackos@qq.com
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#pragma once

#include "Component.h"
#include "graphics/Camera.h"
#include "graphics/MeshRenderer.h"
#include "graphics/Skybox.h"
#include "graphics/Light.h"
#include "animation/Animation.h"
#include "ui/CanvasRenderer.h"
#include "ui/Sprite.h"
#include "ui/Label.h"
#include "time/Time.h"
#include "Engine.h"
#include "Resources.h"
#include "Input.h"

namespace Viry3D
{
	class App : public Component
	{
	public:
		Camera* m_camera = nullptr;
		GameObject* m_skin = nullptr;
		Vector2 m_last_touch_pos;
		Vector3 m_camera_rot = Vector3(5, 180, 0);
		Label* m_fps_label = nullptr;

		App()
		{
			auto cube = Resources::LoadMesh("Library/unity default resources.Cube.mesh");
			auto texture = Resources::LoadTexture("texture/checkflag.png.tex");
			auto cubemap = Resources::LoadTexture("texture/env/prefilter.tex");

			auto material = RefMake<Material>(Shader::Find("Unlit/Texture"));
			material->SetTexture(MaterialProperty::TEXTURE, texture);
			material->SetVector(MaterialProperty::TEXTURE_SCALE_OFFSET, Vector4(40, 20, 0, 0));

			auto camera = GameObject::Create("")->AddComponent<Camera>();
			camera->GetTransform()->SetPosition(Vector3(0, 1, 3));
			camera->GetTransform()->SetRotation(Quaternion::Euler(m_camera_rot));
			camera->SetDepth(0);
			camera->SetCullingMask(1 << 0);
			m_camera = camera.get();

			auto skybox = GameObject::Create("")->AddComponent<Skybox>();
			skybox->SetTexture(cubemap, 0.0f);

			auto floor = GameObject::Create("")->AddComponent<MeshRenderer>();
			floor->GetTransform()->SetPosition(Vector3(0, -5, 0));
			floor->GetTransform()->SetScale(Vector3(20, 10, 10));
			floor->SetMesh(cube);
			floor->SetMaterial(material);

			auto obj_0 = Resources::LoadGameObject("Resources/res/animations/unitychan/unitychan.go");
			obj_0->GetTransform()->SetPosition(Vector3(0, 0, 0));
			auto anim = obj_0->GetComponent<Animation>();
			anim->Play(0);

			auto obj_1 = Resources::LoadGameObject("Resources/res/animations/unitychan/unitychan.go");
			obj_1->GetTransform()->SetPosition(Vector3(-2, 0, -2));

			auto obj_2 = Resources::LoadGameObject("Resources/res/animations/unitychan/unitychan.go");
			obj_2->GetTransform()->SetPosition(Vector3(2, 0, -2));
			m_skin = obj_2.get();

			auto ui_camera = GameObject::Create("")->AddComponent<Camera>();
			ui_camera->SetClearFlags(CameraClearFlags::Nothing);
			ui_camera->SetDepth(1);
			ui_camera->SetCullingMask(1 << 1);

			auto canvas = GameObject::Create("")->AddComponent<CanvasRenderer>(FilterMode::Linear);
			canvas->GetGameObject()->SetLayer(1);
			canvas->SetCamera(ui_camera);

			auto label = RefMake<Label>();
			label->SetAlignment(ViewAlignment::Left | ViewAlignment::Top);
			label->SetPivot(Vector2(0, 0));
			label->SetColor(Color(0, 0, 0, 1));
			label->SetTextAlignment(ViewAlignment::Left | ViewAlignment::Top);
			canvas->AddView(label);
			m_fps_label = label.get();
            
            auto sphere = GameObject::Create("")->AddComponent<MeshRenderer>();
            sphere->SetMaterial(RefMake<Material>(Shader::Find("Diffuse")));
            sphere->SetMesh(Resources::LoadMesh("Library/unity default resources.Sphere.mesh"));
            sphere->GetTransform()->SetPosition(Vector3(-1, 1, 0));
            
			auto sphere2 = GameObject::Create("")->AddComponent<MeshRenderer>();
			sphere2->SetMaterial(RefMake<Material>(Shader::Find("Diffuse")));
			sphere2->SetMesh(Resources::LoadMesh("Library/unity default resources.Sphere.mesh"));
			sphere2->GetTransform()->SetPosition(Vector3(1, 1, 0));

            auto light = GameObject::Create("")->AddComponent<Light>();
            light->GetTransform()->SetRotation(Quaternion::Euler(0, 180, 0));
            
#if 0
			auto color = Texture::CreateRenderTexture(
				1280,
				720,
				TextureFormat::R8G8B8A8,
				FilterMode::Linear,
				SamplerAddressMode::ClampToEdge);
			auto depth = Texture::CreateRenderTexture(
				1280,
				720,
				Texture::SelectDepthFormat(),
				FilterMode::Linear,
				SamplerAddressMode::ClampToEdge);
			camera->SetRenderTarget(color, depth);

			auto blit_camera = GameObject::Create("")->AddComponent<Camera>();
			blit_camera->SetOrthographic(true);
			blit_camera->SetOrthographicSize(1);
			blit_camera->SetNearClip(-1);
			blit_camera->SetFarClip(1);
			blit_camera->SetDepth(2);
			blit_camera->SetCullingMask(1 << 2);

			material = RefMake<Material>(Shader::Find("Unlit/Texture"));
			material->SetTexture(MaterialProperty::TEXTURE, color);
			if (Engine::Instance()->GetBackend() == filament::backend::Backend::OPENGL)
			{
				material->SetVector(MaterialProperty::TEXTURE_SCALE_OFFSET, Vector4(1, -1, 0, 1));
			}

			auto quad = GameObject::Create("")->AddComponent<MeshRenderer>();
			quad->GetGameObject()->SetLayer(2);
			quad->GetTransform()->SetScale(Vector3(1280 / 720.0f, 1, 1));
			quad->SetMesh(Mesh::GetSharedQuadMesh());
			quad->SetMaterial(material);
#endif
		}

		virtual ~App()
		{
			
		}

		virtual void Update()
		{
			if (m_fps_label)
			{
				m_fps_label->SetText(String::Format("FPS:%d", Time::GetFPS()));
			}

			static float deg = 0;
			deg += 1.0f;
			if (m_skin)
			{
				m_skin->GetTransform()->SetRotation(Quaternion::Euler(0, deg, 0));
			}

			// camera control
			if (Input::GetTouchCount() > 0)
			{
				const Touch& touch = Input::GetTouch(0);
				if (touch.phase == TouchPhase::Began)
				{
					m_last_touch_pos = touch.position;
				}
				else if (touch.phase == TouchPhase::Moved)
				{
					Vector2 delta = touch.position - m_last_touch_pos;
					m_last_touch_pos = touch.position;

					m_camera_rot.y += delta.x * 0.1f;
					m_camera_rot.x += -delta.y * 0.1f;
					m_camera->GetTransform()->SetRotation(Quaternion::Euler(m_camera_rot));
				}
			}
			if (Input::GetKey(KeyCode::W))
			{
				Vector3 forward = m_camera->GetTransform()->GetForward();
				Vector3 pos = m_camera->GetTransform()->GetPosition() + forward * 0.1f;
				m_camera->GetTransform()->SetPosition(pos);
			}
			if (Input::GetKey(KeyCode::S))
			{
				Vector3 forward = m_camera->GetTransform()->GetForward();
				Vector3 pos = m_camera->GetTransform()->GetPosition() - forward * 0.1f;
				m_camera->GetTransform()->SetPosition(pos);
			}
			if (Input::GetKey(KeyCode::A))
			{
				Vector3 right = m_camera->GetTransform()->GetRight();
				Vector3 pos = m_camera->GetTransform()->GetPosition() - right * 0.1f;
				m_camera->GetTransform()->SetPosition(pos);
			}
			if (Input::GetKey(KeyCode::D))
			{
				Vector3 right = m_camera->GetTransform()->GetRight();
				Vector3 pos = m_camera->GetTransform()->GetPosition() + right * 0.1f;
				m_camera->GetTransform()->SetPosition(pos);
			}
		}
	};
}
