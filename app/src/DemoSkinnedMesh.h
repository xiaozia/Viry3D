/*
* Viry3D
* Copyright 2014-2018 by Stack - stackos@qq.com
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

#include "DemoMesh.h"
#include "Resources.h"

namespace Viry3D
{
    class DemoSkinnedMesh : public DemoMesh
    {
    public:
        Ref<Node> m_node;

        void AddRenderer(const Ref<Node>& node, const Ref<Material>& material)
        {
            auto renderer = RefCast<MeshRenderer>(node);
            if (renderer)
            {
                renderer->SetMaterial(material);
                m_camera->AddRenderer(renderer);
                m_renderers.Add(renderer);
            }

            int child_count = node->GetChildCount();
            for (int i = 0; i < child_count; ++i)
            {
                this->AddRenderer(node->GetChild(i), material);
            }
        }

        virtual void InitMesh()
        {
            auto shader = CreateDiffuseShader();

            this->InitGround(shader);

            auto texture = Texture::LoadTexture2DFromFile(Application::Instance()->GetDataPath() + "/res/model/ToonSoldier 1/tex.png", VK_FILTER_LINEAR, VK_SAMPLER_ADDRESS_MODE_REPEAT, true);

            auto material = RefMake<Material>(shader);
            material->SetTexture("u_texture", texture);
            material->SetVector("u_uv_scale_offset", Vector4(1, 1, 0, 0));
            material->SetMatrix("u_view_matrix", m_view);
            material->SetMatrix("u_projection_matrix", m_projection);
            material->SetColor("u_light_color", m_light_color);
            material->SetVector("u_light_dir", m_light_dir);
            material->SetFloat("u_light_intensity", m_light_intensity);

            m_node = Resources::Load(Application::Instance()->GetDataPath() + "/res/model/ToonSoldier 1/ToonSoldier 1.go");
            this->AddRenderer(m_node, material);

            m_node->SetLocalPosition(Vector3(0, 0, 0));
        }

        virtual void Init()
        {
            DemoMesh::Init();
        }

        virtual void Done()
        {
            m_node.reset();

            DemoMesh::Done();
        }

        virtual void Update()
        {
            DemoMesh::Update();
        }

        virtual void OnResize(int width, int height)
        {
            DemoMesh::OnResize(width, height);
        }
    };
}
