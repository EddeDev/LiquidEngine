#include "LiquidPCH.h"
#include "Application.h"

namespace Liquid {

	Application::Application(const ApplicationCreateInfo& createInfo)
		: m_CreateInfo(createInfo)
	{
		WindowCreateInfo windowCreateInfo;
		windowCreateInfo.Width = 1280;
		windowCreateInfo.Height = 720;
		windowCreateInfo.Title = "Liquid Engine";

		m_Window = Window::Create(windowCreateInfo);
	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		while (m_Running)
		{
			m_Window->PollEvents();
		}
	}

}