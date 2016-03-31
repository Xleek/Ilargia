/*************************************************************************
* Ilargia Engine - http://github.com/Xleek/Ilargia
* C++ Modular Data Oriented Game Enginee
*------------------------------------------------------------------------
* Copyright (c) 2014-2015, Louis Schnellbach
*
* This software is provided 'as-is', without any express or implied
* warranty. In no event will the authors be held liable for any damages
* arising from the use of this software.
*
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
*
* 1. The origin of this software must not be misrepresented; you must not
*    claim that you wrote the original software. If you use this software
*    in a product, an acknowledgment in the product documentation would
*    be appreciated but is not required.
*
* 2. Altered source versions must be plainly marked as such, and must not
*    be misrepresented as being the original software.
*
* 3. This notice may not be removed or altered from any source
*    distribution.
*
*************************************************************************/

#ifndef INCLUDE_ILARGIA_ICOMPONENTLESSMANAGER_HPP
#define INCLUDE_ILARGIA_ICOMPONENTLESSMANAGER_HPP

#include "Ilargia/Manager/IBaseManager.hpp"

namespace ilg
{
	class ManagerFactory;
	class ILARGIA_API IComponentlessManager : public IBaseManager
	{
	public:
		IComponentlessManager(const m::String& name, m::i32 updateOrder);
		virtual ~IComponentlessManager();

		virtual bool onInit() = 0;
		virtual bool onUpdate(m::f32 deltaTime) = 0;
		virtual bool onTerm() = 0;

		virtual void onKeyCallback(void* windowHandle, int key, int scancode, int action, int modifier);
		virtual void onComponentAdd(Entity* entity, Component& component);
		virtual void onComponentRemove(Entity* entity, Component& component);

	private:
		virtual Component createComponent();
		virtual void destroyComponent(Component& component);
		virtual void* getComponent(m::i32 index);
		virtual Component getComponent(void* object);
	};
}
#endif