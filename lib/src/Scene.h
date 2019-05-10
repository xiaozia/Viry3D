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

#include "Object.h"
#include "container/Map.h"
#include "container/Vector.h"

namespace Viry3D
{
	class GameObject;

    class Scene : public Object
    {
    public:
		static Scene* Instance();
        Scene();
        virtual ~Scene();
        void Update();
        Ref<GameObject> GetGameObject(const GameObject* obj);

	private:
		friend class GameObject;
		void AddGameObject(const Ref<GameObject>& obj);
		void RemoveGameObject(const Ref<GameObject>& obj);

	private:
		static Scene* m_instance;
		Map<int, Ref<GameObject>> m_objects;
		Vector<Ref<GameObject>> m_added_objects;
		Vector<Ref<GameObject>> m_removed_objects;
    };
}
