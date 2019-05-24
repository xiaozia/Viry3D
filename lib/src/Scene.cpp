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

#include "Scene.h"
#include "GameObject.h"

// test
#include "graphics/Camera.h"
#include "graphics/MeshRenderer.h"
#include "animation/Animation.h"
#include "Engine.h"
#include "Resources.h"

namespace Viry3D
{
	// test
	GameObject* skin;
    // test

	Scene* Scene::m_instance = nullptr;

	Scene* Scene::Instance()
	{
		return m_instance;
	}

    Scene::Scene()
    {
		m_instance = this;

		// test
        auto cylinder = Resources::LoadMesh("Library/unity default resources.Cylinder.mesh");
        auto texture = Resources::LoadTexture("texture/checkflag.png.tex");
        
        auto material = RefMake<Material>(Shader::Find("Unlit/Texture"));
        material->SetTexture(MaterialProperty::TEXTURE, texture);
		material->SetVector(MaterialProperty::TEXTURE_SCALE_OFFSET, Vector4(20, 20, 0, 0));

		auto camera = GameObject::Create("")->AddComponent<Camera>();
        camera->GetTransform()->SetPosition(Vector3(0, 1, 3));
		camera->GetTransform()->SetRotation(Quaternion::Euler(5, 180, 0));

        auto floor = GameObject::Create("")->AddComponent<MeshRenderer>();
		floor->GetTransform()->SetPosition(Vector3(0, -0.01f, 0));
		floor->GetTransform()->SetScale(Vector3(20, 0.02f, 20));
		floor->SetMesh(cylinder);
		floor->SetMaterial(material);

		auto obj_0 = Resources::LoadGameObject("Resources/res/animations/unitychan/unitychan.go");
		obj_0->GetTransform()->SetPosition(Vector3(0, 0, 0));
		auto anim = obj_0->GetComponent<Animation>();
		anim->Play(0);

		auto obj_1 = Resources::LoadGameObject("Resources/res/animations/unitychan/unitychan.go");
		obj_1->GetTransform()->SetPosition(Vector3(-2, 0, -2));

		auto obj_2 = Resources::LoadGameObject("Resources/res/animations/unitychan/unitychan.go");
		obj_2->GetTransform()->SetPosition(Vector3(2, 0, -2));
		skin = obj_2.get();
    }
    
    Scene::~Scene()
    {
		m_added_objects.Clear();
		m_removed_objects.Clear();
		m_objects.Clear();

		m_instance = nullptr;
    }

	void Scene::AddGameObject(const Ref<GameObject>& obj)
	{
		m_added_objects.Add(obj);
	}

	void Scene::RemoveGameObject(const Ref<GameObject>& obj)
	{
        if (!m_added_objects.Remove(obj))
        {
            m_removed_objects.Add(obj);
        }
	}
    
    void Scene::Update()
    {
		for (auto& i : m_objects)
		{
			auto& obj = i.second;
			if (obj->IsActiveInTree())
			{
				obj->Update();
			}
		}

		do
		{
			Vector<Ref<GameObject>> added = m_added_objects;
			m_added_objects.Clear();

			for (int i = 0; i < added.Size(); ++i)
			{
				auto& obj = added[i];
				if (obj->IsActiveInTree())
				{
					obj->Update();
				}
				m_objects.Add(obj->GetId(), obj);
			}
			added.Clear();
		} while (m_added_objects.Size() > 0);

		for (int i = 0; i < m_removed_objects.Size(); ++i)
		{
			const auto& obj = m_removed_objects[i];
			m_objects.Remove(obj->GetId());
		}
		m_removed_objects.Clear();
        
        // test
        static float deg = 0;
        deg += 1.0f;
		skin->GetTransform()->SetLocalRotation(Quaternion::Euler(0, deg, 0));
    }
    
    Ref<GameObject> Scene::GetGameObject(const GameObject* obj)
    {
        for (int i = 0; i < m_added_objects.Size(); ++i)
        {
            const auto& obj_ref = m_added_objects[i];
            if (obj_ref.get() == obj)
            {
                return obj_ref;
            }
        }
        
        Ref<GameObject>* find;
        if (m_objects.TryGet(obj->GetId(), &find))
        {
            return *find;
        }
        
        return Ref<GameObject>();
    }
}
