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

#include <stdint.h>
#include "string/String.h"

namespace Viry3D
{
	class EnginePrivate;

    class Engine
    {
	public:
		static Engine* Create(void* native_window, int width, int height, uint64_t flags = 0, void* shared_gl_context = nullptr);
		static void Destroy(Engine** engine);
		static Engine* Instance();
		void Execute();
		const String& GetDataPath();
		const String& GetSavePath();
#if VR_ANDROID || VR_UWP
		void SetDataPath(const String& path);
		void SetSavePath(const String& path);
#endif
		void OnResize(void* native_window, int width, int height, uint64_t flags = 0);
		int GetWidth() const;
		int GetHeight() const;

		void InitTest();
		void ShutdownTest();

	private:
		Engine(void* native_window, int width, int height, uint64_t flags, void* shared_gl_context);
		~Engine();

	private:
		EnginePrivate* m_private;
    };
}
